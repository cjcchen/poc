#pragma once

#include <thread>

#include "common/queue/blocking_queue.h"
#include "config/xxxdb_config.h"
#include "proto/replica_info.pb.h"
#include "proto/xxxdb.pb.h"
#include "server/xxxdb_replica_client.h"
#include "server/xxxdb_service.h"
#include "statistic/stats.h"

namespace xxxdb {

// ConsensusService is an consus algorithm implimentation of ConsensusService.
// It receives the messages from XXXDBServer and running a consus algorithm
// like PBFT to commit.
// It also handles the public key information exchanged from others.
class ConsensusService : public XXXDBService {
public:
  ConsensusService(const XXXDBConfig &config);
  virtual ~ConsensusService();

  // Process a request receied from XXXDBServer.
  // context contains the client socket and request_info contains the data
  // received from the network.
  virtual int Process(std::unique_ptr<Context> context,
                      std::unique_ptr<DataInfo> request_info);

  bool IsReady() const;
  void Stop();

  // Should be called by the instance or test.
  void Start();

protected:
  // BroadCast will generate signatures whiling sending data to other replicas.
  virtual void BroadCast(const Request &request);
  virtual void SendMessage(const google::protobuf::Message &message,
                           int64_t node_id);
  virtual void SendMessage(const google::protobuf::Message &message,
                           const ReplicaInfo &client_info);

  virtual int Dispatch(std::unique_ptr<Context> context,
                       std::unique_ptr<Request> request);
  virtual int ConsensusCommit(std::unique_ptr<Context> context,
                              std::unique_ptr<Request> request);

  // =============== default function ======================
  int ProcessClientCert(std::unique_ptr<Context> context,
                        std::unique_ptr<Request> request);
  int ProcessHeartBeat(std::unique_ptr<Context> context,
                       std::unique_ptr<Request> request);
  // =======================================================

  virtual std::unique_ptr<XXXDBReplicaClient>
  GetReplicaClient(const std::vector<ReplicaInfo> &replicas,
                   bool is_use_long_conn = false);

  virtual std::vector<ReplicaInfo> GetReplicas() = 0;
  virtual std::vector<ReplicaInfo> GetClientReplicas();
  virtual void AddNewReplica(const ReplicaInfo &info);
  void AddNewClient(const ReplicaInfo &info);

  XXXDBReplicaClient *GetBroadCastClient();
  // Update broad cast client to reflush the replica list.
  void UpdateBroadCastClient();

  SignatureVerifier *GetSignatureVerifier();

private:
  void HeartBeat();
  void BroadCastThread();

protected:
  XXXDBConfig config_;
  std::unique_ptr<SignatureVerifier> verifier_;
  struct QueueItem {
    std::unique_ptr<Request> request;
    std::vector<ReplicaInfo> replicas;
  };

private:
  std::thread heartbeat_thread_;
  std::atomic<bool> is_ready_ = false;
  std::unique_ptr<XXXDBReplicaClient> bc_client_;
  std::vector<ReplicaInfo> clients_;
  Stats *global_stats_;
};

} // namespace xxxdb
