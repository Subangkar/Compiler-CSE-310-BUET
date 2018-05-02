%option c++
%option noyywrap

%{ 
#include <iostream>
#include<fstream>
#include<string>
#include "1305037.h"
using namespace std;

SymbolTable hashTable(30);
ofstream token,output,a;
ifstream input;

int lCount=1,errCount=0;
int *arr,temp; 
string str;

%}
WHITESPACE[ \t\v\f\r]+

NEWLINE [\n]

LETTER [_a-zA-Z]

CHAR [ -~]{-}[']
CHARS {CHAR}+

CHARACTER ('({CHAR})')
SPECIAL_CHAR '\\[tvar"afb0n]'|'\\'
ILL_CHAR ('({CHARS}|['])*')
UNRE_CHAR ('{CHAR}*[^'\n]?)


STRING \"([^\"\n]|\\{NEWLINE})*\"
UNFINISHED_STRING \"([^\"\n]|(\\{NEWLINE}))*


SINGLELINE_COMMENT "//"{CHARS}$ 


MULTILINE_COMMENT_START "/*"
MULTILINE_COMMENT_END "*/"
ANYTHING_EXCEPT_STAR [^*]
STAR_NOT_FOLLOWED_BY_SLASH "*"[^/]


COMMENT ({MULTILINE_COMMENT_START}({ANYTHING_EXCEPT_STAR}|{STAR_NOT_FOLLOWED_BY_SLASH})*{MULTILINE_COMMENT_END})
UNFINISHED_COMMENT ({MULTILINE_COMMENT_START}[^"*/"]*)

DIGIT [0-9]
DIGITS {DIGIT}+
DECP (\.)*


INTEGER {DIGITS}
FLOATING_POINT {DIGITS}*(\.{DIGITS})(E[+-]?{DIGITS})?

TOO_DEC {DIGITS}*((\.)+{DIGITS}+)+(E{DECP}[+-]?{DECP}{DIGITS})?{DECP}{DIGITS}*
ILL_NUM ({DIGITS}|{FLOATING_POINT})[E]*{FLOATING_POINT}*{DIGITS}*

IDENTIFIER {LETTER}({LETTER}|{DIGIT})*
NOT_ID {DIGIT}({LETTER}|{DIGIT})*
%%


{WHITESPACE}    {}
{NEWLINE}  	{lCount++;}

{INTEGER}       {
		
		arr=hashTable.Insert(yytext,"CONST_INT",output);
		output<<"Line No. "<<lCount<<":  Token <CONST_INT> Lexeme "<<yytext<<" found\n";output.flush();

		token<<"<CONST_INT,"<<yytext<<">";token.flush();
		}

{FLOATING_POINT} {

		arr=hashTable.Insert(yytext,"CONST_FLOAT",output);
		output<<"Line No. "<<lCount<<":  Token <CONST_FLOAT> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<CONST_FLOAT,"<<yytext<<">";token.flush();

}
 
{TOO_DEC} {output<<"Error at Line No. "<<lCount<<":  Too many decimal point "<<yytext<<"\n";output.flush(); errCount++;}
{ILL_NUM} {output<<"Error at Line No. "<<lCount<<":  Ill formed number "<<yytext<<"\n";output.flush();errCount++; }

{CHARACTER}		{
		arr=hashTable.Insert(yytext,"CONST_CHAR",output);
		output<<"Line No. "<<lCount<<":  Token <CONST_CHAR> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<CONST_CHAR,"<<yytext<<">";token.flush();
}

{SPECIAL_CHAR} { arr=hashTable.Insert(yytext,"CONST_CHAR",output);
		output<<"Line No. "<<lCount<<":  Token <CONST_CHAR> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<CONST_CHAR,"<<yytext<<">";token.flush();
}

{ILL_CHAR} { output<<"Error at Line No. "<<lCount<<":  Ill formed character "<<yytext<<"\n";output.flush();errCount++; }

