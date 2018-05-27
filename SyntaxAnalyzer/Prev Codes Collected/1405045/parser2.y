%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <limits>
#include "SymTable.h"

//#define YYSTYPE SymbolInfo*

using namespace std;

string variable_type;
int yyparse(void);
int yylex(void);
extern FILE *yyin;

SymbolTable table(13);
int IDargs = 0;
vector<string> args; 
bool funcDef = false;
extern int lCount;
int semErrors;
vector<SymbolInfo> params; 

ofstream logFile, errorFile;

void yyerror(const char *s)
{
	//write your code
}


%}
%union{
SymbolInfo* symVal;
}

%token COMMENT IF ELSE FOR WHILE DO BREAK CONTINUE INT FLOAT CHAR DOUBLE VOID RETURN SWITCH CASE DEFAULT INCOP DECOP ASSIGNOP LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD SEMICOLON COMMA STRING NOT PRINTLN
%token <symVal>ID 
%token <symVal>CONST_INT 
%token <symVal>CONST_FLOAT 
%token <symVal>CONST_CHAR 
%token <symVal>ADDOP
%token <symVal>MULOP
%token <symVal>LOGICOP
%token <symVal>RELOP

%type <symVal>type_specifier expression logic_expression rel_expression simple_expression term unary_expression factor variable

%nonassoc second_precedence
%nonassoc ELSE
%error-verbose
%%

start : program
		{
		//write your code in this block in all the similar blocks below
			logFile << "Line " << lCount << " : start : program\n"<< endl;
		}
		;

program : program unit
		{
			logFile << "Line " << lCount << " : program : program unit\n"<< endl;
		} 
		| 
		unit
		{	
			logFile << "Line " << lCount << " : program : unit\n"<< endl;
		}
		;
	
unit : 	var_declaration
		{
			logFile << "Line " << lCount << " : unit : var_declaration\n"<< endl;
		}
     	| 
     	func_declaration
     	{
			logFile << "Line " << lCount << " : unit : func_declaration\n"<< endl;
     	}
     	| 
     	func_definition
     	{
			logFile << "Line " << lCount << " : unit : func_definition\n"<< endl;
     	}
     	;
     
func_declaration : 	type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
					{
						logFile << "Line " << lCount << " : func_declaration : 	type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n"; 
						logFile << $2->getName() << endl << endl;
						SymbolInfo *temp = table.lookUp($2->getName(), "FUNC");
						if(temp != NULL){
							errorFile << "Error at line " << lCount << " Function "<< $2->getName() <<" already declared" << endl << endl;
							semErrors++;
						}else {
							SymbolInfo* temp2 = table.InsertandGet($2->getName(), "ID");
							temp2->setIDType("FUNC");
							temp2->setFuncRet($1->getVarType());
							for(int i = 0; i<args.size(); i++){
								temp2->ParamList.push_back(args[i]);					
							}
							args.clear();
						}
						//funcParam.exitScope();
					}
					|type_specifier ID LPAREN parameter_list RPAREN error
					{
						errorFile << "Error at line " << lCount << "; missing" << endl << endl;
						semErrors++;
					}
		 			;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
				{
				logFile << "Line " << lCount << " : func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement\n";
				logFile << $2->getName() << endl << endl;
				SymbolInfo *temp = table.lookUp($2->getName(), "FUNC");
				if(args.size() != IDargs){
					errorFile << "Error at line " << lCount << " Parameter mismatch for Function "<< $2->getName() << endl << endl;
					args.clear(); IDargs = 0;
					semErrors++;
				}												
				if(temp != NULL){
					//logFile << "Function " << $2->getName() <<" already declared" << endl;
					if(temp->isFuncDefined()== true){
						errorFile << "Error at line " << lCount << "Function "<< $2->getName() <<" already defined" << endl << endl;
						semErrors++;
						args.clear();
						IDargs = 0;
					}
					else if(temp->getFuncRet() != $1->getVarType()){
						errorFile << "Error at line " << lCount << "Function "<< $2->getName() <<" :return type doesn't match declaration" << endl << endl;
						semErrors++;
						args.clear();IDargs = 0; 
					} 
					else if(temp->ParamList.size() != args.size()){
						errorFile << "Error at line " << lCount << "Function "<< $2->getName() <<" :Parameter list doesn not match declaration" << endl << endl;
						args.clear();IDargs = 0;
						semErrors++;					
					}
					else{
						for(int i = 0; i<temp->ParamList.size(); i++){
							if(temp->ParamList[i] != args[i]){
								errorFile << "Error at line " << lCount << "Function "<< $2->getName()<< " :argument mismatch" << endl << endl;
								args.clear();IDargs = 0;
								semErrors++;	
							}
						}				
					}
				}
				else{
					SymbolInfo* temp = new SymbolInfo();
					temp = table.InsertandGet($2->getName(), "ID");
					//logFile << $2->getName() << " Function inserted" << endl;
					temp->setIDType("FUNC");
					temp->setFuncRet($1->getVarType());
					//
					for(int i = 0; i<args.size(); i++){
						temp->ParamList.push_back(args[i]);					
					}
					temp->setFuncDefined();
				}
				args.clear();
				IDargs = 0;
				
	
				//funcParam.exitScope();
				}
 		 		;
 		 
