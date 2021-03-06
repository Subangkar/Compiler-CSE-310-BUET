//
// Created by subangkar on 7/11/18.
//

#ifndef IRGENERATOR_ASMBASE_H
#define IRGENERATOR_ASMBASE_H

#define PROC_START(x)   string(x)+" PROC\r\n"
#define PROC_END(x)     string(x)+" ENDP\r\n"
#define SCOPE_NO(x) "$"+(x)
#define NEWLINE_ASM string("\r\n")
#define ASM_INT_TYPE " DW "


#define FILE_OUTDEC_PROC "Dec16bitOutput.asm"
#define SYMBOL_TABLE_SIZE 73

#include <iostream>
#include "DataStructure.h"
#include "Utils.h"
#include "ASMParser.h"
#include <stack>
#include <sstream>
#include <utility>
#include <cstdlib>


ofstream asmFile, optAsmFile;
SymbolTable table(SYMBOL_TABLE_SIZE);


string codeSegment, dataSegment;

int labelCount = 0;
int tempCount = 0;
int pTempCount = 0;
int maxTemp = -1;
int maxpTemp = -1;


void makeCRLF(string &str) {
	StringUtils::replaceAll(str, "\r", "");
	StringUtils::replaceAll(str, "\n", "\r\n");

	if (str[str.size() - 1] != '\n')str += "\r\n";
}

string getOUTDEC_PROC() {
	static string _OUTDEC_PROC;
	if (!_OUTDEC_PROC.empty()) return _OUTDEC_PROC;
	std::ifstream file(FILE_OUTDEC_PROC);
	if (file) {
		file.seekg(0, std::ios::end);
		std::streampos length = file.tellg();
		file.seekg(0, std::ios::beg);
		auto *buff = new char[length + (std::streampos) 1];
		file.read(buff, length);
		buff[length] = NULL;
		_OUTDEC_PROC = buff;
		delete[] buff;
	}
	StringUtils::replaceAll(_OUTDEC_PROC, "\r", "");
	StringUtils::replaceAll(_OUTDEC_PROC, "\n", NEWLINE_ASM);
	return _OUTDEC_PROC;
}

string ASM_VAR_NAME(const string &cVarName) {
	SymbolInfo *symbol = table.lookUp(cVarName);
	if (symbol == nullptr || symbol->getType() != "ID") return cVarName;
	return cVarName + SCOPE_NO(table.lookUp(cVarName)->getScopeID());
}

void addData(const string &varname, bool isArray = false) {
	string code;
	if (!isArray) code = ASM_VAR_NAME(varname) + ASM_INT_TYPE + "0";
	else code = ASM_VAR_NAME(varname) + ASM_INT_TYPE + to_string(table.lookUp(varname)->getArrSize()) + " DUP(0)";
	makeCRLF(code);
	dataSegment += code;
}

void addCode(const string &code) {
	string formattedCode = code;
	makeCRLF(formattedCode);
	codeSegment += formattedCode;
}


string getInst(const string &code) {
	string formattedCode = code;
	makeCRLF(formattedCode);
	return formattedCode;
}


string newLabel() {
	string lb = "L";
	char b[3];
	sprintf(b, "%d", labelCount);
	labelCount++;
	lb += b;
	return lb;
}

string newRetLabel(const string &funcName) {
	return "RET_" + funcName;
}

string newTemp(bool isLogical = false) {
	string t = !isLogical ? "t" : "p";
	char b[3];
	sprintf(b, "%d", !isLogical ? tempCount : pTempCount);
	t += b;
	if (!isLogical && tempCount > maxTemp) {
		maxTemp++;
		addData(t);
	} else if (isLogical && pTempCount > maxpTemp) {
		maxpTemp++;
		addData(t);
	}
	!isLogical ? tempCount++ : pTempCount++;
	return t;
}


