#include <glog/logging.h>

#include "client/xxxdb_txn_client.h"
#include "config/xxxdb_config_utils.h"
#include "proto/kv_server.pb.h"

using xxxdb::BatchClientRequest;
using xxxdb::GenerateXXXDBConfig;
using xxxdb::KVRequest;
using xxxdb::Request;
using xxxdb::XXXDBConfig;
using xxxdb::XXXDBTxnClient;

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("<config path> <min_seq> <max_seq>\n");
    return 0;
  }
  std::string config_file = argv[1];
  uint64_t min_seq = 1;
  uint64_t max_seq = 1;
  if (argc >= 3) {
    min_seq = atoi(argv[2]);
  }
  if (argc >= 4) {
    max_seq = atoi(argv[3]);
  }

  XXXDBConfig config = GenerateXXXDBConfig(config_file);

  XXXDBTxnClient client(config);
  auto resp = client.GetTxn(min_seq, max_seq);
  absl::StatusOr<std::vector<std::pair<uint64_t, std::string>>> GetTxn(
      uint64_t min_seq, uint64_t max_seq);
  if (!resp.ok()) {
    LOG(ERROR) << "get replica state fail";
    exit(1);
  }
  for (auto &txn : *resp) {
    BatchClientRequest request;
    KVRequest kv_request;
    if (request.ParseFromString(txn.second)) {
      for (auto &sub_req : request.client_requests()) {
        kv_request.ParseFromString(sub_req.request().data());
        printf("data {\nseq: %lu\n%s}\n", txn.first,
               kv_request.DebugString().c_str());
      }
    }
  }
}
