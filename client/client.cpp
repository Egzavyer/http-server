#include "client.h"
#include "../common/common.h"

#pragma comment(lib, "ws2_32.lib") // Link with the Winsock library

/**
 * @brief The entry point of the program.
 *
 * Initializes the Winsock library, sets up the necessary addrinfo instances,
 * creates a socket, connects to the server, sends a message to the server, receives data
 * from the server, and cleans up the resources before exiting.
 *
 * @return 0 if the program executed successfully, otherwise a non-zero value.
 */
int main()
{
    // Initialize the Winsock library
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cout << "WSASTARTUP FAILED: " << iResult << std::endl;
        return 1;
    }

    // Initialize addrinfo instances for later
    struct addrinfo *result = NULL, *ptr = NULL, hints;

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

    // Create a socket for connecting to the server
    SOCKET ConnectSocket = INVALID_SOCKET;
    ptr = result;
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }
    printf("Connecting to Server...\n");
    // TODO: Should try next address if fails but instead free resources
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET)
    {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send a message to the server
    const char *sendbuf = "Hello from Client";
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