parameter_list  : parameter_list COMMA type_specifier ID
				{
					logFile << "Line " << lCount << " : parameter_list  : parameter_list COMMA type_specifier ID\n";
					logFile << $4->getName() << endl << endl;
					args.push_back(variable_type);//changed from $3->getVarType()
					IDargs++;
					//SymbolInfo* temp = funcParam.InsertandGet($4->getName(), $4->getType());					
					//temp->setIDType("VAR");
					//temp->setVarType($3->getVarType());
					$4->setIDType("VAR");
					$4->setVarType(variable_type);//changed from $3->getVarType()
					SymbolInfo* temp = new SymbolInfo($4->getName(), $4->getType());
					temp->setIDType("VAR");
					
					params.push_back(*temp);
				
				} 
 				| parameter_list COMMA type_specifier	
				{					
					logFile << "Line " << lCount << " : parameter_list  : parameter_list COMMA type_specifier\n"<< endl;
					args.push_back($3->getVarType());
				}
 				| type_specifier ID
				{
					logFile << "Line " << lCount << " : parameter_list  : type_specifier ID\n";
					logFile << $2->getName() << endl << endl;
					//args.clear();
					args.push_back(variable_type);//$1->getVarType()
					IDargs++;
					//SymbolInfo* temp = funcParam.InsertandGet($2->getName(), $2->getType());
					//temp->setIDType("VAR");
					//temp->setVarType($1->getVarType());
					$2->setIDType("VAR");
					$2->setVarType(variable_type);//$1->getVarType()
					params.push_back(*$2);
				
				}
 				| type_specifier
				{
					logFile << "Line " << lCount << " : parameter_list  : type_specifier\n"<< endl;
					//args.clear();
					args.push_back(variable_type);
				}
				|{//logFile << "Empty parameter " << lCount << endl;
				}
 				;
 		
compound_statement : 	LCURL 
						{
							table.enterScope(); 
							for(int i = 0; i<params.size(); i++) table.Insert(params[i]);
							params.clear();
						} statements {table.printAll(logFile);}RCURL{table.exitScope();}
						{
							logFile << "Line " << lCount << " : compound_statement : LCURL statements RCURL\n"<< endl;
						}
 		    			| LCURL RCURL{logFile << "Line " << lCount << " : compound_statement : LCURL RCURL\n"<< endl;}
 		    			;
 		    
var_declaration : type_specifier declaration_list SEMICOLON{logFile << "Line " << lCount << " : var_declaration : type_specifier declaration_list SEMICOLON\n"<< endl;}
				|type_specifier declaration_list error
				{
						errorFile << "Error at line " << lCount << "; missing" << endl << endl;
						semErrors++;
				} 		 		
				;
 		 
type_specifier	: INT
				{
					logFile << "Line " << lCount << " : type_specifier	: INT\n"<< endl; 
					SymbolInfo* s= new SymbolInfo("INT");
					variable_type = "INT";
					$$ = s;
				}
 				| FLOAT
				{	
					logFile << "Line " << lCount << " : type_specifier	: FLOAT\n"<< endl;
					SymbolInfo* s= new SymbolInfo("FLOAT");
					variable_type = "FLOAT";
					$$ = s;
				}
 				| VOID
				{
					logFile << "Line " << lCount << " : type_specifier	: VOID\n"<< endl;
					SymbolInfo* s= new SymbolInfo("VOID");
					variable_type = "VOID";
					$$ = s;
				}
 				;
 		
