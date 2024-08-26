//
// Created by xcppl on 2024-08-26.
//

#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#include "../common/common.h"


class Connection {
public:
    Connection();
    bool setupConnection();
    SOCKET getListenSocket() const;
    SOCKET getClientSocket() const;

private:
    struct addrinfo *result, *ptr, hints{};
    static int iResult;
    SOCKET ListenSocket,ClientSocket;

    static bool initialiseWinsock();
    bool createSocket();
    bool bindSocket() const;
    bool listenOnSocket() const;
    bool acceptConnection();
};


#endif //HTTP_SERVER_CONNECTION_H
