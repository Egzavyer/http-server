#include "server.h"
#include "../common/common.h"
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

std::vector<std::string> parseRequestLine(const std::string& requestLine){
    std::vector<std::string> splitRequestLine(3);

    std::cout << "-----------------------------------\nREQUEST LINE:\n";
    std::cout << requestLine << "\n";

    std::size_t methodEnd = requestLine.find(' ', 0);
    splitRequestLine[0] = requestLine.substr(0,methodEnd);

    std::size_t uriEnd = requestLine.find(' ', methodEnd+1);
    splitRequestLine[1] = requestLine.substr(methodEnd+1, uriEnd-(methodEnd+1));

    splitRequestLine[2] = requestLine.substr(uriEnd+1);

    return splitRequestLine;
}

std::string extractHeader(const std::string& request) {
    std::string headerLine;
    std::size_t headerEnd = request.find("\r\n");

    if (headerEnd != std::string::npos)
        headerLine = request.substr(0, headerEnd);
    return headerLine;
}

std::pair<std::string,std::string> parseHeaders(const std::string& header){

    std::pair<std::string,std::string> parts;
    std::size_t firstPartEnd = header.find(':');
    parts.first = header.substr(0,firstPartEnd);
    parts.second = header.substr(firstPartEnd+2, header.size()-firstPartEnd+2);
    return parts;
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
                request = request.substr(requestLine.length() + 2);
                state = ParseState::Headers;
                std::cout << "Method:" << method << "\n";
                std::cout << "URI:" << uri << "\n";
                std::cout << "Version:" << version << "\n\n";
                break;
            }

            case ParseState::Headers: {
                std::cout << "HEADERS:\r\n";
                while (!request.empty()) {
                    std::string header = extractHeader(request);
                    request = request.substr(header.length() + 2);
                    if (header.empty()){
                        state = ParseState::Body;
                        break;
                    }
                    auto parts = parseHeaders(header);
                    std::cout << parts.first << " : " << parts.second << "\n";
                    headers[parts.first] = parts.second;
                }
                break;
            }
            case ParseState::Body:
                if (headers.count("Content-Length")) {
                    int contentLength = std::stoi(headers["Content-Length"]);
                    body = request.substr(0,contentLength);
                    std::cout << "\nBody: " << body << "\n-----------------------------------\n";
                    request = request.substr(body.length());
                    state = ParseState::Done;
                }
                break;

            case ParseState::Done:
                std::cout << "PARSING COMPLETE\n";
                break;
        }
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
            parseHTTPRequest(data);

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
    struct addrinfo *result = nullptr, hints;

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
        printf("Error at socket(): %d\n", WSAGetLastError());
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
    ClientSocket = accept(ListenSocket, nullptr, nullptr);
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