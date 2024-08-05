#include "server.h"
#include "../common/common.h"
#include <unordered_map>
#include <vector>
#pragma comment(lib, "ws2_32.lib")

enum class ParseState{
    RequestLine,
    Headers,
    Body,
    Done
};

std::string extractRequestLine(const std::string& request) {
    std::string requestLine;
    std::size_t requestLineEnd = request.find("\r\n", 0);
    if (requestLineEnd != std::string::npos)
        requestLine = request.substr(0, requestLineEnd);
    return requestLine;
}

std::vector<std::string> parseRequestLine(std::string requestLine){
    std::vector<std::string> splitRequestLine(3);

    std::cout << "REQUEST LINE:\n";
    std::cout << requestLine << "\n";

    std::size_t methodEnd = requestLine.find(' ', 0);
    splitRequestLine[0] = requestLine.substr(0,methodEnd);

    std::size_t uriEnd = requestLine.find(' ', methodEnd+1);
    splitRequestLine[1] = requestLine.substr(methodEnd+1, uriEnd-(methodEnd+1));

    splitRequestLine[2] = requestLine.substr(uriEnd+1);

    return splitRequestLine;
}

void parseHeaders(std::string headers){
    std::cout << "HEADERS:\n";
    std::cout << headers << "\n";

}

void parseHTTPRequest(std::string request) {
    ParseState state = ParseState::RequestLine;
    std::string requestLine;
    std::unordered_map<std::string,std::string> headers;
    std::string body;

    while (!request.empty()) {
        switch (state) {
            case ParseState::RequestLine: {
                std::string method, uri, version;
                requestLine = extractRequestLine(request);
                auto parts = parseRequestLine(requestLine);
                method = parts[0];
                uri = parts[1];
                version = parts[2];
                state = ParseState::Headers;
                std::cout << "Method:" << method << "\n";
                std::cout << "URI:" << uri << "\n";
                std::cout << "Version:" << version << "\n";
                std::cout << "Remaining:" << request << "\n";
                break;
            }
            case ParseState::Headers: {
                break;
            }

            case ParseState::Body: {
                break;
            }

            case ParseState::Done: {
                break;
            }
        }
        break;
    }
}


int serverIO(SOCKET ClientSocket)
{
    char recvbuf[DEFAULT_BUFLEN];
    int iResult, iSendResult;

    // Receive until the client closes the connection
    do
    {
        iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN - 1, 0);
        if (iResult > 0)
        {
            recvbuf[iResult] = '\0'; //Null-terminate the buffer
            printf("Bytes Received: %d\n", iResult);
            printf("%s\n", recvbuf);
            std::string data(recvbuf);
            parseHTTPRequest(data); //TEST


            //std::size_t headersEnd = data.find("\r\n\r\n",requestLineEnd+1);
            //std::string headers = data.substr(requestLineEnd+2, headersEnd-(requestLineEnd+4));
            //parseHeaders(headers);

            // Echo the buffer back to the client
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR)
            {
                printf("SEND FAILED: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes Sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
        {
            printf("Connection Closing...\n");
        }
        else
        {
            printf("RECV FAILED: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);
    return 0;
}


int main()
{
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;

    // Initialize the Winsock library
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Set the parameters of hints to specify the type of socket
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;       // IPV4
    hints.ai_socktype = SOCK_STREAM; // TCP Socket
    hints.ai_protocol = IPPROTO_TCP; // TCP Protocol
    hints.ai_flags = AI_PASSIVE;     // Passive socket for listening

    // Resolve the server address and port
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("SERVER GETADDRINFO FAILED: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a socket for listening to incoming connections
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Bind the socket to the address and port
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("BIND FAILED: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    freeaddrinfo(result);

    // Listen for incoming connections
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("LISTEN FAILED: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // TODO: Listen and accept more than one connection. Handle using threads
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET)
    {
        printf("ACCEPT FAILED: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    // Handle server-side I/O operations
    serverIO(ClientSocket);

    // Shutdown the connection for sending
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("SERVER SHUTDOWN FAILED: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // Cleanup
    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}