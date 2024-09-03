#ifndef HTTP_SERVER_HTTPRESPONSE_H
#define HTTP_SERVER_HTTPRESPONSE_H

#include "../common/common.h"
#include <unordered_map>

class HTTPResponse {
public:
    HTTPResponse();
    std::string getResponse();
    void setVersion(std::string version);
    void setStatusCode(std::string code);
    void setStatusText(std::string text);
    void setHeader(std::pair<std::string, std::string> currentHeader);
    void setBody(std::string requestBody);
    std::string getBodyMIMEType(const std::string &requestURI);

private:
    std::string statusLine;
    std::string version;
    std::string statusCode;
    std::string statusText;
    std::unordered_map<std::string,std::string> headers;
    std::string body;

    std::string headersToString(const std::unordered_map<std::string,std::string>& responseHeaders);

};


#endif //HTTP_SERVER_HTTPRESPONSE_H
