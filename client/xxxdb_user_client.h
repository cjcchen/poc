#pragma once

#include "absl/status/statusor.h"
#include "client/xxxdb_client.h"
#include "config/xxxdb_config.h"

namespace xxxdb {

// XXXDBUserClient to send data to one server located in the config replicas
// and receive data from it.
// Inside XXXDBUserClient, it does two things:
// 1. Broadcast the public key obtained from the config
//    and receive the public keys of each replicas and the up-to-date replica
//    list.
// 2. Send the messages to one server with the certificate signed by the
//    private key and receive the response if needed. Each response contains
//    at least f+1 valid sub response from distinct replicas (can be verified
//    by the public keys obtained from 1).
class XXXDBUserClient : public XXXDBClient {
public:
  XXXDBUserClient(const XXXDBConfig &config);
  virtual ~XXXDBUserClient() = default;

  // Send request with a command.
  int SendRequest(const google::protobuf::Message &message,
                  Request::Type type = Request::TYPE_CLIENT_REQUEST);
  // Send request with a command and wait for a response.
  int SendRequest(const google::protobuf::Message &message,
                  google::protobuf::Message *response,
                  Request::Type type = Request::TYPE_CLIENT_REQUEST);

  // Only For test.
  void DisableSignatureCheck();

private:
  absl::StatusOr<std::string> GetResponseData(const Response &response);

private:
  XXXDBConfig config_;
  int64_t timeout_ms_; // microsecond for timeout.
  bool is_check_signature_;
};

} // namespace xxxdb
