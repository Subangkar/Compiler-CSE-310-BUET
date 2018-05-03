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


#define ADDOP_TOKEN "ADDOP"
#define MULOP_TOKEN "MULOP"
#define INCOP_TOKEN "INCOP"
#define RELOP_TOKEN "RELOP"
#define ASSIGNOP_TOKEN "ASSIGNOP"
#define LOGICOP_TOKEN "LOGICOP"
#define BITOP_TOKEN "BITOP"
#define NOT_TOKEN "NOT"
#define LPAREN_TOKEN "LPAREN"
#define RPAREN_TOKEN "RPAREN"
#define LCURL_TOKEN "LCURL"
#define RCURL_TOKEN "RCURL"
#define LTHIRD_TOKEN "LTHIRD"
#define RTHIRD_TOKEN "RTHIRD"
#define COMMA_TOKEN "COMMA"
#define SEMICOLON_TOKEN "SEMICOLON"


#define _TOKEN ""
#define _TOKEN ""
#define _TOKEN ""
#define _TOKEN ""




%}

WHITESPACE [ \t\f\r\v]+

LETTER [a-zA-Z]
DIGIT [0-9]
NEWLINE \r?\n
ALPHABET {LETTER}|_
ALPHANUMERIC {ALPHABET}|{DIGIT}

IDENTIFIER {ALPHABET}({ALPHANUMERIC})*
NOT_IDENTIFIER {DIGIT}({ALPHANUMERIC})*

DIGITS {DIGIT}+
DECPOINT (\.)*
FRACTION (\.{DIGITS})

INTEGER {DIGITS}
FLOATING_POINT ({INTEGER})?(\.?{DIGITS})(E[+-]?{INTEGER})?
TOO_DECPOINT {DIGIT}*((\.)+{DIGITS})+(E{DECPOINT}[+-]?{DECPOINT}{DIGITS})?{DECPOINT}{DIGITS}*
ILL_NUM ({DIGITS}|{FLOATING_POINT})[E]*{FLOATING_POINT}*



CHARACTER ('(\\.|[^\\'\n])')
ILL_CHARACTER ('(')?')
MULTI_CHARACTER '.(.)+'
UNFINISHED_CHARACTER '[^'\n]*|'[\\]'




STRING \"([^\"\n]|\\{NEWLINE})*\"
UNFINISHED_STRING \"([^\"\n]|(\\{NEWLINE}))*




STAR_COMMENT_START "/*"
STAR_COMMENT_END "*/"
ANYTHING_EXCEPT_STAR [^\*]
STAR_NOT_FOLLOWED_BY_SLASH [\*][^/]

STAR_COMMENT {STAR_COMMENT_START}({ANYTHING_EXCEPT_STAR}|{STAR_NOT_FOLLOWED_BY_SLASH}|NEWLINE)*"*/"

SLASH_COMMENT "//"(.*([\\]("\r\n"|[\n]))?.*)*
UNFINISHED_COMMENT {STAR_COMMENT_START}({ANYTHING_EXCEPT_STAR}|{STAR_NOT_FOLLOWED_BY_SLASH}|NEWLINE)*


ADDOP ("+"|"-")
%%


{NEWLINE} {line_count++;}
{WHITESPACE}+ {}


{SLASH_COMMENT}	| 
{STAR_COMMENT}			{comment();}

{UNFINISHED_COMMENT}	{printError("Unfinished comment");}



{INTEGER}				{addToken_const_int();}
{FLOATING_POINT}		{addToken_const_float();}
{CHARACTER} 			{addToken_const_char();}
{STRING}				{addToken_string();}

{TOO_DECPOINT}			{printError("Too many decimal point");}
{ILL_NUM}				{printError("Ill formed number");}
{ILL_CHARACTER}			{printError("Invalid character");}
{MULTI_CHARACTER}		{printError("Multi character constant ");}
{UNFINISHED_CHARACTER}	{printError("Unfinished character");}
{UNFINISHED_STRING}		{printError("Unfinished string");}


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



{ADDOP}							{addToken_operator(ADDOP_TOKEN);}
("++"|"--")						{addToken_operator(INCOP_TOKEN);}
("*"|"/"|"%")					{addToken_operator(MULOP_TOKEN);}
("<="|">="|"=="|"!="|"<"|">") 	{addToken_operator(RELOP_TOKEN);}
("&&"|"||")						{addToken_operator(LOGICOP_TOKEN);}
("&"|"|"|"^"|"<<"|">>")			{addToken_operator(BITOP_TOKEN);}
("=")							{addToken_operatorNotStore(ASSIGNOP_TOKEN);}
"!"								{addToken_operatorNotStore(NOT_TOKEN);}
"("								{addToken_operatorNotStore(LPAREN_TOKEN);}
")"								{addToken_operatorNotStore(RPAREN_TOKEN);}
"{"								{addToken_operatorNotStore(LCURL_TOKEN);}
"}"								{addToken_operatorNotStore(RCURL_TOKEN);}
"["								{addToken_operatorNotStore(LTHIRD_TOKEN);}
"]"								{addToken_operatorNotStore(RTHIRD_TOKEN);}
","								{addToken_operatorNotStore(COMMA_TOKEN);}
";"								{addToken_operatorNotStore(SEMICOLON_TOKEN);}




{IDENTIFIER}		{addToken_identifier();}

{NOT_IDENTIFIER}	{printError("Invalid numeric constant or identifier");}

. {printError("Unrecognized character");}

<<EOF>> {fprintf(logout,"\nTotal Line Number: %d\nTotal Errors: %d\n",line_count,err_count); return 0;}


%%

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
	hashTable.printAllScope();
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;

}