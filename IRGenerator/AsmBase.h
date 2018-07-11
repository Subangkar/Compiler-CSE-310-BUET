//
// Created by subangkar on 7/11/18.
//

#ifndef IRGENERATOR_ASMBASE_H
#define IRGENERATOR_ASMBASE_H

#define PROC_START(x)   string(x)+" PROC\r\n"
#define PROC_END(x)     string(x)+" ENDP\r\n"
#define SCOPE_NO(x) "$"+(x)
#define NEWLINE_ASM "\r\n"
#define ASM_INT_TYPE " DW "


#define FILE_OUTDEC_PROC "Assembly Procedures/Dec16bitOutput.asm"
#define SYMBOL_TABLE_SIZE 73

#include <iostream>
#include "DataStructure.h"
#include "Utils.h"
#include <stack>
#include <sstream>
#include <utility>
#include <cstdlib>


ofstream asmFile;
SymbolTable table(SYMBOL_TABLE_SIZE);


string codeSegment, dataSegment;

int labelCount = 0;
int tempCount = 0;
int maxTemp = -1;


void makeCRLF(string &str) {
	StringUtils::replaceAll(str, "\r", "");
	StringUtils::replaceAll(str, "\n", "\r\n");

	if (str[str.size() - 1] != '\n')str += "\r\n";
}

string _OUTDEC_PROC;

string getOUTDEC_PROC() {
	if (!_OUTDEC_PROC.empty()) return _OUTDEC_PROC;
	std::ifstream file(FILE_OUTDEC_PROC);
	if (file) {
		file.seekg(0, std::ios::end);
		std::streampos length = file.tellg();
		file.seekg(0, std::ios::beg);
		auto *buff = new char[length];
		file.read(buff, length);
		_OUTDEC_PROC = buff;
		delete[] buff;

	}
	StringUtils::replaceAll(_OUTDEC_PROC, "\r", "");
	StringUtils::replaceAll(_OUTDEC_PROC, "\n", "\r\n");
	return _OUTDEC_PROC;
}

string ASM_VAR_NAME(const string &cVarName) {
	SymbolInfo *symbol = table.lookUp(cVarName);
	if (symbol == nullptr || symbol->getType() != "ID") return cVarName;
	return cVarName + SCOPE_NO(table.lookUp(cVarName)->getScopeID());
}

void appendCode(const string &code) {
	string formattedCode = code;
	makeCRLF(formattedCode);
	asmFile << formattedCode;
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


char *newLabel() {
	auto *lb = new char[4];
	strcpy(lb, "L");
	char b[3];
	sprintf(b, "%d", labelCount);
	labelCount++;
	strcat(lb, b);
	return lb;
}

string newTemp() {
	string t = "t";
	char b[3];
	sprintf(b, "%d", tempCount);
	t += b;
	if (tempCount > maxTemp) {
		maxTemp++;
		addData(t);
	}
	tempCount++;
	return t;
}


string printVarValue(SymbolInfo *symbol) {
	symbol = table.lookUp(symbol->getName());

	if (symbol != nullptr && symbol->isVariable()) {
		return getInst("MOV AX," + ASM_VAR_NAME(symbol->getName())) + getInst("CALL OUTDEC");
	} else {

	}
}

void writeASM() {

	string initDataSeg = "\tMOV AX,@DATA\r\n\tMOV DS,AX\r\n";
	string retOS = "MOV AH, 4ch\r\n\tMOV AL,0\r\n\tINT 21h\r\n" + PROC_END("main");
	StringUtils::replaceAll(codeSegment, PROC_START("main"), PROC_START("main") + initDataSeg);
	StringUtils::replaceAll(codeSegment, PROC_END("main"), retOS);
	codeSegment += getOUTDEC_PROC();

	asmFile << ".model small\r\n";
	asmFile << "\r\n.stack 100h\r\n";
	asmFile << "\r\n.data\r\n" << dataSegment;
	asmFile << "\r\n.code\r\n" << codeSegment;

	asmFile << NEWLINE_ASM;
	asmFile << "\r\nend main";
}


string assignToMemory(const string &dest, const string &src) {
	string code = "MOV DX," + ASM_VAR_NAME(src) + NEWLINE_ASM;
	code += "MOV " + ASM_VAR_NAME(dest) + ", DX" + NEWLINE_ASM;
//	if (toTemp) code += "MOV " + newTemp() + ", DX" + NEWLINE_ASM;
	return code;
}

string assignToMemory(const string &dest, const string &offsetVar, const string &src) {
	string code;
	code += "MOV SI," + ASM_VAR_NAME(offsetVar) + NEWLINE_ASM;
	code += string("ADD SI,SI") + NEWLINE_ASM;
	code += "MOV DX," + ASM_VAR_NAME(src) + NEWLINE_ASM;
	code += "MOV " + ASM_VAR_NAME(dest) + "[SI], DX" + NEWLINE_ASM;
	return code;
}

string assignToMemory(const string &dest, int destVar, const string &src, const string &srcOffsetVar) {
	string code;
	code += "MOV SI," + ASM_VAR_NAME(srcOffsetVar) + NEWLINE_ASM;
	code += string("ADD SI,SI") + NEWLINE_ASM;
	code += "MOV DX," + ASM_VAR_NAME(src) + "[SI]" + NEWLINE_ASM;
	code += "MOV " + ASM_VAR_NAME(dest) + ", DX" + NEWLINE_ASM;
	return code;
}

string assignToMemory(const string &dest, const string &destOffsetVar, const string &src, const string &srcOffsetVar) {
	string code;
	code += "MOV SI," + ASM_VAR_NAME(srcOffsetVar) + NEWLINE_ASM;
	code += string("ADD SI,SI") + NEWLINE_ASM;
	code += "MOV DI," + ASM_VAR_NAME(destOffsetVar) + NEWLINE_ASM;
	code += string("ADD DI,DI") + NEWLINE_ASM;
	code += "MOV DX," + ASM_VAR_NAME(src) + "[SI]" + NEWLINE_ASM;
	code += "MOV " + ASM_VAR_NAME(dest) + "[DI], DX" + NEWLINE_ASM;
	return code;
}

string copyFromCurSI(const string &dest, const string &src){
	string code;
	code += "MOV DX," + ASM_VAR_NAME(src) + "[SI]" + NEWLINE_ASM;
	code += "MOV " + ASM_VAR_NAME(dest) + ", DX" + NEWLINE_ASM;
	return code;
}

string incMemoryValue(const string &mem, const string &opType) {
	string code;
	code += opType + " " + ASM_VAR_NAME(mem) + NEWLINE_ASM;
	return code;
}

string incMemoryValue(const string &mem, const string &offsetVar, const string &opType) {
	string code;
	code += "MOV SI," + ASM_VAR_NAME(offsetVar) + NEWLINE_ASM;
	code += string("ADD SI,SI") + NEWLINE_ASM;
	code += opType + " " + ASM_VAR_NAME(mem) + "[SI]" + NEWLINE_ASM;
	return code;
}

string notMemoryValue(const string &dest,const string &mem) {
	return assignToMemory(dest, mem) + "NOT " + dest + NEWLINE_ASM;
}

string notMemoryValue(const string &dest,const string &mem, const string &offsetVar) {
	return assignToMemory(dest, 0, mem, offsetVar) + "NOT " + dest + NEWLINE_ASM;
}

#endif //IRGENERATOR_ASMBASE_H

