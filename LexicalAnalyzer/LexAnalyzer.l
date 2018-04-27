%option noyywrap

%{

#include <iostream>
#include <cmath>

#include "DataStructures/ScopeTable/ScopeTable.h"


#include "DataStructures/SymbolTable/SymbolTable.h"

using namespace std;

%}

%%




%%


int main(int argc,char *argv[]){
	
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	logout= fopen("log.txt","w");
	tokenout= fopen("token.txt","w");

	yyin= fin;
	yylex();
	st.print();
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;
	
}
