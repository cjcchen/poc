package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "pow_server",
    srcs = ["pow_server.cpp"],
    deps = [
        "//config:xxxdb_config_utils",
        "//ordering/poc/pow:consensus_service_pow",
        "//server:xxxdb_server",
        "//statistic:stats",
    ],
)

cc_binary(
    name = "pbft_server",
    srcs = ["pbft_server.cpp"],
    deps = [
        "//config:xxxdb_config_utils",
        "//ordering/pbft:consensus_service_pbft",
        "//server:xxxdb_server",
    ],
)

cc_library(
    name = "pbft_client",
    srcs = ["pbft_client.cpp"],
    hdrs = ["pbft_client.h"],
    deps = [
        "//client:xxxdb_user_client",
        "//ordering/poc/proto:transaction_cc_proto",
    ],
)

cc_binary(
    name = "pbft_client_main",
    srcs = ["pbft_client_main.cpp"],
    deps = [
        ":pbft_client",
        "//config:xxxdb_config_utils",
    ],
)
