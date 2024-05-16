# The following two targets are basically for testing the external
# libzmq library.

cc_library(
    name = "agents",
    srcs = ["agents.cc"],
    hdrs = ["agents.h"],
)

cc_library(
    name = "memory_allocator",
    srcs = ["memory_allocator.cc"],
    hdrs = ["memory_allocator.h"],
    deps = [],
)

cc_library(
    name = "threadpool",
    srcs = ["threadpool.cc"],
    hdrs = ["threadpool.h"],
    deps = [],
)

cc_binary(
    name = "zmq_example_server",
    srcs = ["zmq_example_server.cc"],
    deps = [
        "@libzmq",
    ],
)

cc_binary(
    name = "zmq_example_client",
    srcs = ["zmq_example_client.cc"],
    deps = [
        "@libzmq",
    ],
)

cc_binary(
    name = "zmq_multithread_example",
    srcs = ["zmq_multithread_example.cc"],
    deps = [
        "@libzmq",
    ],
)

cc_binary(
    name = "benchmark",
    srcs = ["benchmark.cc"],
    deps = [
        ":agents",
        ":threadpool",
        "@benchmark//:benchmark_main",
        "@libzmq",
    ],
)
