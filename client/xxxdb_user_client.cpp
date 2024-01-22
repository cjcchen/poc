#include "client/xxxdb_user_client.h"

#include <glog/logging.h>

namespace xxxdb {

XXXDBUserClient::XXXDBUserClient(const XXXDBConfig &config)
    : XXXDBClient("", 0), config_(config),
      timeout_ms_(
          config.GetClientTimeoutMs()), // default 2s for process timeout
      is_check_signature_(true) {
  socket_->SetRecvTimeout(timeout_ms_);
}

void XXXDBUserClient::DisableSignatureCheck() { is_check_signature_ = false; }

absl::StatusOr<std::string>
XXXDBUserClient::GetResponseData(const Response &response) {
  std::string hash_;
  std::set<int64_t> hash_counter;
  std::string resp_str;
  for (const auto &each_resp : response.resp()) {
    // Check signature
    std::string hash = SignatureVerifier::CalculateHash(each_resp.data());

    if (!hash_.empty() && hash != hash_) {
      LOG(ERROR) << "hash not the same";
      return absl::InvalidArgumentError("hash not match");
    }
    if (hash_.empty()) {
      hash_ = hash;
      resp_str = each_resp.data();
    }
    hash_counter.insert(each_resp.signature().node_id());
  }
  // LOG(INFO) << "recv hash:" << hash_counter.size()
  //         << " need:" << config_.GetMinClientReceiveNum()
  //        << " data len:" << resp_str.size();
  if (hash_counter.size() >=
      static_cast<size_t>(config_.GetMinClientReceiveNum())) {
    return resp_str;
  }
  return absl::InvalidArgumentError("data not enough");
}

int XXXDBUserClient::SendRequest(const google::protobuf::Message &message,
                                 Request::Type type) {
  // Use the replica obtained from the server.
  XXXDBClient::SetDestReplicaInfo(config_.GetReplicaInfos()[0]);
  return XXXDBClient::SendRequest(message, type, false);
}

int XXXDBUserClient::SendRequest(const google::protobuf::Message &message,
                                 google::protobuf::Message *response,
                                 Request::Type type) {
  XXXDBClient::SetDestReplicaInfo(config_.GetReplicaInfos()[0]);
  int ret = XXXDBClient::SendRequest(message, type, true);
  if (ret == 0) {
    std::string resp_str;
    int ret = XXXDBClient::RecvRawMessageData(&resp_str);
    if (ret >= 0) {
      if (!response->ParseFromString(resp_str)) {
        LOG(ERROR) << "parse response fail:" << resp_str.size();
        return -2;
      }
      return 0;
    }
  }
  return -1;
}

} // namespace xxxdb
