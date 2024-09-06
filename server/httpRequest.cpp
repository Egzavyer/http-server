#include "httpRequest.h"

HTTPRequest::HTTPRequest() {
    method = "";
    uri = "";
    version = "";
    headers;
    body = "";
    state = ParseState::METHOD;
}

std::string HTTPRequest::getMethod() {
    return this->method;
}

void HTTPRequest::setMethod(std::string requestMethod) {
    this->method = requestMethod;
}

std::string HTTPRequest::getURI() {
    return this->uri;
}

void HTTPRequest::setURI(std::string requestURI) {
    this->uri = requestURI;
}

std::string HTTPRequest::getVersion() {
    return this->version;
}

void HTTPRequest::setVersion(std::string requestVersion) {
    this->version = requestVersion;
}

std::unordered_map<std::string, std::string> HTTPRequest::getHeaders() {
    return this->headers;
}

void HTTPRequest::setHeader(std::pair<std::string, std::string> currentHeader) {
    this->headers[currentHeader.first] = currentHeader.second;
}

std::string HTTPRequest::getBody() {
    return this->body;
}

void HTTPRequest::setBody(std::string requestBody) {
    this->body = requestBody;
}

HTTPRequest::ParseState HTTPRequest::getState() {
    return this->state;
}

void HTTPRequest::setState(const ParseState currentState) {
    this->state = currentState;
}
