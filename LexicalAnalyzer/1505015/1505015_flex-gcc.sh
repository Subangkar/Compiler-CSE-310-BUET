#!/usr/bin/env bash
flex -o LexAnalyzer.cpp 1505015.l
g++ LexAnalyzer.cpp -std=c++11 -lfl -o LexAnalyzer.out
./LexAnalyzer.out "input.txt"
# "/home/subangkar/Desktop/Compiler/LexicalAnalyzer/LexResources/sample io/Sample 2/input.txt"
#"LexResources/sample io/input1.txt"