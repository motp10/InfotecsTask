#include "logger.h"

#include <cerrno>
#include <cstring>
#include <iomanip>
#include <netdb.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

SocketLogger::SocketLogger(const std::string& host, uint16_t port,
                           ImportanceLevel default_importance_level)
    : socket_(-1),
      default_importance_level_(default_importance_level) {
    const std::string port_string = std::to_string(port);

    struct addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* result = nullptr;

    const int status = getaddrinfo(
        host.c_str(),
        port_string.c_str(),
        &hints,
        &result);

    if (status != 0) {
        throw std::runtime_error(
            "Failed to resolve host: " +
            std::string(gai_strerror(status)));
    }

    for (struct addrinfo* address = result;
         address != nullptr;
         address = address->ai_next) {

        socket_ = socket(
            address->ai_family,
            address->ai_socktype,
            address->ai_protocol);

        if (socket_ == -1) {
            continue;
        }

        if (connect(
                socket_,
                address->ai_addr,
                address->ai_addrlen) == 0) {
            break;
        }

        close(socket_);
        socket_ = -1;
    }

    freeaddrinfo(result);

    if (socket_ == -1) {
        throw std::runtime_error(
            "Failed to connect to " + host + ":" + port_string);
    }
}

SocketLogger::~SocketLogger() {
    if (socket_ != -1) {
        close(socket_);
    }
}

LogResult SocketLogger::Log(
    const std::string& message,
    ImportanceLevel level) {

    std::lock_guard<std::mutex> lock(mutex_);

    if (level < default_importance_level_) {
        return LogResult::Filtered;
    }

    const std::string formatted_message =
        FormatMessage(message, level);

    SendAll(formatted_message);

    return LogResult::Logged;
}

bool SocketLogger::SetImportanceLevel(
    ImportanceLevel new_level) {

    std::lock_guard<std::mutex> lock(mutex_);

    default_importance_level_ = new_level;

    return true;
}

void SocketLogger::SendAll(const std::string& data) {
    std::size_t total_sent = 0;

    while (total_sent < data.size()) {
        const ssize_t bytes_sent = send(
            socket_,
            data.data() + total_sent,
            data.size() - total_sent,
            MSG_NOSIGNAL);

        if (bytes_sent == -1) {
            throw std::runtime_error(
                "Failed to send log message: " +
                std::string(std::strerror(errno)));
        }

        if (bytes_sent == 0) {
            throw std::runtime_error(
                "Socket connection was closed");
        }

        total_sent += static_cast<std::size_t>(bytes_sent);
    }
}

std::string SocketLogger::FormatMessage(
    const std::string& message,
    ImportanceLevel level) const {

    std::ostringstream output;

    const std::time_t now = std::time(nullptr);
    std::tm local_time{};

    if (localtime_r(&now, &local_time) == nullptr) {
        throw std::runtime_error(
            "Failed to get local time");
    }

    output << '['
           << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S")
           << "] [";

    switch (level) {
        case ImportanceLevel::LOW:
            output << "LOW";
            break;

        case ImportanceLevel::MEDIUM:
            output << "MEDIUM";
            break;

        case ImportanceLevel::HIGH:
            output << "HIGH";
            break;
    }

    output << "] " << message << '\n';

    return output.str();
}