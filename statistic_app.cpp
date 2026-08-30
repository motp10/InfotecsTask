#include <chrono>
#include <cstddef>
#include <iostream>

#include "server/server.h"
#include "statistics/statistics.h"
#include "statistics/statistics_printer.h"
#include "common_lib/time_utils.h"
#include "message_deserializer/message_deserializer.h"

int main() {
  constexpr uint16_t kPort = 8080;
  constexpr std::size_t kMessagesToShow = 10;
  constexpr auto kPeriod = std::chrono::seconds(5);
  

  try {
    Server server(kPort);
    if (server.Start() != ServerResult::kOk) {
      std::cerr << "Failed to start server\n";
      return 1;
    }

    MessageDeserializer deserializer;
    Statistics statistics;
    StatisticPrinter printer;
    server.AcceptClient();

    auto last_print_time = CurrentTime();
    std::size_t messages_since_print = 0;
    auto time_to_poll = std::chrono::duration_cast<std::chrono::milliseconds>(kPeriod);

    while (true) {
      const ReceiveResult receive_result = server.ReceiveMessage(time_to_poll);
      if (!receive_result.Ok()) {
        std::cerr << "Failed to receive message\n";
        return 1;
      }

      const auto& data = receive_result.message;

      const auto time_from_last_print =
        std::chrono::duration_cast<std::chrono::milliseconds>(
          CurrentTime() - last_print_time);

      time_to_poll =
        std::chrono::duration_cast<std::chrono::milliseconds>(
              kPeriod) - time_from_last_print;

      if (data.has_value()) {
        const DeserializeResult result = deserializer.Deserialize(*data);
        if (!result.Ok()) {
          std::cerr << "Received an invalid message\n";
          continue;
        }

        const Message& message = result.message;

        const auto received_at = CurrentTime();

        statistics.AddMessage(
            message.text.size(),
            message.level,
            received_at);

        ++messages_since_print;

        if (messages_since_print >= kMessagesToShow) {
            printer.Print(statistics);

            messages_since_print = 0;
        }
      } else {
        const auto now = CurrentTime();

        if (time_to_poll <= std::chrono::milliseconds(0)) {
          printer.Print(statistics);

          last_print_time = now;
        }
      }
    }

  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}
