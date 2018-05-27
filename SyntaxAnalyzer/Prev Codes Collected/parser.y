%{
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<vector>
#include "SymTable.h"
//#define YYSTYPE SymbolInfo*

using namespace std;

string variable_type;
int yyparse(void);
int yylex(void);
extern FILE *yyin;

SymbolTable table(8);
SymbolTable funcParam(5);
int IDargs = 0;
vector<string> args; 
bool funcDef = false;

void yyerror(char *s)
{
	//write your code
}


%}
%union{
SymbolInfo* symVal;
}

%token COMMENT IF ELSE FOR WHILE DO BREAK CONTINUE INT FLOAT CHAR DOUBLE VOID RETURN SWITCH CASE DEFAULT INCOP DECOP ADDOP MULOP RELOP ASSIGNOP LOGICOP LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD SEMICOLON COMMA STRING NOT PRINTLN
%token <symVal>ID 
%token <symVal>CONST_INT 
%token <symVal>CONST_FLOAT 
%token <symVal>CONST_CHAR 

%type <symVal>type_specifier expression logic_expression rel_expression simple_expression term unary_expression factor variable

%nonassoc second_precedence
%nonassoc ELSE

%%

start : program
		{
		//write your code in this block in all the similar blocks below
		}
		;

program : program unit
		{

		} 
		| 
		unit
		{	

		}
		;
	
unit : 	var_declaration
		{

		}
     	| 
     	func_declaration
     	{

     	}
     	| 
     	func_definition
     	{

     	}
     	;
     
func_declaration : 	type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
					{
						SymbolInfo *temp = table.lookUp($2->getName(), "FUNC");
						if(temp != NULL){
						//error message	
						}else {
							SymbolInfo* temp = table.InsertandGet($2->getName(), "ID");
							temp->setIDType("FUNC");
							temp->setFuncRet($1->getVarType());
							for(int i = 0; i<args.size(); i++){
								temp->ParamList.push_back(args[i]);					
							}
							args.clear();
						}
						funcParam.exitScope();
					}
		 			;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
				{
				SymbolInfo *temp = table.lookUp($2->getName(), "FUNC");
				if(args.size() != IDargs){
					//error
					args.clear(); IDargs = 0;
				}												
				if(temp != NULL){
					if(temp->isFuncDefined()== true){
					//error message
					}
					else if(temp->getFuncRet() != $2->getFuncRet()){
						//error
						args.clear(); 
					} 
					else if(temp->ParamList.size() != args.size()){
						//error
						args.clear();					
					}
					else{
						for(int i = 0; i<temp->ParamList.size(); i++){
							if(temp->ParamList[i] != args[i]){
								//error
								args.clear();							
							}
						}				
					}
				}
				else{
					SymbolInfo* temp = new SymbolInfo();
					temp = table.InsertandGet($2->getName(), "ID");
					temp->setIDType("FUNC");
					temp->setFuncRet($1->getVarType());
					//
					for(int i = 0; i<args.size(); i++){
						temp->ParamList.push_back(args[i]);					
					}
					temp->setFuncDefined();
				}
				args.clear();
				funcParam.exitScope();
				}
 		 		;
 		 
parameter_list  : parameter_list COMMA type_specifier ID
				{
					args.push_back($3->getVarType());
					IDargs++;
					funcParam.enterScope();
					SymbolInfo* temp = funcParam.InsertandGet($4->getName(), $4->getType());
					temp->setIDType("VAR");
					temp->setVarType($3->getVarType());
					string dataType = $3->getVarType();
					if(dataType == "INT")temp->AraElem = new vector<int>(1);
					else if(dataType == "FLOAT")temp->AraElem = new vector<float>(1);
					else if(dataType == "CHAR")temp->AraElem = new vector<char>(1);
				} 
 				| parameter_list COMMA type_specifier	
				{					
					args.push_back($3->getVarType());
				}
 				| type_specifier ID
				{
					args.clear();
					args.push_back($1->getVarType());
					IDargs++;
					SymbolInfo* temp = funcParam.InsertandGet($2->getName(), $2->getType());
					temp->setIDType("VAR");
					temp->setVarType($1->getVarType());
					string dataType = variable_type;
					if(dataType == "INT")temp->AraElem = new vector<int>(1);
					else if(dataType == "FLOAT")temp->AraElem = new vector<float>(1);
					else if(dataType == "CHAR")temp->AraElem = new vector<char>(1);
				}
 				| type_specifier
				{
					args.clear();
					args.push_back($1->getVarType());
				}
				|
 				;
 		
compound_statement : 	LCURL {table.enterScope();} statements RCURL{table.exitScope();}
 		    			| LCURL RCURL
 		    			;
 		    
var_declaration : type_specifier declaration_list SEMICOLON
 		 		;
 		 
type_specifier	: INT
				{
					SymbolInfo* s= new SymbolInfo("INT");
					variable_type = "INT";
					$$ = s;
				}
 				| FLOAT
				{
					SymbolInfo* s= new SymbolInfo("FLOAT");
					variable_type = "FLOAT";
					$$ = s;
				}
 				| VOID
				{
					SymbolInfo* s= new SymbolInfo("VOID");
					variable_type = "VOID";
					$$ = s;
				}
 				;
 		
