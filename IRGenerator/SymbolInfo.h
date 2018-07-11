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

	size_t arrSize;
	size_t arrIndex;

	int defInt=0;
	float defFlt=0.0;

	int scopeID=-1;

public:

	vector<string> paramList;    //INT, FLOAT, STRING, CHAR

	vector<int> intData;
	vector<float> floatData;
//	vector<char> charData;
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

//	explicit SymbolInfo(string name, string type = "");

	explicit SymbolInfo(const string name, const string type = "") : name(name), type(type) { arrIndex = 0; }

	SymbolInfo(const SymbolInfo &symbolInfo) {
		this->name = symbolInfo.name;
		this->type = symbolInfo.type;
		this->arrSize = symbolInfo.arrSize;
		this->arrIndex = symbolInfo.arrIndex;

		this->varType = symbolInfo.varType;
		this->idType = symbolInfo.idType;

		this->funcDefined = symbolInfo.funcDefined;
		this->funcRetType = symbolInfo.funcRetType;

		this->intData = symbolInfo.intData;
		this->floatData = symbolInfo.floatData;

		this->paramList = symbolInfo.paramList;

		this->scopeID = symbolInfo.scopeID;

		this->code = symbolInfo.code;
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

	const string &getIDType() const {
		return idType;
	}

	void setIDType(const string &IDType) {
		SymbolInfo::idType = IDType;
	}

	const string &getVarType() const {
		return varType;
	}

	void setVarType(const string &VarType) {
		this->varType = VarType;
		if (VarType == INT_TYPE) intData.push_back(0);
		else if (VarType == FLOAT_TYPE) floatData.push_back(0);
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

	size_t getArrIndex() const {
		return isArrayVar() ? arrIndex : 0;
	}

	void setArrIndex(size_t ArrIndex) {
		if (varType != ARRAY)
			return;
		this->arrIndex = ArrIndex;
	}

	bool isFuncDefined() const {
		return funcDefined;
	}

	void setFuncDefined(bool FuncDefined) {
		SymbolInfo::funcDefined = FuncDefined;
	}

	int setIndexValue(int val) {
		while (intData.size() <= getArrIndex()) intData.push_back(0);
		return intValue() = val;
	}

	float setIndexValue(float val) {
		while (floatData.size() <= getArrIndex()) floatData.push_back(0);
		return fltValue() = val;
	}

	int setVarValue(int val) {
		if (intData.empty()) intData.push_back(val);
		else intData[0] = val;
		return intData[0];
	}

	float setVarValue(float val) {
		if (floatData.empty()) floatData.push_back(val);
		else floatData[0] = val;
		return floatData[0];
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


	int &intValue() {
		if (idType == VARIABLE) {
			if (intData.empty()) intData.push_back(0);
			return varType == FLOAT_TYPE ? intData[0] = static_cast<int>(fltValue()) : intData[0];
		} else if (idType == ARRAY && varType == INT_TYPE) {
			return intData[getArrIndex()];
		}
		cout << "Not Variable/Array/Int Type" << endl;
		return defInt;
	}

	float &fltValue() {
		if (idType == VARIABLE) {
			if (floatData.empty()) floatData.push_back(0);
			return varType == INT_TYPE ? floatData[0]=intValue() : floatData[0];
		} else if (idType == ARRAY && varType == FLOAT_TYPE) {
			return floatData[getArrIndex()];
		}
		cout << "Not Variable/Array/Flt Type" << endl;
		return defFlt;
	}
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
