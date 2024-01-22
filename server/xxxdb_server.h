#pragma once
#include <memory>

#include "common/data_comm/data_comm.h"
#include "common/network/socket.h"
#include "common/queue/lock_free_queue.h"
#include "config/xxxdb_config.h"
#include "server/async_acceptor.h"
#include "server/xxxdb_service.h"
#include "statistic/stats.h"

namespace xxxdb {

struct QueueItem {
  std::unique_ptr<Socket> socket;
  std::unique_ptr<DataInfo> data;
};
// XXXDBServer is a service running in BFT environment.
// It receives messages from other servers or clients and delivers them to
// XXXDBService to process.
// service will be running in a multi-thread module.
class XXXDBServer {
public:
  // While running XXXDBServer, it will lisenten to ip:port.
  XXXDBServer(const XXXDBConfig &config, std::unique_ptr<XXXDBService> service);
  virtual ~XXXDBServer();

  // Run XXXDBServer as background.
  void Run();
  void Stop();
  // Whether the service is ready to process the request.
  bool ServiceIsReady() const;

private:
  void Process();
  void Process(std::unique_ptr<QueueItem> client_socket);
  bool IsRunning();
  void InputProcess();
  void AcceptorHandler(const char *buffer, size_t data_len);

private:
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<XXXDBService> service_;
  bool is_running = false;
  LockFreeQueue<QueueItem> input_queue_, resp_queue_;
  std::unique_ptr<AsyncAcceptor> async_acceptor_;
  XXXDBConfig config_;
  Stats *global_stats_;
};

} // namespace xxxdb
