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
    this->setStatusLine();
    return this->statusLine + this->headersToString(this->headers) + this->body;
}

std::string HTTPResponse::getStatusLine() {
    return this->statusLine;
}

void HTTPResponse::setStatusLine() {
    this->statusLine = this->version + ' ' + this->statusCode + ' ' + this->statusText;
}

std::string HTTPResponse::getVersion() {
    return this->version;
}

void HTTPResponse::setVersion(std::string version) {
    this->version = version;
}

std::string HTTPResponse::getStatusCode() {
    return this->statusCode;
}

void HTTPResponse::setStatusCode(std::string code) {
    this->statusCode = code;
}

std::string HTTPResponse::getStatusText() {
    return this->statusText;
}

void HTTPResponse::setStatusText(std::string text) {
    this->statusText = text;
}

std::unordered_map<std::string, std::string> HTTPResponse::getHeaders() {
    return this->headers;
}

void HTTPResponse::setHeader(std::pair<std::string, std::string> currentHeader) {
    this->headers[currentHeader.first] = currentHeader.second;
}

std::string HTTPResponse::getBody() {
    return this->body;
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
