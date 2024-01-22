#include "execution/transaction_executor_impl.h"

namespace xxxdb {

std::unique_ptr<std::string>
TransactionExecutorImpl::ExecuteData(const std::string &request) {
  return std::make_unique<std::string>();
}

} // namespace xxxdb
