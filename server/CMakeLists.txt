add_library(server src/Server.cpp)

target_link_libraries(server PRIVATE shared Boost::asio Poco::Foundation)    # ADD Boost::asio Poco::Foundation
