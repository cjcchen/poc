package(default_visibility = ["//visibility:public"])

cc_library(
    name = "check_point_info",
    srcs = ["check_point_info.cpp"],
    hdrs = ["check_point_info.h"],
    deps = [
        "//common:comm",
        "//common/logging",
        "//config:xxxdb_config",
        "//crypto:signature_verifier",
        "//proto:checkpoint_info_cc_proto",
        "//proto:xxxdb_cc_proto",
    ],
)
