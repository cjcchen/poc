#pragma once

#include "common/queue/blocking_queue.h"
#include "config/xxxdb_poc_config.h"
#include "ordering/poc/pow/block_manager.h"
#include "ordering/poc/pow/miner_manager.h"
#include "ordering/poc/pow/shift_manager.h"
#include "ordering/poc/pow/transaction_accessor.h"
#include "ordering/poc/proto/pow.pb.h"
#include "server/xxxdb_replica_client.h"

namespace xxxdb {

class PoWManager {
public:
  PoWManager(const XXXDBPoCConfig &config, XXXDBReplicaClient *bc_client);
  virtual ~PoWManager();

  void Start();
  void Stop();
  bool IsRunning();
  void Reset();
  void Commit(std::unique_ptr<Block> block);
  void AddShiftMsg(const SliceInfo &slice_info);

  enum MiningStatus {
    OK = 0,
    TIMEOUT = 1,
    FAIL = 2,
  };

  enum BlockStatus {
    GENERATE_NEW = 0,
    NEXT_NEWBLOCK = 1,
  };

  enum MiningType {
    NEWBLOCK = 0,
    SHIFTING = 1,
  };

protected:
  virtual std::unique_ptr<TransactionAccessor>
  GetTransactionAccessor(const XXXDBPoCConfig &config);
  virtual std::unique_ptr<ShiftManager>
  GetShiftManager(const XXXDBPoCConfig &config);
  virtual std::unique_ptr<BlockManager>
  GetBlockManager(const XXXDBPoCConfig &config);

  virtual MiningStatus Wait();
  virtual void NotifyBroadCast();
  virtual int GetShiftMsg(const SliceInfo &slice_info);

  int GetMiningTxn(MiningType type);
  void NotifyNextBlock();
  absl::Status WaitBlockDone();
  BlockStatus GetBlockStatus();

  void SendShiftMsg();
  void MiningProcess();
  int BroadCastNewBlock(const Block &block);
  int BroadCastShiftMsg(const SliceInfo &slice_info);
  void GossipProcess();

private:
  XXXDBPoCConfig config_;
  std::unique_ptr<BlockManager> block_manager_;
  std::unique_ptr<ShiftManager> shift_manager_;
  std::unique_ptr<TransactionAccessor> transaction_accessor_;
  std::thread miner_thread_, gossip_thread_;
  std::atomic<bool> is_stop_;

  std::mutex broad_cast_mtx_, mutex_, tx_mutex_;
  std::condition_variable broad_cast_cv_, cv_;
  std::atomic<BlockStatus> current_status_ = BlockStatus::GENERATE_NEW;
  XXXDBReplicaClient *bc_client_;
  SliceInfo need_slice_info_;
  PrometheusHandler *prometheus_handler_;
};

} // namespace xxxdb
