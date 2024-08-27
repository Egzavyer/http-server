#include "httpParser.h"

void HTTPParser::parseHTTPRequest(std::string &rawRequest, HTTPRequest request) {

    std::string requestLine = HTTPParser::extractRequestLine(rawRequest);
    std::pair<std::string,std::string> currentHeader;

    while (request.getState() != HTTPRequest::ParseState::DONE) {
        switch (request.getState()) {
            case HTTPRequest::ParseState::METHOD: {
                request.setMethod(HTTPParser::extractMethod(requestLine));
                std::cerr << request.getMethod() << '\n';
                request.setState(HTTPRequest::ParseState::URI);
                break;
            }
            case HTTPRequest::ParseState::URI: {
                request.setURI(HTTPParser::extractURI(requestLine));
                std::cerr << request.getURI() << '\n';
                request.setState(HTTPRequest::ParseState::DONE);
                break;
            }
            case HTTPRequest::ParseState::VERSION : {
                request.setVersion(HTTPParser::extractVersion(requestLine));
                break;
            }
            case HTTPRequest::ParseState::HEADERS: {
                request.setHeader(HTTPParser::extractHeader(rawRequest));
                break;
            }
            case HTTPRequest::ParseState::BODY: {
                request.setBody(HTTPParser::extractBody(rawRequest));
                break;
            }
            case HTTPRequest::ParseState::DONE: {
                std::cout << "Parsing Complete...\n";
                break;
            }
        }
    }
    std::cout << "Parsing Complete...\n";
}

std::string HTTPParser::extractRequestLine(std::string& request) {
    size_t requestLineEnd = request.find("\r\n");
    std::string requestLine = request.substr(0,requestLineEnd);
    request = request.substr(requestLineEnd+2);
    std::cout << requestLine << '\n';
    std::cout << request << '\n';

    return requestLine;
}

std::string HTTPParser::extractMethod(std::string &requestLine) {
    size_t methodEnd = requestLine.find(' ');
    std::string method = requestLine.substr(0,methodEnd);
    requestLine = requestLine.substr(methodEnd+1);
    return method;
}

std::string HTTPParser::extractURI(std::string &requestLine) {
    size_t uriEnd = requestLine.find(' ');
    std::string uri = requestLine.substr(0,uriEnd);
    requestLine = requestLine.substr(uriEnd+1);
    return uri;
}

std::string HTTPParser::extractVersion(std::string requestLine) {
    return std::string();
}

std::pair<std::string,std::string> HTTPParser::extractHeader(std::string request) {

}

std::string HTTPParser::extractBody(std::string request) {
    return std::string();
}
