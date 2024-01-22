package(default_visibility = ["//visibility:public"])

cc_library(
    name = "socket",
    hdrs = ["socket.h"],
    deps = [
        "//common:comm",
    ],
)

cc_library(
    name = "tcp_socket",
    srcs = ["tcp_socket.cpp"],
    hdrs = ["tcp_socket.h"],
    deps = [
        ":socket",
    ],
)

