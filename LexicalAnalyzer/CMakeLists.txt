cmake_minimum_required(VERSION 3.10)
project(LexicalAnalyzer)

set(CMAKE_CXX_STANDARD 11)

add_executable(LexicalAnalyzer DataStructure.h LexAnalyzer.cpp Utils.h LexBase.h)