#pragma once

#include "config/xxxdb_poc_config.h"
#include "ordering/poc/pow/miner_manager.h"
#include "ordering/poc/pow/pow_manager.h"
#include "server/consensus_service.h"

namespace xxxdb {

class ConsensusServicePoW : public ConsensusService {
public:
  ConsensusServicePoW(const XXXDBPoCConfig &config);
  virtual ~ConsensusServicePoW();

  // Start the service.
  void Start() override;

  int ConsensusCommit(std::unique_ptr<Context> context,
                      std::unique_ptr<Request> request) override;

  std::vector<ReplicaInfo> GetReplicas() override;

protected:
  std::unique_ptr<PoWManager> pow_manager_;
  std::unique_ptr<MinerManager> miner_manager_;
};

} // namespace xxxdb
