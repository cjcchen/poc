#pragma once

#include "common/queue/batch_queue.h"
#include "config/xxxdb_config.h"
#include "ordering/pbft/response_manager.h"
#include "ordering/pbft/transaction_manager.h"
#include "server/xxxdb_replica_client.h"
#include "statistic/stats.h"

namespace xxxdb {

class Commitment {
public:
  Commitment(const XXXDBConfig &config, TransactionManager *transaction_manager,
             XXXDBReplicaClient *replica_client, SignatureVerifier *verifier);
  virtual ~Commitment();

  virtual int ProcessNewRequest(std::unique_ptr<Context> context,
                                std::unique_ptr<Request> client_request);

  virtual int ProcessProposeMsg(std::unique_ptr<Context> context,
                                std::unique_ptr<Request> request);
  virtual int ProcessPrepareMsg(std::unique_ptr<Context> context,
                                std::unique_ptr<Request> request);
  virtual int ProcessCommitMsg(std::unique_ptr<Context> context,
                               std::unique_ptr<Request> request);

protected:
  virtual int PostProcessExecutedMsg();

protected:
  XXXDBConfig config_;
  TransactionManager *transaction_manager_;
  std::thread executed_thread_;
  std::atomic<bool> stop_;
  XXXDBReplicaClient *replica_client_;

  SignatureVerifier *verifier_;
  Stats *global_stats_;
};

} // namespace xxxdb
