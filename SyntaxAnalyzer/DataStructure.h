#pragma once


//
// Created by SubangkarKr on 01-May-18.
//

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H


#include "DSBasic.h"
#include "SymbolInfo.h"
/*
class SymbolInfo{
private:
	string Name;
	string Type;
	string IDType;				//VAR, ARA, FUNC
	string VarType;				//INT, FLOAT, CHAR
	int AraSize; 				//CONST_INT
	string FuncRet;				//INT, FLOAT, VOID
	int araIndex;
	bool FuncDefined = false;	//true, false	//true,false


	int64_p reverseDigits(int64_p n) const {
		int64_p revN = 0;
		while (n > 0) {
			revN = revN * 10 + n % 10;
			n /= 10;
		}
		return revN;
	}


public:

	vector<string> ParamList;	//INT, FLOAT, STRING, CHAR
	//vector AraElem;	//INT, FLOAT, STRING, CHAR
	vector<int> ints;
	vector<float> floats;
	vector<char> chars;


	SymbolInfo(){
	}

	SymbolInfo(string type){
		VarType = type;
		if(type == "INT") ints.push_back(0);
		else if(type == "FLOAT") floats.push_back(0);
	}

	SymbolInfo(string name, string type){
		Name = name;
		Type = type;
	}

	void setName(string name){	Name = name;	}
	void setType(string type){ 	Type = type;	}
	void setIDType(string type){IDType = type;	}
	void setVarType(string type){
		VarType = type;
		if(type == "INT") ints.push_back(0);
		else if(type == "FLOAT") floats.push_back(0);
	}
	void setAraSize(int size){	AraSize = size;	}
	void setFuncRet(string type){FuncRet = type;}
	void setFuncDefined(){ FuncDefined = true;	}
	void setAraIndex(int index){araIndex = index;}

	const string getName() const{ return Name;}
	string getType(){ return Type;}
	string getIDType(){return IDType;}
	string getVarType(){return VarType;}
	int getAraSize(){return AraSize;}
	string getFuncRet(){return FuncRet;}
	bool isFuncDefined(){return FuncDefined;}
	int getAraIndex(){return araIndex;}

	SymbolInfo & operator = (const SymbolInfo &sym){
		this->Name = sym.Name;
		this->Type = sym.Type;
		return *this;
	}

	bool operator==(const SymbolInfo &rhs) const {
//    return name == rhs.name && type == rhs.type;
		return Name == rhs.Name;
	}



	void printValues(ofstream& file){
		if(IDType == "VAR"){
			file << " , ";
			if(VarType == "INT") file << ints[0] ;
			else file << floats[0];
		}
		else if(IDType == "ARA"){
			file << " , { " ;
			if(VarType == "INT"){
				for(int i = 0; i< AraSize; i++) file << ints[i] << " ";
			}
			else if(VarType == "FLOAT"){
				for(int i = 0; i< AraSize; i++) file << floats[i] << " ";
			}
			file << "}" ;
		}
		else if(IDType == "FUNC"){

		}
	}

	int64_p hashValue() const {

		string key = this->Name;

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

};
*/

/*
#ifndef SYMBOLTABLE_SYMBOLINFO_H
#define SYMBOLTABLE_SYMBOLINFO_H


class SymbolInfo {
	string name;
	string type;


public:

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

	const string &getName() const;

	explicit SymbolInfo(string name, string type = "");

	void setName(const string &name);

	const string &getType() const;

	void setType(const string &type);

	SymbolInfo(const SymbolInfo &symbolInfo);

	SymbolInfo &operator=(const SymbolInfo &symbolInfo);

	bool operator==(const SymbolInfo &rhs) const;

	bool operator!=(const SymbolInfo &rhs) const;

	int64_p hashValue() const;

	friend std::ostream &operator<<(std::ostream &os, const SymbolInfo &info);


private:

	int64_p reverseDigits(int64_p n) const;

};


#endif //SYMBOLTABLE_SYMBOLINFO_H


#ifndef SYMBOLTABLE_SYMBOLINFO_CPP
#define SYMBOLTABLE_SYMBOLINFO_CPP


const string &SymbolInfo::getName() const {
	return name;
}

void SymbolInfo::setName(const string &name) {
	SymbolInfo::name = name;
}

const string &SymbolInfo::getType() const {
	return type;
}

void SymbolInfo::setType(const string &type) {
	SymbolInfo::type = type;
}

SymbolInfo::SymbolInfo(string name, string type) : name(std::move(name)), type(std::move(type)) {}

SymbolInfo::SymbolInfo(const SymbolInfo &symbolInfo) {
	this->name = symbolInfo.name;
	this->type = symbolInfo.type;
}

bool SymbolInfo::operator==(const SymbolInfo &rhs) const {
//    return name == rhs.name && type == rhs.type;
	return name == rhs.name;
}

bool SymbolInfo::operator!=(const SymbolInfo &rhs) const {
	return !(rhs == *this);
}

SymbolInfo &SymbolInfo::operator=(const SymbolInfo &symbolInfo) {
	this->name = symbolInfo.name;
	this->type = symbolInfo.type;

	return *this;
}

int64_p SymbolInfo::hashValue() const {

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

std::ostream &operator<<(std::ostream &os, const SymbolInfo &info) {
	os << "< " << info.name << " : " << info.type << " > ";
	return os;
}

int64_p SymbolInfo::reverseDigits(int64_p n) const {
	int64_p revN = 0;
	while (n > 0) {
		revN = revN * 10 + n % 10;
		n /= 10;
	}
	return revN;
}


#endif //SYMBOLTABLE_SYMBOLINFO_CPP
*/
#ifndef SYMBOLTABLE_SCOPETABLE_H
#define SYMBOLTABLE_SCOPETABLE_H


