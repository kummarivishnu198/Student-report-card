cmake_minimum_required(VERSION 3.16)
project(StudentReportCardSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SQLite3 REQUIRED)

add_executable(student_report_card
    src/main.cpp
    src/Database.cpp
)

target_include_directories(student_report_card PRIVATE include)
target_link_libraries(student_report_card PRIVATE SQLite::SQLite3)

if(MSVC)
    target_compile_options(student_report_card PRIVATE /W4)
else()
    target_compile_options(student_report_card PRIVATE -Wall -Wextra -Wpedantic)
endif()
