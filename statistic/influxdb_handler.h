#pragma once

#include "statistic/influxdb.hpp"
#include <glog/logging.h>

namespace xxxdb {

class InfluxdbHandler {
public:
  InfluxdbHandler(const std::string &server_address);
  ~InfluxdbHandler();

  void IncQPS();
};

} // namespace xxxdb
