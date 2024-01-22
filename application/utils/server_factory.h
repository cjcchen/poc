#pragma once

#include "execution/transaction_executor_impl.h"
#include "server/xxxdb_server.h"

namespace xxxdb {

class ServerFactory {
public:
  std::unique_ptr<XXXDBServer>
  CreateXXXDBServer(char *config_file, char *private_key_file, char *cert_file,
                    std::unique_ptr<TransactionExecutorImpl> executor,
                    char *logging_dir,
                    std::function<void(XXXDBConfig *config)> config_handler);
};

std::unique_ptr<XXXDBServer> GenerateXXXDBServer(
    char *config_file, char *private_key_file, char *cert_file,
    std::unique_ptr<TransactionExecutorImpl> executor,
    char *logging_dir = nullptr,
    std::function<void(XXXDBConfig *config)> config_handler = nullptr);
} // namespace xxxdb
