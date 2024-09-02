#include "server.h"
#include "../common/common.h"
#pragma comment(lib, "ws2_32.lib")

bool Server::receiveData(const SOCKET* ClientSocket) {
    char recvbuf[DEFAULT_BUFLEN];
    HTTPRequest request;
    std::string requestData;
    int iResult, iSendResult;
    do {
        iResult = recv(*ClientSocket, recvbuf, DEFAULT_BUFLEN,0);
        if (iResult > 0) {
            std::cout << "Bytes received: " << iResult << '\n';

            requestData.append(recvbuf,iResult);
            HTTPParser::parseHTTPRequest(requestData, request);

            std::string sendbuf = HTTPHandler::response(request);

            iSendResult = send(*ClientSocket, sendbuf.c_str(), (int)sendbuf.length(), 0);
            if (iSendResult == SOCKET_ERROR) {
                std::cerr << "send FAILED: " << WSAGetLastError() << '\n';
                closesocket(*ClientSocket);
                WSACleanup();
                return false;
            }
            std::cout << "Bytes sent: " << iSendResult << '\n';
        } else if (iResult == 0) {
            std::cout << "Connection Closing...\n";
        } else {
            std::cerr << "recv FAILED: " << WSAGetLastError() << '\n';
            closesocket(*ClientSocket);
            WSACleanup();
            return false;
        }
    } while (iResult > 0);
    return true;
}

void Server::shutdownServer(const SOCKET *ClientSocket) {
    int iResult = shutdown(*ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "shutdown FAILED: " << WSAGetLastError() << '\n';
        closesocket(*ClientSocket);
        WSACleanup();
    }
    closesocket(*ClientSocket);
    WSACleanup();
}

int main() {
    Connection connection;
    if (connection.setupConnection()) {
        std::cout << "\nSuccessful Connection on: " << connection.address << '\n';
    } else {
        std::cerr << "Connection FAILED\n";
    }

    if (!Server::receiveData(connection.getClientSocket())){
        std::cerr << "receiveData FAILED\n";
    }

    std::cout << "Shutting Down Server...\n";
    Server::shutdownServer(connection.getClientSocket());

    std::cout << "Server Shutdown: OK...\n";
    return 0;
}
