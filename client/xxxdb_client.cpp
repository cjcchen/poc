#include "client/xxxdb_client.h"

#include <glog/logging.h>

#include "common/data_comm/data_comm.h"
#include "common/network/tcp_socket.h"

namespace xxxdb {

XXXDBClient::XXXDBClient(const std::string &ip, int port)
    : ip_(ip), port_(port) {
  socket_ = std::make_unique<TcpSocket>();
  socket_->SetSendTimeout(300);
  socket_->SetRecvTimeout(read_timeouts_);
}

void XXXDBClient::SetRecvTimeout(int microseconds) {
  read_timeouts_ = microseconds;
  socket_->SetRecvTimeout(read_timeouts_);
}

void XXXDBClient::SetAsyncSend(bool is_async_send) {
  is_async_send_ = is_async_send;
}

XXXDBClient::XXXDBClient(std::unique_ptr<Socket> socket, bool connected) {
  SetSocket(std::move(socket));
  connected_ = connected;
}

void XXXDBClient::Close() { socket_->Close(); }

void XXXDBClient::SetSignatureVerifier(SignatureVerifier *verifier) {
  verifier_ = verifier;
}

void XXXDBClient::SetSocket(std::unique_ptr<Socket> socket) {
  socket_ = std::move(socket);
}

void XXXDBClient::SetDestReplicaInfo(const ReplicaInfo &replica) {
  ip_ = replica.ip();
  port_ = replica.port();
}

void XXXDBClient::IsLongConnection(bool long_connect_tion) {
  long_connect_tion_ = long_connect_tion;
}

int XXXDBClient::Connect() {
  socket_->ReInit();
  socket_->SetAsync(is_async_send_);
  return socket_->Connect(ip_, port_);
}

int XXXDBClient::SendDataInternal(const std::string &data) {
  return socket_->Send(data);
}

// Connect to the server if not connected and send data.
int XXXDBClient::SendFromKeepAlive(const std::string &data) {
  for (int i = 0; i < max_retry_time_; ++i) {
    if (!long_connecting_) {
      if (Connect() == 0) {
        long_connecting_ = true;
      }
      if (!long_connecting_) {
        LOG(ERROR) << "connect fail:" << ip_ << " port:" << port_;
        continue;
      }
    }
    int ret = SendDataInternal(data);
    if (ret >= 0) {
      return ret;
    }
    long_connecting_ = false;
  }
  return -1;
}

int XXXDBClient::Send(const std::string &data) {
  if (long_connect_tion_) {
    return SendFromKeepAlive(data);
  }
  for (int i = 0; i < max_retry_time_; ++i) {
    if (!connected_) {
      if (Connect()) {
        LOG(ERROR) << "connect fail:" << ip_ << " port:" << port_;
        continue;
      }
    }
    int ret = SendDataInternal(data);
    if (ret >= 0) {
      return ret;
    } else if (ret == -1) {
      LOG(ERROR) << "send data fail:" << ip_ << " port:" << port_;
    }
  }
  return -1;
}

// Receive data from the server.
int XXXDBClient::Recv(std::string *data) {
  std::unique_ptr<DataInfo> resp = std::make_unique<DataInfo>();
  int ret = socket_->Recv(&resp->buff, &resp->data_len);
  if (ret > 0) {
    *data = std::string((char *)resp->buff, resp->data_len);
  }
  return ret;
}

// Sign the message if verifier has been provied and send the message to the
// server.
std::string
XXXDBClient::GetRawMessageString(const google::protobuf::Message &message,
                                 SignatureVerifier *verifier) {
  XXXDBMessage sig_message;
  if (!message.SerializeToString(sig_message.mutable_data())) {
    return "";
  }

  if (verifier != nullptr) {
    auto signature_or = verifier->SignMessage(sig_message.data());
    if (!signature_or.ok()) {
      LOG(ERROR) << "Sign message fail";
      return "";
    }
    sig_message.mutable_signature()->Swap(&(*signature_or));
  } else {
    LOG(ERROR) << " no verifier";
  }

  std::string message_str;
  if (!sig_message.SerializeToString(&message_str)) {
    return "";
  }
  return message_str;
}

int XXXDBClient::SendRawMessageData(const std::string &message_str) {
  return Send(message_str);
}

int XXXDBClient::RecvRawMessageData(std::string *message_str) {
  return Recv(message_str);
}

int XXXDBClient::SendRawMessage(const google::protobuf::Message &message) {
  XXXDBMessage sig_message;
  if (!message.SerializeToString(sig_message.mutable_data())) {
    return -1;
  }

  if (verifier_ != nullptr) {
    auto signature_or = verifier_->SignMessage(sig_message.data());
    if (!signature_or.ok()) {
      LOG(ERROR) << "Sign message fail";
      return -1;
    }
    sig_message.mutable_signature()->Swap(&(*signature_or));
  }
  std::string message_str;
  if (!sig_message.SerializeToString(&message_str)) {
    return -1;
  }
  return Send(message_str);
}

int XXXDBClient::RecvRawMessageStr(std::string *message) {
  std::string recv_str;
  int ret = Recv(&recv_str);
  if (ret <= 0) {
    return ret;
  }
  XXXDBMessage sig_message;
  if (!sig_message.ParseFromString(recv_str)) {
    LOG(ERROR) << "parse to sig fail";
    return -1;
  }
  *message = sig_message.data();
  return 0;
}

int XXXDBClient::RecvRawMessage(google::protobuf::Message *message) {
  std::string resp_data;
  int ret = Recv(&resp_data);
  if (ret <= 0) {
    return -1;
  }
  XXXDBMessage sig_message;
  if (!sig_message.ParseFromString(resp_data)) {
    LOG(ERROR) << "parse sig msg fail";
    return -1;
  }
  if (!message->ParseFromString(sig_message.data())) {
    LOG(ERROR) << "parse response msg fail";
    return -1;
  }
  return 0;
}

int XXXDBClient::SendRequest(const google::protobuf::Message &message,
                             Request::Type type, bool need_response) {
  Request request;
  request.set_type(type);
  request.set_need_response(need_response);
  if (!message.SerializeToString(request.mutable_data())) {
    return -1;
  }
  return SendRawMessage(request);
}

} // namespace xxxdb