void writeASM() {

	string initDataSeg = "\tMOV AX,@DATA\r\n\tMOV DS,AX\r\n" + NEWLINE_ASM;
	string retOS = "MOV AH, 4ch\r\n\tMOV AL,0\r\n\tINT 21h\r\n" + PROC_END("main");
	StringUtils::replaceAll(codeSegment, PROC_START("main"), PROC_START("main") + initDataSeg);
	StringUtils::replaceAll(codeSegment, PROC_END("main"), retOS);

	string optCodeSegment = ASMParser::optimizedASM(codeSegment);

	codeSegment += getOUTDEC_PROC();

	asmFile << ".model small\r\n";
	asmFile << "\r\n.stack 100h\r\n";
	asmFile << "\r\n.data\r\n" << dataSegment;
	asmFile << "\r\n.code\r\n" << codeSegment;

	asmFile << NEWLINE_ASM << NEWLINE_ASM;
	asmFile << "end main";


	optCodeSegment += getOUTDEC_PROC();

	optAsmFile << ".model small\r\n";
	optAsmFile << "\r\n.stack 100h\r\n";
	optAsmFile << "\r\n.data\r\n" << dataSegment;
	optAsmFile << "\r\n.code\r\n" << optCodeSegment;

	optAsmFile << NEWLINE_ASM << NEWLINE_ASM;
	optAsmFile << "end main";
}

string setIndexInREG(const string &reg, const string &offsetVar) {
	string code;
	code += "MOV " + reg + "," + ASM_VAR_NAME(offsetVar) + NEWLINE_ASM;
	code += "ADD " + reg + "," + reg + NEWLINE_ASM;
	return code;
}

string arrayToReg(const string &reg, const string &array, const string &offsetVar, const string &cmd = "MOV") {
	string code;
	code += setIndexInREG("SI", offsetVar);
	code += cmd + " " + reg + " ," + ASM_VAR_NAME(array) + "[SI]" + NEWLINE_ASM;
	return code;
}

string memorToReg(const string &reg, const string &mem, const string &cmd = "MOV") {
	return cmd + " " + reg + "," + ASM_VAR_NAME(mem) + NEWLINE_ASM;
}

string operToReg(const string &reg, const SymbolInfo &symbolInfo, const string &cmd = "MOV") {
	if (symbolInfo.isArrayVar()) return arrayToReg(reg, symbolInfo.getName(), symbolInfo.getArrIndexVarName(), cmd);
	else return memorToReg(reg, symbolInfo.getName(), cmd);
}

string compareREG(const string &reg, const SymbolInfo &symbolInfo) {
	string code;
	string oper2 = ASM_VAR_NAME(symbolInfo.getName());
	if (symbolInfo.isArrayVar()) {
		code += setIndexInREG("SI", symbolInfo.getArrIndexVarName());
		oper2 += "[SI]";
	}
	code += "CMP " + reg + "," + oper2 + NEWLINE_ASM;
	return code;
}

// reg=const_val set memory to
string regToMemory(const SymbolInfo &mem, const string &reg) {
	string code;
	string idx;
	if (mem.isArrayVar()) {
		code += setIndexInREG("DI", mem.getArrIndexVarName());
		idx = "[DI]";
	}
	code += "MOV " + ASM_VAR_NAME(mem.getName()) + idx + "," + reg + NEWLINE_ASM;
	return code;
}

string jumpTo(const string &label, const string &jmpInst = "", const SymbolInfo *sym1 = nullptr,
              const SymbolInfo *sym2 = nullptr) {
	if (jmpInst.empty()) return "JMP " + label + NEWLINE_ASM;
	string code;
	code += operToReg("AX", *sym1);
	code += compareREG("AX", *sym2);
	code += jmpInst + " " + label + NEWLINE_ASM;
	return code;
}

string setConstValue(const string &dest, const string &val) {
	return "MOV " + dest + "," + val + NEWLINE_ASM;
}

string addLabel(const string &label) {
	return label + ":" + NEWLINE_ASM;
}

string stackOp(const string &oper, const SymbolInfo &varName) {
	return oper + " " + ASM_VAR_NAME(varName.getName()) + NEWLINE_ASM;
}

