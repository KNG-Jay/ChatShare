# Description

    - Live Chat Server That Allows Clients To Chat In A Global Room And Also Share Files

# Tools

    - pqxx (DATABASE ENGINE)
    - Boost.Asio (MESSAGING) = Async Net. Operations, TCP Protocol Support
    - POCO (FILE_TRANSPORT) = WebSocket Support, FTP Support, HTTP Support
    - QT (GUI)
    - Reflect-cpp (SERIALIZATION)

# Needed Classes

    - Client (QT) [ Sends/Receives Messages And Files ]
    - Server (Boost.Asio, POCO, Reflect-cpp) [ Manages Connections, Message Routing, File Transfers Between Clients ]
    - Database (pqxx) [ Stores User Data, Chat History, And File Metadata ]

# Needed Implementation

    - User Authentication
    - Real-time Messaging Using Sockets
    - File Upload/Download With Progress Indicators
    - Presence Managment

# Notes

    -