class ScopeTable : private HashTable<SymbolInfo> {

	ScopeTable *parentScope;
	int id;


public:

	explicit ScopeTable(ScopeTable *parentScope = nullptr);

	ScopeTable(ScopeTable *parentScope, int id);

	ScopeTable(ScopeTable *parentScope, int id, int64_p size);

	~ScopeTable() override;


	ScopeTable *getParentScope() const;

	void setParentScope(ScopeTable *parentScope);

	int getId() const;

	void setId(int id);


	bool Insert(const SymbolInfo &s);

	bool Insert(const string &name, const string &type);

	// returns the original data pointer if exists otherwise NULL
	SymbolInfo *LookUp(const SymbolInfo &symbol);

	// returns the original data pointer if exists otherwise NULL
	SymbolInfo *LookUp(const string &symbol);

	bool Delete(const string &symbol);

	void Print(FILE *printStream = stdout);


	HASH_POS GetPos(const string &key);

	HASH_POS GetPos(const SymbolInfo &key);
};


#endif //SYMBOLTABLE_SCOPETABLE_H


#ifndef SYMBOLTABLE_SCOPETABLE_CPP
#define SYMBOLTABLE_SCOPETABLE_CPP



ScopeTable::ScopeTable(ScopeTable *parentScope) {

	this->parentScope = parentScope;

	if (!parentScope) id = 0;
	else id = parentScope->id + 1;
}

ScopeTable::ScopeTable(ScopeTable *parentScope, int id) : parentScope(parentScope), id(id) {}

ScopeTable::~ScopeTable() {
	id = -1;
	parentScope = NULL;
};

ScopeTable *ScopeTable::getParentScope() const {
	return parentScope;
}

void ScopeTable::setParentScope(ScopeTable *parentScope) {
	ScopeTable::parentScope = parentScope;
}

int ScopeTable::getId() const {
	return id;
}

void ScopeTable::setId(int id) {
	ScopeTable::id = id;
}

bool ScopeTable::Insert(const SymbolInfo &s) {
	return insert(s);
}

SymbolInfo *ScopeTable::LookUp(const SymbolInfo &symbol) {
	return get(symbol);
}

SymbolInfo *ScopeTable::LookUp(const string &symbol) {
	return LookUp(SymbolInfo(symbol));
}

bool ScopeTable::Delete(const string &symbol) {
	return remove(SymbolInfo(symbol));
}

void ScopeTable::Print(FILE *printStream) {
//	cout << " ScopeTable # " << id << endl;
	fprintf(printStream, " ScopeTable # %d\n", id);
	printTable(printStream);
}

bool ScopeTable::Insert(const string &name, const string &type) {
	return Insert(SymbolInfo(name, type));
}

ScopeTable::ScopeTable(ScopeTable *parentScope, int id, int64_p size)
		: HashTable(size), parentScope(parentScope), id(id) {}

HASH_POS ScopeTable::GetPos(const string &key) {
	return GetPos(SymbolInfo(key));
}

HASH_POS ScopeTable::GetPos(const SymbolInfo &key) {
	return HASH_POS(getLOC(key), getPOS(key));
}


#endif //SYMBOLTABLE_SCOPETABLE_CPP


#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H


class SymbolTable {

	ScopeTable *currentScope;

	int max_id;
	int64_p tableSize;

public:
	SymbolTable(int64_p size = HASH_TABLE_MAXSIZE);

