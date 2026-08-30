#include <iomanip>
#include <netdb.h>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include "socket_logger.h"
#include "message_formater.h"

SocketLogger::SocketLogger(const std::string& host, uint16_t port,
                           ImportanceLevel default_importance_level)
    : socket_(-1)
    , default_importance_level_(default_importance_level) {
    const std::string port_string = std::to_string(port);

    addrinfo hints {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* result = nullptr;

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

    for (addrinfo* address = result;
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

    if (level < default_importance_level_) {
        return LogResult::kFiltered;
    }

    const FormatResult formatted_message = MessageFormatter::FormatMessage(message, level);

    if (!formatted_message.Ok()) {
        if (formatted_message.error == FormatError::kInvalidImportanceLevel) {
            return LogResult::kInvalidLevel;
        }

        return LogResult::kTimestampError;
    }

    return SendAll(formatted_message.text);
}

bool SocketLogger::SetImportanceLevel(
    ImportanceLevel new_level) {

    default_importance_level_ = new_level;

    return true;
}

LogResult SocketLogger::SendAll(const std::string& data) {
    std::size_t total_sent = 0;

    while (total_sent < data.size()) {
        const ssize_t bytes_sent = send(
            socket_,
            data.data() + total_sent,
            data.size() - total_sent,
            MSG_NOSIGNAL);

        if (bytes_sent <= 0) {
            return LogResult::kWriteError;
        }

        total_sent += static_cast<std::size_t>(bytes_sent);
    }

    return LogResult::kWritten;
}
