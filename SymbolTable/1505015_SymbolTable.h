//
// Created by SubangkarKr on 07-Apr-18.
//

#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H


#include "1505015_ScopeTable.h"

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

	void printAllScope();

};


#endif //SYMBOLTABLE_SYMBOLTABLE_H
