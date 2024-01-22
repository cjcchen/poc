#pragma once

#include <thread>

#include "client/xxxdb_txn_client.h"
#include "common/queue/lock_free_queue.h"
#include "config/xxxdb_poc_config.h"
#include "ordering/poc/proto/pow.pb.h"
#include "statistic/stats.h"

namespace xxxdb {

// TransactionAccessor obtains the transaction from BFT cluster.
// It broadcasts the request to all the replicas in BFT cluster
// and waits for 2f+1 same response, but only return one to the
// caller.
class TransactionAccessor {
public:
  // For test, it is started by the tester.
  TransactionAccessor(const XXXDBPoCConfig &config, bool auto_start = true);
  virtual ~TransactionAccessor();

  // consume the transaction between [seq, seq+batch_num-1]
  virtual std::unique_ptr<BatchClientTransactions>
  ConsumeTransactions(uint64_t seq);

  // For test.
  void Start();

protected:
  void TransactionFetching();
  virtual std::unique_ptr<XXXDBTxnClient> GetXXXDBTxnClient();

private:
  XXXDBPoCConfig config_;
  std::atomic<bool> stop_;
  std::thread fetching_thread_;
  std::atomic<uint64_t> max_received_seq_;
  LockFreeQueue<ClientTransactions> queue_;
  uint64_t next_consume_ = 0;

  std::condition_variable cv_;
  std::mutex mutex_;
  uint64_t last_time_ = 0;

  PrometheusHandler *prometheus_handler_;
};

} // namespace xxxdb
