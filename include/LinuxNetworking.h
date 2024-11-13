#ifdef __linux__

#ifndef LINUX_NETWORKING_H
#define LINUX_NETWORKING_H

#include "Networking.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

class LinuxNetworking : public Networking {
    LinuxNetworking();

    void createSocket() override;

    void bindSocket() override;

    void listenOnSocket() override;

    unsigned long long getSock() override;

private:
    int sock;
    struct sockaddr_in address;

};


#endif //LINUX_NETWORKING_H
#endif //__linux__
