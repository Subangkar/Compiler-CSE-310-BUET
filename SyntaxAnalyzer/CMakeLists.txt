cmake_minimum_required(VERSION 3.10)
project(SyntaxAnalyzer)

set(CMAKE_CXX_STANDARD 11)

add_executable(SyntaxAnalyzer main.cpp DataStructure.h LexBase.h Utils.h SynBase.h SymTable.h DSBasic.h SymbolInfo.h)