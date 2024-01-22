#include "ordering/pbft/response_manager.h"

#include <glog/logging.h>

#include "common/utils/utils.h"

namespace xxxdb {
ResponseManager::ResponseManager(const XXXDBConfig &config,
                                 XXXDBReplicaClient *replica_client,
                                 SystemInfo *system_info)
    : config_(config), replica_client_(replica_client),
      collector_pool_(std::make_unique<LockFreeCollectorPool>(
          "response", config_.GetMaxProcessTxn(), nullptr)),
      context_pool_(std::make_unique<LockFreeCollectorPool>(
          "context", config_.GetMaxProcessTxn(), nullptr)),
      batch_queue_("client request"), system_info_(system_info) {
  stop_ = false;
  eval_started_ = false;
  eval_ready_future_ = eval_ready_promise_.get_future();
  if (config_.GetPublicKeyCertificateInfo()
          .public_key()
          .public_key_info()
          .type() == CertificateKeyInfo::CLIENT) {
    for (int i = 0; i < 2; ++i) {
      user_req_thread_[i] =
          std::thread(&ResponseManager::BatchProposeMsg, this);
    }
  }

  global_stats_ = Stats::GetGlobalStats();
  send_num_ = 0;
  total_num_ = 0;
}

ResponseManager::~ResponseManager() {
  stop_ = true;
  for (int i = 0; i < 16; ++i) {
    if (user_req_thread_[i].joinable()) {
      user_req_thread_[i].join();
    }
  }
}

// use system info
int ResponseManager::GetPrimary() { return system_info_->GetPrimaryId(); }

std::unique_ptr<Request> ResponseManager::GenerateUserRequest() {
  std::unique_ptr<Request> request = std::make_unique<Request>();
  request->set_data(data_func_());
  return request;
}

void ResponseManager::SetDataFunc(std::function<std::string()> func) {
  LOG(ERROR) << "set data func";
  data_func_ = std::move(func);
}

int ResponseManager::StartEval() {
  if (eval_started_) {
    return 0;
  }
  LOG(WARNING) << "start eval";
  eval_started_ = true;
  for (int i = 0; i < 60000000; ++i) {
    std::unique_ptr<QueueItem> queue_item = std::make_unique<QueueItem>();
    queue_item->context = nullptr;
    queue_item->client_request = GenerateUserRequest();
    batch_queue_.Push(std::move(queue_item));
    if (i == 20000) {
      eval_ready_promise_.set_value(true);
    }
    if (i % 10000 == 0) {
      LOG(ERROR) << "data done:" << i;
    }
  }
  LOG(WARNING) << "start eval done";
  return 0;
}

// =================== response ========================
// handle the response message. If receive f+1 commit messages, send back to the
// client.
int ResponseManager::ProcessResponseMsg(std::unique_ptr<Context> context,
                                        std::unique_ptr<Request> request) {
  std::unique_ptr<Request> response;
  std::string hash = request->hash();
  uint64_t seq = request->seq();
  /*
  if (request->ret() == -2) {
    // LOG(INFO) << "get response fail:" << request->ret();
     send_num_--;
    return 0;
  }
  */
  // Add the response message, and use the call back to collect the received
  // messages.
  // The callback will be triggered if it received f+1 messages.
  CollectorResultCode ret =
      AddResponseMsg(context->signature, std::move(request),
                     [&](const Request &request,
                         const TransactionCollector::CollectorDataType *) {
                       response = std::make_unique<Request>(request);
                       return;
                     });

  if (ret == CollectorResultCode::STATE_CHANGED) {
    BatchClientResponse batch_response;
    if (batch_response.ParseFromString(response->data())) {
      SendResponseToClient(batch_response);
    } else {
      LOG(ERROR) << "parse response fail:";
    }
  }
  return ret == CollectorResultCode::INVALID ? -2 : 0;
}

bool ResponseManager::MayConsensusChangeStatus(
    int type, int received_count, std::atomic<TransactionStatue> *status) {
  switch (type) {
  case Request::TYPE_RESPONSE:
    // if receive f+1 response results, ack to the client.
    if (*status == TransactionStatue::None &&
        config_.GetMinClientReceiveNum() <= received_count) {
      TransactionStatue old_status = TransactionStatue::None;
      return status->compare_exchange_strong(
          old_status, TransactionStatue::EXECUTED, std::memory_order_acq_rel,
          std::memory_order_acq_rel);
    }
    break;
  }
  return false;
}

CollectorResultCode ResponseManager::AddResponseMsg(
    const SignatureInfo &signature, std::unique_ptr<Request> request,
    std::function<void(const Request &,
                       const TransactionCollector::CollectorDataType *)>
        response_call_back) {
  if (request == nullptr) {
    return CollectorResultCode::INVALID;
  }

  int type = request->type();
  uint64_t seq = request->seq();
  int resp_received_count = 0;
  int ret = collector_pool_->GetCollector(seq)->AddRequest(
      std::move(request), signature, false,
      [&](const Request &request, int received_count,
          TransactionCollector::CollectorDataType *data,
          std::atomic<TransactionStatue> *status) {
        if (MayConsensusChangeStatus(type, received_count, status)) {
          resp_received_count = 1;
          response_call_back(request, data);
        }
      });
  if (ret != 0) {
    return CollectorResultCode::INVALID;
  }
  if (resp_received_count > 0) {
    collector_pool_->Update(seq);
    return CollectorResultCode::STATE_CHANGED;
  }
  return CollectorResultCode::OK;
}

void ResponseManager::SendResponseToClient(
    const BatchClientResponse &batch_response) {
  uint64_t create_time = batch_response.createtime();
  uint64_t local_id = batch_response.local_id();
  // LOG(ERROR) << " send to client";
  if (create_time > 0) {
    uint64_t run_time = get_sys_clock() - create_time;
    global_stats_->AddLatency(run_time);
  } else {
    LOG(ERROR) << "seq:" << local_id << " no resp";
  }

  send_num_--;
}

// =================== request ========================
int ResponseManager::BatchProposeMsg() {
  LOG(ERROR) << "batch wait time?:" << config_.ClientBatchWaitTimeMS()
             << " batch num:" << config_.ClientBatchNum();
  std::vector<std::unique_ptr<QueueItem>> batch_req;
  eval_ready_future_.get();
  LOG(ERROR) << "start max txn:" << config_.GetMaxProcessTxn();
  while (!stop_) {
    if (send_num_ >= config_.GetMaxProcessTxn()) {
      usleep(100000);
      continue;
    }
    if (batch_req.size() < config_.ClientBatchNum()) {
      std::unique_ptr<QueueItem> item =
          batch_queue_.Pop(config_.ClientBatchWaitTimeMS());
      if (item == nullptr) {
        continue;
      }
      batch_req.push_back(std::move(item));
      if (batch_req.size() < config_.ClientBatchNum()) {
        continue;
      }
    }
    int ret = DoBatch(batch_req);
    batch_req.clear();
    // usleep(10);
  }
  return 0;
}

int ResponseManager::DoBatch(
    const std::vector<std::unique_ptr<QueueItem>> &batch_req) {
  auto new_request =
      NewRequest(Request::TYPE_NEW_TXNS, Request(), config_.GetSelfInfo().id());
  if (new_request == nullptr) {
    return -2;
  }
  BatchClientRequest batch_request;
  for (size_t i = 0; i < batch_req.size(); ++i) {
    BatchClientRequest::ClientRequest *req =
        batch_request.add_client_requests();
    req->mutable_request()->Swap(batch_req[i]->client_request.get());
    req->set_id(i);
  }

  batch_request.set_createtime(get_sys_clock());
  batch_request.SerializeToString(new_request->mutable_data());
  new_request->set_hash(SignatureVerifier::CalculateHash(new_request->data()));
  new_request->set_proxy_id(config_.GetSelfInfo().id());
  replica_client_->SendMessage(*new_request, GetPrimary());

  send_num_++;
  if (total_num_++ == 1000000) {
    stop_ = true;
    LOG(WARNING) << "total num is done:" << total_num_;
  }
  if (total_num_ % 10000 == 0) {
    LOG(WARNING) << "total num is :" << total_num_;
  }
  global_stats_->IncClientCall();
  return 0;
}

} // namespace xxxdb
