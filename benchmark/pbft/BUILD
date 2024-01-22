package(default_visibility = ["//visibility:public"])

cc_library(
    name = "benchmark_server_impl",
    srcs = ["benchmark_server_impl.cpp"],
    hdrs = ["benchmark_server_impl.h"],
    deps = [
        "//benchmark/pbft/proto:benchmark_cc_proto",
        "//common:comm",
        "//execution:transaction_executor_impl",
    ],
)

cc_binary(
    name = "benchmark_server",
    srcs = ["benchmark_server.cpp"],
    deps = [
        ":benchmark_server_impl",
        "//application/utils:server_factory",
        "//config:xxxdb_config_utils",
        "//ordering/pbft:consensus_service_pbft",
        "//proto:kv_server_cc_proto",
        "//server:xxxdb_server",
    ],
)

cc_library(
    name = "benchmark_client",
    srcs = ["benchmark_client.cpp"],
    hdrs = ["benchmark_client.h"],
    deps = [
        "//benchmark/pbft/proto:benchmark_cc_proto",
        "//client:xxxdb_user_client",
        "//config:xxxdb_config_utils",
    ],
)

cc_binary(
    name = "benchmark_client_main",
    srcs = ["benchmark_client_main.cpp"],
    deps = [
        ":benchmark_client",
        "//common/utils",
        "//config:xxxdb_config_utils",
    ],
)
