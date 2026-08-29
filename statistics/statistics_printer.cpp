#include <iostream>

#include "statistics_printer.h"
void StatisticPrinter::Print(const Statistics& statistic) const {
  std::cout << "Statistics:\n";

  std::cout << "Total messages: "
            << statistic.TotalMessages()
            << '\n';

  std::cout << "Messages in the last hour: "
            << statistic.MessagesLastHour()
            << '\n';

  std::cout << "Minimum message length: "
            << statistic.MinMessageLength()
            << '\n';

  std::cout << "Maximum message length: "
            << statistic.MaxMessageLength()
            << '\n';

  std::cout << "Average message length: "
            << statistic.AverageMessageLength()
            << '\n';

  std::cout << "LOW: "
            << statistic.MessagesByImportance(ImportanceLevel::kLow)
            << '\n';

  std::cout << "MEDIUM: "
            << statistic.MessagesByImportance(ImportanceLevel::kMedium)
            << '\n';

  std::cout << "HIGH: "
            << statistic.MessagesByImportance(ImportanceLevel::kHigh)
            << '\n';
}