%{
/****************************************************************************
mylexer.l
ParserWizard generated Lex file.

Date: Saturday, January 23, 2010
****************************************************************************/

#include "myparser.h"





#include<stdlib.h>

%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// lexical analyser name
%name mylexer

// class definition
{
	// place any extra class members here
}

// constructor
{
	// place any extra initialisation code here
}

// destructor
{
	// place any extra cleanup code here
}

// place any declarations here

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

%{
	// extract yylval for use later on in actions
//	YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
	#define YYSTYPE SymbolInfo*
    extern YYSTYPE yylval;
%}

[\n]		{return NEWLINE;
			}
"program"	{return PROGRAM;
			}
"procedure"  { return PROCEDURE;
			 }		
"function"		{return FUNCTION;}	
"begin"			{yylval=*yytext;
				return BEGINN;
				}
"end"			{
					yylval=*yytext;
					return END;
				}
"enddo"			{
					return ENDDO;
				}
			
[\(\),;:]		{return *yytext;}
":="			{return ASSIGN;	}

">"            {return RELOP1;}
"<"            {return RELOP2;}

"if"			{return IF;}
"else"			{return ELSE;}
"then"			{return THEN;}
"while"			{return WHILE;}
"do"			{return DO;}
"write"			{return WRITE;}
"var"			{return VAR;}
"integer"		{return INTEGER;}
"real"			{return REAL;}
[\[\]]			{return *yytext;}
[*]			    {return MULOP;}
[/]			    {return DIV;}
"+"				{return PLUS;}
"-"				{return SUB;}
">="			{return RELOP3;}
"<="			{return RELOP4;}
"="				{return RELOP5;}
"not"			{return NOT;}

[0-9]+			{
						  
				    yylval->addr=atoi(yytext);				  
					return NUM;
				}

[a-zA-Z]*	{	
			yylval->addr=*yytext;			
			return ID;
			}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section
char* getlabel()
{
	strcpy(s,"");
	strcpy(s,"label");
	char s1[20];
	sprintf(s1,"%d",labelcount);
	labelcount++;
	strcat(s,s1);
	return s;

}
