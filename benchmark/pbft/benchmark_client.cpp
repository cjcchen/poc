#include "benchmark/pbft/benchmark_client.h"

#include <glog/logging.h>

#include "benchmark/pbft/proto/benchmark.pb.h"

namespace xxxdb {

BenchmarkClient::BenchmarkClient(const XXXDBConfig &config)
    : XXXDBUserClient(config) {}

int BenchmarkClient::Set(const std::string &data) {
  BenchmarkMessage request;
  request.set_data(data);
  return SendRequest(request);
  //  BenchmarkResponse response;
  //  return SendRequest(request, &response);
}

} // namespace xxxdb
