#include "httpHandler.h"

 std::string HTTPHandler::response(HTTPRequest &request) {
    if (request.getMethod().contains("GET")) {
        return handleGET(request);
    } else if (request.getMethod().contains("POST")) {
        return handlePOST(request);
    } else {
        std::cerr << "ERROR: Unhandled Method\n";
    }
}

std::string HTTPHandler::handleGET(HTTPRequest &request) {
    HTTPResponse httpResponse;
    std::string statusCode, statusText;
    std::pair<std::string,std::string> header;

    std::string file = readFromHTML(request.getURI());
    if (!file.empty()) {
        statusCode = "200";
        statusText = "OK";
        //TODO: handle MIME and headers more efficiently
        httpResponse.setHeader("Content-Length", std::to_string(file.length()));
        httpResponse.setHeader("Content-Type", httpResponse.getBodyMIMEType(request.getURI()));
        httpResponse.setBody(file);
    } else {
        statusCode = "404";
        statusText = "Not Found";
        httpResponse.setBody(readFromHTML("/404NotFound.html"));
    }

    httpResponse.setVersion(request.getVersion());
    httpResponse.setStatusCode(statusCode);
    httpResponse.setStatusText(statusText);
    return httpResponse.getResponse();
}

std::string HTTPHandler::handlePOST(HTTPRequest &request) {
    HTTPResponse httpResponse;
    std::string statusCode, statusText;
    std::pair<std::string,std::string> header;
    std::string fileLocation = "../public" + request.getURI() + "/users.txt";

    std::string file = readFromHTML("/index.html");
    if (writeToFile(fileLocation, request)){
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

std::string HTTPHandler::readFromHTML(const std::string& filename) { //TODO: put in helper class
    std::string path;
    path = "../public" + filename;
    std::ifstream file(path);
    if (!file){
        std::cerr << "readFromHTML FAILED: Unable to Open File " << path << '\n';
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool HTTPHandler::writeToFile(const std::string &filename, HTTPRequest &request) {
    std::ofstream file;
    file.open(filename);
    if (file.is_open()){
        file << parseForm(request.getBody());
        file.close();
        return true;
    } else {
        return false;
    }
}

std::string HTTPHandler::parseForm(const std::string &formData) { //TODO: make more reusable
    size_t nameEnd = formData.find('&');
    size_t userEnd = formData.find('%',nameEnd+1);
    std::string parsedForm = "==========================================\nName: " +
                             formData.substr(5,nameEnd-5) +
                             "\nEmail: " + formData.substr(nameEnd+7,userEnd-(7+nameEnd)) + '@' + formData.substr(userEnd+3);
    return parsedForm;
}