declaration_list : 	declaration_list COMMA ID
					{
						if(variable_type == "VOID"){
							//error
						}
						else{
							SymbolInfo* temp = table.lookUp($3->getName(), $3->getIDType());
							if(temp!= NULL){
								//error			
							}
							else{
								SymbolInfo* temp2 = table.InsertandGet($3->getName(), $3->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("VAR");
								string dataType = variable_type;
								if(dataType == "INT")temp2->AraElem = new vector<int>(1);
								else if(dataType == "FLOAT")temp2->AraElem = new vector<float>(1);
								else if(dataType == "CHAR")temp2->AraElem = new vector<char>(1);				
							}
						}
					}
 					| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
					{
						if(variable_type == "VOID"){
							//error
						}
						else{
							SymbolInfo* temp = table.lookUp($3->getName(), $3->getIDType());
							if(temp!= NULL){
								//error			
							}
							else{
								SymbolInfo* temp2 = table.InsertandGet($3->getName(), $3->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("VAR");
								int araSize = atoi(($3->getName()).c_str());
								temp2->setAraSize(araSize);
								if(variable_type == "INT")temp2->AraElem = new vector<int>(araSize);
								else if(variable_type == "FLOAT")temp2->AraElem = new vector<float>(araSize);
								else if(variable_type == "CHAR")temp2->AraElem = new vector<char>(araSize);						
							}
						}

					}
 					| ID
					{
						if(variable_type == "VOID"){
							//error
						}
						else{
							SymbolInfo* temp = table.lookUp($1->getName(), $1->getIDType());
							if(temp!= NULL){
								//error			
							}
							else{
								SymbolInfo* temp2 = table.InsertandGet($1->getName(), $1->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("VAR");
								if(variable_type == "INT")temp2->AraElem = new vector<int>(1);
								else if(variable_type == "FLOAT")temp2->AraElem = new vector<float>(1);
								else if(variable_type == "CHAR")temp2->AraElem = new vector<char>(1);						
							}
						}
					}
					| ID LTHIRD CONST_INT RTHIRD
					{
						if(variable_type == "VOID"){
							//error
						}
						else{
							SymbolInfo* temp = table.lookUp($1->getName(), $1->getIDType());
							if(temp!= NULL){
								//error			
							}
							else{
								SymbolInfo* temp2 = table.InsertandGet($1->getName(), $1->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("ARA");
								int araSize = atoi(($3->getName()).c_str());
								temp2->setAraSize(araSize);
								if(variable_type == "INT")temp2->AraElem = new vector<int>(araSize);
								else if(variable_type == "FLOAT")temp2->AraElem = new vector<float>(araSize);
								else if(variable_type == "CHAR")temp2->AraElem = new vector<char>(araSize);						
							}
						}
					}						
					;
 		  
statements : statement
	   | statements statement
	   ;
	   
statement : var_declaration
	  | expression_statement
	  | compound_statement
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
	  | IF LPAREN expression RPAREN statement %prec second_precedence 
	  | IF LPAREN expression RPAREN statement ELSE statement
	  | WHILE LPAREN expression RPAREN statement
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
	  | RETURN expression SEMICOLON
	  ;
	  
expression_statement 	: SEMICOLON			
						| expression SEMICOLON 
						;
	  
variable : 	ID
			{
				SymbolInfo* temp = table.lookUp($1->getName(),$1->getIDType());
				if(temp == NULL){
					//error	
				}
				else{
					$$ = temp;
					variable_type = temp->getVarType();
				}			
			} 		
	 		| ID LTHIRD expression RTHIRD
			{
				SymbolInfo* temp = table.lookUp($1->getName(),$1->getIDType());
				if(temp == NULL){
					//error				
				}
				else{
					$$ = temp;
					$$->setAraIndex($3->AraElem[0]);
					variable_type = temp->getVarType();
				}			
			} 
	 		;
	 
expression : logic_expression	
	   		| variable ASSIGNOP logic_expression
			{
				
			} 	
	   		;
			
logic_expression 	: rel_expression 	
		 			| rel_expression LOGICOP rel_expression 	
		 			;
			
rel_expression	: simple_expression 
				| simple_expression RELOP simple_expression	
				;
				
simple_expression 	: term 
		 			| simple_expression ADDOP term 
					;
					
term :	unary_expression
     |  term MULOP unary_expression
     ;

unary_expression: ADDOP unary_expression  
				{$$=$2;}
		 		| NOT unary_expression 
				{$$=$2;}
		 		| factor 
		 		;
	
factor	: variable{} 
		| ID LPAREN argument_list RPAREN
		{
		SymbolInfo *temp=new SymbolInfo();
			temp = table.lookUp($1->getName(), "FUNC");
			if(temp == NULL){
				//error
			}
			else{
				if(temp->getFuncRet() == "VOID"){
					//error
				} 
				else{ 
					SymbolInfo *temp2 = new SymbolInfo($1->getFuncRet());
					temp2->AraElem[0] = 0;
				}
			} 
		}
		| LPAREN expression RPAREN
		{
			$$ = $2;
		}
		| CONST_INT //{variable_type = "INT";} 
		| CONST_FLOAT //{variable_type = "FLOAT";}
		| CONST_CHAR //{variable_type = "CHAR";}
		| variable INCOP
		{
			$$ = $1;
			$$->AraElem[0] = $$->AraElem[0] +1;
		} 
		| variable DECOP
		{
			$$ = $1;
			$$->AraElem[0] = $$->AraElem[0] -1;
		}
		;
	
argument_list	: arguments
				|
				;

arguments	:arguments COMMA logic_expression
			|logic_expression
			;
 

%%
int main(int argc,char *argv[])
{

	if((yyin=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	
	

	//yyin=fp;
	yyparse();
	

	return 0;
}

