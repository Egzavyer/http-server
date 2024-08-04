#include "server.h"
#include "../common/common.h"
#pragma comment(lib, "ws2_32.lib")

int serverIO(SOCKET ClientSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    int iResult, iSendResult;

    //Receive until client closes connection
    do {
        iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
        if (iResult > 0) {
            printf("Bytes Received: %d\n", iResult);
            printf("%s\n", recvbuf);

            //Echo the buffer back
            iSendResult = send(ClientSocket, recvbuf,iResult,0);
            if (iSendResult == SOCKET_ERROR) {
                printf("SEND FAILED: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes Sent: %d\n", iSendResult);
        } else if (iResult == 0)
            printf("Connection Closing...\n");
        else {
            printf("SERVER RECV FAILED: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);
    return 0;
}

int main() {
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    // Initialize the Winsock library
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo("localhost",DEFAULT_PORT,&hints,&result);
    if (iResult != 0) {
        printf("SERVER GETADDRINFO FAILED: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype,result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n",WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    iResult = bind(ListenSocket, result->ai_addr,(int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("BIND FAILED: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    if (listen(ListenSocket,SOMAXCONN) == SOCKET_ERROR) {
        printf("LISTEN FAILED: %d\n",WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    //TODO: Listen and accept more than one connection. Handle using threads
    ClientSocket = accept(ListenSocket,NULL,NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("ACCEPT FAILED: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    serverIO(ClientSocket);

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("SERVER SHUTDOWN FAILED: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}





