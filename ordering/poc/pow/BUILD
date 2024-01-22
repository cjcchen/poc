package(default_visibility = ["//visibility:public"])

cc_library(
    name = "miner_utils",
    srcs = ["miner_utils.cpp"],
    hdrs = ["miner_utils.h"],
    deps = [
        "//crypto:signature_verifier",
        "//ordering/poc/proto:pow_cc_proto",
        "@boost//:format",
    ],
)


cc_library(
    name = "merkle",
    srcs = ["merkle.cpp"],
    hdrs = ["merkle.h"],
    deps = [
        ":miner_utils",
    ],
)


cc_library(
    name = "miner",
    srcs = ["miner.cpp"],
    hdrs = ["miner.h"],
    deps = [
        ":miner_utils",
        "//config:xxxdb_poc_config",
        "//crypto:signature_verifier",
        "//ordering/poc/proto:pow_cc_proto",
        "@boost//:format",
    ],
)

cc_library(
    name = "miner_manager",
    srcs = ["miner_manager.cpp"],
    hdrs = ["miner_manager.h"],
    deps = [
        "//config:xxxdb_poc_config",
    ],
)

cc_library(
    name = "block_manager",
    srcs = ["block_manager.cpp"],
    hdrs = ["block_manager.h"],
    deps = [
        ":merkle",
        ":miner",
        "//common:comm",
        "//config:xxxdb_poc_config",
        "//ordering/poc/proto:pow_cc_proto",
        "//proto:xxxdb_cc_proto",
        "//statistic:stats",
    ],
)

cc_library(
    name = "transaction_accessor",
    srcs = ["transaction_accessor.cpp"],
    hdrs = ["transaction_accessor.h"],
    deps = [
        ":miner_manager",
        "//client:xxxdb_txn_client",
        "//common/queue:lock_free_queue",
        "//common/utils",
        "//config:xxxdb_poc_config",
        "//ordering/poc/proto:pow_cc_proto",
        "//statistic:stats",
    ],
)

cc_library(
    name = "consensus_service_pow",
    srcs = ["consensus_service_pow.cpp"],
    hdrs = ["consensus_service_pow.h"],
    deps = [
        ":block_manager",
        ":pow_manager",
        ":transaction_accessor",
        "//common/queue:blocking_queue",
        "//common/utils",
        "//ordering/poc/proto:pow_cc_proto",
        "//server:consensus_service",
    ],
)

cc_library(
    name = "shift_manager",
    srcs = ["shift_manager.cpp"],
    hdrs = ["shift_manager.h"],
    deps = [
        "//config:xxxdb_poc_config",
        "//ordering/poc/proto:pow_cc_proto",
    ],
)

cc_library(
    name = "pow_manager",
    srcs = ["pow_manager.cpp"],
    hdrs = ["pow_manager.h"],
    deps = [
        ":block_manager",
        ":shift_manager",
        ":transaction_accessor",
        "//common/queue:blocking_queue",
        "//common/utils",
        "//ordering/poc/proto:pow_cc_proto",
        "//server:xxxdb_replica_client",
    ],
)

