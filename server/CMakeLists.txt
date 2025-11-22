add_library(server src/Server.cpp)

target_link_libraries(server PRIVATE shared db-engine Boost::asio Poco::Foundation)
