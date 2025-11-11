add_library(client src/Client.cpp)

target_link_libraries(client PRIVATE shared)