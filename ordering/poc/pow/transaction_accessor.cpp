#include "ordering/poc/pow/transaction_accessor.h"

#include "common/utils/utils.h"
#include <glog/logging.h>

namespace xxxdb {

TransactionAccessor::TransactionAccessor(const XXXDBPoCConfig &config,
                                         bool auto_start)
    : config_(config) {
  stop_ = false;
  max_received_seq_ = 0;
  next_consume_ = 1;
  if (auto_start) {
    fetching_thread_ =
        std::thread(&TransactionAccessor::TransactionFetching, this);
  }
}

TransactionAccessor::~TransactionAccessor() {
  stop_ = true;
  if (fetching_thread_.joinable()) {
    fetching_thread_.join();
  }
}

void TransactionAccessor::Start() {
  fetching_thread_ =
      std::thread(&TransactionAccessor::TransactionFetching, this);
}

void TransactionAccessor::TransactionFetching() {
  std::unique_ptr<XXXDBTxnClient> client = GetXXXDBTxnClient();
  assert(client != nullptr);

  uint64_t last_time = 0;
  uint64_t current_time = get_current_time();
  sleep(10);
  int cur_seq_pbft = 1;
  srand(12345);

  while (!stop_) {
    uint64_t cur_seq = max_received_seq_ + 1;
    if (cur_seq > 100000000)
      break;
    absl::StatusOr<std::vector<std::pair<uint64_t, std::string>>> ret;
    std::vector<std::pair<uint64_t, std::string>> ret1;
    for (int i = 0; i < 1000; ++i) {
      BatchClientRequest batch_request;
      std::string data;
      batch_request.SerializeToString(&data);
      ret1.push_back(std::make_pair(cur_seq + i, data));
    }
    ret = ret1;

    // double need_time = 2.0/1063;
    double need_time = 2.0 / 1000;
    uint64_t current_time1 = get_current_time();
    for (auto &res : (*ret)) {
      uint64_t current_time = get_current_time();

      if (last_time > 0) {
        double sleep_time =
            std::max((uint64_t)0, current_time - last_time) / 1000000.0;
        //	LOG(ERROR)<<"sleep:"<<sleep_time<<" "<<(need_time-sleep_time)<<"
        //"<<(int)((need_time - sleep_time)*1000000);
        if (need_time - sleep_time > 0) {
          usleep((int)((need_time - sleep_time) * 1000000));
        }
      }

      // LOG(ERROR)<<"current time:"<<current_time;
      last_time = current_time;
      std::unique_ptr<ClientTransactions> client_txn =
          std::make_unique<ClientTransactions>();
      // client_txn->set_transaction_data(res.second);
      client_txn->set_seq(cur_seq);
      client_txn->set_create_time(get_current_time());
      // prometheus_handler_->Inc(TRANSACTION_INPUT, 1);
      queue_.Push(std::move(client_txn));
      cur_seq++;
    }
    LOG(ERROR) << "add seq :" << max_received_seq_ << " cur:" << cur_seq
               << " spend:" << get_current_time() - current_time1 << " "
               << (*ret).size() << " new:" << get_sys_clock()
               << " current time:" << get_current_time();
    max_received_seq_ = cur_seq;

    std::lock_guard<std::mutex> lk(mutex_);
    cv_.notify_all();
  }

  return;
}

std::unique_ptr<XXXDBTxnClient> TransactionAccessor::GetXXXDBTxnClient() {
  return std::make_unique<XXXDBTxnClient>(*config_.GetBFTConfig());
}

// obtain [seq, seq+batch_num-1] transactions
std::unique_ptr<BatchClientTransactions>
TransactionAccessor::ConsumeTransactions(uint64_t seq) {
  uint64_t next_want = config_.BatchTransactionNum();
  LOG(ERROR) << "consume transaction:" << seq
             << " batch:" << config_.BatchTransactionNum()
             << " received max seq:" << max_received_seq_;
  if (seq + config_.BatchTransactionNum() + next_want > max_received_seq_ + 1) {
    std::unique_lock<std::mutex> lk(mutex_);
    cv_.wait_for(lk, std::chrono::seconds(1), [&] {
      return seq + config_.BatchTransactionNum() <= max_received_seq_ + 1;
    });

    return nullptr;
  }
  while (seq > next_consume_) {
    *queue_.Pop();
    next_consume_++;
  }
  if (seq != next_consume_) {
    LOG(ERROR) << "next should consume:" << next_consume_;
    return nullptr;
  }

  std::unique_ptr<BatchClientTransactions> batch_transactions =
      std::make_unique<BatchClientTransactions>();
  for (uint32_t i = 0; i < config_.BatchTransactionNum() + next_want; ++i) {
    *batch_transactions->add_transactions() = *queue_.Pop();
  }
  batch_transactions->set_min_seq(seq);
  batch_transactions->set_max_seq(seq + config_.BatchTransactionNum() +
                                  next_want - 1);
  next_consume_ = next_consume_ + config_.BatchTransactionNum() + next_want;
  LOG(ERROR) << "get batch:" << get_current_time();
  return batch_transactions;
}

} // namespace xxxdb
