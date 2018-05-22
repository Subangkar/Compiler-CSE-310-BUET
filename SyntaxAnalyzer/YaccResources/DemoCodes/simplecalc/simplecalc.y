
%{
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE double      /* yyparse() stack type */

void yyerror(char *s){
	printf("%s\n",s);
}

int yylex(void);

%}

%token NEWLINE NUMBER PLUS MINUS SLASH ASTERISK LPAREN RPAREN


%%
input:              /* empty string */
    | input line
    ;
line: NEWLINE
    | expr NEWLINE           { printf("\t%.10g\n",$1); }
    ;
expr: expr PLUS term         { $$ = $1 + $3; }
    | expr MINUS term        { $$ = $1 - $3; }
    | term
    ;
term: term ASTERISK factor   { $$ = $1 * $3; }
    | term SLASH factor      { $$ = $1 / $3; }
    | factor
    ;
factor:  LPAREN expr RPAREN  { $$ = $2; }
      | NUMBER
      ;
%%

main()
{
    yyparse();
    exit(0);
}
