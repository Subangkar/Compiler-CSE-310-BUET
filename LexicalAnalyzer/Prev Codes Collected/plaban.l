%{
extern "C"
{ extern int yylex();
}

#include<stdlib.h>
#include<stdio.h>
#include<FlexLexer.h>
#include<string.h>
#include<iostream>
#include"SymbolTable.h"

#define ID (char *)"ID"
#define NUM (char *)"NUMBER"
#define RELOP (char *)"RELOP"
#define ADDOP (char *)"ADDOP"
#define MULOP (char *)"MULOP"
#define ASSIGNOP (char *)"ASSIGNOP"

using namespace std;

int idCount=0;
int lineCount=1;
SymbolTable symbolTable(10);
%}


alpha_ [a-zA-Z_]
alphanum_ [a-zA-Z0-9_]
digit [0-9]
relop (=)|(<>)|(<)|(<=)|(>=)|(>)

number {digit}+(\.{digit}+)?+((E|e)[+-]?{digit}+)?

variable {alpha_}{alphanum_}*

all_char (.|\n)


%%
[\n]	{ lineCount++; ECHO; }

[ \t\n]	{ECHO;}

(\/\*){all_char}*(\*\/)	{cout << "C_Comments ";}

\{[^}{]*\}	{cout << "Pascle Comments ";}

\"{all_char}*\"	{cout << "STRING ";}

{relop}	{ cout << "RELOP_" << yytext << " "; symbolTable.Insert(yytext,RELOP); }
[+-]|(or)	{ cout << "ADDOP_" << yytext << " "; symbolTable.Insert(yytext,ADDOP); }
[\*\/]|(div)|(mod)|(and)	{ cout << "MULOP_" << yytext <<" "; symbolTable.Insert(yytext,MULOP); }
(:=)	{ cout << "ASSIGNOP_"; symbolTable.Insert(yytext,ASSIGNOP); } 

\[	{ cout << "OPENING_SQARE_BRACKET "; }
\]	{ cout << "CLOSING_SQARE_BRACKET "; }
\(	{ cout << "OPENING_PARANTHESIS "; }
\)	{ cout << "CLOSING_PARANTHESIS "; }
\.\.	{ cout << "DOTDOT "; }
,	{ cout << "COMMA "; }
;	{ cout << "SEMICOLON "; }
:	{ cout << "COLON "; }

program	{ cout << "PROGRAM_"<<lineCount<<" "; }
if	{ cout << "IF_"<<lineCount<<" "; }
not	{ cout << "NOT_"<<lineCount<<" "; }
end	{ cout << "END_"<<lineCount<<" "; }
begin	{ cout << "BEGIN_"<<lineCount<<" "; }
else	{ cout << "ELSE_"<<lineCount<<" "; }
then	{ cout << "THEN_"<<lineCount<<" "; }
do	{ cout << "DO_"<<lineCount<<" "; }
while	{ cout << "WHILE_"<<lineCount<<" "; }
function	{ cout << "FUNCTION_"<<lineCount<<" "; }
Procedure	{ cout << "PROCEDURE_"<<lineCount<<" "; }
integer	{ cout << "INTEGER_"<<lineCount<<" "; }
real	{ cout << "REAL_"<<lineCount<<" "; }
var	{ cout << "VAR_"<<lineCount<<" "; }
oh	{ cout << "OH_"<<lineCount<<" "; }
array	{ cout << "ARRAY_"<<lineCount<<" "; }
write	{ cout << "WRITE_"<<lineCount<<" "; }

{variable}	{ if(symbolTable.Insert(yytext,ID)) cout << "ID "; else cout << "ID_EXISTS "; }

{number}	{ cout << "NUM "; symbolTable.Insert(yytext,NUM); }
%%

int main(int argc, char **argv)
{
	yylex();
	symbolTable.Dump();
	cout<<"Total line in source: "<<lineCount<<endl;
	return 0;
}


