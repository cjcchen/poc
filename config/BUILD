package(default_visibility = ["//visibility:public"])

cc_library(
    name = "xxxdb_config",
    srcs = ["xxxdb_config.cpp"],
    hdrs = ["xxxdb_config.h"],
    deps = [
        "//common:comm",
        "//proto:replica_info_cc_proto",
    ],
)

cc_library(
    name = "xxxdb_config_utils",
    srcs = ["xxxdb_config_utils.cpp"],
    hdrs = ["xxxdb_config_utils.h"],
    deps = [
        ":xxxdb_config",
    ],
)

cc_library(
    name = "xxxdb_poc_config",
    srcs = ["xxxdb_poc_config.cpp"],
    hdrs = ["xxxdb_poc_config.h"],
    deps = [
        ":xxxdb_config",
    ],
)
