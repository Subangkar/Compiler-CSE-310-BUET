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
#include <utility>

using std::stack;


#define SYMBOL_TABLE_SIZE 73


std::ofstream logFile, errorFile, parserFile;
SymbolTable table(SYMBOL_TABLE_SIZE);

int IDargs = 0;
vector<string> args;
bool funcDef = false;
int semErrors;
vector<SymbolInfo> params;
string variable_type;


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

const string ruleName[] = {"start", "program", "unit", "func_declaration", "func_definition", "parameter_list",
                           "compound_statement", "var_declaration", "type_specifier", "declaration_list", "statements",
                           "statement", "expression_statement", "variable", "expression", "logic_expression",
                           "rel_expression", "simple_expression", "term", "unary_expression", "factor", "argument_list",
                           "arguments"};

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
		if (nonterminal < start || nonterminal > arguments || terminalBuf[nonterminal].empty()) return string("");
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
	return val + ((val[val.length() - 1] == ' ') ? "" : " ");
}


void printRule(const string &rule) {
	logFile << "At line no: " << line_count << " " << rule << endl << endl;
}

// after pushing value
void printCode(NONTERMINAL_TYPE ruleNonterminal) {
	logFile << CodeParser::formatCCode(nonTerminalBuffer.getValue(ruleNonterminal)) << endl << endl;
//	logFile << (nonTerminalBuffer.getValue(ruleNonterminal)) << endl << endl;
}


void printRuleLog(NONTERMINAL_TYPE nonterminal, const string &rule) {
	printRule(ruleName[nonterminal] + ": " + rule);
	printCode(nonterminal);
}

void printErrorLog(string msg) {
	errorFile << " >> Error at line " << line_count << msg << endl << endl;
	++semErrors;
}

void printWarningLog(string msg) {
	errorFile << " >> Warning at line " << line_count << msg << endl << endl;
//	++semErrors;
}


SymbolInfo *insertToTable(SymbolInfo *symbolInfo) {
	table.insert(*symbolInfo);
	return table.lookUp(symbolInfo->getName());
}


// function ret type not set
void insertFunc(SymbolInfo *funcVal, SymbolInfo *retVal) {
	if (table.lookUp(*funcVal) != nullptr) {
		printErrorLog("Function " + funcVal->getName() + " already declared");
		return;
	}
//	table.printAllScope(logFile);
//	funcVal->setType("FUNC"); // not sure with this
	SymbolInfo *temp = insertToTable(funcVal);
	temp->setIDType("FUNC");
	temp->setFuncRet(retVal->getVarType());
	temp->ParamList = args;
	args.clear();
	table.printAllScope(logFile);
}

void addFuncDef(SymbolInfo *funcVal, SymbolInfo *retVal) {
	SymbolInfo *temp = table.lookUp(*funcVal);

	// to prevent f(int x,float,int y){} type defn
	if (args.size() != IDargs) {
		printErrorLog("Parameter mismatch for Function " + funcVal->getName());
	}

	if (temp != nullptr) {
		if (temp->isFuncDefined()) {
			printErrorLog("Function " + funcVal->getName() + " already defined");
		} else if (temp->getFuncRet() != retVal->getVarType()) {
			printErrorLog("Function " + funcVal->getName() + " :return type doesn't match declaration");
		} else if (temp->ParamList.size() != args.size()) {
			printErrorLog("Function " + funcVal->getName() + " :Parameter list doesn not match declaration");
		} else if (temp->ParamList != args) {
			printErrorLog("Function " + funcVal->getName() + " :argument mismatch");
		}
	} else {
		insertFunc(funcVal, retVal);
		table.lookUp(*funcVal)->setFuncDefined(true);
	}

	args.clear();
	IDargs = 0;
}

SymbolInfo *insertVar(SymbolInfo *idVal) {
	if (variable_type == "VOID") {
		printErrorLog("variable type can't be void");
	} else {
		SymbolInfo *temp = table.lookUp(idVal->getName(), true);
		if (temp != nullptr) {
			printErrorLog("Variable " + idVal->getName() + " already declared");
		} else {
			SymbolInfo *temp2 = insertToTable(idVal);
			temp2->setVarType(variable_type);
			temp2->setIDType("VAR");
			return temp2;
		}
	}
	return nullptr;
}

void insertArr(SymbolInfo *idVal, SymbolInfo *size) {
	SymbolInfo *arr = insertVar(idVal);
	arr->setIDType("ARA");
	arr->setArrSize(static_cast<size_t>(atoi(size->getName().data())));
	for (int i = 0; i < arr->getArrSize(); i++) {
		arr->ints.push_back(0);
//		arr->chars.push_back(0);
		arr->floats.push_back(0);
	}
}

