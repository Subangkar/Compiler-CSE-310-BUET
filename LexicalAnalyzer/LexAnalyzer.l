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

%}

ANYTHING_EXCEPT_NEWLINE [^\r\n]

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



CHAR_CONST ((\\{ANYTHING_EXCEPT_NEWLINE})|[^\\'(\r\n)])

CHARACTER ('{CHAR_CONST}')
EMPTY_CHARACTER ('')
MULTI_CHARACTER '{CHAR_CONST}{2,}'
UNFINISHED_CHARACTER '({CHAR_CONST}|\\)*


ANYTHING_EXCEPT_SLASH [^\\]
SLASH [\\]
NOT_QUOTE_SLASH_OR_NEWLINE [^"\\\r\n]
ANYTHING ({ANYTHING_EXCEPT_NEWLINE}|{NEWLINE})


UNFINISHED_STRING \"({NOT_QUOTE_SLASH_OR_NEWLINE}|{SLASH}{ANYTHING})*
STRING {UNFINISHED_STRING}\"




STAR_COMMENT_START "/*"
STAR_COMMENT_END "*/"
ANYTHING_EXCEPT_STAR [^\*]
STAR_NOT_FOLLOWED_BY_SLASH [\*][^/]

SLASH_COMMENT "//"({ANYTHING_EXCEPT_NEWLINE}*({SLASH}({NEWLINE}))?{ANYTHING_EXCEPT_NEWLINE}*)*

/* UNFINISHED_STAR_COMMENT {STAR_COMMENT_START}({ANYTHING_EXCEPT_STAR}|{STAR_NOT_FOLLOWED_BY_SLASH})* */
UNFINISHED_STAR_COMMENT {STAR_COMMENT_START}({ANYTHING_EXCEPT_STAR}|([\*]+[^*/]))*
/* STAR_COMMENT {UNFINISHED_STAR_COMMENT}{STAR_COMMENT_END} */
STAR_COMMENT {UNFINISHED_STAR_COMMENT}[\*]*{STAR_COMMENT_END}


ADDOP ("+"|"-")
%%


{NEWLINE} {line_count++;}
{WHITESPACE}+ {}


{SLASH_COMMENT}	|
{STAR_COMMENT}			{comment();}

{UNFINISHED_STAR_COMMENT}	{printError("Unfinished comment");}



{INTEGER}				{addToken_const_int();}
{FLOATING_POINT}		{addToken_const_float();}
{CHARACTER} 			{addToken_const_char();}
{STRING}				{addToken_string();}

{TOO_DECPOINT}			{printError("Too many decimal point");}
{ILL_NUM}				{printError("Ill formed number");}
{UNFINISHED_CHARACTER}	{printError("Unfinished character");}
{EMPTY_CHARACTER}		{printError("Empty character");}
{MULTI_CHARACTER}		{printError("Multi character constant ");}
{UNFINISHED_STRING}		{printError("Unfinished string");}


"if" 		|
"for" 		|
"do" 		|
"int" 		|
"float" 	|
"void" 		|
"switch" 	|
"default" 	|
"else" 		|
"while" 	|
"break" 	|
"char" 		|
"double" 	|
"return"	|
"case" 		|
"continue" 	{addToken_keyword();}



{ADDOP}							{addToken_operator(ADDOP_TOKEN);}
("++"|"--")						{addToken_operator(INCOP_TOKEN);}
("*"|"/"|"%")					{addToken_operator(MULOP_TOKEN);}
("<="|">="|"=="|"!="|"<"|">") 	{addToken_operator(RELOP_TOKEN);}
("&&"|"||")						{addToken_operator(LOGICOP_TOKEN);}
("&"|"|"|"^"|"<<"|">>")			{addToken_operator(BITOP_TOKEN);}
("=")							{addToken_operator(ASSIGNOP_TOKEN);}
"!"								{addToken_operator(NOT_TOKEN);}
"("								{addToken_operator(LPAREN_TOKEN);}
")"								{addToken_operator(RPAREN_TOKEN);}
"{"								{addToken_operator(LCURL_TOKEN);}
"}"								{addToken_operator(RCURL_TOKEN);}
"["								{addToken_operator(LTHIRD_TOKEN);}
"]"								{addToken_operator(RTHIRD_TOKEN);}
","								{addToken_operator(COMMA_TOKEN);}
";"								{addToken_operator(SEMICOLON_TOKEN);}




{IDENTIFIER}		{addToken_identifier();}

{NOT_IDENTIFIER}	{printError("Invalid numeric constant or identifier");}

{ANYTHING} {printError("Unrecognized character");}

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
	// hashTable.setPrintEmptyListOn(true);
	hashTable.printAllScope(stdout);
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;
}
