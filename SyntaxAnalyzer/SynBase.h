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



string indent(string fileName)//Autoindent a file
{

	int pos = 0, count =0, j=0;
	string ch;
	stringstream file(fileName);
//	ifstream file(fileName);
//	while(file.good())
//	{
//		pos++;//return value at end of code, number of lines
//		getline(file,ch);
//	}
	pos = StringUtils::occCount(fileName,"\n");
	vector<string> hold(pos+1);//holds the actual content of the line.
	vector<int> bracketcount(pos+1);//keeps track of number of nested brackets

//	file.clear();
//	file.seekg(0);
	while(getline(file,hold[count]))
	{
		count++;
	}
	int endline = count;
	int p=0;
	while(p<=endline)
	{
		bracketcount[p]=0;
		p++;
	}
	count = 0;
	int len=0,i = 0, current=0;
	while(count<endline)
	{
		len = static_cast<int>(hold[count].length());
		i=0;
		while(i<=len)
		{
			if(hold[count][i] == '{')
			{
				current=count;
				while(current <endline)
				{
					current++;
					bracketcount[current]++;
				}
			}
			if(hold[count][i] == '}')
			{
				current=count;
				while(current <endline)
				{
					bracketcount[current]--;
					current++;
				}
			}
			i++;
		}
		count++;
	}
//	file.close();



//	ofstream file2("temp.txt");// consider this a safety net, change to fileName if you trust the code.
	stringstream file2;
	// I'm probably just being overly cautious.
	vector<string> indents(static_cast<unsigned long>(pos + 1));
	p=0;
	while(p<endline)
	{
		count=0;
		while(hold[p][count]=='\t')//check if there were already tabs in the file
		{
			count++;
		}
		while(bracketcount[p]>0)//tells indents[p] how many indents should be in a line
		{
			bracketcount[p]--;
			indents[p] +="\t";
		}
		if(count==0)//if the line wasn't previously indented in the code
		{
			file2<<indents[p]<<hold[p]<<endl;
		}
		else if(count>0)//If there were already indents in the code then set them to the right amount
		{
			file2<<indents[p];
			len = static_cast<int>(hold[p].length());
			i=0;
			while(i<len-count)
			{
				file2<<hold[p][i+count];
				i++;
			}
			file2<<endl;
		}
		p++;
	}
//	file2.close();
//	return pos;
	return file2.str();
}


string formatCCode(const string &code)
{
	string formattedCode = code;

	StringUtils::replaceAll(formattedCode,";",";\n");
	StringUtils::replaceAll(formattedCode,";\n\n",";\n");

	StringUtils::replaceAll(formattedCode,"\n ","\n");

	StringUtils::replaceAll(formattedCode,"{","\n{\n");

	StringUtils::replaceAll(formattedCode,"}","\n}");
	StringUtils::replaceAll(formattedCode,"\n\n}","\n}");

	return indent(formattedCode);
}





void printRule(const string &rule) {
	logFile << "At line no: " << line_count << " " << rule << endl << endl;
}

// after pushing value
void printCode(NONTERMINAL_TYPE ruleNonterminal){
	logFile << formatCCode(nonTerminalBuffer.getValue(ruleNonterminal)) << endl << endl;
}


void printRuleLog(NONTERMINAL_TYPE nonterminal, const string &rule)
{
	printRule(ruleName[nonterminal]+": "+rule);
	printCode(nonterminal);
}











#endif //SYNTAXANALYZER_SYNBASE_H