SymbolInfo *getType(string type) {
	variable_type = std::move(type);
	return new SymbolInfo(variable_type,variable_type);
}

void addTypeArgtoParamList(const string &type) {
	args.push_back(type);//changed from rhs->getVarType()
}

void addIDenArgtoParamList(SymbolInfo *idVal) {
	addTypeArgtoParamList(variable_type);

	IDargs++;
	idVal->setIDType("VAR");
	idVal->setVarType(variable_type);//changed from rhs->getVarType()
	SymbolInfo *temp = new SymbolInfo(idVal->getName(), idVal->getType());
	temp->setIDType("VAR");

	params.push_back(*temp);
}

void enterFuncScope() {
	table.enterScope();
	for (const auto &param : params) table.insert(param);
	params.clear();
}

void exitFuncScope() {
	table.printAllScope(logFile);
	table.exitScope();
}


SymbolInfo *getAssignExpVal(SymbolInfo *lhs, SymbolInfo *rhs) {
	if (lhs->getIDType() == "ARA") {
		lhs->ints.push_back(0);

		if (rhs->getVarType() == "INT") {
			if (rhs->getIDType() == "VAR")lhs->setIndexValue(rhs->ints[0]);
			else lhs->setIndexValue(rhs->ints[rhs->getArrIndex()]);
		} else {
			printWarningLog("Assigning float value to integer");
			if (rhs->getIDType() == "VAR")lhs->setIndexValue((int) rhs->floats[0]);
			else lhs->setIndexValue((int) rhs->floats[rhs->getArrIndex()]);
		}
	} else if (lhs->getIDType() == "VAR") {
		if (rhs->getVarType() == "INT") {
			if (rhs->getIDType() == "VAR")lhs->setVarValue(rhs->ints[0]);
			else lhs->setVarValue(rhs->ints[rhs->getArrIndex()]);
		} else {
			printWarningLog("Assigning float value to integer");
			if (rhs->getIDType() == "VAR")lhs->setVarValue(rhs->floats[0]);
			else lhs->setVarValue(rhs->floats[rhs->getArrIndex()]);
		}
	}
	return lhs;
}

SymbolInfo* getLogicOpVal(SymbolInfo *left,SymbolInfo *right,SymbolInfo *op){
	SymbolInfo* opVal = new SymbolInfo("","");//new SymbolInfo("INT");// ?
	opVal->setVarType("INT");
	if(left->getVarType() == "CHAR" || right->getVarType() == "CHAR"){
		opVal->ints.push_back(0);
	}

	const string &logicop = op->getName();
	if(logicop == "&&"){
		if(left->getVarType() == "FLOAT"){
			left->floats.push_back(0);

			if(right->getVarType() == "FLOAT"){
				right->floats.push_back(0);
				opVal->ints[0] = (left->floats[0] == 0) && (right->floats[0] == 0);
			}
			else if(right->getVarType() == "INT"){
				right->ints.push_back(0);
				opVal->ints[0] = (left->floats[0] == 0) && (right->ints[0] == 0);
			}
		}
		else if(left->getVarType() == "INT"){
			left->ints.push_back(0);
			if(right->getVarType() == "FLOAT"){
				right->floats.push_back(0);
				opVal->ints[0] = (left->ints[0] == 0) && (right->floats[0] == 0);
			}
			else if(right->getVarType() == "INT"){
				right->ints.push_back(0);
				opVal->ints[0] = (left->ints[0] == 0) && (right->ints[0] == 0);
			}
		}
	}
	else if(logicop == "||"){
		if(left->getVarType() == "FLOAT"){
			left->floats.push_back(0);
			if(left->floats[0] != 0){
				opVal->ints[0] = 1;
			}
			else if(right->getVarType() == "FLOAT"){
				right->floats.push_back(0);
				if(right->floats[0] != 0) opVal->ints[0] = 1;
				else opVal->ints[0] =0;
			}
			else if(right->getVarType() == "INT"){
				if(right->ints[0] != 0) opVal->ints[0] = 1;
				else opVal->ints[0] =0;
			}
		}
		else if(left->getVarType() == "INT"){
			left->ints.push_back(0);
			if(left->ints[0] != 0) opVal->ints[0] = 1;
			else if(right->getVarType() == "FLOAT"){
				right->floats.push_back(0);
				if(right->floats[0] != 0) opVal->ints[0] = 1;
				else opVal->ints[0] =0;
			}
			else if(right->getVarType() == "INT"){
				right->ints.push_back(0);
				if(right->ints[0] != 0) opVal->ints[0] = 1;
				else opVal->ints[0] =0;
			}
		}
	}
	return opVal;
}

#endif //SYNTAXANALYZER_SYNBASE_H
