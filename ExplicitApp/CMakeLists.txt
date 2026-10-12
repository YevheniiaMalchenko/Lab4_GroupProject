cmake_minimum_required(VERSION 3.20)
project(ExplicitApp LANGUAGES CXX)

add_executable(ExplicitApp main.cpp)

add_custom_command(TARGET ExplicitApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/AnomalyDetection.dll"
        "$<TARGET_FILE_DIR:ExplicitApp>"
)