package(default_visibility = ["//visibility:public"])

cc_library(
    name = "blocking_queue",
    hdrs = [
        "blocking_queue.h",
    ],
)

cc_library(
    name = "lock_free_queue",
    hdrs = [
        "lock_free_queue.h",
    ],
    deps = [
        "//common:boost_lockfree",
        "//common:comm",
    ],
)


cc_library(
    name = "batch_queue",
    hdrs = [
        "batch_queue.h",
    ],
    deps = [
        "//common:comm",
    ],
)

