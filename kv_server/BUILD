package(default_visibility = ["//visibility:public"])

cc_library(
    name = "kv_server_executor",
    srcs = ["kv_server_executor.cpp"],
    hdrs = ["kv_server_executor.h"],
    deps = [
        "//common:comm",
        "//execution:transaction_executor_impl",
        "//proto:kv_server_cc_proto",
    ],
)

cc_test(
    name = "kv_server_executor_test",
    srcs = ["kv_server_executor_test.cpp"],
    deps = [
        ":kv_server_executor",
        "//common/test:test_main",
    ],
)

cc_binary(
    name = "kv_server",
    srcs = ["kv_server.cpp"],
    deps = [
        ":kv_server_executor",
        "//application/utils:server_factory",
    ],
)
