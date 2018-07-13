//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H

#include "AsmBase.h"


using std::stack;


std::ofstream logFile, errorFile;

size_t syntaxErrors = 0;
size_t warnings = 0;

size_t IDargsNo = 0;
vector<string> argsType;
vector<SymbolInfo> parameters;
vector<SymbolInfo> argsFunc;
SymbolInfo *currentFunc = nullptr;

string variableType;


extern int line_count;
extern int err_count_lex;

extern FILE *yyin;
extern char *yytext;

int yyparse();

int yylex();




void printLog(const string &str) {
	logFile << str << endl;
}

void printErrorLog(const string &msg) {
	errorFile << " >> Error at line " << line_count << ": " << msg << endl << endl;
	++syntaxErrors;
}

void printWarningLog(const string &msg) {
	errorFile << " >> Warning at line " << line_count << ": " << msg << endl << endl;
	warnings++;
}

void printDebug(const string &msg) {
	cout << ">> " << "line no " << line_count << ": " << msg << endl;
}


void yyerror(const char *s) {
	printDebug(string(s) + " < " + yytext + " >");
}

SymbolInfo *nullVal() {
	static SymbolInfo *defVal = nullptr;
	if (defVal == nullptr) {
		defVal = new SymbolInfo("$NULL$", "");
		defVal->setIDType(VARIABLE);
		defVal->setVarType(INT_TYPE);
	}
	return defVal;
}

SymbolInfo *insertToTable(SymbolInfo *symbolInfo) {
	symbolInfo->setScopeID(table.getCurrentId());
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
			addData(var->getName());
			return var;
		}
	}
	return nullVal();
}

void insertArr(SymbolInfo *idVal, SymbolInfo *size) {
	if (variableType == VOID_TYPE) {
		printErrorLog("variable type can't be void");
	} else {
		if (table.lookUp(idVal->getName(), true) != nullptr) {
			printErrorLog("Symbol " + idVal->getName() + " already declared in this scope");
		}
	}
	SymbolInfo *arr = insertToTable(idVal);
	arr->setIDType(ARRAY);
	arr->setArrSize(static_cast<size_t>(StringParser::toInteger(size->getName())));
	addData(arr->getName(), true);
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
	func->setReturnLabel(newRetLabel(func->getName()));

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
			currentFunc = func;
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
	for (auto &param : parameters) {
		param.setScopeID(table.getCurrentId());
		table.insert(param);
		SymbolInfo sym(ASM_VAR_NAME(param.getName()));
		currentFunc->parameters.push_back(sym);
		addData(param.getName());
	}

	clearFunctionArgs();
}

void exitFuncScope() {
//	currentFunc = nullptr;

	table.printAllScope(logFile);
	printLog("\n >> ScopeTable #" + to_string(table.getCurrentId()) + " removed");
	table.exitScope();
}


SymbolInfo *getConstVal(SymbolInfo *constVal, const string &constVarType) {
	constVal->setIDType(VARIABLE);
	constVal->setVarType(constVarType);
	return constVal;
}

SymbolInfo *getConstVal(const string &value = "$CONST$", const string &constVarType = INT_TYPE) {
	SymbolInfo *constVal = new SymbolInfo(value, "CONST");
	constVal->setIDType(VARIABLE);
	constVal->setVarType(constVarType);

	if (constVarType == FLOAT_TYPE) {
		constVal->setType("CONST_FLOAT");
	} else if (constVarType == INT_TYPE) {
		constVal->setType("CONST_INT");
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
		var->code = "";
		return var;
	}
	return nullVal();
}

SymbolInfo *getArrIndexVar(SymbolInfo *arrVal, SymbolInfo *idxVal) {
	auto arr = table.lookUp(arrVal->getName());
	if (arr == nullptr) {
		printErrorLog(arrVal->getName() + " doesn't exist");
		return nullVal();
	} else {
		if (!arr->isArrayVar()) {
			printErrorLog(arrVal->getName() + " is not an array");
		} else if (idxVal->getVarType() != INT_TYPE) {
			printErrorLog(arrVal->getName() + " array index must be an integer");
		} else {
			arr->setArrIndexVarName(idxVal->getName());
		}
	}
	auto *ret = new SymbolInfo(*arr);
	ret->setType(TEMPORARY);
	return ret;
}


