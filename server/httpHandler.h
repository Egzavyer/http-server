#ifndef HTTP_SERVER_HTTPHANDLER_H
#define HTTP_SERVER_HTTPHANDLER_H

#include "../common/util.h"
#include "httpRequest.h"

class HTTPHandler {
public:
    static std::string response(HTTPRequest &request);

private:
    static std::string handleGET(HTTPRequest &request);

    static std::string handlePOST(HTTPRequest &request);
};


#endif //HTTP_SERVER_HTTPHANDLER_H
