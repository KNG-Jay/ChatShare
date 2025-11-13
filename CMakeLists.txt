cmake_minimum_required(VERSION 3.15)

project(ChatShare
    VERSION 0.1
    DESCRIPTION "A Live Application That Allows Users To Share Files And Also Real-Time Chat"
)

add_subdirectory(db-engine)
add_subdirectory(client)
add_subdirectory(server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

# Set the CMake toolchain file path for vcpkg
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/arm64-linux")
else()
    set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/amd64-linux")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

find_package(GTest CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(boost_asio_core CONFIG REQUIRED)
find_package(Poco REQUIRED COMPONENTS Foundation)
find_package(reflectcpp CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(runUnitTests
        test/ChatShareTests.cpp
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

target_link_libraries(shared PUBLIC GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)  # ADD reflectorcpp::reflectorcpp
target_link_libraries(${PROJECT_NAME} PRIVATE shared)
target_link_libraries(runUnitTests PRIVATE shared)

#add_test(AllTestsInMain ${PROJECT_NAME})
include(GoogleTest)
