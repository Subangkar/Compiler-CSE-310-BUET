//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H


#include <iostream>
#include "1505015_DataStructure.h"
#include "1505015_Utils.h"
#include <stack>
#include <sstream>
#include <utility>
#include <cstdlib>


using std::stack;


#define ERROR_VAL popVal(error)//"$ERROR$" //

#define INFINITY_INT  numeric_limits<int>::max();
#define INFINITY_FLOAT numeric_limits<float>::infinity()

#define SYMBOL_TABLE_SIZE 73


std::ofstream logFile, errorFile, parserFile;
SymbolTable table(SYMBOL_TABLE_SIZE);

size_t syntaxErrors = 0;
size_t warnings = 0;

size_t IDargsNo = 0;
vector<string> argsType;
vector<SymbolInfo> parameters;
vector<SymbolInfo> argsFunc;
SymbolInfo *currentFunc = nullptr;

string variableType;

bool errorRule = false;
string lookAheadBuf;


extern int line_count;
extern int err_count_lex;

extern FILE *yyin;
extern char *yytext;

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
	arguments,
	error
};

class NonTerminalBuffer {
private:
	stack<string> nonTerminalBuf[NONTERMINAL_TYPE::error + 1];
	NONTERMINAL_TYPE lastNonTerminal = NONTERMINAL_TYPE::error;
public:
	string getValue(NONTERMINAL_TYPE nonterminal) {
		if (nonterminal < start || nonterminal > lastNonTerminal || nonTerminalBuf[nonterminal].empty())
			return string("");
		return nonTerminalBuf[nonterminal].top();
	}

	string extractValue(NONTERMINAL_TYPE nonterminal) {
		if (nonterminal < start || nonterminal > lastNonTerminal) return string("");

		if (nonTerminalBuf[nonterminal].empty()) return "";

		string str = nonTerminalBuf[nonterminal].top();
		nonTerminalBuf[nonterminal].pop();
		return str;
	}

	void pushValue(NONTERMINAL_TYPE nonterminal, const string &val) {
		if (nonterminal < start || nonterminal > lastNonTerminal) return;

		nonTerminalBuf[nonterminal].push(val);
	}
};


NonTerminalBuffer nonTerminalBuffer;
#pragma endregion


void pushVal(NONTERMINAL_TYPE nonterminal, const string &val) {
	nonTerminalBuffer.pushValue(nonterminal, val);
}

string popVal(NONTERMINAL_TYPE nonterminal) {
	string val = nonTerminalBuffer.extractValue(nonterminal);
	val = (isalnum(val[0]) ? " " : "") + val + (isalnum(val[val.length() - 1]) ? " " : "");
	return val + ((val[val.length() - 1] == ' ') ? "" : " ");
}


void printRule(const string &rule) {
	logFile << "At line no: " << line_count << " " << rule << endl << endl;
	if (!errorRule) parserFile << "At line no: " << line_count << " " << rule << endl << endl;
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
	++syntaxErrors;
}

void printErrorRuleLog(const string &msg, NONTERMINAL_TYPE nonterminal, const string &rule) {
	if (!msg.empty()) printErrorLog(msg);
	errorRule = true;
	printRuleLog(nonterminal, rule);
	errorRule = false;
	popVal(error);
	lookAheadBuf.clear();
}

void printWarningLog(const string &msg) {
	errorFile << " >> Warning at line " << line_count << ": " << msg << endl << endl;
	warnings++;
}

void printDebug(const string &msg) {
	cout << ">> " << "line no " << line_count << ": " << msg << endl;
}


void yyerror(const char *s) {
	printDebug(string(s) + " < " + yytext + ":=>:" + lookAheadBuf + " >");
	pushVal(error, ERROR_VAL + lookAheadBuf);
	errorRule = true;
	lookAheadBuf = yytext;
}


SymbolInfo *nullVal() {
	static SymbolInfo *defVal = nullptr;
	if (defVal == nullptr) {
		defVal = new SymbolInfo("$NULL$", "");
		defVal->setIDType(VARIABLE);
		defVal->setVarType(INT_TYPE);
		defVal->intData.push_back(0);
		defVal->floatData.push_back(0);
	}
	return defVal;
}

SymbolInfo *insertToTable(SymbolInfo *symbolInfo) {
	table.insert(*symbolInfo);
	return table.lookUp(symbolInfo->getName());
}


