#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


/*
1. What is net programming?
net programming is program codes to make two separate computers to exchange stat
2. How to make it?
firstly, physical connection, that has been ensured
sencondly, software that is telling what's the rule
but, that is also been ensured, which is named "socket"
so the only thing we need to do is use the socket programming
*/

/*
socket networking has two types, firstly we use TCP
*/


/*
use socket to implemnet TCP 

1. create socket, which is like physical telephone

int socket(int domain, int type, int protocol);

2. bind the with an address, which is like: telephone bind with a number 
*/
int main(){
    /*
    1. creat socket{AF_INET means IPV4, SOCK_STREAM means TCP(SOCK_DGRAM means UDP)
    ,0 means default protocol
    the socket while return a file decriptor, which is only reprenting the socket
    */
    int server_fd =socket(AF_INET,SOCK_STREAM,0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    /*
    2. bind an address, and this address is ruled by a specific rule,
    and we regularly use a data structure "sockadddr_inaddress" to do that
    */
    struct sockaddr_in address;
    address.sin_family =AF_INET; // rule it is IPV4
    address.sin_addr.s_addr =INADDR_ANY; // rule it can listen all ips
    address.sin_port = htons(8080); // rule its port is 8080

    // question1 : why make address change into a sockaddr pointer
    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))< 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    /*
    3. listen, change the status be that anyone can call it
    */
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server listening on port " << 8080 << "...\n";
    /*
    4. accept. if someone call it ,just accept, then can exchange
    */
    socklen_t addrlen = sizeof(address);
    int new_socket=accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    // it is client
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Client connected.\n";
    /*
    5. now can exchange
    */
    char buffer[1024] = {0};
    while (true) {
        memset(buffer, 0, 1024);

        int valread = read(new_socket, buffer, 1024);

        if (valread <= 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        std::cout << "Received: " << buffer << std::endl;

        send(new_socket, buffer, strlen(buffer), 0); // return the stat
    }

    close(new_socket);
    close(server_fd);
    return 0;
    
}