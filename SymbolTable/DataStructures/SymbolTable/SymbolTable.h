//
// Created by SubangkarKr on 07-Apr-18.
//

#ifndef SYMBOLTABLE_SYMBOLTABLE_H
#define SYMBOLTABLE_SYMBOLTABLE_H


#include "../ScopeTable/ScopeTable.h"

class SymbolTable {

	ScopeTable* currentScope;

	int max_id;

public:
    SymbolTable();

    virtual ~SymbolTable();


    void EnterScope();
    void ExitScope();

    bool Insert();

    bool Remove();

    SymbolInfo* LookUp(const string& symbol);

    void printCurrentScope();

    void printAllScope();

};


#endif //SYMBOLTABLE_SYMBOLTABLE_H
