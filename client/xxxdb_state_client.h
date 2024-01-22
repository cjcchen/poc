#pragma once

#include "absl/status/statusor.h"
#include "client/xxxdb_client.h"
#include "config/xxxdb_config.h"
#include "proto/replica_info.pb.h"

namespace xxxdb {

// XXXDBStateClient used to obtain the server state of each replica in XXXDB.
// The addresses of each replica are provided from the config.
class XXXDBStateClient {
public:
  XXXDBStateClient(const XXXDBConfig &config);
  virtual ~XXXDBStateClient() = default;

  // Obtain ReplicaState of each replica.
  absl::StatusOr<std::vector<ReplicaState>> GetReplicaStates();

protected:
  virtual std::unique_ptr<XXXDBClient> GetXXXDBClient(const std::string &ip,
                                                      int port);

private:
  XXXDBConfig config_;
};

} // namespace xxxdb
