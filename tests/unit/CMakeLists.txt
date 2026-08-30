add_executable(unit_tests
    unit_tests.cpp
    argument_parsers_test.cpp
    file_logger_test.cpp
    message_deserializer_test.cpp
    message_parser_test.cpp
    message_queue_test.cpp
    statistics_test.cpp
    ${PROJECT_SOURCE_DIR}/argument_parsers/file_argument_parser.cpp
    ${PROJECT_SOURCE_DIR}/argument_parsers/socket_argument_parser.cpp
    ${PROJECT_SOURCE_DIR}/argument_parsers/statistic_argument_parser.cpp
    ${PROJECT_SOURCE_DIR}/message_deserializer/message_deserializer.cpp
    ${PROJECT_SOURCE_DIR}/message_parser/message_parser.cpp
    ${PROJECT_SOURCE_DIR}/messages/message_queue.cpp
    ${PROJECT_SOURCE_DIR}/statistics/statistics.cpp
    ${PROJECT_SOURCE_DIR}/common_lib/importance_level.cpp
    ${PROJECT_SOURCE_DIR}/common_lib/time_utils.cpp
)

target_include_directories(unit_tests PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/common_lib
)
target_link_libraries(unit_tests PRIVATE Threads::Threads)
target_link_libraries(unit_tests PRIVATE fileLogger_static)

add_test(NAME unit_argument_parsers COMMAND unit_tests TestArgumentParsers)
add_test(NAME unit_file_logger COMMAND unit_tests TestFileLogger)
add_test(NAME unit_message_deserializer COMMAND unit_tests TestMessageDeserializer)
add_test(NAME unit_message_parser COMMAND unit_tests TestMessageParser)
add_test(NAME unit_message_queue COMMAND unit_tests TestMessageQueue)
add_test(NAME unit_statistics COMMAND unit_tests TestStatistics)
