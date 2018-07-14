cmake_minimum_required(VERSION 3.10)
project(IRGenerator)

set(CMAKE_CXX_STANDARD 11)

add_executable(IRGenerator DataStructure.h DSBasic.h 1505015.y 1505015.l LexBase.h SymbolInfo.h SynBase.h Utils.h 1505015.sh AsmBase.h ASMParser.h Dec16bitOutput.asm)