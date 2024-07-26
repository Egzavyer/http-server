#include "client.h"
#include "../common/common.h"

#pragma comment(lib,"ws2_32.lib")

int main() {
    //Initialize the Winsock library
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    //Initialize addrinfo instances for later
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    //Set the parameters of hints to specify the type of socket
    ZeroMemory(&hints,sizeof(hints));
    hints.ai_family = AF_INET; //IPV4
    hints.ai_socktype = SOCK_STREAM; //TCP Socket
    hints.ai_protocol = IPPROTO_TCP; //TCP Protocol

    iResult = getaddrinfo("localhost", "27015", &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed %d\n", iResult);
        WSACleanup();
        return 1;
    }

    //Create socket
    SOCKET ConnectSocket = INVALID_SOCKET;
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): %ld\n", WSAGetLastError();
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

}

