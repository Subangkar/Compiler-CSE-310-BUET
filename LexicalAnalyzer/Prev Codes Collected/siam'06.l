%{
/****************************************************************************
mylexer.l
ParserWizard generated Lex file.

Date: Sunday, January 31, 2010
****************************************************************************/

#include "myparser.h"
#include<iostream>
#include<stdlib.h>
#include<string.h>

using namespace std;

class symbolinfo;
#define YYSTYPE symbolinfo*
extern YYSTYPE yylval;
#define tableSize 100



int linecount=1; 
int flag=1;
int findkey(char* str);
int hashfunction(int key);
void init();
char s[30],r[30];
int labelcount,tempcount;
					///// symbolinfo
class symbolinfo
{
public:
	char* addr;
	char* code;
	symbolinfo *next;

symbolinfo()
{
  addr=new char[30];
  code=new char[1000];
  next = NULL;
}	
};


// symboltable
class symboltable
{
public:

	symbolinfo *node[tableSize];
	
symboltable()
{
	int i;
	for(i=0;i<tableSize;i++)
		node[i]=NULL;
}

symbolinfo* insert(char * str)
{
	int nodenumber;
	nodenumber=hashfunction(findkey(str));
	
	symbolinfo *temp=lookup(str);
	
	if(temp==NULL)
	{
		symbolinfo *newmember=new symbolinfo ;
		newmember->addr=new char[strlen(str)+1];
		newmember->code=new char[1];
		strcpy(newmember->addr,str);
		strcpy(newmember->code," ");
		node[nodenumber]=newmember;
		newmember->next=temp;
		return newmember;	
	}
	else
		return temp;	
}

symbolinfo* insert1(char * str,char * codeval)
{
	int nodenumber;
	nodenumber=hashfunction(findkey(str));
	
	symbolinfo *temp=lookup(str);
	
	if(temp==NULL)
	{
		symbolinfo *newmember=new symbolinfo ;
		newmember->addr=new char[strlen(str)+1];
		newmember->code=new char[strlen(codeval)+1];
		strcpy(newmember->addr,str);
		strcpy(newmember->code,codeval);
		node[nodenumber]=newmember;
		newmember->next=temp;
		return newmember;	
	}
	else
		return temp;
	

	
}
symbolinfo* lookup(char* str )
{
	int nodenumber;
	nodenumber=hashfunction(findkey(str));	
	symbolinfo *temp;
	symbolinfo *temp1;
	temp=node[nodenumber];	
	
	if(temp==NULL)
		return temp;
	while(temp!=NULL)
	{
		if(!strcmp(temp->addr,str))
			return temp;

		else
			temp=temp->next;
			temp1 = temp;
	}
	return temp1;
}
void dump()
{
	int i;
	for(i=0;i<tableSize;i++)
	{
		symbolinfo *temp=node[i];
		printf("%d :",i+1);
		if(temp==NULL);
			
		else
		{
			while(temp!=NULL)
			{
				printf("(%s,%s)\t",temp->addr);	
				temp=temp->next;
				
		
			}
		}
		printf("\n\n");

	}

}
};

int findkey(char* str)
{
	int len=strlen(str);
	int i,key;
	key=0;
        for(i=0;i<len;i++)
	{
		key+=str[i];	
	}
	return key;
}
int hashfunction(int key)
{

	return (key%tableSize);
}
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
char* gettemp()
{
	strcpy(r,"");
	strcpy(r,"temp");
	char s1[30];
	sprintf(s1,"%d",tempcount);
	tempcount++;
	strcat(r,s1);
	return r;
}

//extern SymbolTable st;
char* retValue;

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
	YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
%}

// place your Lex rules here
[ \t\n]+		;
program			return PROGRAM;
var				return VAR;
array			return ARRAY;
of				return OF;
integer			return INTEGER;
real			return REAL;
function		return FUNCTION;
procedure		return PROCEDURE;
begin			return BEGIN_;
end				return END;
if				return IF;
then			return THEN;
else			return ELSE;
while			return WHILE;
do				return DO;
read			return READ;
write			return WRITE;
[a-z][a-z0-9]* 
	{
		/*yylval.s = new SymbolInfo;
		yylval.s->addr = string(yytext);*/
		return ID;
	}

[+-]			{ /*yylval.c = new string(yytext);*/ return ADDOP; }
//[*/%]			{ yylval.c = new string(yytext);return MULOP; }
":="			return ASSIGNOP;
">="|"<="|"<>"|"="|"<"|">"	{ /*yylval.c = new string(yytext);*/ return RELOP; }
".."			return DOTDOT;
[():;,]			return *yytext;
[0-9]+			{ /*yylval.i = atoi(yytext); */return NUM; }


%%

/////////////////////////////////////////////////////////////////////////////
// programs section

