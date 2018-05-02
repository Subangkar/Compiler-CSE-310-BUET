//
// Created by subangkar on 5/2/18.
//

#ifndef LEXICALANALYZER_LEXBASE_H
#define LEXICALANALYZER_LEXBASE_H

#include "DataStructure.h"
#include "Utils.h"

SymbolTable st(10);
FILE *logout,*tokenout;
int line_count = 1;
int keyword_count=0;



void addToken_keyword(string token_name)
{
	string token = string("<")+token_name+string(">");
	fprintf(tokenout,"%s",token.data());
	fprintf(logout,"Line no %d: TOKEN %s Lexeme %s found\n",line_count,token.c_str(),yytext);
	keyword_count++;
}


void printError(string msg)
{
//	cout << "Line no 5: "<<msg<<endl;
	fprintf(logout,"Line no %d: %s\n",line_count,msg.data());	

}

void comment()
{
	fprintf(logout,"Line no %d: TOKEN <COMMENT> Lexeme <%s> found\n",line_count,yytext);

//	string s(yytext);
	line_count += StringUtils::occCount(yytext,'\n');
}

#endif //LEXICALANALYZER_LEXBASE_H
