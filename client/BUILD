package(default_visibility = ["//visibility:public"])

cc_library(
    name = "xxxdb_client",
    srcs = ["xxxdb_client.cpp"],
    hdrs = ["xxxdb_client.h"],
    deps = [
        "//common:comm",
        "//common/data_comm",
        "//common/network:tcp_socket",
        "//config:xxxdb_config",
        "//crypto:signature_verifier",
        "//proto:xxxdb_cc_proto",
    ],
)

cc_library(
    name = "xxxdb_user_client",
    srcs = ["xxxdb_user_client.cpp"],
    hdrs = ["xxxdb_user_client.h"],
    deps = [
        ":xxxdb_client",
        "//common/data_comm",
    ],
)

cc_library(
    name = "xxxdb_state_client",
    srcs = ["xxxdb_state_client.cpp"],
    hdrs = ["xxxdb_state_client.h"],
    deps = [
        ":xxxdb_user_client",
        "//common:comm",
        "//proto:replica_info_cc_proto",
        "//proto:xxxdb_cc_proto",
    ],
)

cc_library(
    name = "xxxdb_txn_client",
    srcs = ["xxxdb_txn_client.cpp"],
    hdrs = ["xxxdb_txn_client.h"],
    deps = [
        ":xxxdb_client",
        "//common:comm",
        "//proto:replica_info_cc_proto",
        "//proto:xxxdb_cc_proto",
    ],
)

