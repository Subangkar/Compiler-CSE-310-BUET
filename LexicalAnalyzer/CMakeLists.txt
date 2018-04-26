# cmake_minimum_required(VERSION <specify CMake version here>)
project(SymbolTable)

set(CMAKE_CXX_STANDARD 11)

add_executable(SymbolTable main.cpp DataStructures/ScopeTable/ScopeTable.cpp DataStructures/ScopeTable/ScopeTable.h DataTypes/SymbolInfo/SymbolInfo.cpp DataTypes/SymbolInfo/SymbolInfo.h DataStructures/SymbolTable/SymbolTable.cpp DataStructures/SymbolTable/SymbolTable.h DataStructures/LinkedList/LinkedList.cpp DataStructures/LinkedList/LinkedList.h DataStructures/HashTable/HashTable.cpp DataStructures/HashTable/HashTable.h Data/Const.h DataTypes/TypeDefn.h)