#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler() {
    clientSocket = -1;
}


void ConnectionHandler::handleConnection(Networking &n, const unsigned long long &client) {
    clientSocket = client;
}