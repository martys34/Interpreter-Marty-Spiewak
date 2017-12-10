cmake_minimum_required(VERSION 3.8)
project(Interpreter_Marty_Spiewak)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES interpreter.c structures.h firsts.h variables.h)
add_executable(Interpreter_Marty_Spiewak ${SOURCE_FILES})