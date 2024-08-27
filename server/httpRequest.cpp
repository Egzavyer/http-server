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

void HTTPRequest::setMethod(std::basic_string<char> requestMethod) {
    this->method = requestMethod;
}

std::string HTTPRequest::getURI() {
    return this->uri;
}

void HTTPRequest::setURI(std::basic_string<char> requestURI) {
    this->uri = requestURI;
}

std::string HTTPRequest::getVersion() {
    return this->version;
}

void HTTPRequest::setVersion(std::basic_string<char> requestVersion) {
    this->version = requestVersion;
}

void HTTPRequest::setHeader(std::pair<std::string, std::string> currentHeader) {
    this->headers[currentHeader.first] = currentHeader.second;
}

std::string HTTPRequest::getBody() {
    return this->body;
}

void HTTPRequest::setBody(std::basic_string<char> requestBody) {
    this->body = requestBody;
}

HTTPRequest::ParseState HTTPRequest::getState() {
    return this->state;
}

void HTTPRequest::setState(ParseState currentState) {
    this->state = currentState;
}
