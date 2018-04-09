//
// Created by SubangkarKr on 07-Apr-18.
//

#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H


#include "../ScopeTable/ScopeTable.h"

class SymbolTable {

	ScopeTable *currentScope;

	int max_id;
	int64_p tableSize;

public:
	SymbolTable(int64_p size = HASH_TABLE_MAXSIZE);

	virtual ~SymbolTable();


	void EnterScope();

	void ExitScope();

	bool Insert(const SymbolInfo &symbol);

	bool Insert(const string &name, const string &type);

	bool Remove(const SymbolInfo &symbol);

	bool Remove(const string &name);

	SymbolInfo *LookUp(const string &symbol);

	void printCurrentScope();

	void printAllScope();

};


#endif //SYMBOLTABLE_SYMBOLTABLE_H
