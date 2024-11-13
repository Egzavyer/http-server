#include "Server.h"

Server::Server(Networking &netInterface, ConnectionHandler &connHandler) : n(netInterface), h(connHandler) {
    n.createSocket();
    n.bindSocket();
    n.listenOnSocket();
}

void Server::runServer() {
    try {
        while (true) {
            //unsigned long long client = n.acceptConnection();
            connections.emplace_back(&ConnectionHandler::handleConnection, std::ref(this->h), std::ref(this->n),
                                     n.acceptConnection());

            for (auto &t: connections) {
                //TODO: find way to remove the thread from the vector after it gets joined
                if (t.joinable()) {
                    //connections.erase(connections.begin());
                    std::cout << "Joined thread: " << t.get_id() << "\n";
                    t.join();
                }
            }
        }
        n.shutdownSocket(n.getSock());
    } catch (std::exception &e) {
        throw e;
    }
}
