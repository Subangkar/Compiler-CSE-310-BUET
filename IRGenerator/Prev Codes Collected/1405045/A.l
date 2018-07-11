%option noyywrap

%{

#include"SymTable.h"
#include"y.tab.h"

//ofstream log, token;

int lCount = 1, errCount = 0;

%}

WSP		[ \t\f\r\v]+
NEWLINE		[\n]

CHAR		[ -~]{-}['\\]
CHAR_STRING	[ -~]{-}['\"]

			
CHARACTER 	('({CHAR})')
MULTI_CHAR	('({CHAR}[ -~]+)')
UNFIN_CHAR	('({CHAR}))
UNFINSP_CHAR	('\\')
EMP_CHAR	('')

DOT		{\.}
DIGIT		[0-9]
INTEGER		[0-9]+

FLOAT1		[0-9]+(\.[0-9]+)?(E[+-]?[0-9]+)?
FLOAT2		(\.[0-9]+)(E[+-]?[0-9]+)?
FLOAT_ERROR1	[0-9]*(\.[0-9]+)(\.[0-9\.E+-]*)
FLOAT_ERROR2	[0-9]*(\.[0-9]+)?(E[+-]?[0-9]*\.[0-9]*)

STRING		(\"([^\"\n]|\\[\n])*\")
UNFIN_STRING	(\"([^\"\n]|\\[\n])*)


S_COMMENT	"//"[^\n]*$

M_COMMENT	("/*"([^"*"]|"*"[^"/"])+"*/")
UNFIN_COMMENT	("/*"([^"*"]|"*"[^"/"])+[^"*/"])

IDENTIFIER	[a-zA-Z_][a-zA-Z_0-9]*
INV_SUF		[0-9]+[a-zA-Z_0-9]+

SP_CHAR		'\\[ntafrbv0\"\\]'

%%

{WSP}		;

{INTEGER}       {
			//log<<"Line No. "<<lCount<<":  Token <CONST_INT> Lexeme "<<yytext<<" found\n" << endl;
			//if(table.Insert(yytext,"CONST_INT")){
				//table.printCurrent(log);
			//}

			//token<<"<CONST_INT,"<<yytext<<"> ";
			SymbolInfo* s =new  SymbolInfo(yytext, "CONST_INT");
			yylval.symVal= s;
			return CONST_INT;
		}

{FLOAT1}|{FLOAT2}  {
			//log<<"Line No. "<<lCount<<":  Token <CONST_FLOAT> Lexeme "<<yytext<<" found\n" << endl;
			//if(table.Insert(yytext,"CONST_FLOAT")){
				//table.printCurrent(log);
			//}
			//token<<"<CONST_FLOAT,"<<yytext<<"> ";
			SymbolInfo* s =new  SymbolInfo(yytext, "CONST_FLOAT");
			yylval.symVal = s;
			return CONST_FLOAT;
		}


{CHARACTER}     {
			//log<<"Line No. "<<lCount<<":  Token <CONST_CHAR> Lexeme "<<yytext<<" found\n" << endl; 
			//if(table.Insert(yytext,"CONST_CHAR")){
				//table.printCurrent(log);
			//}	
			//token<<"<CONST_CHAR,"<<yytext[1]<<"> ";
			string name;
			name.push_back(yytext[1]);
			SymbolInfo* s =new SymbolInfo(name, "CONST_CHAR");
			yylval.symVal= s;
			return CONST_CHAR;
		}

{NEWLINE}	lCount++;

{SP_CHAR}	{
			//log<<"Line No. "<<lCount<<":  Token <CONST_CHAR> Lexeme "<<yytext<<" found\n" << endl; 
			//if(table.Insert(yytext,"CONST_CHAR")){
				//table.printCurrent(log);
			//}
			//token<<"<CONST_CHAR, "<<yytext<<"> ";
			char text;
			switch(yytext[2]){
				case 'n': text = '\n'; break;
				case 't': text = '\t'; break;
				case 'v': text = '\v'; break;
				case 'a': text = '\a'; break;
				case 'f': text = '\f'; break;
				case 'r': text = '\r'; break;
				case 'b': text = '\b'; break;
				case '0': text = '\0'; break;
				default: break;			
			}
			yytext[0] = text;
			yytext[1] = '\0';
			SymbolInfo* s =new  SymbolInfo(yytext, "CONST_CHAR");
			yylval.symVal= s;
			return CONST_CHAR;
		}


{S_COMMENT}	{
			//log<<"Line No. "<<lCount<<":  Token <COMMENT> Lexeme "<<yytext<<" found\n" << endl; 
			for(int i = 0; i<yyleng ; i++){if(yytext[i] == '\n') lCount++;}
			return COMMENT; 
		}

{M_COMMENT}	{
			//log<<"Line No. "<<lCount<<":  Token <COMMENT> Lexeme "<<yytext<<" found\n" << endl; 
			for(int i = 0; i<yyleng ; i++){if(yytext[i] == '\n') lCount++;}
			return COMMENT;
		}

{STRING}	{
			//log<<"Line No. "<<lCount<<":  Token <STRING> Lexeme "<<yytext<<" found\n" << endl; 
			yytext[yyleng-1] = '\0';
			yytext++;
			for(int i = 0; i<yyleng ; i++){
				if((yytext[i] =='\\')){
					lCount++;
					for(int j = i; j<yyleng+1; j++){
						yytext[j] = yytext[j+2];					
					}				
				}
			}
			//token<<"<STRING, "<<yytext<<"> ";
			//SymbolInfo* s =new  SymbolInfo(yytext.substr(1), "STRING");
			//yylval.symVal= s;
			return STRING;
		}
"println" {return PRINTLN;}

"if"		{ 
			//log<<"Line No. "<<lCount<<":  Token <IF> Lexeme if found\n "<<endl;
			//token<<"<IF> ";
			return IF;
 		}

"for" 	   	{ 
			//log<<"Line No. "<<lCount<<":  Token <FOR> Lexeme for found\n "<<endl;  	
			//token<<"<FOR> ";
			return FOR;
		}

"do"	   	{ 
			//log<<"Line No. "<<lCount<<":  Token <DO> Lexeme do found\n "<<endl;    	
			//token<<"<DO> ";
			return DO;
		}

"int" 	   	{ 
			//log<<"Line No. "<<lCount<<":  Token <INT> Lexeme int found\n"<<endl;  	
			//token<<"<INT> ";
			return INT;
		}

"float"    	{ 
			//log<<"Line No. "<<lCount<<":  Token <FLOAT> Lexeme float found\n"<<endl; 	
			//token<<"<FLOAT> ";
			return FLOAT;
		}

"void"     	{ 
			//log<<"Line No. "<<lCount<<":  Token <VOID> Lexeme void found\n"<<endl; 
			//token<<"<void> ";
			return VOID;
		}

"switch"   	{ 
			//log<<"Line No. "<<lCount<<":  Token <SWITCH> Lexeme switch found\n"<<endl;
			//token<<"<SWITCH> ";
			return SWITCH;
		}

"default"  	{ 
			//log<<"Line No. "<<lCount<<":  Token <DEFAULT> Lexeme default found\n"<<endl;
			//token<<"<DEFAULT> ";
			return DEFAULT;
		}
"else"     	{ 
			//log<<"Line No. "<<lCount<<":  Token <ELSE> Lexeme else found\n"<<endl; 	
			//token<<"<ELSE> ";
			return ELSE;
		}

"while"    	{ 
			//log<<"Line No. "<<lCount<<":  Token <WHILE> Lexeme while found\n"<<endl; 	
			//token<<"<WHILE> ";
			return WHILE;
		}

"break"    	{ 
			//log<<"Line No. "<<lCount<<":  Token <BREAK> Lexeme break found\n"<<endl; 	
			//token<<"<BREAK> ";
			return BREAK;
		}

"char"     	{ 
			//log<<"Line No. "<<lCount<<":  Token <CHAR> Lexeme char found\n"<<endl;
			//token<<"<CHAR> ";
			return CHAR;
		}

"double"   	{ 
			//log<<"Line No. "<<lCount<<":  Token <DOUBLE> Lexeme double found\n"<<endl;
			//token<<"<DOUBLE> ";
			return DOUBLE;
		}

"return"   	{ 
			//log<<"Line No. "<<lCount<<":  Token <RETURN> Lexeme return found\n"<<endl;
			//token<<"<RETURN> ";
			return RETURN;
		}

"case"     	{ 
			//log<<"Line No. "<<lCount<<":  Token <CASE> Lexeme case found\n"<<endl;
			//token<<"<CASE> ";
			return CASE;
		}

"continue" 	{ 
			//log<<"Line No. "<<lCount<<":  Token <CONTINUE> Lexeme continue found\n"<<endl;
			//token<<"<CONTINUE> ";
			return CONTINUE;
		}

{IDENTIFIER}	{
			//log<<"Line No. "<<lCount<<":  Token <ID> Lexeme "<<yytext<<" found\n" << endl; 
			//if(table.Insert(yytext,"ID")){
				//table.printCurrent(log);
			//}
			//token<<"<ID,"<<yytext<<"> ";
			SymbolInfo* s =new  SymbolInfo(yytext, "ID");
			yylval.symVal= s;
			return ID;
		}


("++") 	{
			//log <<"Line No. "<<lCount<<":  Token <INCOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<INCOP,"<<yytext<<"> "; 
			return INCOP;
		}

("--")  {return DECOP;}


("+"|"-")   	{
			//log<<"Line No. "<<lCount<<":  Token <ADDOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<ADDOP,"<<yytext<<"> "; 
			SymbolInfo * s = new SymbolInfo(yytext, "ADDOP");
			yylval.symVal = s;
			return ADDOP;
		}
	
("*"|"/"|"%") 	{
			//log<<"Line No. "<<lCount<<":  Token <MULOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<MULOP,"<<yytext<<"> "; 
			SymbolInfo* s = new SymbolInfo(yytext,"MULOP");
			yylval.symVal = s;			
			return MULOP;
		}
	
("<="|">="|"=="|"!=") {
			//log<<"Line No. "<<lCount<<":  Token <RELOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<RELOP,"<<yytext<<"> ";
			SymbolInfo* s = new SymbolInfo(yytext, "RELOP");
			yylval.symVal = s; 
			return RELOP;
		}

("<"|">")	{
			//log<<"Line No. "<<lCount<<":  Token <RELOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<RELOP,"<<yytext<<"> "; 
			SymbolInfo* s = new SymbolInfo(yytext, "RELOP");
			yylval.symVal = s; 			
			return RELOP;
		}

("=") 		{ 
			//log<<"Line No. "<<lCount<<":  Token <ASSIGNOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<ASSIGNOP,"<<yytext<<"> "; 
			return ASSIGNOP;
		}

("&&"|"||") {
			//log<<"Line No. "<<lCount<<":  Token <LOGICOP> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<LOGICOP,"<<yytext<<"> "; 
			SymbolInfo* s = new SymbolInfo(yytext, "LOGICOP");
			yylval.symVal = s; 			
			return LOGICOP;
		}

"(" 		{	
			//log<<"Line No. "<<lCount<<":  Token <LPAREN> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<LPAREN,"<<yytext<<"> "; 
			return LPAREN;
		}

")" 		{		
			//log<<"Line No. "<<lCount<<":  Token <RPAREN> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<RPAREN,"<<yytext<<"> "; 
			return RPAREN;
		}

("{") 		{
			//log<<"Line No. "<<lCount<<":  Token <LCURL> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<LCURL,"<<yytext<<"> "; 
			return LCURL;
		}

("}") 		{
			//log<<"Line No. "<<lCount<<":  Token <RCURL> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<RCURL,"<<yytext<<"> "; 
			return RCURL;
		}

("[") 		{	
			//log<<"Line No. "<<lCount<<":  Token <LTHIRD> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<LTHIRD,"<<yytext<<"> "; 
			return LTHIRD;
		}


("]") 		{	
			//log<<"Line No. "<<lCount<<":  Token <RTHIRD> Lexeme "<<yytext<<"found\n" << endl; 
			//token<<"<RTHIRD,"<<yytext<<"> "; 
			return RTHIRD;
		}

(";") 		{
			//log<<"Line No. "<<lCount<<":  Token <SEMICOLON> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<SEMICOLON,"<<yytext<<"> "; 
			return SEMICOLON;
		}

(",") 		{
			//log<<"Line No. "<<lCount<<":  Token <COMMA> Lexeme "<<yytext<<" found\n" << endl; 
			//token<<"<COMMA,"<<yytext<<"> "; 
			return COMMA;		
			}
("!")		{return NOT;}


{MULTI_CHAR}	{
			//log<<"Error at Line No. "<<lCount<<":  Multi character constant error "<<yytext<<"\n" << endl;
			errCount++;
		}

{UNFIN_CHAR}|{UNFINSP_CHAR} {
			//log<<"Error at Line No. "<<lCount<<":  Unterminated character error "<<yytext<<"\n" << endl;
			errCount++;
		}

{EMP_CHAR}	{
			//log<<"Error at Line No. "<<lCount<<":  Empty character constant error "<<yytext<<"\n" << endl;
			errCount++;			
		}

{UNFIN_STRING}	{
			//log<<"Error at Line No. "<<lCount<<":  Unterminated String "<<yytext<<"\n" << endl;
			for(int i = 0; i<yyleng ; i++){if(yytext[i] == '\n') lCount++;}
			errCount++;
		}

{UNFIN_COMMENT} {
			//log<<"Error at Line No. "<<lCount<<":  Unterminated Comment "<<yytext<<"\n" << endl;
			for(int i = 0; i<yyleng ; i++){if(yytext[i] == '\n') lCount++;}
			errCount++;
		}

{FLOAT_ERROR1}	{
			//log<<"Error at Line No. "<<lCount<<":  Too many decimal point "<<yytext<<"\n" << endl;
			errCount++;
		}

{FLOAT_ERROR2}  {
			//log<<"Error at Line No. "<<lCount<<":  Ill formed number "<<yytext<<"\n" << endl;
			errCount++;
		}

{INV_SUF}	{
			//log<<"Error at Line No. "<<lCount<<":  Invalid prefix on ID or invalid suffix on Number "<<yytext<<"\n" << endl;
			errCount++;
		}

.		{
			//log<<"Error at Line No. "<<lCount<<":  Unrecognized character "<<yytext<<"\n" << endl;
			errCount++;
		}

%%
/*
int main(int argc,char *argv[]){
	
	
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE* input = fopen(argv[1], "r");
	
	if(input==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

 	
	token.open("1405045_token.txt");
	log.open("1405045_log.txt");
	
	yyin = input;
	yylex();
	fclose(yyin);

	log << endl;
	log << "Total Lines: " << --lCount << endl;
	log << "Total Error: " << errCount << endl;	

	log.close();
	token.close();

	return 0;

}
*/
