#pragma once

#include "ordering/poc/proto/pow.pb.h"

namespace xxxdb {

class Merkle {
public:
  static HashValue MakeHash(const BatchClientTransactions &transaction);
};

} // namespace xxxdb
