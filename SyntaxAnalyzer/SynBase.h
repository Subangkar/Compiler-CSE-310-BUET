//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H


#include <iostream>
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
vector<SymbolInfo> argsFunc;
bool funcDef = false;
size_t semErrors = 0;
size_t warnings = 0;
vector<SymbolInfo> params;
string variable_type;


extern FILE *yyin;
extern int line_count;

extern const char *yytext;

int yyparse();

int yylex();


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
	parserFile << "At line no: " << line_count << " " << rule << endl << endl;
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
	warnings++;
}

void printDebug(const string &msg) {
	cout << ">> " << "line no " << line_count << ": " << msg << endl;
}


void yyerror(const char *s) {
	//write your code
//	printErrorLog("Invalid Syntax: "+string(yytext));
//	printDebug("Syntax Error: "+string(yytext));
//	yyparse();
//	cout << "HERE: " << s <<endl;
//	printDebug(s);
//	printErrorLog(s);
	printLog(" >> Syntax Error :: ");
}


SymbolInfo *nullVal() {
	static SymbolInfo *defVal = nullptr;
	if (defVal == nullptr) {
		defVal = new SymbolInfo("$NULL$", "");
		defVal->setIDType(VARIABLE);
		defVal->setVarType(INT_TYPE);
		defVal->ints.push_back(0);
		defVal->floats.push_back(0);
	}
	return defVal;
}

SymbolInfo *insertToTable(SymbolInfo *symbolInfo) {
	table.insert(*symbolInfo);
	return table.lookUp(symbolInfo->getName());
}


SymbolInfo *getType(string type) {
	variable_type = std::move(type);
	return new SymbolInfo(variable_type, variable_type);
}

SymbolInfo *insertVar(SymbolInfo *idVal) {
	if (variable_type == VOID_TYPE) {
		printErrorLog("variable type can't be void");
	} else {
		if (table.lookUp(idVal->getName(), true) != nullptr) {
			printErrorLog("Symbol " + idVal->getName() + " already declared in this scope");
		} else {
			SymbolInfo *var = insertToTable(idVal);
			var->setVarType(variable_type);
			var->setIDType(VARIABLE);
			return var;
		}
	}
	return nullVal();
}

void insertArr(SymbolInfo *idVal, SymbolInfo *size) {
	SymbolInfo *arr = insertVar(idVal);
	arr->setIDType(ARRAY);
	arr->setArrSize(static_cast<size_t>(atoi(size->getName().data())));
	for (int i = 0; i < arr->getArrSize(); i++) {
		arr->ints.push_back(0);
		arr->floats.push_back(0);
	}
}


void clearFunctionArgs() {
	args.clear();
	argsFunc.clear();
	params.clear();
	IDargs = 0;
}

void insertFunc(SymbolInfo *funcVal, SymbolInfo *retType) {
	if (table.lookUp(*funcVal) != nullptr) {
		printErrorLog("Function " + funcVal->getName() + " already declared");
		clearFunctionArgs();
		return;
	}
	SymbolInfo *func = insertToTable(funcVal);
	func->setType("ID");
	func->setIDType(FUNCTION);
	func->setFuncRet(retType->getName());
	func->setVarType(retType->getName());
	func->ParamList = args;
	clearFunctionArgs();
}

void addFuncDef(SymbolInfo *funcVal, SymbolInfo *retType) {
	SymbolInfo *func = table.lookUp(*funcVal);
	// to prevent f(int x,float,int y){} type defn
	if (args.size() != IDargs) {
		printErrorLog("Parameter mismatch for Function " + funcVal->getName());
	} else if (func != nullptr) {
		if (func->isFuncDefined()) {
			printErrorLog("Function " + funcVal->getName() + " already defined");
		} else if (func->getFuncRet() != retType->getName()) {
			printErrorLog("Function " + funcVal->getName() + " :return type doesn't match declaration");
		} else if (func->ParamList.size() != args.size()) {
			printErrorLog("Function " + funcVal->getName() + " :Parameter list doesn not match declaration");
		} else if (func->ParamList != args) {
			printErrorLog("Function " + funcVal->getName() + " :argument mismatch");
		} else {
			func->setFuncDefined(true);
		}
	} else {
		insertFunc(funcVal, retType);
		table.lookUp(*funcVal)->setFuncDefined(true);
	}
	clearFunctionArgs();
}

