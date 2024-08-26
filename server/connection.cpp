#include "connection.h"

int Connection::iResult;

Connection::Connection() {
    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;
    ptr = nullptr;
    result = nullptr;
}

bool Connection::initialiseWinsock() {
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSASTARTUP FAILED: %d",iResult);
        return false;
    }
    return true;
}

bool Connection::createSocket() {
    result = nullptr;
    ptr = nullptr;
    ListenSocket = INVALID_SOCKET; //TODO: REFACTOR

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(nullptr, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << '\n';
        WSACleanup();
        return false;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << '\n';
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    return true;
}

bool Connection::bindSocket() const {
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << '\n';
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }
    freeaddrinfo(result);
    return true;
}

bool Connection::listenOnSocket() const {
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << '\n';
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }
    return true;
}

bool Connection::acceptConnection() { //TODO: accept multiple connections using threads
    ClientSocket = INVALID_SOCKET;
    ClientSocket = accept(ListenSocket, nullptr, nullptr);
    if (ClientSocket == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << '\n';
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }
    return true;
}

bool Connection::setupConnection() {
    if (initialiseWinsock() && this->createSocket() && this->bindSocket() && this->listenOnSocket() && this->acceptConnection()){
        return true;
    }
    return false;
}

SOCKET Connection::getListenSocket() const {
    return ListenSocket;
}

SOCKET Connection::getClientSocket() const {
    return ClientSocket;
}




