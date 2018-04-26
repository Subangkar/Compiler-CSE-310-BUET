flex -o wordcount.c wordcount.l
g++ wordcount.c -lfl -o wordcount.out
./wordcount.out wordcount.txt
