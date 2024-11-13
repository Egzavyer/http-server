#include "Server.h"

Server::Server(Networking &netInterface, ConnectionHandler &connHandler) : n(netInterface), h(connHandler) {
    n.createSocket();
    n.bindSocket();
    n.listenOnSocket();
}

void Server::runServer() {
    try {
        while (true) {
            unsigned long long client = n.acceptConnection();
            connections.emplace_back(&ConnectionHandler::handleConnection, std::ref(this->h), std::ref(this->n),
                                     client);

            for (auto &t: connections) {
                if (t.joinable()) {
                    connections.erase(connections.begin());
                    t.join();
                }
            }
        }
    } catch (std::exception &e) {
        throw e;
    }
}
