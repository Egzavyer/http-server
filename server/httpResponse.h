#ifndef HTTP_SERVER_HTTPRESPONSE_H
#define HTTP_SERVER_HTTPRESPONSE_H

#include "../common/common.h"
#include <unordered_map>

class HTTPResponse {
public:
    HTTPResponse();
    std::string getResponse();
    std::string getStatusLine();
    void setStatusLine(); //TODO: maybe private
    std::string getVersion(); //TODO: maybe remove
    void setVersion(std::string version);
    std::string getStatusCode(); //TODO: maybe remove
    void setStatusCode(std::string code);
    std::string getStatusText(); //TODO: maybe remove
    void setStatusText(std::string text);
    std::unordered_map<std::string, std::string> getHeaders();
    void setHeader(std::pair<std::string, std::string> currentHeader);
    std::string getBody();
    void setBody(std::string requestBody);

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
