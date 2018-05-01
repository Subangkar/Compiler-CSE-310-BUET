# cmake_minimum_required(VERSION <specify CMake version here>)
project(LexicalAnalyzer)

set(CMAKE_CXX_STANDARD 11)

add_executable(LexicalAnalyzer lex.yy.c DataStructure.h)