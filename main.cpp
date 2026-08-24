#include <string>
#include <thread>
#include <iostream>

#include "argument_parser/argument_parser.h"
#include "message_parser/message_parser.h"
#include "messages/message_queue.h"
#include "worker/worker.h"
#include "logger/logger.h"

int main(int argc, char* argv[]) {
    ArgumentParser argument_parser;
    MessageParser message_parser(ImportanceLevel::kMedium);
    MessageQueue message_queue;
    const auto parse_result = argument_parser.ParseCommandLineArguments(argc, argv);
    FileLogger logger(argument_parser.Options().log_file,
                          ImportanceLevel::kMedium);
    std::thread worker_thread([&message_queue, &argument_parser, &logger]() {
        Worker worker(message_queue, logger);
        worker.Run();
    });
    while (true) {
        std::cout << "Enter text\n";
        std::string text;
        if (!std::getline(std::cin, text)) {
            break;
        }

        std::cout << "Enter importance level "
                    "(low, medium, high; empty = default)\n";
        std::string level;
        if (!std::getline(std::cin, level)) {
            break;
        }

        const auto message = message_parser.ParseMessage(text, level);
        if (!message.has_value()) {
            std::cerr << message_parser.ErrorMessage() << '\n';
            continue;
        }

        message_queue.Push(*message);
    }
    return 0;
}