#ifndef HTTP_SERVER_SERVER_H
#define HTTP_SERVER_SERVER_H

#include "connection.h"

class Server {
public:
    static bool receiveData(const SOCKET* ClientSocket);

};


#endif //HTTP_SERVER_SERVER_H
