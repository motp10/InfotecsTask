#include <chrono>
#include <cstddef>
#include <iostream>

#include "argument_parsers/statistic_argument_parser.h"
#include "server/server.h"
#include "statistics/statistics.h"
#include "statistics/statistics_printer.h"
#include "common_lib/time_utils.h"
#include "message_deserializer/message_deserializer.h"

int main(int argc, char* argv[]) {
  constexpr uint16_t kPort = 8080;

  StatisticArgumentParser argument_parser;
  const ParseResult parse_result = argument_parser.ParseCommandLineArguments(argc, argv);

  if (parse_result == ParseResult::kHelpRequested) {
    std::cout << StatisticArgumentParser::HelpText();
    return 0;
  }
  if (parse_result != ParseResult::kOk) {
    std::cerr << argument_parser.ErrorMessage() << '\n';
    return 1;
  }

  const std::size_t messages_to_show = argument_parser.Options().messages_to_show;
  const auto period = std::chrono::seconds(argument_parser.Options().period_seconds);

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

    std::size_t messages_since_print = 0;
    auto next_print_time = std::chrono::steady_clock::now() + period;

    while (true) {
      const auto now = std::chrono::steady_clock::now();
      const auto time_to_poll = now < next_print_time
          ? std::chrono::duration_cast<std::chrono::milliseconds>(
                next_print_time - now)
          : std::chrono::milliseconds(0);

      const ReceiveResult receive_result = server.ReceiveMessage(time_to_poll);
      if (!receive_result.Ok()) {
        std::cerr << "Failed to receive message\n";
        return 1;
      }

      const auto& data = receive_result.message;

      if (data.has_value()) {
        const DeserializeResult result = deserializer.Deserialize(*data);
        if (!result.Ok()) {
          std::cerr << "Received an invalid message\n";
        } else {
          const Message& message = result.message;
          const auto received_at = CurrentTime();

          statistics.AddMessage(
              message.text.size(),
              message.level,
              received_at);

          ++messages_since_print;
        }
      }

      const auto current_time = std::chrono::steady_clock::now();
      if (messages_since_print >= messages_to_show ||
          current_time >= next_print_time) {
        printer.Print(statistics);
        messages_since_print = 0;
        next_print_time = current_time + period;
      }
    }

  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }

  return 0;
}
