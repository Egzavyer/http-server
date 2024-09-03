#include "httpResponse.h"

HTTPResponse::HTTPResponse() {
    statusLine;
    version;
    statusCode;
    statusText;
    headers;
    body;
}

std::string HTTPResponse::getResponse() {
    this->statusLine = this->version + ' ' + this->statusCode + ' ' + this->statusText;
    return this->statusLine + this->headersToString(this->headers) + this->body;
}

void HTTPResponse::setVersion(std::string version) {
    this->version = version;
}

void HTTPResponse::setStatusCode(std::string code) {
    this->statusCode = code;
}

void HTTPResponse::setStatusText(std::string text) {
    this->statusText = text;
}

void HTTPResponse::setHeader(std::pair<std::string, std::string> currentHeader) {
    this->headers[currentHeader.first] = currentHeader.second;
}

void HTTPResponse::setBody(std::string requestBody) {
    this->body = requestBody;
}

std::string HTTPResponse::headersToString(const std::unordered_map<std::string, std::string> &responseHeaders) {
    std::string headerString;
    for (const auto &header : responseHeaders) {
        headerString += "\r\n" + header.first + ": " + header.second;
    }
    return headerString + "\r\n\r\n";
}

std::string HTTPResponse::getBodyMIMEType(const std::string &requestURI) {
    size_t fileExtensionStart = requestURI.find('.');
    std::string fileExtension = requestURI.substr(fileExtensionStart+1);
    if (fileExtension == "html") {
        return "text/html";
    } else {
        return "image/webp";
    }
}