declaration_list : 	declaration_list COMMA ID
					{
						logFile << "Line " << lCount << " : declaration_list : 	declaration_list COMMA ID\n";
						logFile << $3->getName() << endl << endl;
						if(variable_type == "VOID"){
							errorFile << "Error at line " << lCount << " :variable type can't be void" << endl << endl;
							semErrors++;
						}
						else{
							SymbolInfo* temp = table.lookUp($3->getName(), "VAR");
							if(temp != NULL){
							errorFile << "Error at line " << lCount << ": Variable "<< $3->getName() <<" already declared" << endl << endl;	
								semErrors++;	
							}
							else{
								//SymbolInfo* temp2 = table.InsertandGet($3->getName(), $3->getType());
								//temp2->setVarType(variable_type);
								//temp2->setIDType("VAR");
								SymbolInfo* temp2 = new SymbolInfo($3->getName(), $3->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("VAR");
								table.Insert(*temp2);
								//table.printAll(logFile);
							}
						}
					}
 					| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
					{
						logFile << "Line " << lCount << " : declaration_list : 	declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n";
						logFile << $3->getName() << endl;
						logFile << $5->getName() << endl << endl;
						if(variable_type == "VOID"){
							errorFile << "Error at line " << lCount << " : array type can't be void" << endl << endl;
							semErrors++;
						}
						else{
							SymbolInfo* temp = table.lookUp($3->getName(), "ARA");
							if(temp!= NULL){
							errorFile << "Error at line " << lCount << " : Array "<< $3->getName()<< " already declared" << endl << endl;
								semErrors++;			
							}
							else{
								SymbolInfo* temp2 = new SymbolInfo($3->getName(), $3->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("ARA");
								int araSize = atoi(($5->getName()).c_str());
								temp2->setAraSize(araSize);
								if(variable_type == "INT"){								
									for(int i = temp2->ints.size(); i<araSize; i++){
										temp2->ints.push_back(0);
									}							
								}
								else if(variable_type == "FLOAT"){								
									for(int i = temp2->floats.size(); i<araSize; i++){
										temp2->floats.push_back(0);
									}							
								}
								else if(variable_type == "CHAR"){								
									for(int i = temp2->chars.size(); i<araSize; i++){
										temp2->chars.push_back('\0');
									}							
								}
								table.Insert(*temp2);						
								//table.printAll(logFile);
							}
						}

					}
 					| ID
					{
						logFile << "Line " << lCount << " : declaration_list :	ID\n";
						logFile << $1->getName() << endl << endl;
						if(variable_type == "VOID"){
							errorFile << "Error at line " << lCount << " :variable type can't be void" << endl << endl;
							semErrors++;
						}
						else{
							SymbolInfo* temp = table.lookUp($1->getName(), "ARA");
							if(temp!= NULL){
							errorFile << "Error at line " << lCount << ": Variable "<< $1->getName() <<" already declared" << endl << endl;	
								semErrors++;		
							}
							else{
								SymbolInfo* temp2 = new SymbolInfo($1->getName(), $1->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("VAR");
								table.Insert(*temp2);
								//table.printAll(logFile);		
							}
						}
					}
					| ID LTHIRD CONST_INT RTHIRD
					{
						logFile << "Line " << lCount << " : declaration_list :	ID LTHIRD CONST_INT RTHIRD\n";
						logFile << $1->getName() << endl;
						logFile << $3->getName() << endl << endl;
						if(variable_type == "VOID"){
							errorFile << "Error at line " << lCount << " :array type can't be void" << endl << endl;
							semErrors++;
						}
						else{
							SymbolInfo* temp = table.lookUp($1->getName(), "ARA");
							if(temp!= NULL){
								errorFile << "Error at line " << lCount << ": Array "<< $1->getName() <<" already declared" << endl << endl;	
								semErrors++;
							}
							else{
								SymbolInfo* temp2 = new SymbolInfo($1->getName(), $1->getType());
								temp2->setVarType(variable_type);
								temp2->setIDType("ARA");
								int araSize = atoi(($3->getName()).c_str());
								temp2->setAraSize(araSize);
								table.Insert(*temp2);
								//table.printAll(logFile);			
							}
						}
					}						
					;
 		  
statements : statement{logFile << "Line " << lCount << " : statements : statement\n"<< endl;}
	   | statements statement{logFile << "Line " << lCount << " : statements : statements statement\n"<< endl;}
	   ;
	   
statement : var_declaration{logFile << "Line " << lCount << " : statement : var_declaration\n"<< endl;}
	  | expression_statement{logFile << "Line " << lCount << " : statement : expression_statement\n"<< endl;}
	  | compound_statement{logFile << "Line " << lCount << " : statement : compound_statement\n"<< endl;}
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
		{logFile << "Line " << lCount << " : statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n"<< endl;}
	  | IF LPAREN expression RPAREN statement %prec second_precedence
		{logFile << "Line " << lCount << " : statement : IF LPAREN expression RPAREN statement\n"<< endl;} 
	  | IF LPAREN expression RPAREN statement ELSE statement 
		{logFile << "Line " << lCount << " : statement : IF LPAREN expression RPAREN statement ELSE statement\n"<< endl;}
	  | WHILE LPAREN expression RPAREN statement{logFile << "Line " << lCount << " : statement : WHILE LPAREN expression RPAREN statement\n"<< endl;}
	  | PRINTLN LPAREN ID RPAREN SEMICOLON{logFile << "Line " << lCount << " : statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n"<< endl;}
	  | PRINTLN LPAREN ID RPAREN error
	{
		errorFile << "Error at line " << lCount << "; missing" << endl << endl;
		semErrors++;
	} 		 		
	  | RETURN expression SEMICOLON{logFile << "Line " << lCount << " : statement : RETURN expression SEMICOLON\n"<< endl;}
	  |RETURN expression error
	{
			errorFile << "Error at line " << lCount << "; missing" << endl << endl;
			semErrors++;
		} 		 			
	  ;
	  
expression_statement 	: SEMICOLON{logFile << "Line " << lCount << " : expression_statement : SEMICOLON\n"<< endl;}			
						| expression SEMICOLON {logFile << "Line " << lCount << " : expression_statement : expression SEMICOLON\n"<< endl;}
						|expression error
						{
							errorFile << "Error at line " << lCount << "; missing" << endl << endl;
							semErrors++;
						} 		 		
						;
	  
variable : 	ID
			{
				logFile << "Line " << lCount << " : variable : ID\n";
				logFile << $1->getName() << endl << endl;
				SymbolInfo* temp = table.lookUp($1->getName(),"VAR");
				if(temp == NULL){
					//logFile << "Variable " << $1->getName() << " doesn't exist" << endl;
					errorFile << "Error at line " << lCount << " : " << $1->getName() << " doesn't exist" <<  endl << endl;					
					semErrors++;
				}
				else{
					$$ = temp;
					//variable_type = temp->getVarType();
				}			
			} 		
	 		| ID LTHIRD expression RTHIRD
			{
				logFile << "Line " << lCount << " : variable : ID LTHIRD expression RTHIRD\n";
				logFile << $1->getName() << endl << endl;
				SymbolInfo* temp = table.lookUp($1->getName(),"ARA");
				if(temp == NULL){
				errorFile << "Error at line " << lCount << " : " <<$1->getName() << " doesn't exist" <<  endl << endl;					
					semErrors++;				
				}
				else{
					//$$ = temp;
					if($3->ints[0] >= temp->getAraSize()){
				errorFile << "Error at line " << lCount << " : " <<$1->getName() << " array index out of bounds" <<  endl << endl;				
						semErrors++;
						//temp->setAraIndex(0);
					} 
					else temp->setAraIndex($3->ints[0]);
					if(temp->getVarType() == "INT"){
						while(temp->ints.size() <= temp->getAraIndex()){
							temp->ints.push_back(0);
						}
					}
					else if(temp->getVarType() == "FLOAT"){
						while(temp->floats.size() <= temp->getAraIndex()){
							temp->floats.push_back(0);
						}
					}
					else if(temp->getVarType() == "CHAR"){
						while(temp->chars.size() <= temp->getAraIndex()){
							temp->chars.push_back('\0');
						}
					}
					$$ = temp;
					//variable_type = temp->getVarType();
				}			
			} 
	 		;
	 
expression : logic_expression	
			{
				logFile << "Line " << lCount << " : expression : logic_expression\n"<< endl;
				$$ = $1;
				$$->ints.push_back(0);
				$$->floats.push_back(0);
				//logFile << "At Logic, value " << $$->ints[0] << " Type = " << $$->getVarType()<< endl;
			}
	   		| variable ASSIGNOP logic_expression
			{
				logFile << "Line " << lCount << " : expression : variable ASSIGNOP logic_expression\n"<< endl;
				string vType = $1->getVarType();
				if(vType == "INT"){
					if($1->getIDType() == "VAR"){
						$1->ints.push_back(0);
						if($3->getVarType() == "INT"){
							if($3->getIDType() == "VAR")$1->ints[0] = $3->ints[0];
							else $1->ints[0] = $3->ints[$3->getAraIndex()];
						}
						else{
							errorFile << "Warning at line " << lCount <<" : Assigning float value to integer"<<endl << endl;
							if($3->getIDType() == "VAR")$1->ints[0] = (int)$3->floats[0];
							else $1->ints[0] = (int)$3->floats[$3->getAraIndex()];
						}
					}
					else if($1->getIDType() == "ARA"){
						$1->ints.push_back(0);
						if($3->getVarType() == "INT"){
							if($3->getIDType() == "VAR")$1->ints[$1->getAraIndex()] = $3->ints[0];
							else $1->ints[$1->getAraIndex()] = $3->ints[$3->getAraIndex()];
						}
						else{
							errorFile << "Warning at line " << lCount <<" : Assigning float value to integer"<<endl << endl;
							if($3->getIDType() == "VAR")$1->ints[$1->getAraIndex()] = (int)$3->floats[0];
							else $1->ints[$1->getAraIndex()] = (int)$3->floats[$3->getAraIndex()];
						}
					}
				}
				else if(vType == "FLOAT"){
					if($1->getIDType() == "VAR"){
						$1->floats.push_back(0);
						if($3->getVarType() == "INT"){
							if($3->getIDType() == "VAR")$1->floats[0] = (float)$3->ints[0];
							else $1->floats[0] = (float)$3->ints[$3->getAraIndex()];
						}
						else{
							if($3->getIDType() == "VAR")$1->floats[0] = $3->floats[0];
							else $1->floats[0] = $3->floats[$3->getAraIndex()];
						}
					}
					else if($1->getIDType() == "ARA"){
						$1->floats.push_back(0);
						if($3->getVarType() == "INT"){
							if($3->getIDType() == "VAR")$1->floats[$1->getAraIndex()] = (float)$3->ints[0];
							else $1->floats[$1->getAraIndex()] = (float)$3->ints[$3->getAraIndex()];
						}
						else{
							errorFile << "Warning at line " << lCount <<" : Assigning float value to integer"<<endl << endl;
							if($3->getIDType() == "VAR")$1->floats[$1->getAraIndex()] = $3->floats[0];
							else $1->floats[$1->getAraIndex()] = $3->floats[$3->getAraIndex()];
						}
					}
				}
				$$ = $1;
				//$$->ints[0] = 0;
				//$$->floats[0] = 0 ;
				//logFile << $$->getName()<< " = " << $$->ints[0] << endl;
				//logFile << $$->getName()<< " = " << $$->floats[0] << endl;
				table.printAll(logFile);				
			} 	
	   		;
			
logic_expression 	: rel_expression
					{
						logFile << "Line " << lCount << " : logic_expression : rel_expression\n"<< endl;
						$$ = $1; 
						$$->ints.push_back(0);
						$$->floats.push_back(0);
						//logFile << "At Rel, value " << $$->floats[0]<< " Type = " << $$->getVarType() << endl;
					} 	
		 			| rel_expression LOGICOP rel_expression
					{
						logFile << "Line " << lCount << " : logic_expression : rel_expression LOGICOP rel_expression\n"<< endl;
						SymbolInfo* temp = new SymbolInfo("INT");
						if($1->getVarType() == "CHAR" || $3->getVarType() == "CHAR"){
							//logFile << "Logical operation not allowed for char datatype" << endl;
							temp->ints.push_back(0);
						}
						string logicop = $2->getName();
						if(logicop == "&&"){
							if($1->getVarType() == "FLOAT"){
								$1->floats.push_back(0);
								if($1->floats[0] == 0){
									temp->ints[0] = 0;								
								}
								else if($3->getVarType() == "FLOAT"){
									$3->floats.push_back(0);
									if($3->floats[0] == 0) temp->ints[0] = 0;
									else temp->ints[0] = 1;
								}
								else if($3->getVarType() == "INT"){
									$3->ints.push_back(0);
									if($3->ints[0] == 0) temp->ints[0] = 0;
									else temp->ints[0] = 1;
								}
							}
							else if($1->getVarType() == "INT"){
								$1->ints.push_back(0);
								if($1->ints[0] == 0) temp->ints[0] = 0;
								else if($3->getVarType() == "FLOAT"){
									if($3->floats[0] == 0) temp->ints[0] = 0;
									else temp->ints[0] = 1;
								}
								else if($3->getVarType() == "INT"){\
									$3->ints.push_back(0);
									if($3->ints[0] == 0) temp->ints[0] = 0;
									else temp->ints[0] = 1;
								}
							}
						}
						else if(logicop == "||"){
							if($1->getVarType() == "FLOAT"){
								$1->floats.push_back(0);
								if($1->floats[0] != 0){
									temp->ints[0] = 1;								
								}
								else if($3->getVarType() == "FLOAT"){
									$3->floats.push_back(0);
									if($3->floats[0] != 0) temp->ints[0] = 1;
									else temp->ints[0] =0;
								}
								else if($3->getVarType() == "INT"){
									if($3->ints[0] != 0) temp->ints[0] = 1;
									else temp->ints[0] =0;
								}
							}
							else if($1->getVarType() == "INT"){
								$1->ints.push_back(0);
								if($1->ints[0] != 0) temp->ints[0] = 1;
								else if($3->getVarType() == "FLOAT"){
									$3->floats.push_back(0);									
									if($3->floats[0] != 0) temp->ints[0] = 1;
									else temp->ints[0] =0;
								}
								else if($3->getVarType() == "INT"){
									$3->ints.push_back(0);
									if($3->ints[0] != 0) temp->ints[0] = 1;
									else temp->ints[0] =0;
								}
							}
						}
						$$ = temp;	
						//logFile << "At rel LOGICOP rel, value " << $$->ints[0] << " Type = " << $$->getVarType() << endl;
					} 	
		 			;
			
rel_expression	: simple_expression 
				{
					logFile << "Line " << lCount << " : rel_expression : simple_expression\n"<< endl;
					$$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0);
					//logFile << "At Simple, value " << $$->floats[0]<< " Type = " << $$->getVarType() << endl;
				}
				| simple_expression RELOP simple_expression
				{
					logFile << "Line " << lCount << " : rel_expression : simple_expression RELOP simple_expression\n"<< endl;
					SymbolInfo* temp = new SymbolInfo("INT");
					string relop = $2->getName();
					string type1 = $1->getVarType();
					string type2 = $3->getVarType();
					if(relop == "=="){
						if(type1 != type2){
							//logFile << "Type mismatch for == operand" << endl;						
						}
						else if(type1 == "INT"){
							if($1->ints[0] == $3->ints[0]) temp->ints[0] =1;
							else temp->ints[0] =0;
						}
						else if(type1 == "FLOAT"){
							if($1->floats[0] == $3->floats[0]) temp->ints[0] =1;
							else temp->ints[0] =0;		
						}
						else if(type1 == "CHAR"){
							if($1->chars[0] == $3->chars[0]) temp->ints[0] =1;
							else temp->ints[0] =0;		
						}
					}
					else if(relop == "!="){
						if(type1 != type2){
							//logFile << "Type mismatch for != operand" << endl;						
						}
						else if(type1 == "INT"){
							if($1->ints[0] != $3->ints[0]) temp->ints[0] =1;
							else temp->ints[0] =0;
						}
						else if(type1 == "FLOAT"){
							if($1->floats[0] != $3->floats[0]) temp->ints[0] =1;
							else temp->ints[0] =0;	
						}
						else if(type1 == "CHAR"){
							if($1->chars[0] != $3->chars[0]) temp->ints[0] =1;
							else temp->ints[0] =0;	
						}
					}
					else if(relop == "<=" || relop == "<"){
						if(type1 == "INT"){
							if(type2 == "INT"){
								if($1->ints[0] < $3->ints[0]) temp->ints[0] =1;
								else if($1->ints[0] == $3->ints[0]) temp->ints[0] =1;
								else temp->ints[0] =0;
							}
							else if(type2 == "FLOAT"){
								if($1->ints[0] < $3->floats[0]) temp->ints[0] =1;
								else if($1->ints[0] == $3->floats[0]) temp->ints[0] =1;
								else temp->ints[0] =0;
							}
						}
						else if(type1 == "FLOAT"){
							if(type2 == "INT"){
								if($1->floats[0] < $3->ints[0]) temp->ints[0] =1;
								else if($1->floats[0] == $3->ints[0])temp->ints[0] =1;
								else temp->ints[0] =0;
							}
							else if(type2 == "FLOAT"){
								if($1->floats[0] < $3->floats[0]) temp->ints[0] =1;
								else if($1->floats[0] == $3->floats[0]) temp->ints[0] =1;
								else temp->ints[0] =0;
							}
						}

					}
					else if(relop == ">=" || relop == ">"){
						if(type1 == "INT"){
							if(type2 == "INT"){
								if($1->ints[0] > $3->ints[0]) temp->ints[0] =1;
								else if($1->ints[0] == $3->ints[0])temp->ints[0] =1;
								else temp->ints[0] =0;
							}
							else if(type2 == "FLOAT"){
								if($1->ints[0] > $3->floats[0]) temp->ints[0] =1;
								else if($1->ints[0] == $3->floats[0]) temp->ints[0] =1;
								else temp->ints[0] =0;
							}
						}
						else if(type1 == "FLOAT"){
							if(type2 == "INT"){
								if($1->floats[0] > $3->ints[0]) temp->ints[0] =1;
								else if($1->floats[0] == $3->ints[0]) temp->ints[0] =1;
								else temp->ints[0] =0;
							}
							else if(type2 == "FLOAT"){
								if($1->floats[0] > $3->floats[0]) temp->ints[0] =1;
								else if($1->floats[0] == $3->floats[0]) temp->ints[0] =1;
								else temp->ints[0] =0;
							}
						}

					}
					$$ = temp;	
				}	
				;
				
simple_expression 	: term
					{
						logFile << "Line " << lCount << " : simple_expression : term\n"<< endl;
						$$ = $1;
						$$->ints.push_back(0);
						$$->floats.push_back(0);
						//logFile << "At Term,value " << $$->floats[0]<< " Type = " << $$->getVarType() << endl;
					} 
		 			| simple_expression ADDOP term
					{
						logFile << "Line " << lCount << " : simple_expression : simple_expression ADDOP term\n"<< endl;
						string addop = $2->getName();
						//logFile << $1->ints[0] << "+" << $3->ints[0] << endl;
						if(addop == "+"){
							if($1->getIDType() == "VAR"){
								if($3->getIDType() == "VAR"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0] =$1->floats[0] + $3->ints[0];							
										}
										else{
											temp->floats[0]=$1->floats[0] + $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[0] + $3->floats[0];							
										}
										else{
											temp->floats[0]= $1->floats[0] + $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0] = $1->ints[0] + $3->ints[0];
										$$ = temp;
									}
								}
								else if($3->getIDType() == "ARA"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0] = $1->floats[0] + $3->ints[$3->getAraIndex()];							
										}
										else{
											temp->floats[0]= $1->floats[0] + $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[0] + $3->floats[$3->getAraIndex()];							
										}
										else{
											temp->floats[0] = $1->floats[0] + $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0] =$1->ints[0] + $3->ints[$3->getAraIndex()];
										$$ = temp;
									}
								}
							}
							else if($1->getIDType() == "ARA"){
								if($3->getIDType() == "VAR"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0] =$1->floats[$1->getAraIndex()] + $3->ints[0];							
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] + $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[$1->getAraIndex()] + $3->floats[0];							
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] + $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0]=$1->ints[$1->getAraIndex()] + $3->ints[0];
										$$ = temp;
									}
								}
								else if($3->getIDType() == "ARA"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0]=$1->floats[$1->getAraIndex()] + $3->ints[$3->getAraIndex()];
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] + $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[$1->getAraIndex()] + $3->floats[$3->getAraIndex()];
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] + $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0]=$1->ints[$1->getAraIndex()] + $3->ints[$3->getAraIndex()];
										$$ = temp;
									}
								}
							}
						}
						else if(addop == "-"){
							if($1->getIDType() == "VAR"){
								if($3->getIDType() == "VAR"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0]=$1->floats[0] - $3->ints[0];							
										}
										else{
											temp->floats[0]=$1->floats[0] - $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[0] - $3->floats[0];							
										}
										else{
											temp->floats[0]=$1->floats[0] - $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0]=$1->ints[0] - $3->ints[0];
										$$ = temp;
									}
								}
								else if($3->getIDType() == "ARA"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0]=$1->floats[0] - $3->ints[$3->getAraIndex()];							
										}
										else{
											temp->floats[0]=$1->floats[0] - $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[0] - $3->floats[$3->getAraIndex()];							
										}
										else{
											temp->floats[0]=$1->floats[0] - $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0]=$1->ints[0] - $3->ints[$3->getAraIndex()];
										$$ = temp;
									}
								}
							}
							else if($1->getIDType() == "ARA"){
								if($3->getIDType() == "VAR"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0]=$1->floats[$1->getAraIndex()] - $3->ints[0];							
										}
										else{
											temp->floats[0]= $1->floats[$1->getAraIndex()] - $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[$1->getAraIndex()] - $3->floats[0];							
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] - $3->floats[0];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0]=$1->ints[$1->getAraIndex()] - $3->ints[0];
										$$ = temp;
									}
								}
								else if($3->getIDType() == "ARA"){						
									if($1->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($3->getVarType() == "INT"){
											temp->floats[0]=$1->floats[$1->getAraIndex()] - $3->ints[$3->getAraIndex()];
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] - $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "FLOAT"){
										SymbolInfo* temp = new SymbolInfo("FLOAT");
										if($1->getVarType() == "INT"){
											temp->floats[0]=$1->ints[$1->getAraIndex()] - $3->floats[$3->getAraIndex()];
										}
										else{
											temp->floats[0]=$1->floats[$1->getAraIndex()] - $3->floats[$3->getAraIndex()];
										}
										$$ = temp;
									}
									else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
										SymbolInfo* temp = new SymbolInfo("INT");
										temp->ints[0]=$1->ints[$1->getAraIndex()] - $3->ints[$3->getAraIndex()];
										$$ = temp;
									}
								}
							}
						}
					} 	
					;
					
