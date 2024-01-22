package(default_visibility = ["//visibility:public"])

cc_library(
    name = "key_generator",
    srcs = ["key_generator.cpp"],
    hdrs = ["key_generator.h"],
    deps = [
        "//:cryptopp_lib",
        "//common:comm",
        "//proto:signature_info_cc_proto",
    ],
)

cc_library(
    name = "signature_verifier",
    srcs = ["signature_verifier.cpp"],
    hdrs = ["signature_verifier.h"],
    deps = [
        "//:cryptopp_lib",
        "//common:comm",
        "//proto:signature_info_cc_proto",
    ],
)

