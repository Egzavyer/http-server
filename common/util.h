#ifndef HTTP_SERVER_UTIL_H
#define HTTP_SERVER_UTIL_H

#include "../server/httpRequest.h"
#include <fstream>

class Util {
public:
    static std::string readFromHTML(const std::string& filename);
    static bool writeToFile(const std::string& filename, HTTPRequest &request);
    static std::string parseForm(std::string formData);
    static std::string headersToString(const std::unordered_map<std::string,std::string>& responseHeaders);
    static std::string getBodyMIMEType(const std::string &requestURI);

};


#endif //HTTP_SERVER_UTIL_H