{UNRE_CHAR} {output<<"Error at Line No. "<<lCount<<":  Unterminated Character "<<yytext<<"\n";output.flush();errCount++; }

{SINGLELINE_COMMENT}	{
	output<<"Line No. "<<lCount<<":  Token <COMMENT> Lexeme "<<yytext<<" found\n";output.flush();	


}
{COMMENT} {temp=lCount; str=yytext;makeOne(str,2,&lCount);output<<"Line No. "<<temp<<":  Token <COMMENT> Lexeme "<<yytext<<" found\n";output.flush();}
 
{UNFINISHED_COMMENT} {str=yytext;makeOne(str,2,&lCount);output<<"Error at Line No. "<<lCount-1<<":  Unterminated Comment "<<yytext<<"\n";output.flush();errCount++; }


{STRING} {	temp=lCount; str=yytext;str=makeOne(str,1,&lCount); 
		output<<"Line No. "<<temp<<":  Token <STRING> Lexeme "<<str<<" found\n";output.flush();
 		token<<"<STRING,"<<str<<">"; token.flush(); }


{UNFINISHED_STRING} {str=yytext;str=makeOne(str,1,&lCount); output<<"Error at Line No. "<<lCount<<":  Unterminated String "<<yytext<<"\n";output.flush(); errCount++;}

"if" 	   { output<<"Line No. "<<lCount<<":  Token <IF> Lexeme if found "<<endl;    	output.flush();token<<"<IF>"; 		token.flush();}
"for" 	   { output<<"Line No. "<<lCount<<":  Token <FOR> Lexeme for found "<<endl;  	output.flush();token<<"<FOR>";		token.flush();}
"do"	   { output<<"Line No. "<<lCount<<":  Token <DO> Lexeme do found "<<endl;    	output.flush();token<<"<DO>";	 	token.flush();}
"int" 	   { output<<"Line No. "<<lCount<<":  Token <INT> Lexeme int found"<<endl;  	output.flush();token<<"<INT>";		token.flush();}
"float"    { output<<"Line No. "<<lCount<<":  Token <FLOAT> Lexeme float found"<<endl; 	output.flush();token<<"<FLOAT>";	token.flush();}
"void"     { output<<"Line No. "<<lCount<<":  Token <VOID> Lexeme void found"<<endl;   	output.flush();token<<"<void"; 		token.flush();}
"switch"   { output<<"Line No. "<<lCount<<":  Token <SWITCH> Lexeme switch found"<<endl;output.flush();token<<"<SWITCH>";	token.flush();}
"default"  { output<<"Line No. "<<lCount<<":  Token <DEFAULT> Lexeme default found"<<endl;output.flush();token<<"<DEFAULT>";	token.flush();}
"else"     { output<<"Line No. "<<lCount<<":  Token <ELSE> Lexeme else found"<<endl; 	output.flush();token<<"<ELSE>";		token.flush();}
"while"    { output<<"Line No. "<<lCount<<":  Token <WHILE> Lexeme while found"<<endl; 	output.flush();token<<"<WHILE>";	token.flush();}
"break"    { output<<"Line No. "<<lCount<<":  Token <BREAK> Lexeme break found"<<endl; 	output.flush();token<<"<BREAK>";	token.flush();}
"char"     { output<<"Line No. "<<lCount<<":  Token <CHAR> Lexeme char found"<<endl; 	output.flush();token<<"<CHAR>";		token.flush();}
"double"   { output<<"Line No. "<<lCount<<":  Token <DOUBLE> Lexeme double found"<<endl;output.flush();token<<"<DOUBLE>";	token.flush();}
"return"   { output<<"Line No. "<<lCount<<":  Token <RETURN> Lexeme return found"<<endl;output.flush();token<<"<RETURN>";	token.flush();}
"case"     { output<<"Line No. "<<lCount<<":  Token <CASE> Lexeme case"<<endl; 		output.flush();token<<"<CASE>";		token.flush();}
"continue" { output<<"Line No. "<<lCount<<":  Token <CONTINUE> Lexeme continue found"<<endl;output.flush();token<<"<CONTINUE>";	token.flush();}


