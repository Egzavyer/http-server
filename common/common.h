#ifndef HTTP_SERVER_COMMON_H
#define HTTP_SERVER_COMMON_H

#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#endif


#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <iostream>

class Common {

};


#endif //HTTP_SERVER_COMMON_H
