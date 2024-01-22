package(default_visibility = ["//ordering:__subpackages__"])

cc_library(
    name = "transaction_executor_impl",
    srcs = ["transaction_executor_impl.cpp"],
    hdrs = ["transaction_executor_impl.h"],
    visibility = ["//visibility:public"],
    deps = [
    ],
)

cc_library(
    name = "system_info",
    srcs = ["system_info.cpp"],
    hdrs = ["system_info.h"],
    deps = [
        "//config:xxxdb_config",
        "//proto:xxxdb_cc_proto",
    ],
)


cc_library(
    name = "transaction_executor",
    srcs = ["transaction_executor.cpp"],
    hdrs = ["transaction_executor.h"],
    deps = [
        ":system_info",
        ":transaction_executor_impl",
        "//common:comm",
        "//common/queue:lock_free_queue",
        "//config:xxxdb_config",
        "//proto:xxxdb_cc_proto",
        "//statistic:stats",
    ],
)

