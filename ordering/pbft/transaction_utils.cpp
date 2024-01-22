#include "ordering/pbft/transaction_utils.h"

namespace xxxdb {

std::unique_ptr<Request> NewRequest(Request::Type type, const Request &request,
                                    int sender_id) {
  auto new_request = std::make_unique<Request>(request);
  new_request->set_type(type);
  new_request->set_sender_id(sender_id);
  return new_request;
}

} // namespace xxxdb
