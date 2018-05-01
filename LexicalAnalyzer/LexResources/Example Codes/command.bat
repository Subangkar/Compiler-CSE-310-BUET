flex wordcount.l
g++ -c *.cpp
g++ -c *.c
g++ lex.yy.c -o wordcount.exe
wordcount.exe wordcount.txt
