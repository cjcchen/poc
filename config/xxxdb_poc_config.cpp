#include "config/xxxdb_poc_config.h"

namespace xxxdb {

XXXDBPoCConfig::XXXDBPoCConfig(const XXXDBConfig &bft_config,
                               const std::vector<ReplicaInfo> &replicas,
                               const ReplicaInfo &self_info,
                               const KeyInfo &private_key,
                               const CertificateInfo &public_key_cert_info)
    : XXXDBConfig(replicas, self_info, private_key, public_key_cert_info),
      bft_config_(bft_config) {
  SetHeartBeatEnabled(false);
  SetSignatureVerifierEnabled(false);
}

const XXXDBConfig *XXXDBPoCConfig::GetBFTConfig() const { return &bft_config_; }

void XXXDBPoCConfig::SetMaxNonceBit(uint32_t bit) { max_nonce_bit_ = bit; }

uint32_t XXXDBPoCConfig::GetMaxNonceBit() const { return max_nonce_bit_; }

void XXXDBPoCConfig::SetDifficulty(uint32_t difficulty) {
  difficulty_ = difficulty;
}

uint32_t XXXDBPoCConfig::GetDifficulty() const { return difficulty_; }

uint32_t XXXDBPoCConfig::GetTargetValue() const { return target_value_; }

void XXXDBPoCConfig::SetTargetValue(uint32_t target_value) {
  target_value_ = target_value;
}

std::vector<ReplicaInfo> XXXDBPoCConfig::GetBFTReplicas() {
  return bft_replicas_;
}

void XXXDBPoCConfig::SetBFTReplicas(const std::vector<ReplicaInfo> &replicas) {
  bft_replicas_ = replicas;
}

// Batch
uint32_t XXXDBPoCConfig::BatchTransactionNum() const { return batch_num_; }

void XXXDBPoCConfig::SetBatchTransactionNum(uint32_t batch_num) {
  batch_num_ = batch_num;
}

uint32_t XXXDBPoCConfig::GetWokerNum() { return worker_num_; }

void XXXDBPoCConfig::SetWorkerNum(uint32_t worker_num) {
  worker_num_ = worker_num;
}

} // namespace xxxdb
