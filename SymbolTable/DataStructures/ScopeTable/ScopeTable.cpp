//
// Created by SubangkarKr on 07-Apr-18.
//

#include "ScopeTable.h"

ScopeTable::ScopeTable(ScopeTable *parentScope) {

	this->parentScope=parentScope;

	if (!parentScope) id = 0;
	else id = parentScope->id + 1;
}

ScopeTable::ScopeTable(ScopeTable *parentScope, int id) : parentScope(parentScope), id(id) {}

ScopeTable::~ScopeTable() = default;

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

void ScopeTable::Print() {
	cout << "Scope #"<<id<<endl;
	printTable();
}

bool ScopeTable::Insert(const string &name, const string &type) {
	return Insert(SymbolInfo(name,type));
}

