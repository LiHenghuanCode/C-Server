#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){

    /*
    create socket
    */
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    /*
    */
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Connected to server.\n";
    /*
    */
    char buffer[1024] = {0};
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, 1024);
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, 1024);
        int valread = read(sock, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Server disconnected.\n";
            break;
        }
        std::cout << "Server: " << buffer << std::endl;
    }
    close(sock);
    return 0;

}