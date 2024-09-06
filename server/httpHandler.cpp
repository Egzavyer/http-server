#include "httpHandler.h"
#include "../common/common.h"
#include "httpResponse.h"

std::string HTTPHandler::response(HTTPRequest &request) {
    if (request.getMethod() == "GET") {
        return handleGET(request);
    } else if (request.getMethod() == "POST") {
        return handlePOST(request);
    } else {
        std::cerr << "ERROR: Unhandled Method\n";
        return "";
    }
}

std::string HTTPHandler::handleGET(HTTPRequest &request) {
    HTTPResponse httpResponse;
    std::string statusCode, statusText;
    std::pair<std::string, std::string> header;



    std::string file = Util::readFromHTML(request.getURI());
    if (!file.empty()) {
        statusCode = "200";
        statusText = "OK";
        //TODO: handle MIME and headers more efficiently
        httpResponse.setHeader("Content-Length", std::to_string(file.length()));
        httpResponse.setHeader("Content-Type", Util::getBodyMIMEType(request.getURI()));
        httpResponse.setBody(file);
    } else {
        statusCode = "404";
        statusText = "Not Found";
        httpResponse.setBody(Util::readFromHTML("/404NotFound.html"));
    }

    httpResponse.setVersion(request.getVersion());
    httpResponse.setStatusCode(statusCode);
    httpResponse.setStatusText(statusText);
    return httpResponse.getResponse();
}

std::string HTTPHandler::handlePOST(HTTPRequest &request) {
    HTTPResponse httpResponse;
    std::string statusCode, statusText;
    std::pair<std::string, std::string> header;
    std::string fileLocation = "../public" + request.getURI() + "/users.txt";

    std::string file = Util::readFromHTML("/index.html");
    if (Util::writeToFile(fileLocation, request)) {
        statusCode = "201";
        statusText = "Created";
        httpResponse.setBody(file);
    } else {
        statusCode = "500";
        statusText = "Internal Server Error";
    }

    httpResponse.setHeader("Location", fileLocation);
    httpResponse.setHeader("Content-Type", "text/html");
    httpResponse.setHeader("Content-Length", std::to_string(file.length()));

    httpResponse.setVersion(request.getVersion());
    httpResponse.setStatusCode(statusCode);
    httpResponse.setStatusText(statusText);
    return httpResponse.getResponse();
}
