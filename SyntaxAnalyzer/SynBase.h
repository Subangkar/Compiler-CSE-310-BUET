//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H


#include <iostream>
//#include "SymTable.h"
#include "DataStructure.h"
#include <stack>

using std::stack;




#define SYMBOL_TABLE_SIZE 73


std::ofstream logFile, errorFile, parserFile;
SymbolTable table(SYMBOL_TABLE_SIZE);

int IDargs = 0;
vector<string> args;
bool funcDef = false;
int semErrors;
vector<SymbolInfo> params;


stack<string> relEx,simpEx,termV;

extern FILE *yyin;
extern int line_count;




int yyparse();

int yylex();



void yyerror(const char *s) {
	//write your code
}


void printLog(string str)
{
  logFile << str << endl;
}


enum TERMINAL_TYPE
{
	start=0,program,unit ,func_declaration,func_definition,parameter_list,compound_statement,var_declaration,type_specifier,declaration_list,statements,statement,expression_statement,variable,expression,logic_expression,rel_expression	,simple_expression	,term,unary_expression,factor,argument_list,arguments
};

class NonTerminalBuffer
{
private:
	stack<string> terminalBuf[TERMINAL_TYPE::arguments+1];
public:
	string getValue(TERMINAL_TYPE terminalNo)
	{
		if(terminalNo<start || terminalNo>arguments) return string("");
		return terminalBuf[terminalNo].top();
	}

	string extractValue(TERMINAL_TYPE terminalNo)
	{
		if(terminalNo<start || terminalNo>arguments) return string("");

		if(terminalBuf[terminalNo].empty()) return "";

		string str = terminalBuf[terminalNo].top();
		terminalBuf[terminalNo].pop();
		return str;
	}

	void pushValue(TERMINAL_TYPE terminalNo,string val)
	{
		if(terminalNo<start || terminalNo>arguments) return;

		terminalBuf[terminalNo].push(val);
	}
};


NonTerminalBuffer nonTerminalBuffer;

#endif //SYNTAXANALYZER_SYNBASE_H
