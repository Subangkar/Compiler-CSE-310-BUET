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
#include <cstdlib>


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

#pragma-region RULE_CODE_BUF

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
#pragma endregion


void pushVal(NONTERMINAL_TYPE nonterminal, const string &val) {
	nonTerminalBuffer.pushValue(nonterminal, val);
}

string popVal(NONTERMINAL_TYPE nonterminal) {
	string val = nonTerminalBuffer.extractValue(nonterminal);
	val = (isalnum(val[0]) ? " " : "") + val + (isalnum(val[val.size()]) ? " " : "");
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


void printLog(const string &str) {
	logFile << str << endl;
}

void printRuleLog(NONTERMINAL_TYPE nonterminal, const string &rule) {
	printRule(ruleName[nonterminal] + ": " + rule);
	printCode(nonterminal);
}

void printErrorLog(const string &msg) {
	errorFile << " >> Error at line " << line_count << ": " << msg << endl << endl;
	++semErrors;
}

void printWarningLog(const string &msg) {
	errorFile << " >> Warning at line " << line_count << ": " << msg << endl << endl;
//	++semErrors;
}

void printDebug(const string &msg) {
	cout << ">> " << "line no " << line_count << ": " << msg << endl;
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
//	funcVal->setType(FUNCTION); // not sure with this
	SymbolInfo *temp = insertToTable(funcVal);
	temp->setIDType(FUNCTION);
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
	if (variable_type == VOID_TYPE) {
		printErrorLog("variable type can't be void");
	} else {
		SymbolInfo *temp = table.lookUp(idVal->getName(), true);
		if (temp != nullptr) {
			printErrorLog("Variable " + idVal->getName() + " already declared");
		} else {
			SymbolInfo *temp2 = insertToTable(idVal);
			temp2->setVarType(variable_type);
			temp2->setIDType(VARIABLE);
			return temp2;
		}
	}
	return nullptr;
}

void insertArr(SymbolInfo *idVal, SymbolInfo *size) {
	SymbolInfo *arr = insertVar(idVal);
	arr->setIDType(ARRAY);
	arr->setArrSize(static_cast<size_t>(atoi(size->getName().data())));
	for (int i = 0; i < arr->getArrSize(); i++) {
		arr->ints.push_back(0);
//		arr->chars.push_back(0);
		arr->floats.push_back(0);
	}
}

SymbolInfo *getType(string type) {
	variable_type = std::move(type);
	return new SymbolInfo(variable_type, variable_type);
}

void addTypeArgtoParamList(const string &type) {
	args.push_back(type);//changed from rhs->getVarType()
}

void addIDenArgtoParamList(SymbolInfo *idVal) {
	addTypeArgtoParamList(variable_type);

	IDargs++;
	idVal->setIDType(VARIABLE);
	idVal->setVarType(variable_type);//changed from rhs->getVarType()
	SymbolInfo *temp = new SymbolInfo(idVal->getName(), idVal->getType());
	temp->setIDType(VARIABLE);

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
	if (lhs->isArrayVar()) {
		lhs->ints.push_back(0);

		if (rhs->getVarType() == INT_TYPE) {
			if (rhs->isVariable())lhs->setIndexValue(rhs->ints[0]);
			else lhs->setIndexValue(rhs->ints[rhs->getArrIndex()]);
		} else {
			printWarningLog("Assigning float value to integer");
			if (rhs->isVariable())lhs->setIndexValue((int) rhs->floats[0]);
			else lhs->setIndexValue((int) rhs->floats[rhs->getArrIndex()]);
		}
	} else if (lhs->isVariable()) {
		if (rhs->getVarType() == INT_TYPE) {
			if (rhs->isVariable())lhs->setVarValue(rhs->ints[0]);
			else lhs->setVarValue(rhs->ints[rhs->getArrIndex()]);
		} else {
			printWarningLog("Assigning float value to integer");
			if (rhs->isVariable())lhs->setVarValue(rhs->floats[0]);
			else lhs->setVarValue(rhs->floats[rhs->getArrIndex()]);
		}
	}
	return lhs;
}

SymbolInfo *getLogicOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {
	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Operand of void type.");
		return nullptr;
	}

	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal->setVarType(INT_TYPE);
	const string &logicOp = op->getName();
	int leftIVal = 0, rightIVal = 0;
	float leftFVal = 0, rightFVal = 0;
	int &result = opVal->ints[0];

	int8_t cmpMode = 0x00; // 0 -> int F-> float 0F -> int-float

	if (left->getVarType() == INT_TYPE) {
		leftIVal = left->ints[0];
		cmpMode &= 0x0F;
	} else {
		leftFVal = left->floats[0];
		cmpMode |= 0xF0;
	}

	if (right->getVarType() == INT_TYPE) {
		rightIVal = right->ints[0];
		cmpMode &= 0xF0;
	} else {
		rightFVal = right->floats[0];
		cmpMode |= 0x0F;
	}

	if (cmpMode == 0x00) {
		result = logicOp == "&&" ? leftIVal && rightIVal :
		         logicOp == "||" ? leftIVal || rightIVal : 0;
	} else if (cmpMode == 0x0F) {
		result = logicOp == "&&" ? leftIVal && rightFVal :
		         logicOp == "||" ? leftIVal || rightFVal : 0;
	} else if (cmpMode == 0xF0) {
		result = logicOp == "&&" ? leftFVal && rightIVal :
		         logicOp == "||" ? leftFVal || rightIVal : 0;
	} else if (cmpMode == 0xFF) {
		result = logicOp == "&&" ? leftFVal && rightFVal :
		         logicOp == "||" ? leftFVal || rightFVal : 0;
	}

	if (left->getVarType() != right->getVarType()) {
		printWarningLog("Comparision between two different types.");
	}
	printDebug("Logic Exp Val: " + to_string(opVal->ints[0]));
	return opVal;
}

