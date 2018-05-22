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




%%
a	:	b
	|	c
	;
	
b	:	'x'
	;	
c	:	'x'
	;
%%


int main(void){
	/*yydebug=1;*/
	yyparse();
	return 0;
}