#include "httpParser.h"

void HTTPParser::parseHTTPRequest(std::string &rawRequest, HTTPRequest &request) {

    std::string requestLine = HTTPParser::extractRequestLine(rawRequest);
    std::string currentHeader;

    //TODO: handle incomplete or bigger requests
    while (request.getState() != HTTPRequest::ParseState::DONE) {
        switch (request.getState()) {
            case HTTPRequest::ParseState::METHOD: {
                request.setMethod(HTTPParser::extractMethod(requestLine));
                std::cout << request.getMethod() << '\n';
                request.setState(HTTPRequest::ParseState::URI);
                break;
            }
            case HTTPRequest::ParseState::URI: {
                request.setURI(HTTPParser::extractURI(requestLine));
                std::cout << request.getURI() << '\n';
                request.setState(HTTPRequest::ParseState::VERSION);
                break;
            }
            case HTTPRequest::ParseState::VERSION : {
                request.setVersion(HTTPParser::extractVersion(requestLine));
                std::cout << request.getVersion() << '\n';
                request.setState(HTTPRequest::ParseState::HEADERS);
                break;
            }
            case HTTPRequest::ParseState::HEADERS: {
                currentHeader = HTTPParser::extractHeader(rawRequest);
                 do {
                    request.setHeader(HTTPParser::parseHeader(currentHeader));
                     currentHeader = HTTPParser::extractHeader(rawRequest);
                } while (!currentHeader.empty());
                HTTPParser::printHeaders(request.getHeaders());
                request.setState(HTTPRequest::ParseState::BODY);
                break;
            }
            case HTTPRequest::ParseState::BODY: {
                if (request.getHeaders().contains("Content-Length"))
                    request.setBody(rawRequest.substr(0,std::stoi(request.getHeaders()["Content-Length"])));
                //TODO: check if there is remaining, if yes error
                //TODO: handle different Transfer-Encoding (chunked)

                request.setState(HTTPRequest::ParseState::DONE);
                break;
            }
            case HTTPRequest::ParseState::DONE: {
                std::cout << "\nParsing Complete...\n";
                break;
            }
        }
    }
    std::cout << "\nParsing Complete...\n";
}

std::string HTTPParser::extractRequestLine(std::string& request) {
    size_t requestLineEnd = request.find("\r\n");
    std::string requestLine = request.substr(0,requestLineEnd);
    request = request.substr(requestLineEnd+2);
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

std::string HTTPParser::extractVersion(std::string &requestLine) {
    size_t versionEnd = requestLine.find("\r\n");
    std::string version = requestLine.substr(0,versionEnd);
    requestLine = requestLine.substr(versionEnd+2);
    return version;
}

std::string HTTPParser::extractHeader(std::string &request) {
    size_t headerEnd = request.find("\r\n");
    std::string header = request.substr(0, headerEnd);
    request = request.substr(headerEnd+2);
    return header;
}

std::pair<std::string, std::string> HTTPParser::parseHeader(std::string &header) {
    std::pair<std::string,std::string> currentHeader;

    size_t headerNameEnd = header.find(':');
    std::string headerName = header.substr(0,headerNameEnd);
    header = header.substr(headerNameEnd+2);

    currentHeader.first = headerName;
    currentHeader.second = header;
    return currentHeader;
}

void HTTPParser::printHeaders(std::unordered_map<std::string, std::string> headers) {
    for (const auto &header : headers)
        std::cout << header.first << " : " << header.second << '\n';
}
