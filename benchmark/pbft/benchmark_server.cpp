#include "application/utils/server_factory.h"
#include "benchmark/pbft/benchmark_server_impl.h"
#include "config/xxxdb_config_utils.h"
#include "ordering/pbft/consensus_service_pbft.h"
#include "proto/kv_server.pb.h"
#include "statistic/stats.h"

using xxxdb::BenchmarkServerImpl;
using xxxdb::ConsensusServicePBFT;
using xxxdb::GenerateXXXDBConfig;
using xxxdb::GenerateXXXDBServer;
using xxxdb::KVRequest;
using xxxdb::XXXDBConfig;
using xxxdb::XXXDBServer;
using xxxdb::Stats;

void ShowUsage() { printf("<config> <private_key> <cert_file>\n"); }

std::string GetRandomKey() {
  int num1 = rand() % 10;
  int num2 = rand() % 10;
  return std::to_string(num1) + std::to_string(num2);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    ShowUsage();
    exit(0);
  }

  char *config_file = argv[1];
  char *private_key_file = argv[2];
  char *cert_file = argv[3];

  std::unique_ptr<XXXDBConfig> config =
      GenerateXXXDBConfig(config_file, private_key_file, cert_file);

  Stats::GetGlobalStats(/*int sleep_seconds = */ 5);

  auto performance_consens = std::make_unique<ConsensusServicePBFT>(
      *config, std::make_unique<BenchmarkServerImpl>());
  performance_consens->SetupPerformanceDataFunc([]() {
    KVRequest request;
    request.set_cmd(KVRequest::SET);
    request.set_key(GetRandomKey());
    request.set_value("helloworld");
    std::string request_data;
    request.SerializeToString(&request_data);
    return request_data;
  });

  // auto server = std::make_unique<XXXDBServer>(
  //    *config,
  //    std::make_unique<ConsensusServicePBFT>(*config, std::move(executor)));

  auto server =
      std::make_unique<XXXDBServer>(*config, std::move(performance_consens));
  // auto server = GenerateXXXDBServer(
  //    config_file, private_key_file, cert_file,
  //    std::make_unique<BenchmarkServerImpl>(), nullptr,
  //    [&](XXXDBConfig* config) { config->RunningPerformance(true); });

  server->Run();
}
