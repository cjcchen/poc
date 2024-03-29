#include <glog/logging.h>

#include "client/xxxdb_txn_client.h"
#include "config/xxxdb_config_utils.h"

using xxxdb::GenerateReplicaInfo;
using xxxdb::ReplicaInfo;
using xxxdb::XXXDBConfig;
using xxxdb::XXXDBTxnClient;

int main(int argc, char **argv) {
  if (argc < 6) {
    printf(
        "<config path> <private key path> <cert_file> <min_seq> <max_seq>\n");
    return 0;
  }
  std::string config_file = argv[1];
  std::string private_key_file = argv[2];
  std::string cert_file = argv[3];
  uint64_t min_seq = atoi(argv[4]);
  uint64_t max_seq = atoi(argv[5]);

  ReplicaInfo self_info = GenerateReplicaInfo(0, "127.0.0.1", 88888);

  std::unique_ptr<XXXDBConfig> config =
      GenerateXXXDBConfig(config_file, private_key_file, cert_file, self_info);

  XXXDBTxnClient client(*config);
  auto resp = client.GetTxn(min_seq, max_seq);
  absl::StatusOr<std::vector<std::pair<uint64_t, std::string>>> GetTxn(
      uint64_t min_seq, uint64_t max_seq);
  if (!resp.ok()) {
    LOG(ERROR) << "get replica state fail";
    exit(1);
  }
  for (auto &txn : *resp) {
    LOG(ERROR) << "seq:" << txn.first << " txn:" << txn.second;
  }
}
