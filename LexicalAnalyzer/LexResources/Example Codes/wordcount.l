%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>

int word_count=0;
int line_count=1;

void IncWc(){
	word_count++;
}
%}

Letter [a-zA-Z]

%%
[ \t\v]+ {}
{Letter}+ {IncWc(); printf("%s word found\n",yytext);}
[\n] {line_count++;}
<<EOF>> {printf("Total Line Number: %d and word number: %d\n",line_count,word_count); return 0;}
. {}
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
	

	yyin= fin;
	yylex();
	fclose(yyin);
	return 0;
}
