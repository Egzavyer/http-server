#include "httpHandler.h"

 std::string HTTPHandler::response(HTTPRequest &request) {
    if (request.getMethod() == "GET") {
        return handleGET(request);
    } else if (request.getMethod() == "POST") {
        return handlePOST(request);
    } else {
        std::cerr << "ERROR: Unhandled Method\n";
    }
}

std::string HTTPHandler::handleGET(HTTPRequest &request) {
    HTTPResponse httpResponse;
    std::string statusCode, statusText;
    //TODO: read from HTML file
    bool readFromFile = true;
    std::string file = "<!DOCTYPE html>\n"
                       "<html lang=\"en\">\n"
                       "<head>\n"
                       "    <meta charset=\"UTF-8\">\n"
                       "    <title>Index</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "<h1>This is loaded from a file</h1>\n"
                       "<p>On my HTTP server</p>\n"
                       "</body>\n"
                       "</html>";
    if (readFromFile) {
        statusCode = "200";
        statusText = "OK";
        //TODO: set headers
        httpResponse.setBody(file);
    } else {
        statusCode = "404";
        statusText = "Not Found";
    }
    httpResponse.setVersion(request.getVersion());
    httpResponse.setStatusCode(statusCode);
    httpResponse.setStatusText(statusText);
    return httpResponse.getResponse();
}

std::string HTTPHandler::handlePOST(HTTPRequest &request) {
    return std::string();
}
