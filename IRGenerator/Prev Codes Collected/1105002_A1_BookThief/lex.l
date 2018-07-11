%{
#include "symboltable.h"
#include "y.tab.h"
extern YYSTYPE yylval;

using namespace std;

extern SymbolTable st;

int count=0;


%}

WS [ \t]+
NEWLINE [\n]
ASSIGNOP ":="
BRACKET1 [\[]
BRACKET2 [\]]
CURLEY1 [\{]
CURLEY2 [\}]
PAREN1 [\(]
PAREN2 [\)]
COMMA ,
SEMICOLON ;
MULOP1 [*]
MULOP2 [/]
ADDOP1 [+]
ADDOP2 [-]
LESS <(>|=)?
EQUAL (==)|(!=)
OR \|\|
AND \&\&
GREAT >(=)?
RELOP ({LESS}|{EQUAL}|{GREAT}) 
DIGIT [0-9]
NUM {DIGIT}+(\.{DIGIT}+)?(E[+-]?{DIGIT}+(\.{DIGIT}+)?)?
LETTER [A-Za-z]
UNDERSCORE [_]
ID ({LETTER}|{UNDERSCORE})+({LETTER}|{DIGIT}|{UNDERSCORE})*
ADDOP {ADDOP1}|{ADDOP2}
MULOP {MULOP1}|{MULOP2}
%%
"integer"  {return INTEGER; }
"real"     {return REAL;}
"function" {return FUNCTION;}
"procedure" {return PROCEDURE;}
"begin"    {return BEGIN1; }
"end"      {return END; }
"var"       {return VAR; }
"array"	    {return ARRAY;}	
"if"  	   {return IF;}
"then"     {return THEN; }
"else"     {return ELSE; }
"do" 	   {return DO; }
"while"    {return WHILE; }
"write"    {return WRITE;}
"program"  {return PROGRAM; }
"of"       {return OF; }
"mod"      { st.insert(SymbolInfo(yytext,"MULOP"));
	yylval=(YYSTYPE)st.uplook(yytext);
return MULOP;}
{WS} {}
{NEWLINE} {
	count++;
}
{OR} {
	return OR;
	}
{AND} {
	return AND;
	}
{COMMA} {return COMMA;}
{ID}  {
    st.insert(SymbolInfo(yytext,"ID"));
	yylval=(YYSTYPE)st.uplook(yytext);
	
	return ID;
}
{NUM} {
    st.insert(SymbolInfo(yytext,"NUM"));
	yylval=(SymbolInfo*)st.uplook(yytext);
	return NUM;
}
{RELOP} {
    st.insert(SymbolInfo(yytext,"RELOP"));
	yylval=(YYSTYPE)st.uplook(yytext);
	
	return RELOP;
}
{ADDOP} {
    st.insert(SymbolInfo(yytext,"ADDOP"));
	yylval=(YYSTYPE)st.uplook(yytext);
	
	return ADDOP;
}
{MULOP}  {
    st.insert(SymbolInfo(yytext,"MULOP"));
	yylval=(YYSTYPE)st.uplook(yytext);
	
	return MULOP;
}
{ASSIGNOP} {
 	
	return ASSIGNOP;
}
{BRACKET1} {
	return BRACKET1;
}
{CURLEY1} {
	return CURLEY1;
}
{PAREN1} {
	return PAREN1;
}
{BRACKET2} {
	return BRACKET2;
}
{PAREN2} {
	return PAREN2;
}
{CURLEY2} {
	return CURLEY2;
}
{SEMICOLON} {
	return SEMICOLON;
}
".."  {return DOTDOT;}
":"   {return COLON;}
%%