SymbolInfo *getType(const string &type) {
	variableType = type;
	return new SymbolInfo(variableType, variableType);
}

SymbolInfo *insertVar(SymbolInfo *idVal) {
	if (variableType == VOID_TYPE) {
		printErrorLog("variable type can't be void");
	} else {
		if (table.lookUp(idVal->getName(), true) != nullptr) {
			printErrorLog("Symbol " + idVal->getName() + " already declared in this scope");
		} else {
			SymbolInfo *var = insertToTable(idVal);
			var->setVarType(variableType);
			var->setIDType(VARIABLE);
			return var;
		}
	}
	return nullVal();
}

void insertArr(SymbolInfo *idVal, SymbolInfo *size) {
	SymbolInfo *arr = insertVar(idVal);
	arr->setIDType(ARRAY);
	arr->setArrSize(static_cast<size_t>(StringParser::toInteger(size->getName().data())));
	for (int i = 0; i < arr->getArrSize(); i++) {
		arr->intData.push_back(0);
		arr->floatData.push_back(0);
	}
}


void clearFunctionArgs() {
	argsType.clear();
	argsFunc.clear();
	parameters.clear();
	IDargsNo = 0;
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
	func->setFuncRetType(retType->getName());
	func->setVarType(retType->getName());
	func->paramList = argsType;

	if (IDargsNo && find(argsType.begin(), argsType.end(), VOID_TYPE) != argsType.end()) {
		printErrorLog("Function " + funcVal->getName() + ": prototype can't be void");
	}
//	clearFunctionArgs();
}

void addFuncDef(SymbolInfo *funcVal, SymbolInfo *retType) {
	SymbolInfo *func = table.lookUp(*funcVal);
	// to prevent f(int x,float,int y){} type defn but f(void){} allowed
	if (!(argsType.size() == 1 && argsType[0] == VOID_TYPE) && argsType.size() != IDargsNo) {
		printErrorLog("Unnamed prototype Parameter not allowed in Definition of Function " + funcVal->getName());
	} else if (func != nullptr) {
		if (func->isFuncDefined()) {
			printErrorLog("Function " + funcVal->getName() + " already defined");
		} else if (func->getFuncRetType() != retType->getName()) {
			printErrorLog("Function " + funcVal->getName() + " :return type doesn't match declaration");
		} else if (func->paramList.size() != argsType.size()) {
			printErrorLog("Function " + funcVal->getName() + " :Parameter list doesn not match declaration");
		} else if (func->paramList != argsType) {
			printErrorLog("Function " + funcVal->getName() + " :argument mismatch");
		} else {
			func->setFuncDefined(true);
		}
	} else {
		insertFunc(funcVal, retType);
		func = table.lookUp(*funcVal);
		func->setFuncDefined(true);

		currentFunc = func;
	}
}

void addTypeArgtoParamList(const string &type) {
	argsType.push_back(type);//changed from rhs->getVarType()
}

void addIDenArgtoParamList(SymbolInfo *idVal) {
	addTypeArgtoParamList(variableType);

	IDargsNo++;
	idVal->setIDType(VARIABLE);
	idVal->setVarType(variableType);//changed from rhs->getVarType()
	SymbolInfo *id = new SymbolInfo(idVal->getName(), idVal->getType());
	id->setIDType(VARIABLE);
	id->setVarType(variableType);

	parameters.push_back(*id);
}

void checkReturnType(SymbolInfo *exp) {
	if (currentFunc != nullptr && currentFunc->getFuncRetType() != exp->getVarType()) {
		printErrorLog(currentFunc->getName() + ": function return type does not match with return expression type");
	}
}

void enterFuncScope() {
	table.enterScope();
	printLog("\n >> Entered into ScopeTable #" + to_string(table.getCurrentId()));
	for (const auto &param : parameters) table.insert(param);
	clearFunctionArgs();
}

void exitFuncScope() {
	currentFunc = nullptr;

	table.printAllScope(logFile);
	printLog("\n >> ScopeTable #" + to_string(table.getCurrentId()) + " removed");
	table.exitScope();
}


SymbolInfo *getConstVal(SymbolInfo *constVal, const string &constVarType) {
	constVal->setIDType(VARIABLE);
	constVal->setVarType(constVarType);

	if (constVarType == FLOAT_TYPE) {
		constVal->floatData.push_back(0);
		constVal->fltValue() = static_cast<float>(atof(constVal->getName().data()));
	} else if (constVarType == INT_TYPE) {
		constVal->intData.push_back(0);
		constVal->intValue() = StringParser::toInteger(constVal->getName().data());
	}
	return constVal;
}

