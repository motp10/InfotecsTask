#pragma once
#include <string>
#include <cstdint>

class Server {
public:
    Server(uint16_t port);
    ~Server();
    std::string ReceiveMessage();
    void AcceptClient();
    
private:
    uint16_t port_;
    int server_socket_;  
    int client_socket_; 
};