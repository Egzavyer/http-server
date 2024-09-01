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
    //TODO: read from HTML file
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
    std::string statusCode = "200";
    std::string statusText = "Success";
    std::string response = request.getVersion() + ' ' + statusCode + ' ' + statusText + "\r\n" + "Content-Type: " + request.getHeaders()["Content-Type"] + "\r\n" + "Content-Length: " +
            std::to_string(file.length()) + "\r\n\r\n" + file;
    return response;
}

std::string HTTPHandler::handlePOST(HTTPRequest &request) {
    return std::string();
}
