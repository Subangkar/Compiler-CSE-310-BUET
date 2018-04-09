//
// Created by SubangkarKr on 07-Apr-18.
//


#ifndef SYMBOLTABLE_SCOPETABLE_H
#define SYMBOLTABLE_SCOPETABLE_H


#include "../../DataTypes/SymbolInfo/SymbolInfo.h"
#include "../HashTable/HashTable.h"
#include "../HashTable/HashTable.cpp"


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


	HASH_POS Insert(const SymbolInfo &s);

	HASH_POS Insert(const string &name, const string &type);

	// returns the original data pointer if exists otherwise NULL
	SymbolInfo *LookUp(const SymbolInfo &symbol);

	// returns the original data pointer if exists otherwise NULL
	SymbolInfo *LookUp(const string &symbol);

	bool Delete(const string &symbol);

	void Print();


	HASH_POS GetPos(const string &key);

	HASH_POS GetPos(const SymbolInfo &key);
};


#endif //SYMBOLTABLE_SCOPETABLE_H
