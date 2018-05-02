#!/usr/bin/env bash
flex scanner.l
g++ lex.yy.c -lfl -o out.out
./out.out in.c