("++")|("--") {
		arr=hashTable.Insert(yytext,"INCOP",output);
		output<<"Line No. "<<lCount<<":  Token <INCOP> Lexeme "<<yytext<<" found\n";output.flush();	
		token<<"<INCOP,"<<yytext<<">"; 
		token.flush();
}


("+"|"-")=   {
		arr=hashTable.Insert(yytext,"ADDOP",output);
		output<<"Line No. "<<lCount<<":  Token <ADDOP> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<ADDOP,"<<yytext<<">"; 
		token.flush();}
("*"|"/"|"%") {
		arr=hashTable.Insert(yytext,"MULOP",output);
		output<<"Line No. "<<lCount<<":  Token <MULOP> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<MULCOP,"<<yytext<<">"; 
		token.flush();}

("<="|">="|"=="|"!=") {
		arr=hashTable.Insert(yytext,"RELOP",output);
		output<<"Line No. "<<lCount<<":  Token <RELOP> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<RELOP,"<<yytext<<">"; 
		token.flush();}

("<"|">")	{
		arr=hashTable.Insert(yytext,"RELOP",output);
		output<<"Line No. "<<lCount<<":  Token <RELOP> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<RELOP,"<<yytext<<">"; 
		token.flush();}

("=") { 
		arr=hashTable.Insert(yytext,"ASSIGNOP",output);
		output<<"Line No. "<<lCount<<":  Token <ASSIGNOP> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<ASSIGNOP,"<<yytext<<">"; 
		token.flush();}
("&&"|"||"|"!") {
		arr=hashTable.Insert(yytext,"LOGICOP",output);
		output<<"Line No. "<<lCount<<":  Token <LOGICOP> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<LOGICOP,"<<yytext<<">"; 
		token.flush();}

("("|")") {	
		arr=hashTable.Insert(yytext,"PAREN",output);
		output<<"Line No. "<<lCount<<":  Token <PAREN> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<PAREN,"<<yytext<<">"; 
		token.flush();}

("{"|"}") {
		arr=hashTable.Insert(yytext,"CURL",output);		
		output<<"Line No. "<<lCount<<":  Token <CURL> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<CURL,"<<yytext<<">"; 
		token.flush();}

("["|"]") {	
		arr=hashTable.Insert(yytext,"BRACKET",output);	
		output<<"Line No. "<<lCount<<":  Token <BRACKET> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<BRACKET,"<<yytext<<">"; 
		token.flush();}

(";") {
		arr=hashTable.Insert(yytext,"SEMICOLON",output);	
		output<<"Line No. "<<lCount<<":  Token <SEMICOLON> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<SEMICOLON,"<<yytext<<">"; 
		token.flush();}

(",") {
		arr=hashTable.Insert(yytext,"COMMA",output);
		output<<"Line No. "<<lCount<<":  Token <COMMA> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<COMMA,"<<yytext<<">"; 
		token.flush();}


{IDENTIFIER} { arr= hashTable.Insert(yytext,"ID",output);
		output<<"Line No. "<<lCount<<":  Token <ID> Lexeme "<<yytext<<" found\n";output.flush();
		token<<"<ID,"<<yytext<<">"; 
		token.flush();}


{NOT_ID} {output<<"Error at Line No. "<<lCount<<":  Invalid Suffix on numeric constant or invalid prefix on identifier "<<yytext<<"\n";output.flush();errCount++; }

		
%%

int main(int argc,char *argv[]){
	
	
if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	input.open(argv[1]);
	if(input==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

 	
	token.open("1305037_token.txt");
	output.open("1305037_log.txt");
	

	
	 yyFlexLexer* lexer = new yyFlexLexer(input,output);

	while(lexer->yylex()!=0){}
	output<<"\nTotal Lines "<<lCount-1<<"\nTotal Errors "<<errCount;
	input.close();
	output.close();
	token.close();
	return 0;
}
