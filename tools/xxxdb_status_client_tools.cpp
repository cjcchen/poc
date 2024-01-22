#include <glog/logging.h>

#include "client/xxxdb_state_client.h"
#include "config/xxxdb_config_utils.h"

using xxxdb::GenerateReplicaInfo;
using xxxdb::ReplicaInfo;
using xxxdb::XXXDBConfig;
using xxxdb::XXXDBStateClient;

int main(int argc, char **argv) {
  if (argc < 4) {
    printf("<config path> <private key path> <cert_file>\n");
    return 0;
  }
  std::string config_file = argv[1];
  std::string private_key_file = argv[2];
  std::string cert_file = argv[3];

  ReplicaInfo self_info = GenerateReplicaInfo(0, "127.0.0.1", 88888);

  std::unique_ptr<XXXDBConfig> config =
      GenerateXXXDBConfig(config_file, private_key_file, cert_file, self_info);

  XXXDBStateClient client(*config);
  auto states = client.GetReplicaStates();
  if (!states.ok()) {
    LOG(ERROR) << "get replica state fail";
    exit(1);
  }
  for (auto &state : *states) {
    LOG(ERROR) << state.DebugString();
  }
}