string stackOp(const string &oper, const string &reg) {
	return oper + " " + reg + NEWLINE_ASM;
}


string memoryToMemory(const SymbolInfo &destSym, const SymbolInfo &srcSym) {
	string srcIdx;
	string code;

	if (StringUtils::isNumber(srcSym.getName())) {
		return regToMemory(destSym, srcSym.getName());
	}

	if (srcSym.isArrayVar()) {
		code += setIndexInREG("SI", srcSym.getArrIndexVarName());
		srcIdx = "[SI]";
	}
	string srcMem = ASM_VAR_NAME(srcSym.getName()) + srcIdx;

	code += memorToReg("DX", srcMem);
	code += regToMemory(destSym, "DX");
	return code;
}

string incMemoryValue(const SymbolInfo &mem, const string &opType) {
	string code;
	string idx;
	if (mem.isArrayVar()) {
		code += setIndexInREG("SI", ASM_VAR_NAME(mem.getArrIndexVarName()));
		idx = "[SI]";
	}
	code += opType + " " + ASM_VAR_NAME(mem.getName()) + idx + NEWLINE_ASM;
	return code;
}

//dest must be temp
string minusMemoryValue(const SymbolInfo &dest, const SymbolInfo &mem) {
	return memoryToMemory(dest, mem) + "NEG " + dest.getName() + NEWLINE_ASM;//+ "INC " + dest + NEWLINE_ASM;
}

string multMemoryValues(const string &op, const SymbolInfo &dest, const SymbolInfo &mult1, const SymbolInfo &mult2) {
	string oper;
	if (op == "*") oper = "IMUL ";
	else oper = "IDIV ";

	string code;
	string d = ASM_VAR_NAME(dest.getName());
	string mem1 = ASM_VAR_NAME(mult1.getName()), mem2 = ASM_VAR_NAME(mult2.getName());
	code += setConstValue("DX", "0");
	if (StringUtils::isNumber(mult2.getName())) {
		string t = newTemp();
		code += "MOV " + t + "," + mem2 + NEWLINE_ASM;
		mem2 = t;
	}
	if (!mult2.isArrayVar()) {
		code += operToReg("AX", mult1);
		code += oper + mem2 + NEWLINE_ASM;
	} else {
		code += operToReg("AX", mult1);
		code += setIndexInREG("SI", mult2.getArrIndexVarName());
		code += oper + ASM_VAR_NAME(mult2.getName()) + "[SI]" + NEWLINE_ASM;
	}
	if (op == "%") code += "MOV " + d + ", DX" + NEWLINE_ASM;
	else code += "MOV " + d + ", AX" + NEWLINE_ASM;
	return code;
}

string addMemoryValues(const string &op, const SymbolInfo &dest, const SymbolInfo &upper, const SymbolInfo &lower) {
	string oper;
	if (op == "+") oper = "ADD";
	else if (op == "-") oper = "SUB";
	else if (op == "&&") oper = "AND";
	else oper = "OR";

	string code;
	string d = ASM_VAR_NAME(dest.getName());
	code += operToReg("AX", upper, "MOV");
	code += operToReg("AX", lower, oper);
	code += "MOV " + d + ",AX" + NEWLINE_ASM;
	return code;
}

string procRetValue(const SymbolInfo &dest, const SymbolInfo &func) {
	string code;
	code += "PUSH DX" + NEWLINE_ASM;
	code += "CALL " + func.getName() + NEWLINE_ASM;
	code += regToMemory(dest, "DX");
	code += "POP DX" + NEWLINE_ASM;
	return code;
}


const SymbolInfo zero("0"), one("1");
extern SymbolInfo *currentFunc;


void printDebug(const string &);

void printErrorLog(const string &);

void deleteTemp(SymbolInfo *sym1, SymbolInfo *sym2 = nullptr, SymbolInfo *sym3 = nullptr, SymbolInfo *sym4 = nullptr) {
	return;
	if (sym1 != nullptr && sym1->getType() == TEMPORARY)delete sym1;
	if (sym2 != nullptr && sym2->getType() == TEMPORARY)delete sym2;
	if (sym3 != nullptr && sym3->getType() == TEMPORARY)delete sym3;
	if (sym4 != nullptr && sym4->getType() == TEMPORARY)delete sym4;
}