SymbolInfo *getReltnOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {

	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Can't compare with void type expressions");
		return nullptr;
	}

	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal->setVarType(INT_TYPE);
	const string &relop = op->getName();
	int leftIVal = 0, rightIVal = 0;
	float leftFVal = 0, rightFVal = 0;
	int &result = opVal->ints[0];


	int8_t cmpMode = 0x00; // 0 -> int F-> float 0F -> int-float

	if (left->getVarType() == INT_TYPE) {
		leftIVal = left->ints[0];
		cmpMode &= 0x0F;
	} else {
		leftFVal = left->floats[0];
		cmpMode |= 0xF0;
	}

	if (right->getVarType() == INT_TYPE) {
		rightIVal = right->ints[0];
		cmpMode &= 0xF0;
	} else {
		rightFVal = right->floats[0];
		cmpMode |= 0x0F;
	}

	if (cmpMode == 0x00) {
		result = relop == "==" ? leftIVal == rightIVal :
		         relop == "!=" ? leftIVal != rightIVal :
		         relop == "<=" ? leftIVal <= rightIVal :
		         relop == ">=" ? leftIVal >= rightIVal :
		         relop == "<" ? leftIVal < rightIVal :
		         relop == ">" ? leftIVal > rightIVal : 0;
	} else if (cmpMode == 0x0F) {
		result = relop == "==" ? leftIVal == rightFVal :
		         relop == "!=" ? leftIVal != rightFVal :
		         relop == "<=" ? leftIVal <= rightFVal :
		         relop == ">=" ? leftIVal >= rightFVal :
		         relop == "<" ? leftIVal < rightFVal :
		         relop == ">" ? leftIVal > rightFVal : 0;
	} else if (cmpMode == 0xF0) {
		result = relop == "==" ? leftFVal == rightIVal :
		         relop == "!=" ? leftFVal != rightIVal :
		         relop == "<=" ? leftFVal <= rightIVal :
		         relop == ">=" ? leftFVal >= rightIVal :
		         relop == "<" ? leftFVal < rightIVal :
		         relop == ">" ? leftFVal > rightIVal : 0;
	} else if (cmpMode == 0xFF) {
		result = relop == "==" ? leftFVal == rightFVal :
		         relop == "!=" ? leftFVal != rightFVal :
		         relop == "<=" ? leftFVal <= rightFVal :
		         relop == ">=" ? leftFVal >= rightFVal :
		         relop == "<" ? leftFVal < rightFVal :
		         relop == ">" ? leftFVal > rightFVal : 0;
	}

	if (relop == "==" && left->getVarType() != right->getVarType()) {
		printWarningLog("Comparision between two different types.");
	}

//	printDebug("Rel Exp Val: " + to_string(result));
	return opVal;
}

SymbolInfo *getFuncCallValue(SymbolInfo *funcVal) {
	SymbolInfo *func = table.lookUp(funcVal->getName());
	if (func == nullptr) {
		printErrorLog("Function " + funcVal->getName() + " doesn't exist");
	} else if (!func->isFunction()) {
		printErrorLog(funcVal->getName() + " + is not a function");
	} else if (func->isVoidFunc()) {
		printErrorLog("Function " + funcVal->getName() + " returns void");
	}
//	else if(func->ParamList!= nullptr){
//
//	}
	else {
		SymbolInfo *retVal = new SymbolInfo("", "");
		retVal->setVarType(funcVal->getFuncRet());
		if (retVal->getVarType() == INT_TYPE)retVal->ints[0] = 0;
		else if (retVal->getVarType() == FLOAT_TYPE)retVal->floats[0] = 0;
//			else if(retVal->getVarType() == CHAR_TYPE)retVal->chars[0] = '\0';
		return retVal;
	}
	return nullptr;
}

SymbolInfo *getArgValueList() {

}

SymbolInfo *getConstVal(SymbolInfo *constVal, const string &const_type) {
	constVal->setIDType(VARIABLE);
	constVal->setVarType(const_type);

	if (const_type == FLOAT_TYPE)
		constVal->floats[0] = static_cast<float>(atof(constVal->getName().data()));
	else if (const_type == INT_TYPE)
		constVal->ints[0] = atoi(constVal->getName().data());
	return constVal;
}

SymbolInfo *getIncOpVal(SymbolInfo *varVal) {
	if (varVal->isArrayVar()) {
		if (varVal->getVarType() == INT_TYPE) {
			varVal->setIndexValue(varVal->ints[varVal->getArrIndex()] + 1);
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			varVal->setIndexValue((float) (varVal->floats[varVal->getArrIndex()] + 1.0));
		}
	} else if (varVal->isVariable()) {
		if (varVal->getVarType() == INT_TYPE) {
			varVal->ints[0] = varVal->ints[0] + 1;
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			varVal->floats[0] = static_cast<float>(varVal->floats[0] + 1.0);
		}
	}
	return varVal;
}

SymbolInfo *getDecOpVal(SymbolInfo *varVal) {
	if (varVal->isArrayVar()) {
		if (varVal->getVarType() == INT_TYPE) {
			varVal->setIndexValue(varVal->ints[varVal->getArrIndex()] - 1);
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			varVal->setIndexValue((float) (varVal->floats[varVal->getArrIndex()] - 1.0));
		}
	} else if (varVal->isVariable()) {
		if (varVal->getVarType() == INT_TYPE) {
			varVal->ints[0] = varVal->ints[0] - 1;
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			varVal->floats[0] = static_cast<float>(varVal->floats[0] - 1.0);
		}
	}
	return varVal;
}


#endif //SYNTAXANALYZER_SYNBASE_H
