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
    std::pair<std::string,std::string> header;
    //TODO: read from HTML file
    bool readFromFile = true;
    std::string file = readFromHTML(request.getURI());
    if (!file.empty()) {
        statusCode = "200";
        statusText = "OK";
        //TODO: set headers
        header.first = "Content-Length";
        header.second = std::to_string(file.length());
        httpResponse.setHeader(header);
        header.first = "Content-Type";
        header.second = "text/html";
        httpResponse.setHeader(header);
        httpResponse.setBody(file);
    } else {
        statusCode = "404";
        statusText = "Not Found";
        //TODO: set headers
        httpResponse.setBody(readFromHTML("/404NotFound.html"));
    }
    httpResponse.setVersion(request.getVersion());
    httpResponse.setStatusCode(statusCode);
    httpResponse.setStatusText(statusText);
    return httpResponse.getResponse();
}

std::string HTTPHandler::handlePOST(HTTPRequest &request) {
    return std::string();
}

std::string HTTPHandler::readFromHTML(const std::string& filename) { //TODO: put in helper class
    std::string path;
    path = "../public" + filename;
    std::ifstream file(path);
    if (!file){
        std::cerr << "readFromHTML FAILED: Unable to Open File\n";
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
