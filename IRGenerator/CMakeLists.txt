cmake_minimum_required(VERSION 3.10)
project(IRGenerator)

set(CMAKE_CXX_STANDARD 11)

add_executable(IRGenerator DataStructure.h DSBasic.h GrammarParser.y LexAnalyzer.l LexBase.h SymbolInfo.h SynBase.h Utils.h yacc-bison.sh main.cpp AsmBase.h ASMParser.h)