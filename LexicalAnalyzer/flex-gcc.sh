#!/usr/bin/env bash
flex -o LexAnalyzer.c LexAnalyzer.l
g++ LexAnalyzer.c -lfl -o LexAnalyzer.out
./LexAnalyzer.out in.c
