#include <cstring>
#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler() {
    clientSocket = -1;
}


void ConnectionHandler::handleConnection(Networking &n, const unsigned long long &client) {
    clientSocket = client;
    n.receiveData(clientSocket);
    char b[] = "meow\n";
    int sendbuflen = sizeof(b);
    n.sendData(clientSocket, b, sendbuflen);
}