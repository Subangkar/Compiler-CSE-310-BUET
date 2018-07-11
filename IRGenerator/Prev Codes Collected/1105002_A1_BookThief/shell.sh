    bison -d -y yacc.y
    g++ -w -c -o yacc.o y.tab.c
    flex lex.l
    g++ -fpermissive -w -c -o lex.o lex.yy.c
    g++ -o myOwnCompiler lex.o yacc.o -lfl -ly
    ./myOwnCompiler i.txt i_out.txt i_asm.txt 
