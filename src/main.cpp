#include <iostream>
#include "Networking.h"
#include "WinsockNetworking.h"
#include "Server.h"
#include "ConnectionHandler.h"

int main() {
    try {
        WinsockNetworking w;
        ConnectionHandler h;
        Server server(w, h);
        server.runServer();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    // when user runs server, a socket is created and starts listening on chosen port
    // socket creation is different on windows and linux and macOS, maybe factory pattern and inject networking component with socket into server
    // when client tries to connect to server, server accepts the connection in separate thread and keeps listening (inject connection handler in server)
    // when client sends http request, the request needs to get parsed by a http parser class (inject parser into connection handler)
    // parse request, create response, send response back to client
}
