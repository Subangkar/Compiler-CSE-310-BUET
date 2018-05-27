%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include <iostream>
using namespace std;
#define YYSTYPE simple*      /* yyparse() stack type */
int yyparse(void);
int yylex(void);

extern FILE *yyin, *yyout;
char* temp;
FILE * fp1,*fp2,*fp3;
void yyerror(char *s)
{
	fprintf(stdout,"At Line %d, ERROR-> %s\n",10+1,s);
	return;
}
%}


%token  INCLUDE DOT IF ELSE FOR PARENFIRST PARENLAST CURLYFIRST CURLYLAST SEMICOLON ID COMMA UNARYOP RELOP PLUS MINUS ASTERISK SLASH  


%%
PROGRAM:  PRELI MAIN	{printf("Found all\n");}		
	  ;

PRELI:    PRELI INCLUDE	{printf("Include statement found\n");
	  			size_t length = strlen($2->ch);
	  			temp = new char[length + 1];
	  			strcpy(temp, $2->ch);
	  			cout<<"  "<<temp<<"\n";
	  			fprintf(fp2,"Include statement found\n");
	  		}		
	  | 						
	  ;

MAIN:	  MAIN DOT
          | VOID MAINFUNC CURLYFIRST FULLEXPR CURLYLAST		
	  |
	  ;

FULLEXPR: FULLEXPR EXPR1
          | FULLEXPR IF_EXPR1
          | FULLEXPR FOR_LOOP
          |
          ;

IF_EXPR1: IFEXPR
          | IF_EXPR ELSE_EXPR
          ;

IF_EXPR:  IF PARENFIRST EXPR PARENLAST EXPR1
          | IF PARENFIRST EXPR PARENLAST CURLYFIRST FULLEXPR CURLYLAST 
          ;

ELSE_EXPR: ELSE PARENFIRST EXPR PARENLAST EXPR1
          | ELSE PARENFIRST EXPR PARENLAST CURLYFIRST FULLEXPR CURLYLAST 
          ;

EXPR1:    EXPR2 SEMICOLON
          | 
          ;

EXPR2:    EXPR2 COMMA EXPR
          | EXPR
          ;

EXPR:     EXPR RELOP NUM
          | EXPR RELOP ID1
          | ID1
          ;

ID1: ID1 PLUS TERM         { $$ = $1 + $3; printf("%g = %g + %g\n",$$,$1,$3);}
    | ID1 MINUS TERM        { $$ = $1 - $3; printf("%g = %g - %g\n",$$,$1,$3); }
    | TERM		{ $$ = $1; }
    ;
TERM: TERM ASTERISK FACTOR   { $$ = $1 * $3; }
    | TERM SLASH FACTOR      { $$ = $1 / $3; }
    | FACTOR		{ $$ = $1; printf("factor is %g\n",$1); }
    ;
FACTOR:  PARENFIRST ID1 PARENLAST  { $$ = $2; }
      | ID2
      | NUMBER			{ $$ = $1; printf("number is %g\n",$1); }
      ;

ID2:       ID
           | UNARYOP ID
           | ID UNARYOP
           ;

FOR_LOOP: FOR PARENFIRST EXPR2 SEMICOLON EXPR2 SEMICOLON EXPR2 PARENLAST CURLYFIRST FULLEXPR CURLYLAST
          ;
%%

int main(void){
	fp1=fopen("1205032_input.txt","r");	fp2=fopen("1205032_output.txt","w+");
	yyin=fp1;yyout=fp2;
	printf("START\n");
	yyparse();
	printf("END\n");
	//fprintf(fp3,"Input file contains %d lines in total",ln);
	fclose(fp1);fclose(fp2);
        getchar();
	return 0;
}
