#!/usr/bin/env bash
flex -o LexAnalyzer.cpp LexAnalyzer.l
g++ LexAnalyzer.cpp -lfl -o LexAnalyzer.out
./LexAnalyzer.out "in.c"
# "/home/subangkar/Desktop/Compiler/LexicalAnalyzer/LexResources/sample io/Sample 2/input.txt"
#"LexResources/sample io/input1.txt"
