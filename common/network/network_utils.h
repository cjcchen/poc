#pragma once

#include <string>

#include "proto/network_type.pb.h"

namespace xxxdb {

std::string GetDNSName(const std::string &ip, int port, NetworkType type);

std::string GetTcpUrl(const std::string &ip, int port = 0);

} // namespace xxxdb
