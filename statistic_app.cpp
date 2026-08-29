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
    MessageDeserializer deserializer;
    Statistics statistics;
    StatisticPrinter printer;
    server.AcceptClient();

    auto last_print_time = CurrentTime();
    std::size_t messages_at_last_print = 0;

    while (true) {
      const auto data = server.ReceiveMessage(kPeriod);

      if (data.has_value()) {
        const Message message =
            deserializer.Deserialize(*data);

        const auto received_at = CurrentTime();

        statistics.AddMessage(
            message.text.size(),
            message.level,
            received_at);

        const std::size_t messages_since_print =
            statistics.TotalMessages() - messages_at_last_print;

        if (messages_since_print >= kMessagesToShow) {
          printer.Print(statistics);
          last_print_time = CurrentTime();
          messages_at_last_print = statistics.TotalMessages();
        }
      } else {
        const auto now = CurrentTime();

        if (now - last_print_time >= kPeriod &&
            statistics.TotalMessages() > messages_at_last_print) {
          printer.Print(statistics);

          last_print_time = now;
          messages_at_last_print = statistics.TotalMessages();
        }
      }
    }

  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}