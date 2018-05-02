%option noyywrap

%{

#include "LexBase.h"


#define IF_TOKEN "IF"
#define FOR_TOKEN "FOR"
#define DO_TOKEN "D0"
#define INT_TOKEN "INT"
#define FLOAT_TOKEN "FLOAT"
#define VOID_TOKEN "VOID"
#define SWITCH_TOKEN "SWITCH"
#define DEFAULT_TOKEN "DEFAULT"

#define ELSE_TOKEN "ELSE"
#define WHILE_TOKEN "WHILE"
#define BREAK_TOKEN "BREAK"
#define CHAR_TOKEN "CHAR"
#define DOUBLE_TOKEN "DOUBLE"
#define RETURN_TOKEN "RETURN"
#define CASE_TOKEN "CASE"
#define CONTINUE_TOKEN "CONTINUE"


#define _TOKEN ""
#define _TOKEN ""
#define _TOKEN ""





%}

WHITESPACE [ \t\f\r\v]+
LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \n
ALPHABET [{LETTER}_]
ALPHANUMERIC [a-zA-Z0-9_]




SLASH_COMMENT "//"(.*([\\]("\r\n"|[\n]))?.*)*

STAR_COMMENT_START "/*"
STAR_COMMENT_END "*/"
ANYTHING_EXCEPT_STAR [^*]
STAR_NOT_FOLLOWED_BY_SLASH "*"[^/]

STAR_COMMENT "/*"([^\*]|([\*][^/])|"\r\n"|"\n")*"*/"


%%

{NEWLINE} {line_count++;}
{WHITESPACE}+ {}

{SLASH_COMMENT}	|
{STAR_COMMENT}	{comment();}



"if" 		{addToken_keyword(IF_TOKEN);}
"for" 		{addToken_keyword(FOR_TOKEN);}
"do" 		{addToken_keyword(DO_TOKEN);}
"int" 		{addToken_keyword(INT_TOKEN);}
"float" 	{addToken_keyword(FLOAT_TOKEN);}
"void" 		{addToken_keyword(VOID_TOKEN);}
"switch" 	{addToken_keyword(SWITCH_TOKEN);}
"default" 	{addToken_keyword(DEFAULT_TOKEN);}

"else" 		{addToken_keyword(ELSE_TOKEN);}
"while" 	{addToken_keyword(WHILE_TOKEN);}
"break" 	{addToken_keyword(BREAK_TOKEN);}
"char" 		{addToken_keyword(CHAR_TOKEN);}
"double" 	{addToken_keyword(DOUBLE_TOKEN);}
"return"	{addToken_keyword(RETURN_TOKEN);}
"case" 		{addToken_keyword(CASE_TOKEN);}
"continue" 	{addToken_keyword(CONTINUE_TOKEN);}




<<EOF>> {printf("\nTotal Line Number: %d and KeyWords: %d\n",line_count,keyword_count); return 0;}


%%

// int yywrap()
// {
// 	return 0;
// }

int main(int argc,char *argv[]){

	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}

	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

	logout = fopen("log.txt","w");
	tokenout = fopen("token.txt","w");

	yyin= fin;
	yylex();
	st.printAllScope();
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;

}