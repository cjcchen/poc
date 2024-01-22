#pragma once

#include "proto/signature_info.pb.h"

namespace xxxdb {

class KeyGenerator {
public:
  static SecretKey GeneratorKeys(SignatureInfo::HashType type);
};

} // namespace xxxdb
