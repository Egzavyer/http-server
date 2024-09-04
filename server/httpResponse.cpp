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
    return this->statusLine + Util::headersToString(this->headers) + this->body;
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

void HTTPResponse::setHeader(std::string name, std::string value) {
    this->headers[name] = value;
}

void HTTPResponse::setBody(std::string requestBody) {
    this->body = requestBody;
}
