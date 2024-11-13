#include "WinsockNetworking.h"

#ifdef _WIN32

WinsockNetworking::WinsockNetworking() {
    WinsockNetworking::initWinsock();
    sock = INVALID_SOCKET;
    address = nullptr;
}

void WinsockNetworking::initWinsock() {
    WSADATA wsaData;

    int iResult;
    if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(iResult));
    }
}

void WinsockNetworking::createSocket() {
    int iResult;
    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if ((iResult = getaddrinfo(nullptr, Networking::PORT, &hints, &address)) != 0) {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed: " + std::to_string(iResult));
    }

    if ((sock = socket(address->ai_family, address->ai_socktype, address->ai_protocol)) == INVALID_SOCKET) {
        freeaddrinfo(address);
        WSACleanup();
        throw std::runtime_error("socket failed: " + std::to_string(WSAGetLastError()));
    }
}

void WinsockNetworking::bindSocket() {
    int iResult;
    if ((iResult = bind(sock, address->ai_addr, (int) address->ai_addrlen)) == SOCKET_ERROR) {
        freeaddrinfo(address);
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("bind failed: " + std::to_string(WSAGetLastError()));
    }
    freeaddrinfo(address); // TODO: maybe remove if info useful
}

void WinsockNetworking::listenOnSocket() {
    char ipBuf[INET_ADDRSTRLEN];
    if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("listen failed: " + std::to_string(WSAGetLastError()));
    }

    struct sockaddr_in sin;
    int addrlen = sizeof(sin);
    if (getsockname(sock, (struct sockaddr *) &sin, &addrlen) == 0 &&
        sin.sin_family == AF_INET &&
        addrlen == sizeof(sin)) {
        int local_port = ntohs(sin.sin_port);

        std::cout << "Server listening on " << inet_ntop(AF_INET, &sin.sin_addr, ipBuf, INET_ADDRSTRLEN) << ":"
                  << local_port << " on socket: " << sock << std::endl;
    }
}

unsigned long long WinsockNetworking::acceptConnection() {
    SOCKET client = INVALID_SOCKET;
    char ipBuf[INET_ADDRSTRLEN];

    if ((client = accept(sock, nullptr, nullptr)) == INVALID_SOCKET) {
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("accept failed: " + std::to_string(WSAGetLastError()));
    }
    struct sockaddr_in sockaddr;
    int namelen = sizeof(sockaddr);
    if (!getpeername(client, (struct sockaddr *) &sockaddr, &namelen)) {
        printf("Accepted connection from: %s on socket: %llu\n",
               inet_ntop(AF_INET, &sockaddr.sin_addr, ipBuf, INET_ADDRSTRLEN), client);
    }
    return client;
}

unsigned long long WinsockNetworking::getSock() {
    return sock;
}

#endif //_WIN32
