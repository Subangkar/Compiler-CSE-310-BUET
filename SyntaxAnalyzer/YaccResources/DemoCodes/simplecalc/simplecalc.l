%option noyywrap

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "y.tab.h"

%}


WS [ \t]+
DIGIT [0-9]
NUMBER [-]?{DIGIT}+(\.{DIGIT}+)?

%%

{NUMBER}  {
  sscanf(yytext,"%lf", &yylval);
	     return NUMBER;
	  }

"+"       { return PLUS; }
"-"       { return MINUS; }
"/"       { return SLASH; }
"*"       { return ASTERISK; }
"("       { return LPAREN; }
")"       { return RPAREN; }
"\n"      { return NEWLINE; }
{WS} { /* No action and no return */}
