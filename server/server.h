#pragma once
#include <string>
#include <cstdint>
#include <optional>
#include <chrono>

enum class ServerResult {
    kOk,
    kAlreadyStarted,
    kSocketError,
    kSetOptionError,
    kBindError,
    kListenError,
    kPollError,
    kConnectionError,
    kConnectionClosed,
    kReceiveError,
};

struct ReceiveResult {
    std::optional<std::string> message;
    ServerResult result = ServerResult::kOk;

    bool Ok() const {
        return result == ServerResult::kOk;
    }
};

class Server {
public:
    Server(uint16_t port);
    ~Server();
    ServerResult Start();
    ReceiveResult ReceiveMessage(std::chrono::milliseconds timeout);
    void AcceptClient();
    
private:
    uint16_t port_;
    int server_socket_;  
    int client_socket_; 
    std::string receive_buffer_;
};
