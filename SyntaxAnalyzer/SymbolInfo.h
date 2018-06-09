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


class SymbolInfo {

	string name;
	string type;

	string IDType;// FUNC, VAR, ARA
	string VarType; // INT, FLOAT

	string FuncRet;// INT, FLOAT, VOID
	bool FuncDefined = false;

	size_t ArrSize;
	size_t ArrIndex;

public:

	vector<string> ParamList;    //INT, FLOAT, STRING, CHAR

	vector<int> ints;
	vector<float> floats;
//	vector<char> chars;


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

	explicit SymbolInfo(const string name, const string type = "") : name(name), type(type) {ArrIndex=0;}

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
		return IDType;
	}

	void setIDType(const string &IDType) {
		SymbolInfo::IDType = IDType;
	}

	const string &getVarType() const {
		return VarType;
	}

	void setVarType(const string &VarType) {
		SymbolInfo::VarType = VarType;
		if (VarType == "INT") ints.push_back(0);
		else if (VarType == "FLOAT") floats.push_back(0);
	}

	const string &getFuncRet() const {
		return FuncRet;
	}

	void setFuncRet(const string &FuncRet) {
		SymbolInfo::FuncRet = FuncRet;
	}

	size_t getArrSize() const {
		return ArrSize;
	}

	void setArrSize(size_t ArrSize) {
		if(VarType!="ARA")
			return;
		SymbolInfo::ArrSize = ArrSize;
	}

	size_t getArrIndex() const {
		return ArrIndex;
	}

	void setArrIndex(size_t ArrIndex) {
		if(VarType!="ARA")
			return;
		SymbolInfo::ArrIndex = ArrIndex;
	}

	bool isFuncDefined() const {
		return FuncDefined;
	}

	void setFuncDefined(bool FuncDefined) {
		SymbolInfo::FuncDefined = FuncDefined;
	}

	int setIndexValue(int val) {
		while (ints.size() <= getArrIndex()) ints.push_back(0);
		return ints[getArrIndex()] = val;
	}

	float setIndexValue(float val) {
		while (floats.size() <= getArrIndex()) floats.push_back(0);
		return floats[getArrIndex()] = val;
	}

	int setVarValue(int val) {
		if (ints.empty()) ints.push_back(val);
		else ints[0] = val;
		return ints[0];
	}

	float setVarValue(float val) {
		if (floats.empty()) floats.push_back(val);
		else floats[0] = val;
		return floats[0];
	}

	SymbolInfo(const SymbolInfo &symbolInfo) {
		this->name = symbolInfo.name;
		this->type = symbolInfo.type;
	}

	SymbolInfo &operator=(const SymbolInfo &symbolInfo)= default;

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
				value += (reverseDigits(key[i])) * (int64_p) pow(19, i); // std::abs
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
