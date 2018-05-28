#pragma once


//
// Created by SubangkarKr on 01-May-18.
//

#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H


#include "DSBasic.h"
#include "SymbolInfo.h"

#ifndef SYMBOLTABLE_SCOPETABLE_H
#define SYMBOLTABLE_SCOPETABLE_H


class ScopeTable : private HashTable<SymbolInfo> {

	ScopeTable *parentScope;
	int id;


public:

	explicit ScopeTable(ScopeTable *parentScope = nullptr) {

		this->parentScope = parentScope;

		if (!parentScope) id = 0;
		else id = parentScope->id + 1;
	}

	ScopeTable(ScopeTable *parentScope, int id) : parentScope(parentScope), id(id) {}

	ScopeTable(ScopeTable *parentScope, int id, int64_p size)
			: HashTable(size), parentScope(parentScope), id(id) {}


	~ScopeTable() override {}


	ScopeTable *getParentScope() const {
		return parentScope;
	}

	void setParentScope(ScopeTable *parentScope) {
		ScopeTable::parentScope = parentScope;
	}

	int getId() const {
		return id;
	}

	void setId(int id) {
		this->id = id;
	}

	bool Insert(const SymbolInfo &s) {
		return insert(s);
	}

	bool Insert(const string &name, const string &type) {
		return Insert(SymbolInfo(name, type));
	}

	// returns the original data pointer if exists otherwise NULL
	SymbolInfo *LookUp(const SymbolInfo &symbol) {
		return get(symbol);
	}

	// returns the original data pointer if exists otherwise NULL
	SymbolInfo *LookUp(const string &symbol) {
		return LookUp(SymbolInfo(symbol));
	}

	bool Delete(const string &symbol) {
		return remove(SymbolInfo(symbol));
	}

	void Print(FILE *printStream = stdout) {
		//	cout << " ScopeTable # " << id << endl;
		fprintf(printStream, " ScopeTable # %d\n", id);
		printTable(printStream);
	}

	void Print(ofstream& out) {
		out << " ScopeTable # " << id << endl;
//		fprintf(printStream, " ScopeTable # %d\n", id);
		printTable(out);
	}	HASH_POS GetPos(const string &key) {
		return GetPos(SymbolInfo(key));
	}

	HASH_POS GetPos(const SymbolInfo &key) {
		return HASH_POS(getLOC(key), getPOS(key));
	}

	void setPrintEmptyList(bool stat){
		setPrintEmptyListOn(stat);
	}
};


#endif //SYMBOLTABLE_SCOPETABLE_H



#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H


class SymbolTable {

	ScopeTable *currentScope;

	int max_id;
	int64_p tableSize;

public:
	explicit SymbolTable(int64_p size = HASH_TABLE_MAXSIZE) {
		max_id = 0;
		currentScope = nullptr;
		tableSize = size;

		currentScope = new ScopeTable(nullptr, 0, tableSize);
	}

	virtual ~SymbolTable() {
		tableSize = 0;
		while (currentScope) {
			ScopeTable *p = currentScope->getParentScope();
			delete currentScope;
			currentScope = p;
		}
	}

	void enterScope() {
		ScopeTable *scopeTable = new ScopeTable(currentScope, max_id + 1, tableSize);
		currentScope = scopeTable;
		max_id++;
#ifdef DEBUG
		cout << " New ScopeTable with id " << currentScope->getId() << " created" << endl;
#endif // DEBUG
	}

	void exitScope() {
		if (currentScope) {
			ScopeTable *p = currentScope->getParentScope();
#ifdef DEBUG
			cout << " ScopeTable with id " << currentScope->getId() << " removed" << endl;
#endif // DEBUG
			delete currentScope;
			currentScope = p;
		}
	}

	bool insert(const SymbolInfo &symbol) {
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

	bool insert(const string &name, const string &type) {
		return insert(SymbolInfo(name, type));
	}

	bool remove(const SymbolInfo &symbol) {
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

	bool remove(const string &name) {
		return remove(SymbolInfo(name));
	}
	SymbolInfo *lookUp(const string &symbol) {
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


	void printCurrentScope() {
		currentScope->Print();
	}

	void printAllScope(FILE *printStream = stdout) {
		for (ScopeTable *scope = currentScope; scope; scope = scope->getParentScope()) {
			scope->Print(printStream);
			fprintf(printStream, "\n");
		}

	}

	void printAllScope(ofstream& out) {
		for (ScopeTable *scope = currentScope; scope; scope = scope->getParentScope()) {
			scope->Print(out);
//			fprintf(printStream, "\n");
			out << std::endl;
		}

	}

};


#endif //SYMBOLTABLE_SYMBOLTABLE_H



#endif //DATA_STRUCTURE_H
