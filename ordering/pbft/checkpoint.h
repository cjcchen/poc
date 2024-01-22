#pragma once

#include "config/xxxdb_config.h"
#include "ordering/pbft/checkpoint_collector.h"
#include "ordering/status/checkpoint/check_point_info.h"
#include "server/xxxdb_replica_client.h"

namespace xxxdb {

class CheckPoint {
public:
  CheckPoint(const XXXDBConfig &config, XXXDBReplicaClient *replica_client);
  virtual ~CheckPoint();

  virtual int ProcessCheckPoint(std::unique_ptr<Context> context,
                                std::unique_ptr<Request> request);

  CheckPointInfo *GetCheckPointInfo();

private:
  void UpdateCheckPointStatus();

protected:
  XXXDBConfig config_;
  XXXDBReplicaClient *replica_client_;
  std::unique_ptr<CheckPointInfo> checkpoint_info_;
  std::unique_ptr<CheckPointCollector> checkpoint_collector_;
  std::thread checkpoint_thread_;
  std::atomic<bool> stop_;
};

} // namespace xxxdb
