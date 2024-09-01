#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H

#include "connection.h"
#include "httpParser.h"
#include "httpHandler.h"

class Server {
public:
    static bool receiveData(const SOCKET* ClientSocket);
    static void shutdownServer(const SOCKET* ClientSocket);
};


#endif //HTTP_SERVER_SERVER_H
