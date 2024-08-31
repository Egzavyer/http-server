#ifndef HTTP_SERVER_HTTPPARSER_H
#define HTTP_SERVER_HTTPPARSER_H

#include "../common/common.h"
#include "httpRequest.h"

class HTTPParser {
public:

    static void parseHTTPRequest(std::string &rawRequest, HTTPRequest request);

private:
    static std::string extractRequestLine(std::string& request);
    static std::string extractMethod(std::string &requestLine);
    static std::string extractURI(std::string &requestLine);
    static std::string extractVersion(std::string &requestLine);
    static std::string extractHeader(std::string &request);
    static std::pair<std::string, std::string> parseHeader(std::string &header);
    static std::string extractBody(std::string request);

    static void printHeaders(std::unordered_map<std::string,std::string> headers);

};


#endif //HTTP_SERVER_HTTPPARSER_H