SymbolInfo *getAssignExpVal(SymbolInfo *lhs, SymbolInfo *rhs) {
	if (lhs->getVarType() == VOID_TYPE || rhs->getVarType() == VOID_TYPE) {
		printErrorLog("Assign Operation on void type");
		return nullVal();
	}
	lhs->code = rhs->code + memoryToMemory(*lhs, *rhs);
	deleteTemp(rhs);
	return lhs;
}

SymbolInfo *getLogicOpVal(SymbolInfo *lhs, SymbolInfo *rhs, SymbolInfo *op) {
	if (lhs->getVarType() == VOID_TYPE || rhs->getVarType() == VOID_TYPE) {
		printErrorLog("Operand of void type.");
		return nullVal();
	}

	SymbolInfo *opVal = new SymbolInfo(newTemp(true), TEMPORARY);
	opVal->setVarType(INT_TYPE);
	const string &logicOp = op->getName();
	opVal->code = lhs->code + rhs->code;
	string temp = opVal->getName();
	string resIs0 = newLabel();
	string resIs1 = newLabel();

	if (logicOp == "&&") {
		opVal->code += jumpTo(resIs0, "JE", lhs, &zero);
		opVal->code += jumpTo(resIs0, "JE", rhs, &zero);
		opVal->code += setConstValue(temp, "1");
		opVal->code += jumpTo(resIs1);
		opVal->code += addLabel(resIs0);
		opVal->code += setConstValue(temp, "0");
		opVal->code += addLabel(resIs1);
	} else if (logicOp == "||") {
		opVal->code += jumpTo(resIs1, "JNE", lhs, &zero);
		opVal->code += jumpTo(resIs1, "JNE", rhs, &zero);
		opVal->code += setConstValue(temp, "0");
		opVal->code += jumpTo(resIs0);
		opVal->code += addLabel(resIs1);
		opVal->code += setConstValue(temp, "1");
		opVal->code += addLabel(resIs0);
	}

	if (lhs->getVarType() != rhs->getVarType()) {
		printWarningLog("Comparision between two different types.");
	}
	deleteTemp(lhs, rhs);
	return opVal;
}

SymbolInfo *getReltnOpVal(SymbolInfo *lhs, SymbolInfo *rhs, SymbolInfo *op) {

	if (lhs->getVarType() == VOID_TYPE || rhs->getVarType() == VOID_TYPE) {
		printErrorLog("Can't compare with void type expressions");
		return nullVal();
	}

	SymbolInfo *opVal = new SymbolInfo(newTemp(true), TEMPORARY);
	opVal->setVarType(INT_TYPE);
	const string &relop = op->getName();

	opVal->code = lhs->code + rhs->code;
	string temp = opVal->getName();
	string label1 = newLabel();
	string label2 = newLabel();
	string jmpCondition;
	if (relop == "<") {
		jmpCondition = "JL ";
	} else if (relop == ">") {
		jmpCondition = "JG ";
	} else if (relop == ">=") {
		jmpCondition = "JGE ";
	} else if (relop == "<=") {
		jmpCondition = "JLE ";
	} else if (relop == "==") {
		jmpCondition = "JE ";
	} else if (relop == "!=") {
		jmpCondition = "JNE ";
	}

	opVal->code += jumpTo(label1, jmpCondition, lhs, rhs);
	opVal->code += setConstValue(temp, "0");
	opVal->code += jumpTo(label2);
	opVal->code += addLabel(label1);
	opVal->code += setConstValue(temp, "1");
	opVal->code += addLabel(label2);

	if (relop == "==" && lhs->getVarType() != rhs->getVarType()) {
		printWarningLog("Comparision between two different types.");
	}
	deleteTemp(lhs, rhs);
	return opVal;
}


