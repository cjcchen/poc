#pragma once
#include "proto/xxxdb.pb.h"

namespace xxxdb {

enum CollectorResultCode {
  INVALID = -2,
  OK = 0,
  STATE_CHANGED = 1,
};

std::unique_ptr<Request> NewRequest(Request::Type type, const Request &request,
                                    int sender_id);

} // namespace xxxdb
