//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H


#include <iostream>
//#include "SymTable.h"
#include "DataStructure.h"


#define SYMBOL_TABLE_SIZE 73


std::ofstream logFile, errorFile, parserFile;
SymbolTable table(SYMBOL_TABLE_SIZE);

int IDargs = 0;
vector<string> args;
bool funcDef = false;
int semErrors;
vector<SymbolInfo> params;

extern FILE *yyin;
extern int line_count;



int yyparse();

int yylex();



void yyerror(const char *s) {
	//write your code
}


#endif //SYNTAXANALYZER_SYNBASE_H
