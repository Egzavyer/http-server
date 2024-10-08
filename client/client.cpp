#include "client.h"
#include "../common/common.h"

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

int main()
{
    // Initialize the Winsock library
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSASTARTUP FAILED: %d",iResult);
        return 1;
    }

    // Initialize addrinfo instances for later
    struct addrinfo *result = nullptr, *ptr = nullptr, hints;

    // Set the parameters of hints to specify the type of socket
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;       // IPV4
    hints.ai_socktype = SOCK_STREAM; // TCP Socket
    hints.ai_protocol = IPPROTO_TCP; // TCP Protocol

    // Resolve the server address and port
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("GETADDRINFO FAILED %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != nullptr ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("SOCKET FAILED: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    //TODO: Implement a way to retry with backoff strategy
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send a message to the server
    //const char *sendbuf = "GET /index.html HTTP/1.1\r\n"
                          "Host: www.example.com\r\n"
                          "Accept: text/html\r\n\r\n";
    const char *sendbuf = "GET /index.html HTTP/1.1\r\n"
                          "Host: www.example.com\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36\r\n"
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
                          "Accept-Language: en-US,en;q=0.5\r\n"
                          "Accept-Encoding: gzip, deflate\r\n"
                          "Content-Length: 4\r\n"
                          "Connection: keep-alive\r\n\r\n"
                          "MEOW";
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR)
    {
        printf("SEND FAILED: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Shutdown the connection for sending
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("SHUTDOWN FAILED:%d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    const int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[recvbuflen];
    do
    {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
        {
            recvbuf[iResult] = '\0';
            printf("Bytes Received: %d\n", iResult);
            printf("%s\n", recvbuf);
        }
        else if (iResult == 0)
            printf("Connection Closed\n");
        else
            printf("RECV FAILED:%d\n", WSAGetLastError());
    } while (iResult > 0);

    // Shutdown the connection for sending again (redundant)
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("SHUTDOWN FAILED:%d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Cleanup
    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}