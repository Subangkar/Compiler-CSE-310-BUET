%{
#include <stdlib.h>
#include <stdio.h>

//int yydebug;
int yyparse(void);
int yylex(void);
double var[26];

void yyerror(char *s)
{
	fprintf(stderr,"%s\n",s);
	return;
}

%}

%union { double dval; int ivar; }
%token <dval> DOUBLE
%token <ivar> NAME
%type <dval> expr
%type <dval> mulex
%type <dval> term

%%
program:
	line program		{printf("program->line program\n");}
	| line			{printf("program->line\n");}
	;
line:
	expr 	'\n' 		{ printf("line->expr\\n\n");printf("%g\n",$1); }
	| NAME '=' expr '\n' 	{ printf("line->NAME = expr\\n\n");var[$1] = $3; }
	;
expr:
	expr '+' mulex 		{ printf("expr->expr+mulex\n");$$ = $1 + $3; }
	| expr '-' mulex 	{ printf("expr->expr-mulex\n");$$ = $1 - $3; }
	| mulex 		{ printf("expr->mulex\n");$$ = $1; }
	;
mulex:
	mulex '*' term 		{ printf("mulex->mulex*term\n");$$ = $1 * $3; }
	| mulex '/' term 	{ printf("mulex->mulex/term\n");$$ = $1 / $3; }
	| term 			{ printf("mulex->term\n");$$ = $1; }
	;
term:
	'(' expr ')' 		{ printf("term->(expr)\n");$$ = $2; }
	| NAME 			{ printf("term->NAME\n");$$ = var[$1]; }
	| DOUBLE 		{ printf("term->DOUBLE\n");$$ = $1; }
	;
%%

int main(void){
	/*yydebug=1;*/
	yyparse();
	return 0;
}