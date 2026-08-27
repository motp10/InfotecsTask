#include <deque>
#include <chrono>

class StatisticApp {
public:
    StatisticApp(int messages_to_show, int period, int max_message_length, int min_message_length);
    void AddMessage(int message_length, ImportanceLevel importance);
    void PrintStatistics() const;
private:
    int messages_to_show;
    int period_;
    std::deque<std::chrono::system_clock::time_point> message_times_;
    int max_message_length_;
    int min_message_length_;
    int sum_message_length_;
    std::array<int, 3> importance_counts_;
};
