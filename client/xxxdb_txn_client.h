#pragma once

#include "absl/status/statusor.h"
#include "client/xxxdb_client.h"
#include "config/xxxdb_config.h"
#include "proto/replica_info.pb.h"

namespace xxxdb {

// XXXDBTxnClient used to obtain the server state of each replica in XXXDB.
// The addresses of each replica are provided from the config.
class XXXDBTxnClient {
public:
  XXXDBTxnClient(const XXXDBConfig &config);
  virtual ~XXXDBTxnClient() = default;

  // Obtain ReplicaState of each replica.
  virtual absl::StatusOr<std::vector<std::pair<uint64_t, std::string>>>
  GetTxn(uint64_t min_seq, uint64_t max_seq);

protected:
  virtual std::unique_ptr<XXXDBClient> GetXXXDBClient(const std::string &ip,
                                                      int port);

private:
  XXXDBConfig config_;
  std::vector<ReplicaInfo> replicas_;
  int recv_timeout_ = 1;
};

} // namespace xxxdb
