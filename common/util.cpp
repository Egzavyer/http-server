#include "util.h"
#include "common.h"
#include <sstream>

std::string Util::readFromHTML(const std::string& filename) {
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

bool Util::writeToFile(const std::string &filename, HTTPRequest &request) {
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

std::string Util::parseForm(std::string formData) {

    while (formData.contains('+') || formData.contains('&') || formData.contains("%40")) {
        size_t spacePos = formData.find('+');
        size_t separationPos = formData.find('&');
        size_t atPos = formData.find("%40");

        if (spacePos != std::string::npos)
            formData[spacePos] = ' ';
        if (separationPos != std::string::npos)
            formData[separationPos] = '\n';
        if (atPos != std::string::npos) {
            formData[atPos] = '@';
            std::string domain = formData.substr(atPos+3);
            formData = formData.substr(0,atPos+1);
            formData += domain;
        }
    }
    return formData;
}

std::string Util::headersToString(const std::unordered_map<std::string, std::string> &responseHeaders) {
    std::string headerString;
    for (const auto &header : responseHeaders) {
        headerString += "\r\n" + header.first + ": " + header.second;
    }
    return headerString + "\r\n\r\n";
}

std::string Util::getBodyMIMEType(const std::string &requestURI) {
    size_t fileExtensionStart = requestURI.find('.');
    std::string fileExtension = requestURI.substr(fileExtensionStart+1);
    if (fileExtension == "html") {
        return "text/html";
    } else {
        return "image/webp";
    }
}