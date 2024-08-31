#ifndef HTTP_SERVER_HTTPREQUEST_H
#define HTTP_SERVER_HTTPREQUEST_H

#include "../common/common.h"
#include <unordered_map>

class HTTPRequest {
public:
    enum class ParseState {
        METHOD,
        URI,
        VERSION,
        HEADERS,
        BODY,
        DONE
    };

    HTTPRequest();
    std::string getMethod();
    void setMethod(std::basic_string<char> requestMethod);
    std::string getURI();
    void setURI(std::basic_string<char> requestURI);
    std::string getVersion();
    void setVersion(std::basic_string<char> requestVersion);
    std::unordered_map<std::string, std::string> getHeaders();
    void setHeader(std::pair<std::string, std::string> currentHeader);
    std::string getBody();
    void setBody(std::basic_string<char> requestBody);
    ParseState getState();
    void setState(ParseState currentState);

private:
    std::string method;
    std::string uri;
    std::string version;
    std::unordered_map<std::string,std::string> headers;
    std::string body;
    ParseState state;
};


#endif //HTTP_SERVER_HTTPREQUEST_H
