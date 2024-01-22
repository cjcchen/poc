#pragma once

#include "config/xxxdb_config.h"
#include "ordering/pbft/transaction_manager.h"

namespace xxxdb {

class Query {
public:
  Query(const XXXDBConfig &config, TransactionManager *transaction_manager);
  virtual ~Query();

  virtual int ProcessGetReplicaState(std::unique_ptr<Context> context,
                                     std::unique_ptr<Request> request);
  virtual int ProcessQuery(std::unique_ptr<Context> context,
                           std::unique_ptr<Request> request);

protected:
  XXXDBConfig config_;
  TransactionManager *transaction_manager_;
};

} // namespace xxxdb
