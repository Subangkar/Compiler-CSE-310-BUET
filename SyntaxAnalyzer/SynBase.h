//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H


#include <iostream>
//#include "SymTable.h"
#include "DataStructure.h"
#include "Utils.h"
#include <stack>
#include <sstream>

using std::stack;


#define SYMBOL_TABLE_SIZE 73


std::ofstream logFile, errorFile, parserFile;
SymbolTable table(SYMBOL_TABLE_SIZE);

int IDargs = 0;
vector<string> args;
bool funcDef = false;
int semErrors;
vector<SymbolInfo> params;


//stack<string> relEx, simpEx, termV;

extern FILE *yyin;
extern int line_count;


int yyparse();

int yylex();


void yyerror(const char *s) {
	//write your code
}


void printLog(string str) {
	logFile << str << endl;
}

const string ruleName[] = {"start","program","unit" ,"func_declaration","func_definition","parameter_list","compound_statement","var_declaration","type_specifier","declaration_list","statements","statement","expression_statement","variable","expression","logic_expression","rel_expression","simple_expression","term","unary_expression","factor","argument_list","arguments"};

enum NONTERMINAL_TYPE {
	start = 0,
	program,
	unit,
	func_declaration,
	func_definition,
	parameter_list,
	compound_statement,
	var_declaration,
	type_specifier,
	declaration_list,
	statements,
	statement,
	expression_statement,
	variable,
	expression,
	logic_expression,
	rel_expression,
	simple_expression,
	term,
	unary_expression,
	factor,
	argument_list,
	arguments
};

class NonTerminalBuffer {
private:
	stack<string> terminalBuf[NONTERMINAL_TYPE::arguments + 1];
public:
	string getValue(NONTERMINAL_TYPE nonterminal) {
		if (nonterminal < start || nonterminal > arguments) return string("");
		return terminalBuf[nonterminal].top();
	}

	string extractValue(NONTERMINAL_TYPE nonterminal) {
		if (nonterminal < start || nonterminal > arguments) return string("");

		if (terminalBuf[nonterminal].empty()) return "";

		string str = terminalBuf[nonterminal].top();
		terminalBuf[nonterminal].pop();
		return str;
	}

	void pushValue(NONTERMINAL_TYPE nonterminal, const string &val) {
		if (nonterminal < start || nonterminal > arguments) return;

		terminalBuf[nonterminal].push(val);
	}
};


NonTerminalBuffer nonTerminalBuffer;


void pushVal(NONTERMINAL_TYPE nonterminal, const string &val) {
	nonTerminalBuffer.pushValue(nonterminal, val);
}

string popVal(NONTERMINAL_TYPE nonterminal) {
	string val = nonTerminalBuffer.extractValue(nonterminal);
	return val+((val[val.length()-1]==' ')?"":" ");
}




void printRule(const string &rule) {
	logFile << "At line no: " << line_count << " " << rule << endl << endl;
}

// after pushing value
void printCode(NONTERMINAL_TYPE ruleNonterminal){
	logFile << CodeParser::formatCCode(nonTerminalBuffer.getValue(ruleNonterminal)) << endl << endl;
}


void printRuleLog(NONTERMINAL_TYPE nonterminal, const string &rule)
{
	printRule(ruleName[nonterminal]+": "+rule);
	printCode(nonterminal);
}











#endif //SYNTAXANALYZER_SYNBASE_H
