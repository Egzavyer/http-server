
#ifndef HTTP_SERVER_HTTPHANDLER_H
#define HTTP_SERVER_HTTPHANDLER_H

#include "../common/common.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include <fstream>
#include <string>
#include <sstream>

class HTTPHandler {
public:
    static std::string response(HTTPRequest &request);
private:
    static std::string handleGET(HTTPRequest &request);
    static std::string handlePOST(HTTPRequest &request);
    static std::string readFromHTML(const std::string& filename);
    static bool writeToFile(const std::string& filename, HTTPRequest &request);
    static std::string parseForm(const std::string &formData);
};


#endif //HTTP_SERVER_HTTPHANDLER_H
