package(default_visibility = ["//visibility:public"])

cc_library(
    name = "server_factory",
    srcs = ["server_factory.cpp"],
    hdrs = ["server_factory.h"],
    deps = [
        "//config:xxxdb_config_utils",
        "//ordering/pbft:consensus_service_pbft",
        "//server:xxxdb_server",
    ],
)
