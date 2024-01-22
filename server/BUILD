package(default_visibility = ["//visibility:public"])

cc_library(
    name = "server_comm",
    hdrs = ["server_comm.h"],
    deps = [
        "//client:xxxdb_client",
        "//proto:xxxdb_cc_proto",
    ],
)

cc_library(
    name = "xxxdb_service",
    srcs = ["xxxdb_service.cpp"],
    hdrs = ["xxxdb_service.h"],
    deps = [
        ":server_comm",
        "//common/data_comm",
        "//config:xxxdb_config",
    ],
)

cc_library(
    name = "consensus_service",
    srcs = ["consensus_service.cpp"],
    hdrs = ["consensus_service.h"],
    deps = [
        ":xxxdb_replica_client",
        ":xxxdb_service",
        "//common:comm",
        "//common/queue:blocking_queue",
        "//config:xxxdb_config",
        "//proto:broadcast_cc_proto",
        "//proto:xxxdb_cc_proto",
        "//statistic:stats",
    ],
)

cc_library(
    name = "async_acceptor",
    srcs = ["async_acceptor.cpp"],
    hdrs = ["async_acceptor.h"],
    deps = [
        "//common:asio",
        "//common:comm",
        "//config:xxxdb_config",
    ],
)

cc_library(
    name = "xxxdb_server",
    srcs = ["xxxdb_server.cpp"],
    hdrs = ["xxxdb_server.h"],
    deps = [
        ":async_acceptor",
        ":xxxdb_service",
        "//common/data_comm",
        "//common/network:tcp_socket",
        "//common/queue:lock_free_queue",
        "//proto:broadcast_cc_proto",
        "//statistic:stats",
    ],
)

cc_library(
    name = "async_replica_client",
    srcs = ["async_replica_client.cpp"],
    hdrs = ["async_replica_client.h"],
    deps = [
        "//client:xxxdb_client",
        "//common:asio",
        "//common/queue:blocking_queue",
        "//common/queue:lock_free_queue",
        "//proto:broadcast_cc_proto",
    ],
)

cc_library(
    name = "xxxdb_replica_client",
    srcs = ["xxxdb_replica_client.cpp"],
    hdrs = ["xxxdb_replica_client.h"],
    deps = [
        ":async_replica_client",
        "//client:xxxdb_client",
        "//common/queue:batch_queue",
        "//proto:broadcast_cc_proto",
        "//statistic:stats",
    ],
)