SymbolInfo *getConstVal(const string &value = "$CONST$", const string &constVarType = INT_TYPE) {
	SymbolInfo *constVal = new SymbolInfo(value, "CONST");
	constVal->setIDType(VARIABLE);
	constVal->setVarType(constVarType);

	if (constVarType == FLOAT_TYPE) {
		constVal->setType("CONST_FLOAT");
		constVal->floatData.push_back(0);
		constVal->fltValue() = static_cast<float>(atof(constVal->getName().data()));
	} else if (constVarType == INT_TYPE) {
		constVal->setType("CONST_INT");
		constVal->intData.push_back(0);
		constVal->intValue() = StringParser::toInteger(constVal->getName().data());
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
		} else if (idxVal->getVarType() != INT_TYPE) {
			printErrorLog(arrVal->getName() + " array index must be an integer");
		} else {
			arr->setArrIndex(static_cast<size_t>(idxVal->intValue()));
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
		lhs->intData.push_back(0);

		if (rhs->getVarType() == INT_TYPE) {
			if (lhs->getVarType() == FLOAT_TYPE)printWarningLog("Assigning integer value to float");

//			if (rhs->isVariable())lhs->setIndexValue(rhs->intValue());
//			else lhs->setIndexValue(rhs->intValue());
		} else {
			if (lhs->getVarType() == INT_TYPE)printWarningLog("Assigning float value to integer");

//			if (rhs->isVariable())lhs->setIndexValue((int) rhs->fltValue());
//			else lhs->setIndexValue((int) rhs->fltValue());
		}
	} else if (lhs->isVariable()) {
		if (rhs->getVarType() == INT_TYPE) {
			if (lhs->getVarType() == FLOAT_TYPE)printWarningLog("Assigning integer value to float");

//			if (rhs->isVariable())lhs->setVarValue(rhs->intValue());
//			else lhs->setVarValue(rhs->intValue());
		} else {
//			if (lhs->getVarType() == INT_TYPE)printWarningLog("Assigning float value to integer");

//			if (rhs->isVariable())lhs->setVarValue(rhs->fltValue());
//			else lhs->setVarValue(rhs->fltValue());
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
	int &result = opVal->intValue();

	int8_t cmpMode = 0x00; // 0 -> int F-> float 0F -> int-float

	if (left->getVarType() == INT_TYPE) {
		leftIVal = left->intValue();
		cmpMode &= 0x0F;
	} else {
		leftFVal = left->fltValue();
		cmpMode |= 0xF0;
	}

	if (right->getVarType() == INT_TYPE) {
		rightIVal = right->intValue();
		cmpMode &= 0xF0;
	} else {
		rightFVal = right->fltValue();
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
//	printDebug("Logic Exp Val: " + to_string(opVal->intValue()));
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
	int &result = opVal->intValue();


	int8_t cmpMode = 0x00; // 0 -> int F-> float 0F -> int-float

	if (left->getVarType() == INT_TYPE) {
		leftIVal = left->intValue();
		cmpMode &= 0x0F;
	} else {
		leftFVal = left->fltValue();
		cmpMode |= 0xF0;
	}

	if (right->getVarType() == INT_TYPE) {
		rightIVal = right->intValue();
		cmpMode &= 0xF0;
	} else {
		rightFVal = right->fltValue();
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
		opVal = getConstVal(opVal, FLOAT_TYPE);
	} else {
		opVal = getConstVal(opVal, INT_TYPE);
	}
//	if (opVal->getVarType() == FLOAT_TYPE)
//		printDebug(addop + " Operation Val: " + to_string(opVal->fltValue()));
//	else if (opVal->getVarType() == INT_TYPE)
//		printDebug(addop + " Operation Val: " + to_string(opVal->intValue()));

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


//	if (opVal->getVarType() == FLOAT_TYPE)
//		printDebug(mulOp + " Operation Val: " + to_string(opVal->fltValue()));
//	else if (opVal->getVarType() == INT_TYPE)
//		printDebug(mulOp + " Operation Val: " + to_string(opVal->intValue()));

	return opVal;
}


SymbolInfo *getIncOpVal(SymbolInfo *varVal) {
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, varVal->getVarType());
//	if (varVal->isArrayVar()) {
//		if (varVal->getVarType() == INT_TYPE) {
//			opVal->intValue() = ++varVal->intValue();
//		} else if (varVal->getVarType() == FLOAT_TYPE) {
//			opVal->fltValue() = ++varVal->fltValue();
//		}
//	} else if (varVal->isVariable()) {
//		if (varVal->getVarType() == INT_TYPE) {
//			opVal->intValue() = ++varVal->intValue();
//		} else if (varVal->getVarType() == FLOAT_TYPE) {
//			opVal->fltValue() = ++varVal->fltValue();
//		}
//	}
	return opVal;
}

SymbolInfo *getDecOpVal(SymbolInfo *varVal) {
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, varVal->getVarType());
//	if (varVal->isArrayVar()) {
//		if (varVal->getVarType() == INT_TYPE) {
//			opVal->intValue() = --varVal->intValue();
//		} else if (varVal->getVarType() == FLOAT_TYPE) {
//			opVal->fltValue() = --varVal->fltValue();
//		}
//	} else if (varVal->isVariable()) {
//		if (varVal->getVarType() == INT_TYPE) {
//			opVal->intValue() = --varVal->intValue();
//		} else if (varVal->getVarType() == FLOAT_TYPE) {
//			opVal->fltValue() = --varVal->fltValue();
//		}
//	}
	return opVal;
}

SymbolInfo *getNotOpVal(SymbolInfo *factor) {
	if (factor->getVarType() == VOID_TYPE) {
		printErrorLog("Invalid Operand for Logical Not Operation");
		return nullVal();
	}
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, INT_TYPE);
//	int value = 0;
//	if (factor->getVarType() == INT_TYPE) {
//		if (factor->isVariable()) value = factor->intValue();
//		else if (factor->isArrayVar())value = factor->intValue();
//	} else if (factor->getVarType() == FLOAT_TYPE) {
//		if (factor->isVariable()) value = (int) factor->fltValue();
//		else if (factor->isArrayVar()) value = (int) factor->fltValue();
//	}
//
//	opVal->intValue() = !value;
//	printDebug("NOT Exp Val: " + to_string(opVal->intValue()));
	return opVal;
}

