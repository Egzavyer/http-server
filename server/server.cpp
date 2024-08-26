#include "server.h"
#include "../common/common.h"
#pragma comment(lib, "ws2_32.lib")

int main() {
    Connection connection;
    std::cout << connection.setupConnection() << '\n';
    std::cout << connection.getClientSocket() << '\n';
    std::cout << connection.getListenSocket() << '\n';
}