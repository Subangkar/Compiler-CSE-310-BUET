# Assume your parser(yacc/bison) file with .y extention is named simplecalc.y. The followings are the sequence of commands you need to give to compile your parser and scanner(from previous assignment and modified). Find the details of the commands used here from manual of flex and bison. Some explanations are given here also.

bison -d -y -v parser.y	# -d creates the header file y.tab.h that helps in communication(i.e. pass tokens) between parser 					and scanner; -y is something similar to -o y.tab.c, that is it creates the parser; -v creates an 					.output file containing verbose descriptions of the parser states and all the conflicts, both those 					resolved by operator precedence and the unresolved ones
echo '1'
g++ -std=c++0x -w -c -o y.o y.tab.c	# -w stops the list of warnings from showing; -c compiles and assembles the c code, -o creates 					the y.o output file    

echo '2'
flex A.l		#creates the lexical analyzer or scanner named lex.yy.c

echo '3'
g++ -std=c++0x -w -c -o l.o lex.yy.c
# if the above command doesn't work try g++ -fpermissive -w -c -o l.o lex.yy.c

echo '4'
g++ -std=c++0x -o a.out y.o l.o -lfl -ly	#compiles the scanner and parser to create output file a.out; -lfl and -ly includes library files 					for lex and yacc(bison)

echo '5'
./a.out input.c		# you will need to provide proper input files with ./a.out command as instructed in assignment specification
