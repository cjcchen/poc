#pragma once

#include "statistic/influxdb.hpp"
#include <glog/logging.h>

namespace resdb {

class InfluxdbHandler {
public:
  InfluxdbHandler(const std::string &server_address);
  ~InfluxdbHandler();

  void IncQPS();
};

} // namespace resdb
