//
// Created by kng on 11/9/25.
//

#ifndef CHATSHARE_SERVER_H
#define CHATSHARE_SERVER_H

#include "../db-engine/include/DBEngine.h"
#include <boost/asio.hpp>
#include <Poco/Poco.h>

class Server {
public:
    Server();
    ~Server();

    void startup();
    void shutdown();

private:

};


#endif //CHATSHARE_SERVER_H