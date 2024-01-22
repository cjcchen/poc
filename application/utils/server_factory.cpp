#include "application/utils/server_factory.h"

#include "config/xxxdb_config_utils.h"
#include "ordering/pbft/consensus_service_pbft.h"

namespace xxxdb {

std::unique_ptr<XXXDBServer> ServerFactory::CreateXXXDBServer(
    char *config_file, char *private_key_file, char *cert_file,
    std::unique_ptr<TransactionExecutorImpl> executor, char *logging_dir,
    std::function<void(XXXDBConfig *config)> config_handler) {
  std::unique_ptr<XXXDBConfig> config =
      GenerateXXXDBConfig(config_file, private_key_file, cert_file);

  if (config_handler) {
    config_handler(config.get());
  }
  return std::make_unique<XXXDBServer>(
      *config,
      std::make_unique<ConsensusServicePBFT>(*config, std::move(executor)));
}

std::unique_ptr<XXXDBServer>
GenerateXXXDBServer(char *config_file, char *private_key_file, char *cert_file,
                    std::unique_ptr<TransactionExecutorImpl> executor,
                    char *logging_dir,
                    std::function<void(XXXDBConfig *config)> config_handler) {
  return ServerFactory().CreateXXXDBServer(config_file, private_key_file,
                                           cert_file, std::move(executor),
                                           logging_dir, config_handler);
}

} // namespace xxxdb
