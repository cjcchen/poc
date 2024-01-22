#pragma once

#include <memory>

namespace xxxdb {

class TransactionExecutorImpl {
public:
  TransactionExecutorImpl() = default;
  virtual ~TransactionExecutorImpl() = default;

  virtual std::unique_ptr<std::string> ExecuteData(const std::string &request);
};
} // namespace xxxdb
