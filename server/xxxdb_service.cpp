#include "server/xxxdb_service.h"

#include <glog/logging.h>

namespace xxxdb {

int XXXDBService::Process(std::unique_ptr<Context> context,
                          std::unique_ptr<DataInfo> request_info) {
  return 0;
}

bool XXXDBService::IsRunning() const { return is_running_; }

void XXXDBService::SetRunning(bool is_running) { is_running_ = is_running; }

void XXXDBService::Start() { SetRunning(true); }
void XXXDBService::Stop() { SetRunning(false); }

} // namespace xxxdb
