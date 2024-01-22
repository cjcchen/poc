package(default_visibility = ["//visibility:private"])

cc_library(
    name = "lock_free_collector_pool",
    srcs = ["lock_free_collector_pool.cpp"],
    hdrs = ["lock_free_collector_pool.h"],
    deps = [
        ":transaction_collector",
    ],
)

cc_library(
    name = "response_manager",
    srcs = ["response_manager.cpp"],
    hdrs = ["response_manager.h"],
    deps = [
        ":lock_free_collector_pool",
        ":transaction_utils",
        "//server:xxxdb_replica_client",
    ],
)


cc_library(
    name = "transaction_manager",
    srcs = ["transaction_manager.cpp"],
    hdrs = ["transaction_manager.h"],
    deps = [
        ":lock_free_collector_pool",
        ":transaction_collector",
        ":transaction_utils",
        "//config:xxxdb_config",
        "//database:txn_memory_db",
        "//execution:transaction_executor_impl",
        "//ordering/status/checkpoint:check_point_info",
        "//proto:xxxdb_cc_proto",
        "//server:server_comm",
    ],
)

cc_library(
    name = "transaction_utils",
    srcs = ["transaction_utils.cpp"],
    hdrs = ["transaction_utils.h"],
    deps = [
        "//proto:xxxdb_cc_proto",
    ],
)

cc_library(
    name = "commitment",
    srcs = ["commitment.cpp"],
    hdrs = ["commitment.h"],
    deps = [
        ":response_manager",
        ":transaction_manager",
        "//common/queue:batch_queue",
        "//common/utils",
        "//config:xxxdb_config",
        "//proto:xxxdb_cc_proto",
        "//server:xxxdb_replica_client",
        "//statistic:stats",
    ],
)

cc_library(
    name = "recovery",
    srcs = ["recovery.cpp"],
    hdrs = ["recovery.h"],
    deps = [
        ":transaction_manager",
        ":transaction_utils",
        "//config:xxxdb_config",
        "//proto:xxxdb_cc_proto",
        "//server:xxxdb_replica_client",
    ],
)

cc_library(
    name = "checkpoint_collector",
    srcs = ["checkpoint_collector.cpp"],
    hdrs = ["checkpoint_collector.h"],
    deps = [
        ":transaction_collector",
        ":transaction_utils",
        "//config:xxxdb_config",
        "//ordering/status/checkpoint:check_point_info",
        "//proto:xxxdb_cc_proto",
    ],
)

cc_library(
    name = "checkpoint",
    srcs = ["checkpoint.cpp"],
    hdrs = ["checkpoint.h"],
    deps = [
        ":checkpoint_collector",
        ":transaction_manager",
        ":transaction_utils",
        "//config:xxxdb_config",
        "//proto:xxxdb_cc_proto",
        "//server:xxxdb_replica_client",
    ],
)

cc_library(
    name = "query",
    srcs = ["query.cpp"],
    hdrs = ["query.h"],
    deps = [
        ":transaction_manager",
        "//config:xxxdb_config",
        "//proto:xxxdb_cc_proto",
    ],
)

cc_library(
    name = "transaction_collector",
    srcs = ["transaction_collector.cpp"],
    hdrs = ["transaction_collector.h"],
    deps = [
        "//execution:transaction_executor",
        "//proto:xxxdb_cc_proto",
        "//server:server_comm",
        "//statistic:stats",
    ],
)

cc_library(
    name = "consensus_service_pbft",
    srcs = ["consensus_service_pbft.cpp"],
    hdrs = ["consensus_service_pbft.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":checkpoint",
        ":commitment",
        ":query",
        ":recovery",
        ":transaction_manager",
        "//crypto:signature_verifier",
        "//server:consensus_service",
    ],
)
