#pragma once

#include <memory>

#include "config/xxxdb_config.h"
#include "proto/xxxdb.pb.h"

namespace xxxdb {

// SystemInfo managers the cluster information which
// has been agreed on, like the primary, the replicas,etc..
class SystemInfo {
public:
  SystemInfo(const XXXDBConfig &config);
  virtual ~SystemInfo() = default;

  std::vector<ReplicaInfo> GetReplicas() const;
  void SetReplicas(const std::vector<ReplicaInfo> &replicas);
  void AddReplica(const ReplicaInfo &replica);

  void ProcessRequest(const SystemInfoRequest &request);

  uint32_t GetPrimaryId() const;
  void SetPrimary(uint32_t id);

  uint64_t GetCurrentView() const;
  void SetCurrentView(uint64_t);

private:
  std::vector<ReplicaInfo> replicas_;
  std::atomic<uint32_t> primary_id_;
  std::atomic<uint64_t> view_;
};
} // namespace xxxdb
