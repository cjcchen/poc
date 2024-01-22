package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "key_generator_tools",
    srcs = ["key_generator_tools.cpp"],
    deps = [
        "//crypto:key_generator",
    ],
)

cc_binary(
    name = "certificate_tools",
    srcs = ["certificate_tools.cpp"],
    deps = [
        "//crypto:key_generator",
        "//crypto:signature_verifier",
    ],
)

cc_binary(
    name = "xxxdb_status_client_tools",
    srcs = ["xxxdb_status_client_tools.cpp"],
    deps = [
        "//client:xxxdb_state_client",
        "//config:xxxdb_config_utils",
    ],
)

cc_binary(
    name = "xxxdb_txn_client_tools",
    srcs = ["xxxdb_txn_client_tools.cpp"],
    deps = [
        "//client:xxxdb_txn_client",
        "//config:xxxdb_config_utils",
    ],
)

cc_binary(
    name = "kv_client_txn_tools",
    srcs = ["kv_client_txn_tools.cpp"],
    deps = [
        "//client:xxxdb_txn_client",
        "//config:xxxdb_config_utils",
        "//proto:kv_server_cc_proto",
    ],
)
