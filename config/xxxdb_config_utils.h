#pragma once

#include "config/xxxdb_config.h"

namespace xxxdb {

std::vector<ReplicaInfo> ReadConfig(const std::string &file_name);
ReplicaInfo GenerateReplicaInfo(int id, const std::string &ip, int port);

typedef std::function<std::unique_ptr<XXXDBConfig>(
    const std::vector<ReplicaInfo> &replicas, const ReplicaInfo &self_info,
    const KeyInfo &private_key, const CertificateInfo &public_key_cert_info)>
    ConfigGenFunc;

std::unique_ptr<XXXDBConfig>
GenerateXXXDBConfig(const std::string &config_file,
                    const std::string &private_key_file,
                    const std::string &cert_file,
                    std::optional<ReplicaInfo> self_info = std::nullopt,
                    std::optional<ConfigGenFunc> = std::nullopt);

XXXDBConfig GenerateXXXDBConfig(const std::string &config_file);
} // namespace xxxdb
