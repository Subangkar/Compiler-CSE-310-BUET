//
// Created by subangkar on 7/11/18.
//

#ifndef IRGENERATOR_ASMBASE_H
#define IRGENERATOR_ASMBASE_H

#define PROC_START(x)   string(x)+" PROC\r\n"
#define PROC_END(x)     string(x)+" ENDP\r\n"
#define SCOPE_NO(x) "$cp"+(x)
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
		cout << "here" << endl;
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

string getASM_VAR_NAME(const string &cVarName) {
	return cVarName + SCOPE_NO(table.lookUp(cVarName)->getScopeID());
}

void appendCode(const string &code) {
	string formattedCode = code;
	makeCRLF(formattedCode);
	asmFile << formattedCode;
}

void addData(const string &varname, bool isArray = false) {
	string code;
	if (!isArray) code = getASM_VAR_NAME(varname) + ASM_INT_TYPE + "0";
	else code = getASM_VAR_NAME(varname) + ASM_INT_TYPE + to_string(table.lookUp(varname)->getArrSize()) + " DUP(0)";
	makeCRLF(code);
	dataSegment += code;
}

void addCode(const string &code) {
	string formattedCode = code;
	makeCRLF(formattedCode);
	codeSegment += formattedCode;
}


void printVarValue(SymbolInfo *symbol) {
	symbol = table.lookUp(symbol->getName());

	if (symbol != nullptr && symbol->isVariable()) {
//		addCode(PROC_START("main"));
		addCode("MOV AX," + getASM_VAR_NAME(symbol->getName()));
		addCode("CALL OUTDEC");
//		addCode(PROC_END("main"));
	}
	else
	{

	}
}

void writeASM() {

	string initDataSeg = "MOV AX,@DATA\r\nMOV DS,AX\r\n";
	StringUtils::replaceAll(codeSegment, PROC_START("main"), PROC_START("main") + initDataSeg);
	codeSegment += getOUTDEC_PROC();

	asmFile << ".model small\r\n";
	asmFile << "\r\n.stack 100h\r\n";
	asmFile << "\r\n.data\r\n" << dataSegment;
	asmFile << "\r\n.code\r\n" << codeSegment;

	asmFile << NEWLINE_ASM;
	asmFile << "\r\nend main";
}

#endif //IRGENERATOR_ASMBASE_H
