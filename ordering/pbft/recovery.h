#pragma once

#include "config/xxxdb_config.h"
#include "crypto/signature_verifier.h"
#include "ordering/pbft/transaction_manager.h"
#include "server/xxxdb_replica_client.h"

namespace xxxdb {

class Recovery {
public:
  Recovery(const XXXDBConfig &config, TransactionManager *transaction_manager,
           XXXDBReplicaClient *replica_client, SignatureVerifier *verifier);
  virtual ~Recovery();

  virtual int ProcessRecoveryDataResp(std::unique_ptr<Context> context,
                                      std::unique_ptr<Request> request);

  virtual int ProcessRecoveryData(std::unique_ptr<Context> context,
                                  std::unique_ptr<Request> request);

  virtual void AddNewReplica(const ReplicaInfo &info);

private:
  void HealthCheck();
  bool IsPrimary();

protected:
  XXXDBConfig config_;
  TransactionManager *transaction_manager_;
  std::thread healthy_thread_;
  XXXDBReplicaClient *replica_client_;
  SignatureVerifier *verifier_;
  std::atomic<bool> stop_;
};

} // namespace xxxdb
