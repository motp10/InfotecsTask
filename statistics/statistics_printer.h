#include "statistics.h"

class StatisticPrinter {
public:
    ~StatisticPrinter() = default;
    StatisticPrinter() = default;
    void Print(const Statistics& statistic) const;
};