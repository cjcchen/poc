#pragma once

#include "client/xxxdb_client.h"
#include "proto/xxxdb.pb.h"

namespace xxxdb {

struct Context {
  std::unique_ptr<XXXDBClient> client;
  SignatureInfo signature;
};

} // namespace xxxdb
