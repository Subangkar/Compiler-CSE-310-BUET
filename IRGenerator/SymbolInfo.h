//
// Created by subangkar on 5/28/18.
//

#ifndef SYNTAXANALYZER_SYMBOLNFO_H
#define SYNTAXANALYZER_SYMBOLNFO_H

#include <ostream>
#include "DSBasic.h"
//
// Created by SubangkarKr on 07-Apr-18.
//



#define ARRAY "ARR"
#define VARIABLE "VAR"
#define FUNCTION "FUNC"
#define TEMPORARY "TEMP"
#define INT_TYPE "INT"
#define FLOAT_TYPE "FLOAT"
#define VOID_TYPE "VOID"
#define CHAR_TYPE "CHAR"


class SymbolInfo {

	string name;
	string type;

	string idType;// FUNCTION, VARIABLE, ARRAY
	string varType; // INT_TYPE, FLOAT_TYPE, VOID_TYPE

	string funcRetType;// INT_TYPE, FLOAT_TYPE, VOID_TYPE
	bool funcDefined = false;

	size_t arrSize{};

	int scopeID=-1;

	string arrIndexVarName = "";
	string returnLabel;
public:

	vector<string> paramList{};    // INT, FLOAT, STRING, CHAR
	vector<SymbolInfo> parameters;  // x, y
	vector<string> memberVars; // During Function Call all must be stacked asm name

	string code;


	explicit operator string() const {
		return string("< " + name + " : " + type + " > ");
	}

	explicit operator char *() const {
		char *str = new char[("< " + name + " : " + type + " > ").length() + 1];
		strcpy(str, ("< " + name + " : " + type + " > ").data());
		return str;
	}


	string printString() {
		return string("< " + name + " : " + type + " > ");
	}


	const string &getName() const { return name; };

	explicit SymbolInfo(string name, string type = TEMPORARY) : name(std::move(name)), type(std::move(type)){};

	SymbolInfo(const SymbolInfo &symbolInfo) {
		this->name = symbolInfo.name;
		this->type = symbolInfo.type;
		this->arrSize = symbolInfo.arrSize;

		this->varType = symbolInfo.varType;
		this->idType = symbolInfo.idType;

		this->funcDefined = symbolInfo.funcDefined;
		this->funcRetType = symbolInfo.funcRetType;

		this->paramList = symbolInfo.paramList;

		this->scopeID = symbolInfo.scopeID;

		this->code = symbolInfo.code;
		this->arrIndexVarName = symbolInfo.arrIndexVarName;
	}

	SymbolInfo &operator=(const SymbolInfo &symbolInfo)= default;

	void setName(const string &name) {
		this->name = name;
	}

	const string &getType() const {
		return type;
	}

	void setType(const string &type) {
		this->type = type;
	}

	void setIDType(const string &IDType) {
		SymbolInfo::idType = IDType;
	}

	const string &getVarType() const {
		return varType;
	}

	void setVarType(const string &VarType) {
		this->varType = VarType;
	}

	const string &getFuncRetType() const {
		return funcRetType;
	}

	void setFuncRetType(const string &FuncRet) {
		this->funcRetType = FuncRet;
		setVarType(FuncRet);
	}

	string getScopeID() const {
		return to_string(scopeID);
	}

	void setScopeID(int scopeID) {
		this->scopeID = scopeID;
	}

	size_t getArrSize() const {
		return arrSize;
	}

	void setArrSize(size_t ArrSize) {
		if (!isArrayVar())
			return;
		this->arrSize = ArrSize;
	}

	bool isFuncDefined() const {
		return funcDefined;
	}

	void setFuncDefined(bool FuncDefined) {
		SymbolInfo::funcDefined = FuncDefined;
	}

	const string &getArrIndexVarName() const {
		return arrIndexVarName;
	}

	void setArrIndexVarName(const string &arrIndexVarName) {
		SymbolInfo::arrIndexVarName = arrIndexVarName;
	}

	const string &getReturnLabel() const {
		return returnLabel;
	}

	void setReturnLabel(const string &returnLabel) {
		SymbolInfo::returnLabel = returnLabel;
	}

	bool operator==(const SymbolInfo &rhs) const {
//		return name == rhs.name && type == rhs.type;
		return name == rhs.name;
	}

	bool operator!=(const SymbolInfo &rhs) const {
		return !(rhs == *this);
	}

	int64_p hashValue() const {

		string key = this->name;

		int64_p value = 0;

		for (int i = 0; i < key.length(); i++) {
			if (i % 2 == 0) {
				value += (reverseDigits(static_cast<int64_p>(key[i]))) * (int64_p) pow(19, i); // std::abs
			} else {
				value += abs((key[i]) * (int) pow(23, i));
			}
		}

		return value;
	}

	friend ostream &operator<<(ostream &os, const SymbolInfo &info) {
		os << "< " << info.name << " : " << info.type << " > ";
		return os;
	}


	bool isFunction() const { return idType == FUNCTION; }//type == "ID" &&

	bool isArrayVar() const { return idType == ARRAY; }//type == "ID" &&

	bool isVariable() const { return idType == VARIABLE; } //type == "ID" &&

	bool isVoidFunc() const { return isFunction() && getFuncRetType() == VOID_TYPE; }


private:

	int64_p reverseDigits(int64_p n) const {
		int64_p revN = 0;
		while (n > 0) {
			revN = revN * 10 + n % 10;
			n /= 10;
		}
		return revN;
	}
};

#endif //SYNTAXANALYZER_SYMBOLNFO_H
