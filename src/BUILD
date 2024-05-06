# The following two targets are basically for testing the external
# libzmq library.
cc_binary(
    name = "zmq_example_server",
    srcs = ["zmq_example_server.cc"],
    deps = [
        "@libzmq//:libzmq",
    ]
)

cc_binary(
    name = "zmq_example_client",
    srcs = ["zmq_example_client.cc"],
    deps = [
        "@libzmq//:libzmq",
    ]
)