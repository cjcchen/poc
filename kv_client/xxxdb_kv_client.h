#pragma once

#include "client/xxxdb_user_client.h"

namespace xxxdb {

// XXXDBKVClient to send data to the kv server.
class XXXDBKVClient : public XXXDBUserClient {
public:
  XXXDBKVClient(const XXXDBConfig &config);

  int Set(const std::string &key, const std::string &data);
  std::unique_ptr<std::string> Get(const std::string &key);
};

} // namespace xxxdb
