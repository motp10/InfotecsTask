#include <ctime>
#include <iomanip>

#include "time_utils.h"

std::string CurrentLocalTime() {
    const std::time_t now = std::time(nullptr);
    std::tm local_time{};

    localtime_r(&now, &local_time);
    if (localtime_r(&now, &local_time) == nullptr) {
        return {};
    }
    
    std::ostringstream stream;
    stream << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");

    return stream.str();
}