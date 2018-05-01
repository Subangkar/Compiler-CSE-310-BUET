%option noyywrap

%{

#include "DataStructure.h"
SymbolTable st;
FILE *logout,*tokenout;

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
	
	logout = fopen("log.txt","w");
	tokenout = fopen("token.txt","w");

	yyin= fin;
	yylex();
	st.printAllScope();
	fclose(yyin);
	fclose(tokenout);
	fclose(logout);
	return 0;
	
}
