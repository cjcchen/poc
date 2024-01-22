#pragma once

#include <map>
#include <unordered_map>

#include "proto/xxxdb.pb.h"

namespace xxxdb {

class TxnMemoryDB {
public:
  TxnMemoryDB();
  Request *Get(uint64_t seq);
  void Put(std::unique_ptr<Request> request);
  uint64_t GetMaxSeq();

private:
  std::mutex mutex_;
  std::unordered_map<uint64_t, std::unique_ptr<Request>> data_;
  std::atomic<uint64_t> max_seq_;
};

} // namespace xxxdb