SymbolInfo *getAddtnOpVal(SymbolInfo *left, SymbolInfo *right, SymbolInfo *op) {
	if (left->getVarType() == VOID_TYPE || right->getVarType() == VOID_TYPE) {
		printErrorLog("Operand of void type.");
		return nullVal();
	}

	const string &addop = op->getName();
	SymbolInfo *opVal = new SymbolInfo(newTemp(), TEMPORARY);
	opVal->setVarType(INT_TYPE);
	opVal->code = left->code + right->code;
	opVal->code += addMemoryValues(addop, *opVal, *left, *right);
	deleteTemp(left, right);
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
	SymbolInfo *opVal = new SymbolInfo(newTemp(), TEMPORARY);
	opVal->setVarType(INT_TYPE);
	opVal->code = left->code + right->code;
	opVal->code += multMemoryValues(mulOp, *opVal, *left, *right);
	deleteTemp(left, right);
	return opVal;
}


SymbolInfo *getIncOpVal(SymbolInfo *varVal, const string &op = "++") {
	auto opVal = new SymbolInfo(newTemp(), TEMPORARY);
	opVal->setVarType(INT_TYPE);
	opVal->code += memoryToMemory(*opVal, *varVal);
	opVal->code += incMemoryValue(*varVal, op == "++" ? "INC" : "DEC");
	return opVal;
}

SymbolInfo *getNotOpVal(SymbolInfo *factor) {
	if (factor->getVarType() == VOID_TYPE) {
		printErrorLog("Invalid Operand for Logical Not Operation");
		return nullVal();
	}
	auto opVal = new SymbolInfo(newTemp(), TEMPORARY);
	opVal = getConstVal(opVal, INT_TYPE);
	opVal->code = notMemoryValue(*opVal, *factor);
	deleteTemp(factor);
	return opVal;
}

SymbolInfo *getUniAddOpVal(SymbolInfo *varVal, SymbolInfo *op) {
	if (varVal->getVarType() == VOID_TYPE) {
		printErrorLog("Invalid Operand for Unary Operation");
		return nullVal();
	}

	const string &uniOp = op->getName();
	if (uniOp == "+") return varVal;
	auto opVal = new SymbolInfo(newTemp(), TEMPORARY);
	opVal = getConstVal(opVal, varVal->getVarType());
	opVal->code = varVal->code;
	opVal->code += minusMemoryValue(*opVal, *varVal);
	return opVal;
}


SymbolInfo *getFuncCallValue(SymbolInfo *funcVal) {
	SymbolInfo *func = table.lookUp(funcVal->getName()), *retVal = nullVal();
	if (func == nullptr) {
		printErrorLog("Function " + funcVal->getName() + " doesn't exist");
	} else if (!func->isFunction()) {
		printErrorLog(funcVal->getName() + " is not a function");
	}
//	else if (!func->isFuncDefined()) {
//		printErrorLog(funcVal->getName() + " does not have a body");
//	}
	else {
		if (func->paramList.size() != argsType.size()) {
			printErrorLog(func->getName() + ": " + to_string(func->paramList.size()) + " arguments expected" +
			              " instead passed " + to_string(argsType.size()));
		} else if (func->paramList != argsType) {
			printErrorLog(func->getName() + ": argument type Mismatch");
		}

		//pass arguments
//		for(const auto& arg:func->parameters){
////			memoryToMemory(arg,)
//		}

		retVal = new SymbolInfo(newTemp(),TEMPORARY);
		retVal->setVarType(func->getFuncRetType());
		for (int i = 0; i < func->parameters.size(); ++i) {
			retVal->code += memoryToMemory(func->parameters[i],argsFunc[i]);
		}
		retVal->code += procRetValue(*retVal,*funcVal);
//		if (func->isVoidFunc()) printErrorLog("Function " + funcVal->getName() + " returns void");
	}
	clearFunctionArgs();
	return retVal;
}


#endif //SYNTAXANALYZER_SYNBASE_H
