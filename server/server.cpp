#include <cerrno>
#include <cstring>
#include <stdexcept>


#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

#include "server.h"

Server::Server(uint16_t port)
    : port_(port)
    , server_socket_(-1)
    , client_socket_(-1) {

  server_socket_ = socket(
      AF_INET,
      SOCK_STREAM,
      0);

  if (server_socket_ == -1) {
    throw std::runtime_error(
        "Failed to create server socket: " +
        std::string(std::strerror(errno)));
  }

  int reuse_address = 1;

  if (setsockopt(
          server_socket_,
          SOL_SOCKET,
          SO_REUSEADDR,
          &reuse_address,
          sizeof(reuse_address)) == -1) {
    close(server_socket_);
    server_socket_ = -1;

    throw std::runtime_error(
        "Failed to set socket options: " +
        std::string(std::strerror(errno)));
  }

  sockaddr_in server_address{};

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(port_);

  if (bind(
          server_socket_,
          reinterpret_cast<sockaddr*>(&server_address),
          sizeof(server_address)) == -1) {

    close(server_socket_);
    server_socket_ = -1;

    throw std::runtime_error(
        "Failed to bind server socket: " +
        std::string(std::strerror(errno)));
  }

  constexpr int kBacklog = 1;

  if (listen(server_socket_, kBacklog) == -1) {
    close(server_socket_);
    server_socket_ = -1;

    throw std::runtime_error(
        "Failed to listen on port " +
        std::to_string(port_) + ": " +
        std::string(std::strerror(errno)));
  }
}

void Server::AcceptClient() {
  client_socket_ = accept(
      server_socket_,
      nullptr,
      nullptr);

  if (client_socket_ == -1) {
    throw std::runtime_error(
        "Failed to accept client connection: " +
        std::string(std::strerror(errno)));
  }
}

Server::~Server() {
    if (server_socket_ != -1) {
        close(server_socket_);
    }
    if (client_socket_ != -1) {
        close(client_socket_);
    }
}

std::optional<std::string> Server::ReceiveMessage(std::chrono::milliseconds timeout) {

  const std::size_t message_end = receive_buffer_.find('\n');

  if (message_end != std::string::npos) {
    std::string message = receive_buffer_.substr(0, message_end);

    receive_buffer_.erase(0, message_end + 1);

    return message;
  }

  pollfd poll_fd{};

  poll_fd.fd = client_socket_;
  poll_fd.events = POLLIN;

  const int timeout_ms = static_cast<int>(timeout.count());

  const int result = poll(
      &poll_fd,
      1,
      timeout_ms);

  if (result == -1) {
    throw std::runtime_error(
        "Failed to poll socket: " +
        std::string(std::strerror(errno)));
  }

  if (result == 0) {
    return std::nullopt;
  }

  if (poll_fd.revents & (POLLERR | POLLNVAL)) {
    throw std::runtime_error("Socket error occurred");
  }

  if (poll_fd.revents & POLLHUP) {
    throw std::runtime_error("Client closed the connection");
  }

  if (poll_fd.revents & POLLIN) {
    constexpr std::size_t kBufferSize = 4096;

    char buffer[kBufferSize];

    const ssize_t bytes_received = recv(
        client_socket_,
        buffer,
        kBufferSize,
        0);

    if (bytes_received == -1) {
      throw std::runtime_error("Failed to receive message: " +
                                std::string(std::strerror(errno)));
    }

    if (bytes_received == 0) {
      throw std::runtime_error("Client closed the connection");
    }

    receive_buffer_.append(buffer, bytes_received);

    const std::size_t message_end = receive_buffer_.find('\n');

    if (message_end != std::string::npos) {
      std::string message = receive_buffer_.substr(0, message_end);

      receive_buffer_.erase(0, message_end + 1);

      return message;
    }
  }

  return std::nullopt;
}