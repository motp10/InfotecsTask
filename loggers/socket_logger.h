#pragma once
#include <string>
#include <cstdint>
#include "importance_level.h"
#include "i_logger.h"

class SocketLogger final : public ILogger {
public:
    SocketLogger(const std::string& host,
                 uint16_t port,
                 ImportanceLevel default_importance_level);
    ~SocketLogger();

    LogResult Log(const std::string& message,
                  ImportanceLevel level) override;

    bool SetImportanceLevel(ImportanceLevel new_level) override;

private:
    LogResult SendAll(const std::string& message);
    int socket_;
    ImportanceLevel default_importance_level_;
};
