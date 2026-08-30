#pragma once
#include <string>
#include <cstdint>
#include "importance_level.h"
#include "i_logger.h"

enum class SocketLoggerResult {
    kOk,
    kInvalidLevel,
    kResolveError,
    kSocketError,
    kConnectError,
};

class SocketLogger final : public ILogger {
public:
    SocketLogger(const std::string& host,
                 uint16_t port,
                 ImportanceLevel default_importance_level);
    ~SocketLogger();

    SocketLoggerResult Connect();
    LogResult Log(const std::string& message,
                  ImportanceLevel level) override;

    bool SetImportanceLevel(ImportanceLevel new_level) override;

private:
    LogResult SendAll(const std::string& message);
    std::string host_;
    uint16_t port_;
    int socket_;
    ImportanceLevel default_importance_level_;
};
