%{  
  #include<stdlib.h>
  
  int count=1;
%}

delim   [ \t]
ws      {delim}+
letter  [A-Za-z]
digit   [0-9]
id      {letter}({letter}|{digit})*
number  {digit}+(\.{digit}+)?(E[+-]?{digit}+)?
arithmetic 	[-+*/]
newline [\n]

backslash   [\/]
asteric    [*]

mc      {backslash}{asteric}((.)*{newline}*(.)*)*{asteric}{backslash}
string	(\"{id}*{number}*(\\\")*{id}*{number}*\")





%%

{ws} {}
{newline} {count++;}
{number}  		{ printf("<Number 			  	, line_No %d  %s>\n",count,yytext); }
{arithmetic}   	{ printf("<ArithMetic_operator	, line_no %d  %s>\n",count,yytext); }
if              { printf("<Keyword						, line_no %d 	  	%s>\n",count,yytext);}
else            { printf("<Keyword						, line_no %d   	%s>\n",count,yytext);}
then            { printf("<Keyword						, line_no %d   	%s>\n",count,yytext);}
{id}    		{ printf("<ID     			  			, line_No %d  %s>\n",count,yytext); }
"++"|"--"      	{ printf("<ArithMetic_operator					, line_no %d  %s>\n",count,yytext); }
"&&"|"||"|"!"  	{ printf("<Logical_operator   			, line_no %d  %s>\n",count,yytext); }
{mc}            { printf("<Multiline_Comment  			, line_no %d  \n %s>\n",count,yytext); }
{string}        { printf("<String  				, line_no %d  \n %s>\n",count,yytext); }


%%

void main()
{
	freopen ("in.txt", "r",stdin);
	freopen ("output.txt","w",stdout);
	yylex();
}