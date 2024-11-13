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

void WinsockNetworking::receiveData(unsigned long long &client) {
    char recvbuf[512];
    int iResult;
    int recvbuflen = 512;

    //TODO: currently receives until peer shuts down connection, maybe try find other way
    do {
        if ((iResult = recv(client, recvbuf, recvbuflen, 0)) > 0) {
            std::cout << "Bytes received: " << iResult << std::endl;
            std::cout << recvbuf << std::endl;
            //do stuff with the received data, probably pass it to connection handler
        } else if (iResult == 0) {
            std::cout << "Connection closing...\n";
        } else {
            closesocket(client);
            WSACleanup();
            throw std::runtime_error("recv failed: " + std::to_string(WSAGetLastError()));
        }
    } while (iResult > 0);
}

int WinsockNetworking::sendData(unsigned long long &client, char *sendbuf, int &totalBytes) {
    int iSendResult;
    //TODO: send in chunks
    if ((iSendResult = send(client, sendbuf, strlen(sendbuf), 0)) == SOCKET_ERROR) {
        closesocket(client);
        WSACleanup();
        throw std::runtime_error("send failed: " + std::to_string(WSAGetLastError()));
    }
    std::cout << "Bytes sent: " << iSendResult << std::endl;
    return iSendResult;
}

void WinsockNetworking::shutdownSocket(const unsigned long long &client) {
    int iResult;
    if ((iResult = shutdown(client, SD_SEND)) == SOCKET_ERROR) {
        closesocket(client);
        WSACleanup();
        throw std::runtime_error("shutdown failed: " + std::to_string(WSAGetLastError()));
    }

    closesocket(client);
    WSACleanup();
}

unsigned long long WinsockNetworking::getSock() {
    return sock;
}


#endif //_WIN32
