#include "config/xxxdb_config_utils.h"
#include "ordering/pbft/consensus_service_pbft.h"
#include "server/xxxdb_server.h"

using xxxdb::ConsensusServicePBFT;
using xxxdb::GenerateXXXDBConfig;
using xxxdb::XXXDBConfig;
using xxxdb::XXXDBServer;

void ShowUsage() { printf("<config> <private_key> <cert_file>\n"); }

int main(int argc, char **argv) {
  if (argc < 3) {
    ShowUsage();
    exit(0);
  }

  char *config_file = argv[1];
  char *private_key_file = argv[2];
  char *cert_file = argv[3];

  std::unique_ptr<XXXDBConfig> config =
      GenerateXXXDBConfig(config_file, private_key_file, cert_file);

  XXXDBServer server(*config,
                     std::make_unique<ConsensusServicePBFT>(*config, nullptr));
  server.Run();
}
