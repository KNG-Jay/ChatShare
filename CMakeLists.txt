cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0167 NEW)

project(ChatShare
    VERSION 0.1
    DESCRIPTION "A Live Application That Allows Users To Share Files And Also Real-Time Chat"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Set the CMake toolchain file path for vcpkg
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/arm64-linux")
else()
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)
message(STATUS "PostgreSQL include dirs: ${PostgreSQL_INCLUDE_DIRS}")
message(STATUS "PostgreSQL libraries: ${PostgreSQL_LIBRARIES}")
include_directories(${PostgreSQL_INCLUDE_DIRS})
find_package(reflectcpp CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system coroutine regex asio)
find_package(Poco REQUIRED COMPONENTS Foundation)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_subdirectory(db-engine)
add_subdirectory(client)
add_subdirectory(server)

add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(runUnitTests
        test/ChatShareTests.cpp
        test/DBEngineTests.cpp
        test/ClientTests.cpp
        test/ServerTests.cpp
        src/ChatShare.cpp
        db-engine/src/DBEngine.cpp
        client/src/Client.cpp
        server/src/Server.cpp
)

configure_file(config.h.in ${CMAKE_HOME_DIRECTORY}/config.h)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${CMAKE_SOURCE_DIR}/include"
)

add_library(shared SHARED src/main.cpp)

target_link_libraries(shared PUBLIC reflectcpp::reflectcpp)
target_link_libraries(${PROJECT_NAME} PRIVATE shared)
target_link_libraries(runUnitTests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main shared db-engine)

include(GoogleTest)
gtest_add_tests(TARGET runUnitTests
                TEST_PREFIX test_
                TEST_SOURCES test/ChatShareTests.cpp test/ClientTests.cpp test/DBEngineTests.cpp test/ServerTests.cpp)
