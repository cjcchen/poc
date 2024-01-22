#include "client/xxxdb_state_client.h"

#include <glog/logging.h>

#include <future>
#include <thread>

namespace xxxdb {

XXXDBStateClient::XXXDBStateClient(const XXXDBConfig &config)
    : config_(config) {}

std::unique_ptr<XXXDBClient>
XXXDBStateClient::GetXXXDBClient(const std::string &ip, int port) {
  return std::make_unique<XXXDBClient>(ip, port);
}

// Obtain ReplicaState of each replica.
absl::StatusOr<std::vector<ReplicaState>> XXXDBStateClient::GetReplicaStates() {
  std::vector<std::thread> ths;
  Request request;
  std::vector<std::future<std::unique_ptr<ReplicaState>>> resp_future;
  std::vector<ReplicaState> resp;

  for (const auto &replica : config_.GetReplicaInfos()) {
    std::promise<std::unique_ptr<ReplicaState>> state_prom;
    resp_future.push_back(state_prom.get_future());
    ths.push_back(std::thread(
        [&](std::promise<std::unique_ptr<ReplicaState>> state_prom) {
          std::unique_ptr<XXXDBClient> client =
              GetXXXDBClient(replica.ip(), replica.port());
          client->SetRecvTimeout(1000000); // 1s for recv timeout.

          int ret = client->SendRequest(request, Request::TYPE_REPLICA_STATE);
          if (ret) {
            state_prom.set_value(nullptr);
            return;
          }

          std::unique_ptr<ReplicaState> state =
              std::make_unique<ReplicaState>();
          ret = client->RecvRawMessage(state.get());
          if (ret < 0) {
            state_prom.set_value(nullptr);
            return;
          }
          state_prom.set_value(std::move(state));
          return;
        },
        std::move(state_prom)));
  }

  for (auto &future : resp_future) {
    auto state = future.get();
    if (state == nullptr) {
      continue;
    }
    resp.push_back(*state);
  }

  for (auto &th : ths) {
    th.join();
  }
  return resp;
}

} // namespace xxxdb