void addTypeArgtoParamList(const string &type) {
	args.push_back(type);//changed from rhs->getVarType()
}

void addIDenArgtoParamList(SymbolInfo *idVal) {
	addTypeArgtoParamList(variable_type);

	IDargs++;
	idVal->setIDType(VARIABLE);
	idVal->setVarType(variable_type);//changed from rhs->getVarType()
	SymbolInfo *id = new SymbolInfo(idVal->getName(), idVal->getType());
	id->setIDType(VARIABLE);

	params.push_back(*id);
}

void enterFuncScope() {
	table.enterScope();
	printLog("\n >> Entered into ScopeTable #" + to_string(table.getCurrentId()));
	for (const auto &param : params) table.insert(param);
	clearFunctionArgs();
}

void exitFuncScope() {
	table.printAllScope(logFile);
	printLog("\n >> ScopeTable #" + to_string(table.getCurrentId()) + " removed");
	table.exitScope();
}


SymbolInfo *getConstVal(SymbolInfo *constVal, const string &constVarType) {
	constVal->setIDType(VARIABLE);
	constVal->setVarType(constVarType);

	if (constVarType == FLOAT_TYPE) {
		constVal->floats.push_back(0);
		constVal->floats[0] = static_cast<float>(atof(constVal->getName().data()));
	} else if (constVarType == INT_TYPE) {
		constVal->ints.push_back(0);
		constVal->ints[0] = atoi(constVal->getName().data());
	}
	return constVal;
}

SymbolInfo *getConstVal(const string &value = "$CONST$", const string &constVarType = INT_TYPE) {
	SymbolInfo *constVal = new SymbolInfo(value, "CONST");
	constVal->setIDType(VARIABLE);
	constVal->setVarType(constVarType);

	if (constVarType == FLOAT_TYPE) {
		constVal->setType("CONST_FLOAT");
		constVal->floats.push_back(0);
		constVal->floats[0] = static_cast<float>(atof(constVal->getName().data()));
	} else if (constVarType == INT_TYPE) {
		constVal->setType("CONST_INT");
		constVal->ints.push_back(0);
		constVal->ints[0] = atoi(constVal->getName().data());
	}
	return constVal;
}


SymbolInfo *getVariable(SymbolInfo *varVal) {
	SymbolInfo *var = table.lookUp(varVal->getName());
	if (var == nullptr) {
		printErrorLog(varVal->getName() + " doesn't exist");
	} else if (!var->isVariable()) {
		if (var->isArrayVar()) printErrorLog(varVal->getName() + " is an array must be accessed with an index");
		else printErrorLog(varVal->getName() + " is not a variable");
	} else {
		return var;
	}
	return nullVal();
}

SymbolInfo *getArrIndexVar(SymbolInfo *arrVal, SymbolInfo *idxVal) {
	SymbolInfo *arr = table.lookUp(arrVal->getName());
	if (arr == nullptr) {
		printErrorLog(arrVal->getName() + " doesn't exist");
		return nullVal();
	} else {
		if (!arr->isArrayVar()) {
			printErrorLog(arrVal->getName() + " is not an array");
		} else if (idxVal->getVarType() == FLOAT_TYPE) {
			printErrorLog(arrVal->getName() + " array index must be an integer");
		} else if (idxVal->ints[0] >= arr->getArrSize()) {
			printErrorLog(
					arrVal->getName() + " array index out of bounds index=" + to_string(idxVal->ints[0]) + " size=" +
					to_string(arr->getArrSize()));
		} else {
			arr->setArrIndex(static_cast<size_t>(idxVal->ints[0]));
		}
	}
	return arr;
}


