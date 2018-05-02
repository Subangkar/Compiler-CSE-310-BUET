%{
/****************************************************************************
mylexer.l
ParserWizard generated Lex file.

Date: Thursday, January 14, 2010
****************************************************************************/

#include "myparser.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

class SymbolInfo;
extern YYSTYPE yylval;
#define tableSize 100



int linecount=1; 
int flag=1;
int findkey(char* str);
int hashfunction(int key);
void init();

class SymbolInfo
{
public:
	char* addr;
	char* type;
	double val;
	char * code;
	SymbolInfo* next;
		SymbolInfo(){
			addr=new char[30];
			code=new char[200];
			type=new char[100];
			val=0;
			next=NULL;
			
		}
};


class SymbolTable
{
public:

	SymbolInfo *node[tableSize];
	


	SymbolTable()
	{
		int i;
		for(i=0;i<tableSize;i++)
			node[i]=NULL;
	}


	SymbolInfo * insert(char * str,char * strtype)
	{
		int nodenumber;
		nodenumber=hashfunction(findkey(str));
		
		SymbolInfo *temp=lookup(str);
		
		if(temp==NULL)
		{
			SymbolInfo *newmember=new SymbolInfo ;
			newmember->addr=new char[strlen(str)+1];
			newmember->type=new char[strlen(strtype)+1];
			newmember->code=new char[1];
			strcpy(newmember->addr,str);
			strcpy(newmember->type,strtype);
			strcpy(newmember->code," ");
			char * p;
			double d1= strtod(newmember->addr,&p);
			newmember->val=d1;
			node[nodenumber]=newmember;
			newmember->next=temp;
			return newmember;
			
		}
		else
			return temp;
		
	
		
	}
	
	SymbolInfo * insert1(char * str,char * strtype,char * codeval)
	{
		int nodenumber;
		nodenumber=hashfunction(findkey(str));
		
		SymbolInfo *temp=lookup(str);
		
		if(temp==NULL)
		{
			SymbolInfo *newmember=new SymbolInfo ;
			newmember->addr=new char[strlen(str)+1];
			newmember->type=new char[strlen(strtype)+1];
			newmember->code=new char[strlen(codeval)+1];
			strcpy(newmember->addr,str);
			strcpy(newmember->type,strtype);
			strcpy(newmember->code,codeval);
			char * p;
			double d1= strtod(newmember->addr,&p);
			newmember->val=d1;
			node[nodenumber]=newmember;
			newmember->next=temp;
			return newmember;
			
		}
		else
			return temp;
		
	
		
	}
	
	SymbolInfo * lookup(char* str )
	{
		int nodenumber;
		nodenumber=hashfunction(findkey(str));	
		SymbolInfo *temp;
		temp=node[nodenumber];	
		
		if(temp==NULL)
			return temp;
		while(temp!=NULL)
		{
			if(!strcmp(temp->addr,str))
				return temp;
	
			else
				temp=temp->next;
		}
		
		
	
	}
	void dump()
	{
		int i;
		for(i=0;i<tableSize;i++)
		{
			SymbolInfo *temp=node[i];
			printf("%d :",i+1);
			if(temp==NULL);
				
			else
			{
				while(temp!=NULL)
				{
					printf("(%s,%s)\t",temp->addr,temp->type);	
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

	//number {digit}+(\.{digit}+)?([eE][+-]?{digit}+)?
	//ID ({letter}({letter}|{digit})*)
	return (key%tableSize);
}

extern SymbolTable st;
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

delim [ \t]
whitespace {delim}+
digit [0-9]
letter [A-Za-z]
number {digit}+(\.{digit}+)?([eE][+-]?{digit}+)?
id ({letter}({letter}|{digit})*)

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

%{
	// extract yylval for use later on in actions
	YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
%}

// place your Lex rules here

\n		{
			linecount++;
		//	return NEWLINE;
		}

[ \t\n]	{}

"*"		{
			st.insert(yytext,"MUL");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return MUL;
		}
"/"		{
			st.insert(yytext,"DIV");
			yylval = (SymbolInfo*)st.lookup(yytext);
	//		cout<< yytext<<endl;
			return DIV;
		}
"+"	{ 
			st.insert(yytext,"PLUS");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return PLUS; 
			
		}
"-" 	{ 
			st.insert(yytext,"MINUS");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return MINUS; 
			
		}
":="		{
			st.insert(yytext,"ASSIGN");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return ASSIGN;
		}
if 		{
			return IF;
		}
then 		{ 	return THEN;
		}
program 	{
		//	cout<<yytext<<" ";
			return PROGRAM;	
		}
not 		{
			return NOT;
		}
enddo		{
			return ENDDO;
}
endif		{
			return ENDIF;
}
end 		{	
			return END;
		}
begin 		{	
			return BEGIN_;
		}
else 		{	
			return ELSE;
		}
do 		{
			return DO;
		}
while 		{	
			return WHILE;
		}
function 	{ 
//			cout<<"_function_";
			return FUNCTION;
		}
Procedure 	{
			return PROCEDURE;
		}
integer 	{
			return INTEGER;
		}
real 		{
			return REAL;
		}
var  		{
			return VAR;
		}

array 		{
			return ARRAY;
		}
write 		{
			return WRITE;
		}
mod 		{	
			st.insert(yytext,"MOD");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return MOD;
			
		}
and 		{
			st.insert(yytext,"AND");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return AND;
			
		}
or 		{
			st.insert(yytext,"OR");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return OR;
			
		}

"<" 		{
			st.insert(yytext,"LESS");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return LESS;
		}
">" 		{
			st.insert(yytext,"GREATER");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return GREATER;
		}
"=" 		{
			st.insert(yytext,"EQUAL");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return EQUAL;
		}
"<>" 		{
			st.insert(yytext,"NOTEQ");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return NOTEQ;
		}
">=" 		{
			st.insert(yytext,"LESSEQ");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return LESSEQ;
		}
"<=" 		{
			st.insert(yytext,"GREATEREQ");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return GREATEREQ;
		}

//{digit}+(\.{digit}+)?([eE][+-]?{digit}+)? 
[0-9]+ {
				//int insertResult;
					cout<<endl<<yytext<<endl;
				st.insert(yytext,"NUMBER");
				yylval = (SymbolInfo*)st.lookup(yytext);
				cout<<endl<<"lex : " <<atoi(yylval->addr)<<endl;
				return NUMBER;
				//if(insertResult == 0) printf("Value already exist\n");
				//else if(insertResult == -1) printf("Symbol Table capacity full\n");
					
		}
		
[a-zA-Z]+	{
				//int insertResult;
				
				st.insert(yytext,"ID");
				yylval = (SymbolInfo*)st.lookup(yytext);
				//cout<<yytext<<" ";
				return ID;
				//if(insertResult == 0) printf("Value already exist\n");
				//else if(insertResult == -1) printf("Symbol Table capacity full\n");
					
		}

"["			 {
				st.insert(yytext,"BRACKETLEFT");
				yylval = (SymbolInfo*)st.lookup(yytext);
				return BRACKETLEFT;

			}
"]"  			{
				st.insert(yytext,"BRACKETRIGHT");
				yylval = (SymbolInfo*)st.lookup(yytext);
				return BRACKETRIGHT;

			}
"(" 			{
				st.insert(yytext,"PARENLEFT");
				yylval = (SymbolInfo*)st.lookup(yytext);
				//cout<<yytext<<" ";
				return PARENLEFT;

			}
")"			 {
				st.insert(yytext,"PARENRIGHT");
				yylval = (SymbolInfo*)st.lookup(yytext);
				//cout<<yytext<<" ";
				return PARENRIGHT;

			}
".."  {
				st.insert(yytext,"DOTDOT");
				yylval = (SymbolInfo*)st.lookup(yytext);
				return DOTDOT;

			}
","		{
			st.insert(yytext,"COMMA");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return COMMA;
		}
":"	{
			st.insert(yytext,"COLON");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return COLON;
		}
";" 	{
			st.insert(yytext,"SEMICOLON");
			yylval = (SymbolInfo*)st.lookup(yytext);
			return SEMICOLON;
		}
%%

/////////////////////////////////////////////////////////////////////////////
// programs section

