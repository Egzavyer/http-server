#include "LinuxNetworking.h"

#ifdef __linux__

LinuxNetworking::LinuxNetworking() {
    sock = -1;
    address;
}

void LinuxNetworking::createSocket() {
    int opt = 1;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("socket failed: " + std::string(strerror(errno)));
    }

    if (setsockopt(sock,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt.sizeof(opt))){
        throw std::runtime_error("setsockopt failed: " + std::string(strerror(errno)));
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(std::stoi(Networking::PORT));
}

void LinuxNetworking::bindSocket() {
    if (bind(sock, (struct sockaddr*)&address,sizeof(address)) < 0){
        throw std::runtime_error("bind failed: " + std::string(strerror(errno)));
    }
}

void LinuxNetworking::listenOnSocket() {
    if (listen(sock,3) < 0){
        throw std::runtime_error("listen failed: " + std::string(strerror(errno)));
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), ip, INET_ADDRSTRLEN);
    std::cout << "Server listening on " << ip << ":" << PORT << std::endl;
}

unsigned long long LinuxNetworking::getSock() {
    return (unsigned long long) sock;
}

#endif //__linux__
