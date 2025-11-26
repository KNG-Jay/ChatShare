load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_library(
    name = "chatshare",
    srcs = ["src/ChatShare.cpp"],
    hdrs = ["include/ChatShare.h"],
    deps = [
        "//db-engine:db-engine",
        "//server:server",
        "//client:client",
    ]

)

cc_binary(
    name = "ChatShare",
    srcs = ["src/main.cpp"],
    deps = [
        ":chatshare",
    ],
)

cc_test(
    name = "chatshare_tests",
    srcs = ["test/ChatShareTests.cpp"],
    deps = [
        "//client:client",
        "//db-engine:db-engine",
        "//server:server",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "client_tests",
    srcs = ["test/ClientTests.cpp"],
    deps = [
        "//client:client",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "db-engine_tests",
    srcs = ["test/DBEngineTests.cpp"],
    deps = [
        "//db-engine:db-engine",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "server_tests",
    srcs = ["test/ServerTests.cpp"],
    deps = [
        "//server:server",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

test_suite(
    name = "unit_tests",
    tests = [
        ":chatshare_tests",
        ":client_tests",
        ":db-engine_tests",
        ":server_tests",
    ],
)