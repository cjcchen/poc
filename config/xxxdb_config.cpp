#include "config/xxxdb_config.h"

namespace xxxdb {

XXXDBConfig::XXXDBConfig(const std::vector<ReplicaInfo> &replicas,
                         const ReplicaInfo &self_info)
    : replicas_(replicas), self_info_(self_info) {}

XXXDBConfig::XXXDBConfig(const std::vector<ReplicaInfo> &replicas,
                         const ReplicaInfo &self_info,
                         const KeyInfo &private_key,
                         const CertificateInfo &public_key_cert_info)
    : replicas_(replicas), self_info_(self_info), private_key_(private_key),
      public_key_cert_info_(public_key_cert_info) {}

KeyInfo XXXDBConfig::GetPrivateKey() const { return private_key_; }

CertificateInfo XXXDBConfig::GetPublicKeyCertificateInfo() const {
  return public_key_cert_info_;
}

const std::vector<ReplicaInfo> &XXXDBConfig::GetReplicaInfos() const {
  return replicas_;
}

const ReplicaInfo &XXXDBConfig::GetSelfInfo() const { return self_info_; }

size_t XXXDBConfig::GetReplicaNum() const { return replicas_.size(); }

int XXXDBConfig::GetMinDataReceiveNum() const {
  int f = (replicas_.size() - 1) / 3;
  return std::max(2 * f + 1, 1);
}

int XXXDBConfig::GetMinClientReceiveNum() const {
  int f = (replicas_.size() - 1) / 3;
  return std::max(f + 1, 1);
}

size_t XXXDBConfig::GetMaxMaliciousReplicaNum() const {
  int f = (replicas_.size() - 1) / 3;
  return std::max(f, 0);
}

void XXXDBConfig::SetClientTimeoutMs(int timeout_ms) {
  client_timeout_ms_ = timeout_ms;
}

int XXXDBConfig::GetClientTimeoutMs() const { return client_timeout_ms_; }

// Logging
std::string XXXDBConfig::GetCheckPointLoggingPath() const {
  return checkpoint_logging_path_;
}

void XXXDBConfig::SetCheckPointLoggingPath(const std::string &path) {
  checkpoint_logging_path_ = path;
}

int XXXDBConfig::GetCheckPointWaterMark() const {
  return checkpoint_water_mark_;
}

void XXXDBConfig::SetCheckPointWaterMark(int water_mark) {
  checkpoint_water_mark_ = water_mark;
}

void XXXDBConfig::EnableCheckPoint(bool is_enable) {
  is_enable_checkpoint_ = is_enable;
}

bool XXXDBConfig::IsCheckPointEnabled() { return is_enable_checkpoint_; }

bool XXXDBConfig::HeartBeatEnabled() { return hb_enabled_; }

void XXXDBConfig::SetHeartBeatEnabled(bool enable_heartbeat) {
  hb_enabled_ = enable_heartbeat;
}

bool XXXDBConfig::SignatureVerifierEnabled() {
  return signature_verifier_enabled_;
}

void XXXDBConfig::SetSignatureVerifierEnabled(bool enable_sv) {
  signature_verifier_enabled_ = enable_sv;
}

// Performance setting
bool XXXDBConfig::IsPerformanceRunning() { return is_performance_running_; }

void XXXDBConfig::RunningPerformance(bool is_performance_running) {
  is_performance_running_ = is_performance_running;
}

void XXXDBConfig::SetTestMode(bool is_test_mode) {
  is_test_mode_ = is_test_mode;
}

bool XXXDBConfig::IsTestMode() const { return is_test_mode_; }

uint32_t XXXDBConfig::GetMaxProcessTxn() const { return max_process_txn_; }

void XXXDBConfig::SetMaxProcessTxn(uint32_t num) { max_process_txn_ = num; }

uint32_t XXXDBConfig::ClientBatchWaitTimeMS() const {
  return client_batch_wait_time_ms_;
}

void XXXDBConfig::SetClientBatchWaitTimeMS(uint32_t wait_time_ms) {
  client_batch_wait_time_ms_ = wait_time_ms;
}

uint32_t XXXDBConfig::ClientBatchNum() const { return client_batch_num_; }

void XXXDBConfig::SetClientBatchNum(uint32_t num) { client_batch_num_ = num; }

uint32_t XXXDBConfig::GetWorkerNum() const { return worker_num_; }

uint32_t XXXDBConfig::GetInputWorkerNum() const { return input_worker_num_; }

uint32_t XXXDBConfig::GetOutputWorkerNum() const { return output_worker_num_; }

} // namespace xxxdb
