#include "server.h"
#include "../common/common.h"
#pragma comment(lib, "ws2_32.lib")

int main() {
    Connection connection;
    if (connection.setupConnection())
        std::cout << "Successful Connection on: " << connection.address << '\n';
}