#pragma once
#include <memory>

namespace xxxdb {

struct DataInfo {
  DataInfo() : buff(nullptr), data_len(0) {}
  ~DataInfo() {
    if (buff) {
      free(buff);
      buff = nullptr;
    }
  }
  void *buff = nullptr;
  size_t data_len = 0;
};

} // namespace xxxdb
