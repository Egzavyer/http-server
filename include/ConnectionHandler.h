#ifndef CONNECTION_HANDLER_H
#define CONNECTION_HANDLER_H

#include "Networking.h"
#include <unordered_map>
#include <thread>
#include <iostream>

class ConnectionHandler {
public:
    ConnectionHandler();

    void handleConnection(Networking &n, const unsigned long long &client);


private:
    unsigned long long clientSocket;
};


#endif //CONNECTION_HANDLER_H
