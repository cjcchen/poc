#pragma once

#include "client/xxxdb_user_client.h"

namespace xxxdb {

// BenchmarkClient to send data to the pbft cluster.
class BenchmarkClient : public XXXDBUserClient {
public:
  BenchmarkClient(const XXXDBConfig &config);

  int Set(const std::string &data);
};

} // namespace xxxdb
