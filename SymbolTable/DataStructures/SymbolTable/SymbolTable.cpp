//
// Created by SubangkarKr on 07-Apr-18.
//

#ifndef SYMBOLTABLE_SYMBOLTABLE_CPP
#define SYMBOLTABLE_SYMBOLTABLE_CPP


#include "SymbolTable.h"

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

void SymbolTable::EnterScope() {
	ScopeTable *scopeTable = new ScopeTable(currentScope, max_id + 1, tableSize);
	currentScope = scopeTable;
	max_id++;
	cout << " New ScopeTable with id " << currentScope->getId() << " created" << endl;
}

void SymbolTable::ExitScope() {
	if (currentScope) {
		ScopeTable *p = currentScope->getParentScope();
		cout << " ScopeTable with id " << currentScope->getId() << " removed" << endl;
		delete currentScope;
		currentScope = p;
	}
}

bool SymbolTable::Insert(const SymbolInfo &symbol) {
	HASH_POS symbolPos = currentScope->Insert(symbol);
	if (symbolPos.isValid())
		cout << " Inserted in ScopeTable# " << currentScope->getId() << " at position " << symbolPos << endl;
	else cout << " " << symbol << " already exists in current ScopeTable" << endl;
	return symbolPos.isValid();
}

bool SymbolTable::Insert(const string &name, const string &type) {
	return Insert(SymbolInfo(name, type));
}

bool SymbolTable::Remove(const SymbolInfo &symbol) {
	ScopeTable *scope = currentScope;

	while (scope && !scope->GetPos(symbol).isValid()) {
		scope = scope->getParentScope();
	}

	if (scope) {
		cout << " Deleted From ScopeTable# " << scope->getId() << " at position " << scope->GetPos(symbol) << endl;
		return scope->Delete(symbol.getName());
	}

	cout << symbol.getName() << " not found" << endl;
	return false;
}

bool SymbolTable::Remove(const string &name) {
	return Remove(SymbolInfo(name));
}

SymbolInfo *SymbolTable::LookUp(const string &symbol) {
	ScopeTable *scope = currentScope;
	while (scope) {
		HASH_POS hashPos = scope->GetPos(symbol);
		if (hashPos.isValid()) {
			cout << " Found in ScopeTable# " << scope->getId() << " at position " << hashPos << endl;
			return scope->LookUp(symbol);
		} else scope = scope->getParentScope();
	}
	cout << " Not found" << endl;
	return nullptr;
}

void SymbolTable::printCurrentScope() {
	currentScope->Print();
}

void SymbolTable::printAllScope() {
	for (ScopeTable *scope = currentScope; scope; scope = scope->getParentScope()) {
		scope->Print();
		cout << endl;
	}
}


#endif //SYMBOLTABLE_SYMBOLTABLE_CPP