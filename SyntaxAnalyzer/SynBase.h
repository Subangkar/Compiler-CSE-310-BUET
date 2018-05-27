//
// Created by subangkar on 5/27/18.
//

#ifndef SYNTAXANALYZER_SYNBASE_H
#define SYNTAXANALYZER_SYNBASE_H


#include "DataStructure.h"
#include <iostream>


#define SYMBOL_TABLE_SIZE 73


std::ofstream logFile, errorFile;
SymbolTable *table;

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