SymbolInfo *getAssignExpVal(SymbolInfo *lhs, SymbolInfo *rhs) {
	if (lhs->getVarType() == VOID_TYPE || rhs->getVarType() == VOID_TYPE) {
		printErrorLog("Assign Operation on void type");
		return nullVal();
	}

	if (lhs->isArrayVar()) {
		lhs->ints.push_back(0);

		if (rhs->getVarType() == INT_TYPE) {
			if (lhs->getVarType() == FLOAT_TYPE)printWarningLog("Assigning integer value to float");

			if (rhs->isVariable())lhs->setIndexValue(rhs->ints[0]);
			else lhs->setIndexValue(rhs->ints[rhs->getArrIndex()]);
		} else {
			if (lhs->getVarType() == INT_TYPE)printWarningLog("Assigning float value to integer");

			if (rhs->isVariable())lhs->setIndexValue((int) rhs->floats[0]);
			else lhs->setIndexValue((int) rhs->floats[rhs->getArrIndex()]);
		}
	} else if (lhs->isVariable()) {
		if (rhs->getVarType() == INT_TYPE) {
			if (lhs->getVarType() == FLOAT_TYPE)printWarningLog("Assigning integer value to float");

			if (rhs->isVariable())lhs->setVarValue(rhs->ints[0]);
			else lhs->setVarValue(rhs->ints[rhs->getArrIndex()]);
		} else {
			if (lhs->getVarType() == INT_TYPE)printWarningLog("Assigning float value to integer");

			if (rhs->isVariable())lhs->setVarValue(rhs->floats[0]);
			else lhs->setVarValue(rhs->floats[rhs->getArrIndex()]);
		}
	}
	return lhs;
}

SymbolInfo *getLogicOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {
	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Operand of void type.");
		return nullVal();
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
//	printDebug("Logic Exp Val: " + to_string(opVal->ints[0]));
	return opVal;
}

SymbolInfo *getReltnOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {

	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Can't compare with void type expressions");
		return nullVal();
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


SymbolInfo *getAddtnOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {
	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Operand of void type.");
		return nullVal();
	}

	const string &addop = op->getName();
	SymbolInfo *opVal = new SymbolInfo("", "");
	if (left->getVarType() == FLOAT_TYPE || right->getVarType() == FLOAT_TYPE) {
		getConstVal(opVal, FLOAT_TYPE);
	} else {
		getConstVal(opVal, INT_TYPE);
	}

	if (addop == "+") {
		if (left->isVariable()) {
			if (right->isVariable()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[0] + right->ints[0];
					} else {
						opVal->floats[0] = left->floats[0] + right->floats[0];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[0] + right->floats[0];
					} else {
						opVal->floats[0] = left->floats[0] + right->floats[0];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[0] + right->ints[0];
				}
			} else if (right->isArrayVar()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[0] + right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[0] + right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[0] + right->floats[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[0] + right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[0] + right->ints[right->getArrIndex()];
				}
			}
		} else if (left->isArrayVar()) {
			if (right->isVariable()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[left->getArrIndex()] + right->ints[0];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] + right->floats[0];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[left->getArrIndex()] + right->floats[0];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] + right->floats[0];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[left->getArrIndex()] + right->ints[0];
				}
			} else if (right->isArrayVar()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[left->getArrIndex()] + right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] + right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[left->getArrIndex()] + right->floats[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] + right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[left->getArrIndex()] + right->ints[right->getArrIndex()];
				}
			}
		}
	} else if (addop == "-") {
		if (left->isVariable()) {
			if (right->isVariable()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[0] - right->ints[0];
					} else {
						opVal->floats[0] = left->floats[0] - right->floats[0];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[0] - right->floats[0];
					} else {
						opVal->floats[0] = left->floats[0] - right->floats[0];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[0] - right->ints[0];
				}
			} else if (right->isArrayVar()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[0] - right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[0] - right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[0] - right->floats[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[0] - right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[0] - right->ints[right->getArrIndex()];
				}
			}
		} else if (left->isArrayVar()) {
			if (right->isVariable()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[left->getArrIndex()] - right->ints[0];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] - right->floats[0];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[left->getArrIndex()] - right->floats[0];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] - right->floats[0];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[left->getArrIndex()] - right->ints[0];
				}
			} else if (right->isArrayVar()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[left->getArrIndex()] - right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] - right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[left->getArrIndex()] - right->floats[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] - right->floats[right->getArrIndex()];
					}
				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {
					opVal->setVarType(INT_TYPE);
					opVal->ints[0] = left->ints[left->getArrIndex()] - right->ints[right->getArrIndex()];
				}
			}
		}
	}
//	if (opVal->getVarType() == FLOAT_TYPE)
//		printDebug(addop + " Operation Val: " + to_string(opVal->floats[0]));
//	else if (opVal->getVarType() == INT_TYPE)
//		printDebug(addop + " Operation Val: " + to_string(opVal->ints[0]));

	return opVal;
}

