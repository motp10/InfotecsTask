cmake_minimum_required(VERSION 3.16)

project(infotexs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
include(CTest)

set(COMMON_LOGGER_SOURCES
    common_lib/importance_level.cpp
    common_lib/time_utils.cpp
    loggers/message_formater.cpp
)

add_library(fileLogger_static STATIC
    loggers/file_logger.cpp
    ${COMMON_LOGGER_SOURCES}
)
set_target_properties(fileLogger_static PROPERTIES OUTPUT_NAME fileLogger)

add_library(fileLogger_shared SHARED
    loggers/file_logger.cpp
    ${COMMON_LOGGER_SOURCES}
)
set_target_properties(fileLogger_shared PROPERTIES OUTPUT_NAME fileLogger)

foreach(target fileLogger_static fileLogger_shared)
    target_include_directories(${target}
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/common_lib
    )
endforeach()

add_executable(file_logger
    file_logger.cpp
    argument_parsers/file_argument_parser.cpp
    message_parser/message_parser.cpp
    messages/message_queue.cpp
    worker/worker.cpp
)
target_link_libraries(file_logger PRIVATE fileLogger_shared Threads::Threads)

add_executable(socket_logger
    socket_logger.cpp
    argument_parsers/socket_argument_parser.cpp
    message_parser/message_parser.cpp
    messages/message_queue.cpp
    worker/worker.cpp
    loggers/socket_logger.cpp
    ${COMMON_LOGGER_SOURCES}
)
target_include_directories(socket_logger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common_lib
)
target_link_libraries(socket_logger PRIVATE Threads::Threads)

add_executable(statistic_app
    statistic_app.cpp
    argument_parsers/statistic_argument_parser.cpp
    server/server.cpp
    message_deserializer/message_deserializer.cpp
    statistics/statistics.cpp
    statistics/statistics_printer.cpp
    common_lib/importance_level.cpp
    common_lib/time_utils.cpp
)
target_include_directories(statistic_app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common_lib
)

if(BUILD_TESTING)
    add_subdirectory(tests/unit)
endif()
