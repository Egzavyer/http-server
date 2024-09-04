#ifndef HTTP_SERVER_CONNECTION_H
#define HTTP_SERVER_CONNECTION_H

#include "../common/common.h"


class Connection {
public:
    Connection();
    bool setupConnection();
    const SOCKET* getClientSocket() const;
    char address[256]{};

private:
    struct addrinfo *result, *ptr, hints{};
    static int iResult;
    SOCKET ListenSocket,ClientSocket;
    static ULONG addressSize;

    static bool initialiseWinsock();
    bool createSocket();
    bool bindSocket();
    bool listenOnSocket() const;
    bool acceptConnection();
};


#endif //HTTP_SERVER_CONNECTION_H