term :	unary_expression
		{
			logFile << "Line " << lCount << " : term : unary_expression\n"<< endl;
			$$ = $1;
			$$->ints.push_back(0);
			$$->floats.push_back(0);
			//logFile << "At Unary , value " << $$->floats[0]<< " Type = " << $$->getVarType() << endl;
		}
     	|term MULOP unary_expression
		{
			string mulop = $2->getName();
			logFile << "Line " << lCount << " : term : term MULOP unary_expression\n"<< endl;

			if(mulop == "*")
			{
				if($1->getIDType() == "VAR"){	
					if($3->getIDType() == "VAR"){		
						if($1->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($3->getVarType() == "INT"){
								temp->floats[0]=$1->floats[0] * $3->ints[0];							
							}
							else{
								temp->floats[0]=$1->floats[0] * $3->floats[0];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($1->getVarType() == "INT"){
								temp->floats[0]=$1->ints[0] * $3->floats[0];							
							}
							else{
								temp->floats[0]=$1->floats[0] * $3->floats[0];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
							SymbolInfo* temp = new SymbolInfo("INT");
							temp->ints[0]=$1->ints[0] * $3->ints[0];
							$$ = temp;
						}
					}
					else if($3->getIDType() == "ARA"){		
						if($1->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($3->getVarType() == "INT"){
								temp->floats[0]=$1->floats[0] * $3->ints[$3->getAraIndex()];							
							}
							else{
								temp->floats[0]=$1->floats[0] * $3->floats[$3->getAraIndex()];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($1->getVarType() == "INT"){
								temp->floats[0]=$1->ints[0] * $3->floats[$3->getAraIndex()];							
							}
							else{
								temp->floats[0]=$1->floats[0] * $3->floats[$3->getAraIndex()];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
							SymbolInfo* temp = new SymbolInfo("INT");
							temp->ints[0]=$1->ints[0] * $3->ints[0];
							$$ = temp;
						}
					}
				}
				else if($1->getIDType() == "ARA"){	
					if($3->getIDType() == "VAR"){		
						if($1->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($3->getVarType() == "INT"){
								temp->floats[0]=$1->floats[$1->getAraIndex()] * $3->ints[0];							
							}
							else{
								temp->floats[0]=$1->floats[$1->getAraIndex()] * $3->floats[0];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($1->getVarType() == "INT"){
								temp->floats[0]=$1->ints[$1->getAraIndex()] * $3->floats[0];							
							}
							else{
								temp->floats[0]=$1->floats[$1->getAraIndex()] * $3->floats[0];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
							SymbolInfo* temp = new SymbolInfo("INT");
							temp->ints[0]=$1->ints[$1->getAraIndex()] * $3->ints[0];
							$$ = temp;
						}
					}
					else if($3->getIDType() == "ARA"){		
						if($1->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($3->getVarType() == "INT"){
								temp->floats[0]=$1->floats[$1->getAraIndex()] * $3->ints[$3->getAraIndex()];
							}
							else{
								temp->floats[0]=$1->floats[$1->getAraIndex()] * $3->floats[$3->getAraIndex()];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "FLOAT"){
							SymbolInfo* temp = new SymbolInfo("FLOAT");
							if($1->getVarType() == "INT"){
								temp->floats[0]=$1->ints[$1->getAraIndex()] * $3->floats[$3->getAraIndex()];
							}
							else{
								temp->floats[0]=$1->floats[$1->getAraIndex()] * $3->floats[$3->getAraIndex()];
							}
							$$ = temp;
						}
						else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
							SymbolInfo* temp = new SymbolInfo("INT");
							temp->ints[0]=$1->ints[$1->getAraIndex()] * $3->ints[0];
							$$ = temp;
						}
					}
				}
			}
			else if(mulop == "/")
			{
				if($1->getVarType() == "FLOAT"){
					SymbolInfo* temp = new SymbolInfo("FLOAT");
					if($3->getVarType() == "INT"){
						if($1->getIDType() == "VAR"){
							if($3->getIDType() == "VAR"){
								if($3->ints[0] != 0)temp->floats[0]=$1->floats[0] / $3->ints[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++; 
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->ints[$3->getAraIndex()] != 0)temp->floats[0]=$1->floats[0] / $3->ints[$3->getAraIndex()];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
						else if($1->getIDType() == "ARA"){
							if($3->getIDType() == "VAR"){
								if($3->ints[0] != 0)temp->floats[0]=$1->floats[$1->getAraIndex()] / $3->ints[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->ints[$3->getAraIndex()] != 0){
									temp->floats[0]=$1->floats[$1->getAraIndex()] / $3->ints[$3->getAraIndex()];
								}
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
					}
					else if($3->getVarType() == "FLOAT"){
						if($1->getIDType() == "VAR"){
							if($3->getIDType() == "VAR"){
								if($3->floats[0] != 0)temp->floats[0]=$1->floats[0] / $3->floats[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->floats[$3->getAraIndex()] != 0)temp->floats[0]=$1->floats[0] / $3->floats[$3->getAraIndex()];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
						else if($1->getIDType() == "ARA"){
							if($3->getIDType() == "VAR"){
								if($3->floats[0] != 0)temp->floats[0]=$1->floats[$1->getAraIndex()] / $3->floats[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->floats[$3->getAraIndex()] != 0){
									temp->floats[0]=$1->floats[$1->getAraIndex()] / $3->floats[$3->getAraIndex()];
								}
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
					}
					$$ = temp;
				}
				else if($3->getVarType() == "FLOAT"){
					SymbolInfo* temp = new SymbolInfo("FLOAT");
					if($1->getVarType() == "INT"){
						if($1->getIDType() == "VAR"){
							if($3->getIDType() == "VAR"){
								if($3->floats[0] != 0)temp->floats[0]=$1->ints[0] / $3->floats[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->floats[$3->getAraIndex()] != 0)temp->floats[0]=$1->ints[0] / $3->floats[$3->getAraIndex()];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
						else if($1->getIDType() == "ARA"){
							if($3->getIDType() == "VAR"){
								if($3->floats[0] != 0)temp->floats[0]=$1->ints[$1->getAraIndex()] / $3->floats[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->floats[$3->getAraIndex()] != 0){
									temp->floats[0]=$1->ints[$1->getAraIndex()] / $3->floats[$3->getAraIndex()];
								}
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
					}
					else if($1->getVarType() == "FLOAT"){
						if($1->getIDType() == "VAR"){
							if($3->getIDType() == "VAR"){
								if($3->floats[0] != 0)temp->floats[0]=$1->floats[0] / $3->floats[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->floats[$3->getAraIndex()] != 0)temp->floats[0]=$1->floats[0] / $3->floats[$3->getAraIndex()];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
						else if($1->getIDType() == "ARA"){
							if($3->getIDType() == "VAR"){
								if($3->floats[0] != 0)temp->floats[0]=$1->floats[$1->getAraIndex()] / $3->floats[0];
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}							
							}
							else if($3->getIDType() == "ARA"){
								if($3->floats[$3->getAraIndex()] != 0){
									temp->floats[0]=$1->floats[$1->getAraIndex()] / $3->floats[$3->getAraIndex()];
								}
								else{
									temp->floats[0]=numeric_limits<float>::infinity();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
									semErrors++;
								}
							}
						}
					}				
					$$ = temp;
				}
				else if($3->getVarType() == "INT" && $1->getVarType() == "INT"){
					SymbolInfo* temp = new SymbolInfo("INT");
					if($1->getIDType() == "VAR"){	
						if($3->getIDType() == "VAR"){			
							if($3->ints[0] != 0)temp->ints[0]=$1->ints[0] / $3->ints[0];
							else{
								temp->ints[0]=numeric_limits<int>::max();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
								semErrors++;
							}
						}
						else if($3->getIDType() == "ARA"){
							if($3->ints[$3->getAraIndex()] != 0)temp->ints[0]=$1->ints[0] / $3->ints[$3->getAraIndex()];
							else{
								temp->ints[0]=numeric_limits<int>::max();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
								semErrors++;
							}
						}
					}
					else if($1->getIDType() == "ARA"){
						if($3->getIDType() == "VAR"){			
							if($3->ints[0] != 0)temp->ints[0]=$1->ints[$1->getAraIndex()] / $3->ints[0];
							else{
								temp->ints[0]=numeric_limits<int>::max();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;
								semErrors++;
							}
						}
						else if($3->getIDType() == "ARA"){
							if($3->ints[$3->getAraIndex()] != 0)
							{
								temp->ints[0]=$1->ints[$1->getAraIndex()] / $3->ints[$3->getAraIndex()];
							}
							else{
								temp->floats[0]=numeric_limits<int>::max();
							errorFile << "Error at line " << lCount <<" : Divide by zero"<<endl << endl;	
								semErrors++;
							}
						}
					}
					$$ = temp;
				}
			}
			else if(mulop == "%"){
				SymbolInfo* temp = new SymbolInfo("INT");
				temp->setIDType("VAR");
				if($1->getVarType() == "FLOAT" || $3->getVarType() == "FLOAT"){
					errorFile << "Error at line " << lCount <<" : Unsuported operand for mod operator"<<endl << endl;
					semErrors++;
				}
				else if($1->getVarType() == "INT" && $3->getVarType() == "INT"){
					if($1->getIDType() == "VAR"){
						if($3->getIDType() == "VAR")temp->ints[0]=($1->ints[0])%($3->ints[0]);
						else temp->ints[0]= ($1->ints[0])%($3->ints[$3->getAraIndex()]);					
					}
					else{
						if($3->getIDType() == "VAR")temp->ints[0]=($1->ints[$1->getAraIndex()])%($3->ints[0]);
						else temp->ints[0]=($1->ints[$1->getAraIndex()])%($3->ints[$3->getAraIndex()]);					
					}
				}
				$$ = temp;
			}
		}
     	;

unary_expression: ADDOP unary_expression  
				{
					logFile << "Line " << lCount << " : unary_expression : ADDOP unary_expression\n"<< endl;
					if($1->getName() == "-"){
						if($2->getVarType() == "VAR"){
							$2->ints[0] = (-1)*($2->ints[0]);
						}
						else if($2->getVarType() == "ARA"){
							$2->ints[$2->getAraIndex()] = (-1)*($2->ints[$2->getAraIndex()]);
						}
					}
					$$ = $2;
				}
		 		| NOT unary_expression 
				{
					logFile << "Line " << lCount << " : unary_expression : NOT unary_expression\n"<< endl;
					SymbolInfo* temp = new SymbolInfo("INT");
					temp->setIDType("VAR");
					int value;
					if($2->getVarType() == "INT"){
						if($2->getIDType() == "VAR") value = $2->ints[0];
						else if($2->getIDType() == "ARA")value = $2->ints[$2->getAraIndex()];
					}
					else if($2->getVarType() == "FLOAT"){
						if($2->getIDType() == "VAR") value = (int)$2->floats[0];
						else if($2->getIDType() == "ARA") value = (int)$2->floats[$2->getAraIndex()];
					} 
					if(value != 0) value = 0;
					else value = 1;

					temp->ints[0]=value;
					$$=temp;
				}
		 		| factor 
				{
					logFile << "Line " << lCount << " : unary_expression : factor\n"<< endl;
					$$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0);
					//logFile << "At Factor, value " << $$->floats[0] << " Type = " << $$->getVarType() << endl;
				}
		 		;
	
factor	: variable{logFile << "Line " << lCount << " : factor : variable\n"<< endl;} 
		| ID LPAREN argument_list RPAREN
		{

		logFile << "Line " << lCount << " : factor : ID LPAREN argument_list RPAREN\n";
		logFile << $1->getName() << endl << endl;
		SymbolInfo *temp=new SymbolInfo();
			temp = table.lookUp($1->getName(), "FUNC");
			if(temp == NULL){
				errorFile << "Error at line " << lCount <<" : Function " <<$1->getName() <<" doesn't exist"<<endl << endl;
			}
			else{
				if(temp->getFuncRet() == "VOID"){
					errorFile << "Error at line " << lCount <<" : Function " <<$1->getName() <<" returns void"<<endl << endl;
				} 
				else{ 
					SymbolInfo *temp2 = new SymbolInfo($1->getFuncRet());
					if(temp2->getVarType() == "INT")temp2->ints[0] = 0;
					else if(temp2->getVarType() == "FLOAT")temp2->floats[0] = 0;
					else if(temp2->getVarType() == "CHAR")temp2->chars[0] = '\0';
					$$ = temp2;
				}
			}
		}
		| LPAREN expression RPAREN
		{
			logFile << "Line " << lCount << " : factor : LPAREN expression RPAREN\n"<< endl;
			$$ = $2;
		}
		| CONST_INT 
		{ 
			logFile << "Line " << lCount << " : factor : CONST_INT\n";
			logFile << $1->getName() << endl << endl;
			$1->setVarType("INT");			
			$1->ints[0]= atoi($1->getName().c_str());
			$1->setIDType("VAR");
			$$ = $1;	
			
		} 
		| CONST_FLOAT 
		{
			logFile << "Line " << lCount << " : factor : CONST_FLOAT\n";
			logFile << $1->getName() << endl << endl;
			$1->setVarType("FLOAT");
			$1->floats[0]= atof($1->getName().c_str());
			$1->setIDType("VAR");
			$$ = $1;
			//logFile <<"AT CONST_FLOAT , value" <<  $$->floats[0]<< " Type = " << $$->getVarType() << endl;
		}
		| CONST_CHAR
		{
			logFile << "Line " << lCount << " : factor : CONST_CHAR\n";
			logFile << $1->getName() << endl << endl;
			///$1->setVarType("CHA");
			//$1->chars[0]= $1->getName()[0]);
			$$ = $1;
		}
		| variable INCOP
		{
			//logFile << "Line " << lCount << " : factor : variable INCOP\n"<< endl;
			if($1->getIDType() == "ARA"){
				if($1->getVarType() == "INT"){
					$1->ints[$1->getAraIndex()] = $1->ints[$1->getAraIndex()]+1; 
				}
				else if($1->getVarType() == "FLOAT"){
					$1->floats[$1->getAraIndex()] = $1->floats[$1->getAraIndex()]+1.0; 
				}			
			}
			else if($1->getIDType() == "VAR"){
				if($1->getVarType() == "INT"){
					$1->ints[0] = $1->ints[0]+1; 
				}
				else if($1->getVarType() == "FLOAT"){
					$1->floats[0] = $1->floats[0]+1.0; 
				}					
			}
			$$ = $1;
		} 
		| variable DECOP
		{
			logFile << "Line " << lCount << " : factor : variable DECOP\n"<< endl;
			if($1->getIDType() == "ARA"){
				if($1->getVarType() == "INT"){
					$1->ints[$1->getAraIndex()] = $1->ints[$1->getAraIndex()]-1; 
				}
				else if($1->getVarType() == "FLOAT"){
					$1->floats[$1->getAraIndex()] = $1->floats[$1->getAraIndex()]-1.0; 
				}			
			}
			else if($1->getIDType() == "VAR"){
				if($1->getVarType() == "INT"){
					$1->ints[0] = $1->ints[0]-1; 
				}
				else if($1->getVarType() == "FLOAT"){
					$1->floats[0] = $1->floats[0]-1.0; 
				}					
			}
			$$ = $1;
		}
		;
	
argument_list	: arguments{logFile << "Line " << lCount << " : argument_list : arguments\n"<< endl;}
				|
				;

arguments	:arguments COMMA logic_expression {logFile << "Line " << lCount << " : arguments : arguments COMMA logic_expression\n"<< endl;}
			|logic_expression{logFile << "Line " << lCount << " : arguments : logic_expression\n"<< endl;}
			;
 

%%
int main(int argc,char *argv[])
{

	if((yyin=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logFile.open("1405045_log.txt");
	errorFile.open("1405045_errors.txt");
	
	//	logFile << "Kichu" << endl;
	//yyin=fp;
	yyparse();
	
	logFile << "Total Lines : " << lCount << endl << endl; 
	logFile << "Total Errors : " << semErrors << endl;
	logFile.close();
	errorFile.close();

	return 0;
}

