
#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#include "Networking.h"
#include "ConnectionHandler.h"
#include <iostream>
#include <thread>
#include <vector>

class Server {
public:
    Server(Networking &netInterface, ConnectionHandler &connHandler);

    void runServer();

private:
    Networking &n;
    ConnectionHandler &h;
    std::vector<std::thread> connections;
};

#endif // HTTPSERVER_SERVER_H
