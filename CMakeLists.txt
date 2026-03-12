cmake_minimum_required(VERSION 3.10)
project(TerminalSnake)

find_package(Curses REQUIRED)

add_executable(snake main.c)

target_link_libraries(snake ${CURSES_LIBRARIES})
target_include_directories(snake PRIVATE ${CURSES_INCLUDE_DIR})
target_compile_definitions(snake PRIVATE NCURSES_STATIC)