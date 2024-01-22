#pragma once

#include "client/xxxdb_user_client.h"

namespace xxxdb {

// PBFTClient to send data to the pbft cluster.
class PBFTClient : public XXXDBUserClient {
public:
  PBFTClient(const XXXDBConfig &config);

  int Set(const std::string &data);
};

} // namespace xxxdb
