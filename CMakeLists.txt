cmake_minimum_required(VERSION 4.0)

project(ChatShare
    VERSION 0.1
    DESCRIPTION "A Live Application That Allows Users To Share Files And Also Real-Time Chat"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Set the CMake toolchain file path for vcpkg
#set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
#add_executable(runUnitTests test/ src/)

configure_file(config.h.in config.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
#target_link_libraries(runUnitTests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

#add_test(AllTestsInMain ${PROJECT_NAME})
include(GoogleTest)