string printVarValue(SymbolInfo *symbol) {
	symbol = table.lookUp(symbol->getName());

	if (symbol != nullptr && symbol->isVariable()) {
		return getInst("MOV AX," + ASM_VAR_NAME(symbol->getName())) + getInst("CALL OUTDEC");
	} else {
		printErrorLog("Invalid Variable");
	}
}

string ifElseCode(SymbolInfo *expIf, SymbolInfo *stmtIf, SymbolInfo *stmtEls = nullptr) {
	string label_else_body = newLabel(), label_end_ifEls = newLabel();
	SymbolInfo zero = SymbolInfo("0");
	string code;
	code += expIf->code;
	code += jumpTo(label_else_body, "JE", expIf, &zero);
	code += stmtIf->code; // if-part
	if (stmtEls != nullptr) {
		code += jumpTo(label_end_ifEls);
		code += addLabel(label_else_body);
		code += stmtEls->code; // else-part
	}
	if (stmtEls != nullptr)code += addLabel(label_end_ifEls);
	else code += addLabel(label_else_body);

	deleteTemp(expIf);
	deleteTemp(stmtIf, stmtEls);
	return code;
}

string forLoopCode(SymbolInfo *expInit, SymbolInfo *expCond, SymbolInfo *expInc, SymbolInfo *stmt) {
	string code;
	string label1 = newLabel();
	string label2 = newLabel();
	code += expInit->code;
	code += addLabel(label1);
	code += expCond->code;
	code += jumpTo(label2, "JE", expCond, &zero);
	code += stmt->code;
	code += expInc->code;
	code += jumpTo(label1);
	code += addLabel(label2);

	deleteTemp(expInit, expCond);
	deleteTemp(expInc, stmt);
	return code;
}

string whlLoopCode(SymbolInfo *expCond, SymbolInfo *stmt) {
	string loop_start = newLabel(), loop_end = newLabel();
	string code;
	code += addLabel(loop_start);
	code += expCond->code;
	code += jumpTo(loop_end, "JE", expCond, &zero);
	code += stmt->code; // if-part
	code += jumpTo(loop_start);
	code += addLabel(loop_end);
	deleteTemp(expCond, stmt);
	return code;
}

string funcBodyCode(SymbolInfo *funcSrc, SymbolInfo *cstmt) {
	SymbolInfo *func = table.lookUp(funcSrc->getName());
	string code;
	code = PROC_START(func->getName());
	stack<string> memVars;
	if (func->getName() != "main") {
		for (int i = 0; i <= maxTemp; ++i) {
			memVars.push("t" + to_string(i));
			code += stackOp("PUSH", memVars.top());
		}
		for (int i = 0; i <= maxpTemp; ++i) {
			memVars.push("p" + to_string(i));
			code += stackOp("PUSH", memVars.top());
		}
		for (const string &var:func->memberVars) {
			code += stackOp("PUSH", var);
			memVars.push(var);
		}
		code += stackOp("PUSH", "AX");
	}
	code += cstmt->code;
	code += addLabel(func->getReturnLabel());
	if (func->getName() != "main") {
		if (func->getName() != "main") code += stackOp("POP", "AX");
		while (!memVars.empty()) {
			code += stackOp("POP", memVars.top());
			memVars.pop();
		}
	}
	StringUtils::replaceAll(code, "\n", "\n\t");
	if (func->getName() != "main") code += "RET" + NEWLINE_ASM;
	code += PROC_END(func->getName());
	deleteTemp(funcSrc, cstmt);
	return code;
}

// sets ret Value in DX
string returnExpCode(SymbolInfo *exp) {
	string code;
	code += exp->code;
	code += operToReg("DX", *exp);
	code += jumpTo(currentFunc->getReturnLabel());
	return code;
}

#endif //IRGENERATOR_ASMBASE_H
