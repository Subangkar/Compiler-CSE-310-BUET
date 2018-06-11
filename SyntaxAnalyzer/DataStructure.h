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
		fprintf(printStream, " ScopeTable # %d\n", id);
		printTable(printStream);
	}

	void Print(ofstream &out) {
		out << " ScopeTable # " << id << endl;
		printTable(out);
	}

	HASH_POS GetPos(const string &key) {
		return GetPos(SymbolInfo(key));
	}

	HASH_POS GetPos(const SymbolInfo &key) {
		return HASH_POS(getLOC(key), getPOS(key));
	}

	void setPrintEmptyList(bool stat) {
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
	}

	void exitScope() {
		if (currentScope) {
			ScopeTable *p = currentScope->getParentScope();
			delete currentScope;
			currentScope = p;
		}
	}

	bool insert(const SymbolInfo &symbol) {
		return currentScope && currentScope->Insert(symbol);
	}

	bool insert(const string &name, const string &type) {
		return insert(SymbolInfo(name, type));
	}

	bool remove(const SymbolInfo &symbol) {
		return currentScope && currentScope->Delete(symbol.getName());
	}

	bool remove(const string &name) {
		return remove(SymbolInfo(name));
	}

	SymbolInfo *lookUp(const string &symbol, bool inCurrentScopeOnly = false) {
		ScopeTable *scope = currentScope;
		while (scope) {
			HASH_POS hashPos = scope->GetPos(symbol);
			if (hashPos.isValid()) {
				return scope->LookUp(symbol);
			} else if (inCurrentScopeOnly) return nullptr;
			else scope = scope->getParentScope();
		}

		return nullptr;
	}

	SymbolInfo *lookUp(const SymbolInfo &symbolInfo, bool inCurrentScopeOnly = false) {
		return lookUp(symbolInfo.getName());
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

	void printAllScope(ofstream &out) {
		for (ScopeTable *scope = currentScope; scope; scope = scope->getParentScope()) {
			scope->Print(out);
			out << std::endl;
		}

	}

	int getCurrentId() const {
		return currentScope->getId();
	}
};


#endif //SYMBOLTABLE_SYMBOLTABLE_H


#endif //DATA_STRUCTURE_H
