#include "kv_client/xxxdb_kv_client.h"

#include <glog/logging.h>

#include "proto/kv_server.pb.h"

namespace xxxdb {

XXXDBKVClient::XXXDBKVClient(const XXXDBConfig &config)
    : XXXDBUserClient(config) {}

int XXXDBKVClient::Set(const std::string &key, const std::string &data) {
  KVRequest request;
  request.set_cmd(KVRequest::SET);
  request.set_key(key);
  request.set_value(data);
  return SendRequest(request);
}

std::unique_ptr<std::string> XXXDBKVClient::Get(const std::string &key) {
  KVRequest request;
  request.set_cmd(KVRequest::GET);
  request.set_key(key);
  KVResponse response;
  int ret = SendRequest(request, &response);
  if (ret != 0) {
    LOG(ERROR) << "send request fail, ret:" << ret;
    return nullptr;
  }
  return std::make_unique<std::string>(response.value());
}

} // namespace xxxdb