	virtual ~SymbolTable();


	void enterScope();

	void exitScope();

	bool insert(const SymbolInfo &symbol);

	bool insert(const string &name, const string &type);

	bool remove(const SymbolInfo &symbol);

	bool remove(const string &name);

	SymbolInfo *lookUp(const string &symbol);


	void printCurrentScope();

	void printAllScope(FILE *printStream = stdout);

};


#endif //SYMBOLTABLE_SYMBOLTABLE_H


#ifndef SYMBOLTABLE_SYMBOLTABLE_CPP
#define SYMBOLTABLE_SYMBOLTABLE_CPP


SymbolTable::SymbolTable(int64_p size) {
	max_id = 0;
	currentScope = nullptr;
	tableSize = size;

	currentScope = new ScopeTable(nullptr, 0, tableSize);
}

SymbolTable::~SymbolTable() {
	tableSize = 0;
	while (currentScope) {
		ScopeTable *p = currentScope->getParentScope();
		delete currentScope;
		currentScope = p;
	}
}


void SymbolTable::enterScope() {
	ScopeTable *scopeTable = new ScopeTable(currentScope, max_id + 1, tableSize);
	currentScope = scopeTable;
	max_id++;
#ifdef DEBUG
	cout << " New ScopeTable with id " << currentScope->getId() << " created" << endl;
#endif // DEBUG
}

void SymbolTable::exitScope() {
	if (currentScope) {
		ScopeTable *p = currentScope->getParentScope();
#ifdef DEBUG
		cout << " ScopeTable with id " << currentScope->getId() << " removed" << endl;
#endif // DEBUG
		delete currentScope;
		currentScope = p;
	}
}

bool SymbolTable::insert(const SymbolInfo &symbol) {
//	HASH_POS symbolPos = currentScope->insert(symbol);
//	if (symbolPos.isValid())
//		cout << " Inserted in ScopeTable# " << currentScope->getId() << " at position " << symbolPos << endl;
//	else cout << " " << symbol << " already exists in current ScopeTable" << endl;
//	return symbolPos.isValid();

	if (currentScope == nullptr) return false;

	if (currentScope->Insert(symbol)) {
#ifdef DEBUG
		cout << " Inserted in ScopeTable# " << currentScope->getId() << " at position " << currentScope->GetPos(symbol)
			 << endl;
#endif // DEBUG
		return true;
	}
#ifdef DEBUG
	cout << " " << symbol << " already exists in current ScopeTable" << endl;
#endif // DEBUG
	return false;
}

bool SymbolTable::insert(const string &name, const string &type) {
	return insert(SymbolInfo(name, type));
}

bool SymbolTable::remove(const SymbolInfo &symbol) {
	ScopeTable *scope = currentScope;

//	while (scope && !scope->GetPos(symbol).isValid()) {
//		scope = scope->getParentScope();
//	}

//	if (scope) {
//		cout << " Deleted From ScopeTable# " << scope->getId() << " at position " << scope->GetPos(symbol) << endl;
//		return scope->Delete(symbol.getName());
//	}


	if (scope && scope->Delete(symbol.getName())) {
#ifdef DEBUG
		//		cout << " Deleted From ScopeTable# " << scope->getId() << " at position " << scope->GetPos(symbol) << endl;
				cout << " Deleted From current ScopeTable " << endl;
#endif // DEBUG
		return true;
	}

#ifdef DEBUG
	cout << symbol.getName() << " not found" << endl;
#endif // DEBUG
	return false;
}

bool SymbolTable::remove(const string &name) {
	return remove(SymbolInfo(name));
}

SymbolInfo *SymbolTable::lookUp(const string &symbol) {
	ScopeTable *scope = currentScope;
	while (scope) {
		HASH_POS hashPos = scope->GetPos(symbol);
		if (hashPos.isValid()) {
#ifdef DEBUG
			cout << " Found in ScopeTable# " << scope->getId() << " at position " << hashPos << endl;
#endif // DEBUG
			return scope->LookUp(symbol);
		} else scope = scope->getParentScope();
	}
#ifdef DEBUG
	cout << " Not found" << endl;
#endif // DEBUG
	return nullptr;
}

void SymbolTable::printCurrentScope() {
	currentScope->Print();
}

void SymbolTable::printAllScope(FILE *printStream) {
	for (ScopeTable *scope = currentScope; scope; scope = scope->getParentScope()) {
		scope->Print(printStream);
		fprintf(printStream, "\n");
	}

}


#endif //SYMBOLTABLE_SYMBOLTABLE_CPP


#endif //DATA_STRUCTURE_H
