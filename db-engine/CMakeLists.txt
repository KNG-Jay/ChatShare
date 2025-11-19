add_library(db-engine src/DBEngine.cpp)

target_link_libraries(db-engine PUBLIC libpqxx::pqxx)
