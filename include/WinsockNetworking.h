#ifdef _WIN32

#ifndef WINSOCK_NETWORKING_H
#define WINSOCK_NETWORKING_H

#include "Networking.h"
#include "ConnectionHandler.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class WinsockNetworking : public Networking {
public:
    WinsockNetworking();

    void createSocket() override;

    void bindSocket() override;

    void listenOnSocket() override;

    unsigned long long acceptConnection() override;

    void receiveData(unsigned long long &client) override;

    int sendData(unsigned long long &client, char *sendbuf, int &totalBytes) override;

    void shutdownSocket(const unsigned long long &client) override;

    unsigned long long getSock() override;

private:
    static void initWinsock();

    SOCKET sock;
    struct addrinfo *address;

};

#endif //WINSOCK_NETWORKING_H
#endif //_WIN32
