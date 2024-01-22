#include "application/poc/pbft_client.h"

#include "ordering/poc/proto/transaction.pb.h"

namespace xxxdb {

PBFTClient::PBFTClient(const XXXDBConfig &config) : XXXDBUserClient(config) {}

int PBFTClient::Set(const std::string &data) {
  Transaction txn;
  txn.set_data(data);
  return SendRequest(txn);
}

} // namespace xxxdb
