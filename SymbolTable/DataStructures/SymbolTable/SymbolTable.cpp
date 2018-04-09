//
// Created by SubangkarKr on 07-Apr-18.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable() {
	max_id = EOF;
	currentScope = NULL;

	EnterScope();
}

SymbolTable::~SymbolTable() {

}
