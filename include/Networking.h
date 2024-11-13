#ifndef NETWORKING_H
#define NETWORKING_H

class Networking {
public:

    virtual void createSocket() = 0;

    virtual void bindSocket() = 0;

    virtual void listenOnSocket() = 0;

    virtual unsigned long long acceptConnection() = 0;

    virtual unsigned long long getSock() = 0;


    const char *PORT = "8080";

};

#endif //NETWORKING_H
