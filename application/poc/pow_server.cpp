#include "config/xxxdb_config_utils.h"
#include "ordering/poc/pow/consensus_service_pow.h"
#include "server/xxxdb_server.h"
#include "statistic/stats.h"

using xxxdb::CertificateInfo;
using xxxdb::ConsensusServicePoW;
using xxxdb::GenerateReplicaInfo;
using xxxdb::GenerateXXXDBConfig;
using xxxdb::KeyInfo;
using xxxdb::ReadConfig;
using xxxdb::ReplicaInfo;
using xxxdb::XXXDBConfig;
using xxxdb::XXXDBPoCConfig;
using xxxdb::XXXDBServer;
using xxxdb::Stats;

void ShowUsage() { printf("<bft config> <pow config>\n"); }

int main(int argc, char **argv) {
  if (argc < 5) {
    ShowUsage();
    exit(0);
  }

  std::string bft_config_file = argv[1];
  std::string pow_config_file = argv[2];
  std::string private_key_file = argv[3];
  std::string cert_file = argv[4];
  LOG(ERROR) << "pow_config:" << pow_config_file;

  XXXDBConfig bft_config = GenerateXXXDBConfig(bft_config_file);

  std::unique_ptr<XXXDBConfig> pow_config = GenerateXXXDBConfig(
      pow_config_file, private_key_file, cert_file, std::nullopt,
      [&](const std::vector<ReplicaInfo> &replicas,
          const ReplicaInfo &self_info, const KeyInfo &private_key,
          const CertificateInfo &public_key_cert_info) {
        return std::make_unique<XXXDBPoCConfig>(
            bft_config, replicas, self_info, private_key, public_key_cert_info);
      });

  LOG(ERROR) << "elf ip:" << pow_config->GetSelfInfo().ip();
  Stats::InitGlobalPrometheus("0.0.0.0:8091");
  XXXDBPoCConfig *pow_config_ptr =
      static_cast<XXXDBPoCConfig *>(pow_config.get());

  pow_config_ptr->SetMaxNonceBit(42);
  pow_config_ptr->SetDifficulty(28);

  XXXDBServer server(*pow_config_ptr,
                     std::make_unique<ConsensusServicePoW>(*pow_config_ptr));
  server.Run();
}