SymbolInfo *getUniAddOpVal(SymbolInfo *varVal, SymbolInfo *op) {
	if (varVal->getVarType() == VOID_TYPE) {
		printErrorLog("Invalid Operand for Unary Operation");
		return nullVal();
	}
	SymbolInfo *opVal = new SymbolInfo("", "");
	opVal = getConstVal(opVal, varVal->getVarType());
//	const string &uniOp = op->getName();
//	if (varVal->getVarType() == FLOAT_TYPE) {
//		if (varVal->isVariable()) {
//			opVal->fltValue() = uniOp == "+" ? (varVal->fltValue()) : -(varVal->fltValue());
//		} else if (varVal->isArrayVar()) {
//			opVal->fltValue() =
//					uniOp == "+" ? (varVal->fltValue())
//					             : -(varVal->fltValue());
//		}
//	} else if (varVal->getVarType() == INT_TYPE) {
//		if (varVal->isVariable()) {
//			opVal->intValue() = uniOp == "+" ? (varVal->intValue()) : -(varVal->intValue());
//		} else if (varVal->isArrayVar()) {
//			opVal->intValue() =
//					uniOp == "+" ? (varVal->intValue()) : -(varVal->intValue());
//		}
//	}

//	if (opVal->getVarType() == FLOAT_TYPE)
//		printDebug(uniOp + " Unary Operation Val: " + to_string(opVal->fltValue()));
//	else if (opVal->getVarType() == INT_TYPE)
//		printDebug(uniOp + " Unary Operation Val: " + to_string(opVal->intValue()));

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
		if (func->paramList.size() != argsType.size()) {
			printErrorLog(func->getName() + ": " + to_string(func->paramList.size()) + " arguments expected" +
			              " instead passed " + to_string(argsType.size()));
		} else if (func->paramList != argsType) {
			printErrorLog(func->getName() + ": argument type Mismatch");
		}
		retVal = getConstVal("", func->getFuncRetType());
//		if (func->isVoidFunc()) printErrorLog("Function " + funcVal->getName() + " returns void");
	}
	clearFunctionArgs();
	return retVal;
}


#endif //SYNTAXANALYZER_SYNBASE_H