SymbolInfo *getMultpOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {
	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Operand of void type.");
		return nullVal();
	}

	const string &mulOp = op->getName();

	if (mulOp == "%" && (left->getVarType() == FLOAT_TYPE || right->getVarType() == FLOAT_TYPE)) {
		printErrorLog("Float operand for mod operator");
		return nullVal();
	}
	SymbolInfo *opVal = new SymbolInfo("", "");
	if (left->getVarType() == FLOAT_TYPE || right->getVarType() == FLOAT_TYPE) {
		opVal = getConstVal(opVal, FLOAT_TYPE);
	} else {
		opVal = getConstVal(opVal, INT_TYPE);
	}

	if (mulOp == "%") {

//		opVal->setIDType("VAR");// ????
		if (left->getVarType() == INT_TYPE && right->getVarType() == INT_TYPE) {
			if (left->isVariable()) {
				if (right->isVariable())opVal->ints[0] = (left->ints[0]) % (right->ints[0]);
				else opVal->ints[0] = (left->ints[0]) % (right->ints[right->getArrIndex()]);
			} else {
				if (right->isVariable())opVal->ints[0] = (left->ints[left->getArrIndex()]) % (right->ints[0]);
				else opVal->ints[0] = (left->ints[left->getArrIndex()]) % (right->ints[right->getArrIndex()]);
			}
		}

	} else if (mulOp == "*") {
		if (left->isVariable()) {
			if (right->isVariable()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[0] * right->ints[0];
					} else {
						opVal->floats[0] = left->floats[0] * right->floats[0];
					}

				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[0] * right->floats[0];
					} else {
						opVal->floats[0] = left->floats[0] * right->floats[0];
					}

				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {

					opVal->ints[0] = left->ints[0] * right->ints[0];

				}
			} else if (right->isArrayVar()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[0] * right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[0] * right->floats[right->getArrIndex()];
					}

				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[0] * right->floats[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[0] * right->floats[right->getArrIndex()];
					}

				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {

					opVal->ints[0] = left->ints[0] * right->ints[0];

				}
			}
		} else if (left->isArrayVar()) {
			if (right->isVariable()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[left->getArrIndex()] * right->ints[0];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] * right->floats[0];
					}

				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[left->getArrIndex()] * right->floats[0];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] * right->floats[0];
					}

				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {

					opVal->ints[0] = left->ints[left->getArrIndex()] * right->ints[0];

				}
			} else if (right->isArrayVar()) {
				if (left->getVarType() == FLOAT_TYPE) {

					if (right->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->floats[left->getArrIndex()] * right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] * right->floats[right->getArrIndex()];
					}

				} else if (right->getVarType() == FLOAT_TYPE) {

					if (left->getVarType() == INT_TYPE) {
						opVal->floats[0] = left->ints[left->getArrIndex()] * right->floats[right->getArrIndex()];
					} else {
						opVal->floats[0] = left->floats[left->getArrIndex()] * right->floats[right->getArrIndex()];
					}

				} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {

					opVal->ints[0] = left->ints[left->getArrIndex()] * right->ints[0];

				}
			}
		}
	} else if (mulOp == "/") {
		if (left->getVarType() == FLOAT_TYPE) {

			if (right->getVarType() == INT_TYPE) {
				if (left->isVariable()) {
					if (right->isVariable()) {
						if (right->ints[0] != 0)opVal->floats[0] = left->floats[0] / right->ints[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->ints[right->getArrIndex()] != 0)
							opVal->floats[0] = left->floats[0] / right->ints[right->getArrIndex()];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				} else if (left->isArrayVar()) {
					if (right->isVariable()) {
						if (right->ints[0] != 0)opVal->floats[0] = left->floats[left->getArrIndex()] / right->ints[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->ints[right->getArrIndex()] != 0) {
							opVal->floats[0] = left->floats[left->getArrIndex()] / right->ints[right->getArrIndex()];
						} else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				}
			} else if (right->getVarType() == FLOAT_TYPE) {
				if (left->isVariable()) {
					if (right->isVariable()) {
						if (right->floats[0] != 0)opVal->floats[0] = left->floats[0] / right->floats[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->floats[right->getArrIndex()] != 0)
							opVal->floats[0] = left->floats[0] / right->floats[right->getArrIndex()];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				} else if (left->isArrayVar()) {
					if (right->isVariable()) {
						if (right->floats[0] != 0)
							opVal->floats[0] = left->floats[left->getArrIndex()] / right->floats[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->floats[right->getArrIndex()] != 0) {
							opVal->floats[0] = left->floats[left->getArrIndex()] / right->floats[right->getArrIndex()];
						} else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				}
			}

		} else if (right->getVarType() == FLOAT_TYPE) {

			if (left->getVarType() == INT_TYPE) {
				if (left->isVariable()) {
					if (right->isVariable()) {
						if (right->floats[0] != 0)opVal->floats[0] = left->ints[0] / right->floats[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->floats[right->getArrIndex()] != 0)
							opVal->floats[0] = left->ints[0] / right->floats[right->getArrIndex()];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				} else if (left->isArrayVar()) {
					if (right->isVariable()) {
						if (right->floats[0] != 0)opVal->floats[0] = left->ints[left->getArrIndex()] / right->floats[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->floats[right->getArrIndex()] != 0) {
							opVal->floats[0] = left->ints[left->getArrIndex()] / right->floats[right->getArrIndex()];
						} else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				}
			} else if (left->getVarType() == FLOAT_TYPE) {
				if (left->isVariable()) {
					if (right->isVariable()) {
						if (right->floats[0] != 0)opVal->floats[0] = left->floats[0] / right->floats[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->floats[right->getArrIndex()] != 0)
							opVal->floats[0] = left->floats[0] / right->floats[right->getArrIndex()];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				} else if (left->isArrayVar()) {
					if (right->isVariable()) {
						if (right->floats[0] != 0)
							opVal->floats[0] = left->floats[left->getArrIndex()] / right->floats[0];
						else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					} else if (right->isArrayVar()) {
						if (right->floats[right->getArrIndex()] != 0) {
							opVal->floats[0] = left->floats[left->getArrIndex()] / right->floats[right->getArrIndex()];
						} else {
							opVal->floats[0] = numeric_limits<float>::infinity();
							printErrorLog("Divide by zero");
						}
					}
				}
			}

		} else if (right->getVarType() == INT_TYPE && left->getVarType() == INT_TYPE) {

			if (left->isVariable()) {
				if (right->isVariable()) {
					if (right->ints[0] != 0)opVal->ints[0] = left->ints[0] / right->ints[0];
					else {
						opVal->ints[0] = numeric_limits<int>::max();
						printErrorLog("Divide by zero");
					}
				} else if (right->isArrayVar()) {
					if (right->ints[right->getArrIndex()] != 0)
						opVal->ints[0] = left->ints[0] / right->ints[right->getArrIndex()];
					else {
						opVal->ints[0] = numeric_limits<int>::max();
						printErrorLog("Divide by zero");
					}
				}
			} else if (left->isArrayVar()) {
				if (right->isVariable()) {
					if (right->ints[0] != 0)opVal->ints[0] = left->ints[left->getArrIndex()] / right->ints[0];
					else {
						opVal->ints[0] = numeric_limits<int>::max();
						printErrorLog("Divide by zero");
					}
				} else if (right->isArrayVar()) {
					if (right->ints[right->getArrIndex()] != 0) {
						opVal->ints[0] = left->ints[left->getArrIndex()] / right->ints[right->getArrIndex()];
					} else {
						opVal->floats[0] = numeric_limits<int>::max();
						printErrorLog("Divide by zero");
					}
				}
			}

		}
	}

//	if (opVal->getVarType() == FLOAT_TYPE)
//		printDebug(mulOp + " Operation Val: " + to_string(opVal->floats[0]));
//	else if (opVal->getVarType() == INT_TYPE)
//		printDebug(mulOp + " Operation Val: " + to_string(opVal->ints[0]));

	return opVal;
}


SymbolInfo *getIncOpVal(SymbolInfo *varVal) {
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, varVal->getVarType());
	if (varVal->isArrayVar()) {
		if (varVal->getVarType() == INT_TYPE) {
			opVal->ints[0] = ++varVal->ints[varVal->getArrIndex()];
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			opVal->floats[0] = ++varVal->floats[varVal->getArrIndex()];
		}
	} else if (varVal->isVariable()) {
		if (varVal->getVarType() == INT_TYPE) {
			opVal->ints[0] = ++varVal->ints[0];
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			opVal->floats[0] = ++varVal->floats[0];
		}
	}
	return opVal;
}

SymbolInfo *getDecOpVal(SymbolInfo *varVal) {
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, varVal->getVarType());
	if (varVal->isArrayVar()) {
		if (varVal->getVarType() == INT_TYPE) {
			opVal->ints[0] = --varVal->ints[varVal->getArrIndex()];
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			opVal->floats[0] = --varVal->floats[varVal->getArrIndex()];
		}
	} else if (varVal->isVariable()) {
		if (varVal->getVarType() == INT_TYPE) {
			opVal->ints[0] = --varVal->ints[0];
		} else if (varVal->getVarType() == FLOAT_TYPE) {
			opVal->floats[0] = --varVal->floats[0];
		}
	}
	return opVal;
}

SymbolInfo *getNotOpVal(SymbolInfo *factor) {
	if (factor->getVarType() == VOID_TYPE) {
		printErrorLog("Invalid Operand for Logical Not Operation");
		return nullVal();
	}
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, INT_TYPE);
	int value = 0;
	if (factor->getVarType() == INT_TYPE) {
		if (factor->isVariable()) value = factor->ints[0];
		else if (factor->isArrayVar())value = factor->ints[factor->getArrIndex()];
	} else if (factor->getVarType() == FLOAT_TYPE) {
		if (factor->isVariable()) value = (int) factor->floats[0];
		else if (factor->isArrayVar()) value = (int) factor->floats[factor->getArrIndex()];
	}

	opVal->ints[0] = !value;
//	printDebug("NOT Exp Val: " + to_string(opVal->ints[0]));
	return opVal;
}

SymbolInfo *getUniAddOpVal(SymbolInfo *varVal, SymbolInfo *op) {
	if (varVal->getVarType() == VOID_TYPE) {
		printErrorLog("Invalid Operand for Unary Operation");
		return nullVal();
	}
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, varVal->getVarType());
	const string &uniOp = op->getName();
	if (varVal->getVarType() == FLOAT_TYPE) {
		if (varVal->isVariable()) {
			opVal->floats[0] = uniOp == "+" ? (varVal->floats[0]) : -(varVal->floats[0]);
		} else if (varVal->isArrayVar()) {
			opVal->floats[0] =
					uniOp == "+" ? (varVal->floats[varVal->getArrIndex()]) : -(varVal->floats[varVal->getArrIndex()]);
		}
	} else if (varVal->getVarType() == INT_TYPE) {
		if (varVal->isVariable()) {
			opVal->ints[0] = uniOp == "+" ? (varVal->ints[0]) : -(varVal->ints[0]);
		} else if (varVal->isArrayVar()) {
			opVal->ints[0] =
					uniOp == "+" ? (varVal->ints[varVal->getArrIndex()]) : -(varVal->ints[varVal->getArrIndex()]);
		}
	}

	if (opVal->getVarType() == FLOAT_TYPE)
		printDebug(uniOp + " Unary Operation Val: " + to_string(opVal->floats[0]));
	else if (opVal->getVarType() == INT_TYPE)
		printDebug(uniOp + " Unary Operation Val: " + to_string(opVal->ints[0]));

	return opVal;
}


SymbolInfo *getFuncCallValue(SymbolInfo *funcVal) {
	SymbolInfo *func = table.lookUp(funcVal->getName()), *retVal = nullVal();
	if (func == nullptr) {
		printErrorLog("Function " + funcVal->getName() + " doesn't exist");
	} else if (!func->isFunction()) {
		printErrorLog(funcVal->getName() + " is not a function");
	} else if (!func->isFuncDefined()) {
		printErrorLog(funcVal->getName() + " does not have a body");
	} else {
		if (func->ParamList.size() != args.size()) {
			printErrorLog(func->getName() + ": " + to_string(func->ParamList.size()) + " arguments expected" +
			              " instead passed " + to_string(args.size()));
		} else if (func->ParamList != args) {
			printErrorLog(func->getName() + ": argument type Mismatch");
		}
		retVal = getConstVal("", func->getFuncRet());
//		if (func->isVoidFunc()) printErrorLog("Function " + funcVal->getName() + " returns void");
	}
	clearFunctionArgs();
	return retVal;
}


//SymbolInfo *getArgValueList() {
//
//}


#endif //SYNTAXANALYZER_SYNBASE_H
