#pragma once

#include "common/queue/lock_free_queue.h"
#include "config/xxxdb_config.h"
#include "ordering/pbft/lock_free_collector_pool.h"
#include "ordering/pbft/transaction_utils.h"
#include "server/xxxdb_replica_client.h"
#include "statistic/stats.h"

namespace xxxdb {

class ResponseManager {
public:
  ResponseManager(const XXXDBConfig &config, XXXDBReplicaClient *replica_client,
                  SystemInfo *system_info);

  ~ResponseManager();

  std::vector<std::unique_ptr<Context>> FetchContextList(uint64_t id);

  int StartEval();

  int ProcessResponseMsg(std::unique_ptr<Context> context,
                         std::unique_ptr<Request> request);
  void SetDataFunc(std::function<std::string()> func);

private:
  // Add response messages which will be sent back to the client
  // if there are f+1 same messages.
  CollectorResultCode AddResponseMsg(
      const SignatureInfo &signature, std::unique_ptr<Request> request,
      std::function<void(const Request &,
                         const TransactionCollector::CollectorDataType *)>
          call_back);
  void SendResponseToClient(const BatchClientResponse &batch_response);

  struct QueueItem {
    std::unique_ptr<Context> context;
    std::unique_ptr<Request> client_request;
  };
  bool MayConsensusChangeStatus(int type, int received_count,
                                std::atomic<TransactionStatue> *status);
  int DoBatch(const std::vector<std::unique_ptr<QueueItem>> &batch_req);
  int BatchProposeMsg();
  int GetPrimary();
  std::unique_ptr<Request> GenerateUserRequest();

private:
  XXXDBConfig config_;
  XXXDBReplicaClient *replica_client_;
  std::unique_ptr<LockFreeCollectorPool> collector_pool_, context_pool_;
  LockFreeQueue<QueueItem> batch_queue_;
  std::thread user_req_thread_[16];
  std::atomic<bool> stop_;
  uint64_t local_id_ = 0;
  Stats *global_stats_;
  SystemInfo *system_info_;
  int send_num_;
  std::mutex mutex_;
  std::atomic<int> total_num_;
  SignatureVerifier *verifier_;
  SignatureInfo sig_;
  std::function<std::string()> data_func_;
  std::future<bool> eval_ready_future_;
  std::promise<bool> eval_ready_promise_;
  std::atomic<bool> eval_started_;
};

} // namespace xxxdb
