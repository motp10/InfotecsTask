#pragma once
#include <string>
#include <cstdint>
#include <optional>
#include <chrono>
class Server {
public:
    Server(uint16_t port);
    ~Server();
    std::optional<std::string> ReceiveMessage(std::chrono::milliseconds timeout);
    void AcceptClient();
    
private:
    uint16_t port_;
    int server_socket_;  
    int client_socket_; 
    std::string receive_buffer_;
};