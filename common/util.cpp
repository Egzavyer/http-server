#include "util.h"

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

std::string Util::parseForm(const std::string &formData) { //TODO: make more reusable
    size_t nameEnd = formData.find('&');
    size_t userEnd = formData.find('%',nameEnd+1);
    std::string parsedForm = "==========================================\nName: " +
                             formData.substr(5,nameEnd-5) +
                             "\nEmail: " + formData.substr(nameEnd+7,userEnd-(7+nameEnd)) + '@' + formData.substr(userEnd+3);
    return parsedForm;
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