#pragma once

#include <atomic>
#include <memory>

#include "common/data_comm/data_comm.h"
#include "server/server_comm.h"

namespace xxxdb {

class XXXDBService {
public:
  XXXDBService() : is_running_(false) {}
  virtual ~XXXDBService() = default;

  virtual int Process(std::unique_ptr<Context> context,
                      std::unique_ptr<DataInfo> request_info);
  virtual bool IsRunning() const;
  virtual bool IsReady() const { return false; }
  virtual void SetRunning(bool is_running);
  virtual void Start();
  void Stop();

private:
  std::atomic<bool> is_running_;
};

} // namespace xxxdb
