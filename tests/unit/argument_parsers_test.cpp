#include <vector>

#include "argument_parsers/file_argument_parser.h"
#include "argument_parsers/socket_argument_parser.h"
#include "argument_parsers/statistic_argument_parser.h"
#include "test_framework.h"

// Проверяет успешный разбор обязательных параметров файлового и сетевого логгеров.
void TestArgumentParsers() {
  FileArgumentParser file_parser;
  std::vector<char*> file_arguments{
      const_cast<char*>("file_logger"), const_cast<char*>("--file"),
      const_cast<char*>("application.log"), const_cast<char*>("--level"),
      const_cast<char*>("high")};
  CHECK(file_parser.ParseCommandLineArguments(
            static_cast<int>(file_arguments.size()), file_arguments.data()) ==
        ParseResult::kOk);
  CHECK(file_parser.Options().log_file == "application.log");
  CHECK(file_parser.Options().level == ImportanceLevel::kHigh);

  SocketArgumentParser socket_parser;
  std::vector<char*> socket_arguments{
      const_cast<char*>("socket_logger"), const_cast<char*>("--host"),
      const_cast<char*>("127.0.0.1"), const_cast<char*>("--port"),
      const_cast<char*>("8080"), const_cast<char*>("--level"),
      const_cast<char*>("low")};
  CHECK(socket_parser.ParseCommandLineArguments(
            static_cast<int>(socket_arguments.size()), socket_arguments.data()) ==
        ParseResult::kOk);
  CHECK(socket_parser.Options().host == "127.0.0.1");
  CHECK(socket_parser.Options().port == 8080);
  CHECK(socket_parser.Options().level == ImportanceLevel::kLow);

  FileArgumentParser missing_file_parser;
  std::vector<char*> missing_file_arguments{
      const_cast<char*>("file_logger"), const_cast<char*>("--level"),
      const_cast<char*>("low")};
  CHECK(missing_file_parser.ParseCommandLineArguments(
            static_cast<int>(missing_file_arguments.size()),
            missing_file_arguments.data()) == ParseResult::kError);
  CHECK(missing_file_parser.ErrorMessage() == "The: --file option is required");

  SocketArgumentParser invalid_port_parser;
  std::vector<char*> invalid_port_arguments{
      const_cast<char*>("socket_logger"), const_cast<char*>("--host"),
      const_cast<char*>("localhost"), const_cast<char*>("--port"),
      const_cast<char*>("70000"), const_cast<char*>("--level"),
      const_cast<char*>("medium")};
  CHECK(invalid_port_parser.ParseCommandLineArguments(
            static_cast<int>(invalid_port_arguments.size()),
            invalid_port_arguments.data()) == ParseResult::kError);

  StatisticArgumentParser statistic_parser;
  std::vector<char*> statistic_arguments{
      const_cast<char*>("statistic_app"), const_cast<char*>("-t"),
      const_cast<char*>("5"), const_cast<char*>("-n"),
      const_cast<char*>("10")};
  CHECK(statistic_parser.ParseCommandLineArguments(
            static_cast<int>(statistic_arguments.size()),
            statistic_arguments.data()) == ParseResult::kOk);
  CHECK(statistic_parser.Options().period_seconds == 5);
  CHECK(statistic_parser.Options().messages_to_show == 10);

  StatisticArgumentParser invalid_statistic_parser;
  std::vector<char*> invalid_statistic_arguments{
      const_cast<char*>("statistic_app"), const_cast<char*>("-t"),
      const_cast<char*>("0"), const_cast<char*>("-n"),
      const_cast<char*>("10")};
  CHECK(invalid_statistic_parser.ParseCommandLineArguments(
            static_cast<int>(invalid_statistic_arguments.size()),
            invalid_statistic_arguments.data()) == ParseResult::kError);
}
