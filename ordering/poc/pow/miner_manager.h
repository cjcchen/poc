#pragma once

#include "config/xxxdb_poc_config.h"

namespace xxxdb {

class MinerManager {
public:
  MinerManager(const XXXDBPoCConfig &config);

  std::vector<ReplicaInfo> GetReplicas();

private:
  XXXDBPoCConfig config_;
  std::vector<ReplicaInfo> replicas_;
};

} // namespace xxxdb
