%{
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include "SymTable.h"
//#include "SymbolInfo.h"

//#define YYSTYPE SymbolInfo*

using namespace std;

extern int yylex();
void yyerror(const char *s);
extern FILE *yyin;
extern int lCount;
extern int errCount;


int labelCount=0;
int tempCount=0;
int pTempCount = 0;
int maxTemp = 0;


string variable_type;
SymbolTable table(13);
int IDargs = 0;
vector<string> args; 
extern int lCount;
int semErrors;
vector<SymbolInfo> params;
string return_label;

vector<string> variables;
vector<string> arrays;
vector<int> arraySizes;

ofstream logFile, errorFile, asmCode;

char *newLabel()
{
	char *lb= new char[4];
	strcpy(lb,"L");
	char b[3];
	sprintf(b,"%d", labelCount);
	labelCount++;
	strcat(lb,b);
	return lb;
}

char *newTemp()
{
	char *t= new char[4];
	strcpy(t,"t");
	char b[3];
	sprintf(b,"%d", tempCount);
	tempCount++;
	if(maxTemp < tempCount) maxTemp = tempCount;
	strcat(t,b);
	return t;
}

char *newPTemp()
{
	char *pt= new char[4];
	strcpy(pt,"p");
	char b[3];
	sprintf(b,"%d", pTempCount);
	pTempCount++;
	strcat(pt,b);
	return pt;
}

//SymbolTable *table= new SymbolTable(31);

%}

%error-verbose`

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

%type <symVal>type_specifier expression logic_expression rel_expression simple_expression term unary_expression factor variable program unit var_declaration func_declaration func_definition parameter_list compound_statement declaration_list statements statement expression_statement argument_list arguments

%nonassoc THEN
%nonassoc ELSE
%%

/*
Program : compound_statement {
	ofstream fout;
	fout.open("code.asm");
	fout << $1->code;
}
*/

start : program
	{
		//write your code in this block in all the similar blocks below
		if(!errCount && !semErrors){
			$1->code+="\n\nDECIMAL_OUT PROC NEAR\n\n\tpush ax\n\tpush bx\n\tpush cx\n\tpush dx\n\tor ax,ax\n \tjge enddif\n\tpush ax\n\tmov dl,'-'\n\tmov ah,2\n\tint 21h\n\tpop ax\n\tneg ax\nenddif:\n\txor cx,cx\n\tmov bx,10d\nrepeat:\n\txor dx,dx\n\tdiv bx\n\t push dx\n\tinc cx\n\tor ax,ax\n\tjne repeat\n\tmov ah,2\nprint_loop:\n\tpop dx\n\tor dl,30h\n\tint 21h\n\tloop print_loop\n\tpop dx\n\tpop cx\n\tpop bx\n\tpop ax\n\tret\n\nDECIMAL_OUT ENDP\n";
			ofstream fout;
			fout.open("code.asm");
			fout << ".model small\n.stack 100h\n\n.data\n" ;
			for(int i = 0; i<variables.size() ; i++){
				fout << variables[i] << " dw ?\n";			
			}

			for(int i = 0 ; i< arrays.size() ; i++){
				fout << arrays[i] << " dw " << arraySizes[i] << " dup(?)\n";
			}

			fout << "\n.code \n"; 
			fout << $1->code;
			logFile << "Line " << lCount << " : start : program\n"<< endl;
		}
	}
	;

program : program unit
	{
		logFile << "Line " << lCount << " : program : program unit\n"<< endl;
		$$ = $1;
		$$->code += $2->code;
	} 
	| 
	unit
	{
		logFile << "Line " << lCount << " : program : unit\n"<< endl;
		$$ = $1;
	}
	;
	
unit : var_declaration
	{
		logFile << "Line " << lCount << " : unit : var_declaration\n"<< endl;
		$$ = $1;
	}
     	| 
     	func_declaration
     	{
			logFile << "Line " << lCount << " : unit : func_declaration\n"<< endl;
			$$ = $1;
     	}
     	| 
     	func_definition
     	{
			logFile << "Line " << lCount << " : unit : func_definition\n"<< endl;
			$$ = $1;
     	}
     	;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
			{
				logFile << "Line " << lCount << " : func_declaration : 	type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n" ;
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
			}
		 	|type_specifier ID LPAREN parameter_list RPAREN error
			{
				errorFile << "Error at line " << lCount << " ; missing" << endl << endl;
				semErrors++;
			}
		 	;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN 
				{
				
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
					temp->setIDType("FUNC");
					temp->setFuncRet($1->getVarType());
					//
					for(int i = 0; i<args.size(); i++){
						temp->ParamList.push_back(args[i]);					
					}
					temp->setFuncDefined();
				}
				
				
	
				}
			compound_statement
			{
				logFile << "Line " << lCount << " : func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement\n" ;
				logFile << $2->getName() << endl << endl;
				SymbolInfo * func = new SymbolInfo();				
				$$ = func;
				$$->code += $2->getName() + " PROC NEAR\n\n";
				$$->code += $7->code;
				if($2->getName()!="main"){
					$$->code+=string(return_label)+":\n";
				}
				if(args.size()!=0){
					$$->code+="\tpop bp\n";
				}
				if($2->getName()!="main"){
					$$->code+="\tret ";
				}
		
				int p=args.size()*2;
				if(p){
					string Result;       

					ostringstream convert;  
	
					convert << p;    

					Result = convert.str(); 
					$$->code+=Result+"\n";
				}
				$$->code+="\n";
				if($2->getName()=="main"){
					$$->code+="\tmov ah,4ch\n\tint 21h\n";
				}
				$$->code += "\n" + $2->getName() + " ENDP\n\n";
				args.clear();
				IDargs = 0;
				return_label = "";
			}
 		 	;
 		 
parameter_list  : parameter_list COMMA type_specifier ID
				{
					logFile << "Line " << lCount << " : parameter_list  : parameter_list COMMA type_specifier ID\n" ;
					logFile << $4->getName() << endl << endl;
					args.push_back(variable_type);//changed from $3->getVarType()
					IDargs++;
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
			logFile << "Line " << lCount << " : parameter_list  : type_specifier ID\n" ;
			logFile << $2->getName() << endl << endl;
			args.push_back(variable_type);//$1->getVarType()
			IDargs++;
			$2->setIDType("VAR");
			$2->setVarType(variable_type);//$1->getVarType()
			params.push_back(*$2);
				

		}
 		| type_specifier
		{
			logFile << "Line " << lCount << " : parameter_list  : type_specifier\n" ;
			args.push_back(variable_type);
		}
 		|{}
 		;

compound_statement	: LCURL
					{
						table.enterScope(); 
						for(int i = 0; i<params.size(); i++) table.Insert(params[i]);
						params.clear();					
					}statements RCURL
					{	
						table.exitScope();
						$$=$3;
						logFile << "Line " << lCount << " : compound_statement : LCURL statements RCURL\n"<< endl;
					}
					| LCURL RCURL
					{
						$$=new SymbolInfo("compound_statement","dummy");
						logFile << "Line " << lCount << " : compound_statement : LCURL RCURL\n"<< endl;
					}
					;

var_declaration : type_specifier declaration_list SEMICOLON
				{
					logFile << "Line " << lCount << " : var_declaration : type_specifier declaration_list SEMICOLON\n"<< endl;
				}
				|type_specifier declaration_list error
				{
						errorFile << "Error at line " << lCount << " ; missing" << endl << endl;
						semErrors++;
				} 		 		
				;
 		 
type_specifier	: INT
				{
					logFile << "Line " << lCount << " : type_specifier	: INT\n"<< endl; 
					SymbolInfo* s= new SymbolInfo("INT");
					variable_type = "INT" ;
					$$ = s;
				}
 				| FLOAT
				{	
					logFile << "Line " << lCount << " : type_specifier	: FLOAT\n"<< endl;
					SymbolInfo* s= new SymbolInfo("FLOAT");
					variable_type = "FLOAT" ;
					$$ = s;
				}
 				| VOID
				{
					logFile << "Line " << lCount << " : type_specifier	: VOID\n"<< endl;
					SymbolInfo* s= new SymbolInfo("VOID");
					variable_type = "VOID" ;
					$$ = s;
				}
 				;
 		
declaration_list : 	declaration_list COMMA ID
					{
						logFile << "Line " << lCount << " : declaration_list : 	declaration_list COMMA ID\n" ;
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
								//data.push_back(*temp2);
								//table.printAll(logFile);
							}
						}
					}
 					| declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
					{
						logFile << "Line " << lCount << " : declaration_list : 	declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n" ;
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
								//data.push_back(*temp2);		
								//cout << temp2->getName() << endl;			
								//table.printAll(logFile);
							}
						}

					}
 					| ID
					{
						logFile << "Line " << lCount << " : declaration_list :	ID\n" ;
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
								//data.push_back(*temp2);
								//cout << temp2->getName() << endl;
								//table.printAll(logFile);		
							}
						}
					}
					| ID LTHIRD CONST_INT RTHIRD
					{
						logFile << "Line " << lCount << " : declaration_list :	ID LTHIRD CONST_INT RTHIRD\n" ;
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
								//data.push_back(*temp2);
								//cout << temp2->getName() << endl;
								//table.printAll(logFile);			
							}
						}
					}						
					;


statements : statement {
				$$=$1;
				logFile << "Line " << lCount << " : statements : statement\n"<< endl;
			}
	       | statements statement {
				$$=$1;
				$$->code += $2->code;
				delete $2;
				logFile << "Line " << lCount << " : statements : statements statement\n"<< endl;
			}
	       ;


statement 	: 	var_declaration
			{
				logFile << "Line " << lCount << " : statement : var_declaration\n"<< endl;
			}
	  		|	expression_statement {
					logFile << "Line " << lCount << " : statement : expression_statement\n"<< endl;
					$$=$1;
				}
			| 	compound_statement {
					logFile << "Line " << lCount << " : statement : compound_statement\n"<< endl;
					$$=$1;
				}
			|	FOR LPAREN expression_statement expression_statement expression RPAREN statement 				{			
				logFile << "Line " << lCount << " : statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n"<< endl;
					/*
						$3's code at first, which is already done by assigning $$=$3
						create two labels and append one of them in $$->code
						compare $4's symbol with 0
						if equal jump to 2nd label
						append $7's code
						append $5's code
						append the second label in the code
					*/

					$$ = $3;
					char *label1 = newLabel();
					char *label2 = newLabel();
					$$->code += string(label1) + ":\n";
					$$->code+=$4->code;
					$$->code+="\tmov ax , "+$4->getName()+"\n";
					$$->code+="\tcmp ax , 0\n";
					$$->code+="\tje "+string(label2)+"\n";
					$$->code+=$7->code;
					$$->code+=$5->code;
					$$->code+="\tjmp "+string(label1)+"\n";
					$$->code+=string(label2)+":\n";
		
				}
			|	IF LPAREN expression RPAREN statement %prec THEN {
					logFile << "Line " << lCount << " : statement : IF LPAREN expression RPAREN statement\n"<< endl;
					$$=$3;
					
					char *label=newLabel();
					$$->code+="\tmov ax, "+$3->getName()+"\n";
					$$->code+="\tcmp ax, 0\n";
					$$->code+="\tje "+string(label)+"\n";
					$$->code+=$5->code;
					$$->code+=string(label)+":\n";
					
					$$->setName("if");//not necessary
				}
			|	IF LPAREN expression RPAREN statement ELSE statement {
					logFile << "Line " << lCount << " : statement : IF LPAREN expression RPAREN statement ELSE statement\n"<< endl;
					$$=$3;
					//similar to if part
					char *elselabel=newLabel();
					char *exitlabel=newLabel();
					$$->code+="\tmov ax,"+$3->getName()+"\n";
					$$->code+="\tcmp ax,0\n";
					$$->code+="\tje "+string(elselabel)+"\n";
					$$->code+=$5->code;
					$$->code+="\tjmp "+string(exitlabel)+"\n";
					$$->code+=string(elselabel)+":\n";
					$$->code+=$7->code;
					$$->code+=string(exitlabel)+":\n";
				}
			|	WHILE LPAREN expression RPAREN statement {
					logFile << "Line " << lCount << " : statement : WHILE LPAREN expression RPAREN statement\n"<< endl;
					$$ = new SymbolInfo();
					char * label = newLabel();
					char * exit = newLabel();
					$$->code = string(label) + ":\n"; 
					$$->code+=$3->code;
					$$->code+="\tmov ax , "+$3->getName()+"\n";
					$$->code+="\tcmp ax , 0\n";
					$$->code+="\tje "+string(exit)+"\n";
					$$->code+=$5->code;
					$$->code+="\tjmp "+string(label)+"\n";
					$$->code+=string(exit)+":\n";
			
				}
			|	PRINTLN LPAREN ID RPAREN SEMICOLON {
					// write code for printing an ID. You may assume that ID is an integer variable.
					$$=new SymbolInfo("println","nonterminal");
					logFile << "Line " << lCount << " : statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n"<< endl;
					$$->code += "\tmov ax, " + $3->getName() +"\n";
					$$->code += "\tcall DECIMAL_OUT\n";
				}
			| PRINTLN LPAREN ID RPAREN error
			{
				errorFile << "Error at line " << lCount << " ; missing" << endl << endl;
				semErrors++;
			}
			| 	RETURN expression SEMICOLON {
					// write code for return.
					$$=$2;
					$$->code+="\tmov dx,"+$2->getName()+"\n";
					$$->code+="\tjmp   "+string(return_label)+"\n";
		
					logFile << "Line " << lCount << " : statement : RETURN expression SEMICOLON\n"<< endl;
				}
			| RETURN expression error
			{
				errorFile << "Error at line " << lCount << " ; missing" << endl << endl;
				semErrors++;
			}
			;
		
expression_statement	: SEMICOLON	{
							logFile << "Line " << lCount << " : expression_statement : SEMICOLON\n"<< endl;
							$$=new SymbolInfo(";","SEMICOLON");
							$$->code="";
							tempCount = 0;
						}			
					| expression SEMICOLON {
							logFile << "Line " << lCount << " : expression_statement : expression SEMICOLON\n"<< endl;
							$$=$1;
							tempCount = 0;
						}		
					| expression error
						{
							errorFile << "Error at line " << lCount << " ; missing" << endl << endl;
							semErrors++;
						} 		 		
						;
						
variable	: ID {
				logFile << "Line " << lCount << " : variable : ID\n" ;
				logFile << $1->getName() << endl << endl;
				SymbolInfo* temp = table.lookUp($1->getName(),"VAR");
				if(temp == NULL){
					//logFile << "Variable " << $1->getName() << " doesn't exist" << endl;
					errorFile << "Error at line " << lCount << " : " << $1->getName() << " doesn't exist" <<  endl << endl;					
					semErrors++;
				}
				else{
					$$= new SymbolInfo($1);
					$$->code="";
					$$->setName($$->getName()+to_string(table.scopeNum));
					variables.push_back($$->getName()+to_string(table.scopeNum));
					$$->setType("notarray");
				}
		}		
		| ID LTHIRD expression RTHIRD {
				logFile << "Line " << lCount << " : variable : ID LTHIRD expression RTHIRD\n" ;
				logFile << $1->getName() << endl << endl;
				SymbolInfo* temp = table.lookUp($1->getName(),"ARA");
				if(temp == NULL){
				errorFile << "Error at line " << lCount << " : " <<$1->getName() << " doesn't exist" <<  endl << endl;					
					semErrors++;				
				}
				else{
					
					$$= new SymbolInfo($1);
					$$->setType("array");
					$$->setName($$->getName()+to_string(table.scopeNum));
					arrays.push_back($$->getName()+to_string(table.scopeNum));
					arraySizes.push_back($1->getAraSize());
					$$->code=$3->code ;
					$$->code += "\tmov bx, " +$3->getName() +"\n";
					$$->code += "\tadd bx, bx\n";
					
					delete $3;
			}
		}	
		;
			
expression : logic_expression {
			$$= $1;
			logFile << "Line " << lCount << " : expression : logic_expression\n"<< endl;
		}	
		| variable ASSIGNOP logic_expression {
				logFile << "Line " << lCount << " : expression : variable ASSIGNOP logic_expression\n"<< endl;
				$$=$1;
				$$->code=$3->code+$1->code;
				$$->code+="\tmov ax, "+$3->getName()+"\n";
				if($$->getType()=="notarray"){ 
					$$->code+= "\tmov "+$1->getName()+", ax\n";
				}
				
				else{
					$$->code+= "\tmov  "+$1->getName()+"[bx], ax\n";
				}
				delete $3;
			}	
		;
			
logic_expression : rel_expression {
					logFile << "Line " << lCount << " : logic_expression : rel_expression\n"<< endl;
					$$= $1;		
				}	
		| rel_expression LOGICOP rel_expression {
					logFile << "Line " << lCount << " : logic_expression : rel_expression LOGICOP rel_expression\n"<< endl;
					$$=$1;
					$$->code+=$3->code;
					char * label1 = newLabel();
					char * label2 = newLabel();
					char * temp = newPTemp();
					if($2->getName()=="&&"){
						/* 
						Check whether both operands value is 1. If both are one set value of a temporary variable to 1
						otherwise 0
						*/
						$$->code += "\tmov ax , " + $1->getName() +"\n";
						$$->code += "\tcmp ax , 0\n";
				 		$$->code += "\tje " + string(label1) +"\n";
						$$->code += "\tmov ax , " + $3->getName() +"\n";
						$$->code += "\tcmp ax , 0\n";
						$$->code += "\tje " + string(label1) +"\n";
						$$->code += "\tmov " + string(temp) + " , 1\n";
						$$->code += "\tjmp " + string(label2) + "\n";
						$$->code += string(label1) + ":\n" ;
						$$->code += "\tmov " + string(temp) + ", 0\n";
						$$->code += string(label2) + ":\n";
						$$->setName(temp);
						
					}
					else if($2->getName()=="||"){
						$$->code += "\tmov ax , " + $1->getName() +"\n";
						$$->code += "\tcmp ax , 0\n";
				 		$$->code += "\tjne " + string(label1) +"\n";
						$$->code += "\tmov ax , " + $3->getName() +"\n";
						$$->code += "\tcmp ax , 0\n";
						$$->code += "\tjne " + string(label1) +"\n";
						$$->code += "\tmov " + string(temp) + " , 0\n";
						$$->code += "\tjmp " + string(label2) + "\n";
						$$->code += string(label1) + ":\n" ;
						$$->code += "\tmov " + string(temp) + ", 1\n";
						$$->code += string(label2) + ":\n";
						$$->setName(temp);
						
					}
					delete $3;
				}	
			;
			
rel_expression	: simple_expression {
				logFile << "Line " << lCount << " : rel_expression : simple_expression\n"<< endl;
				$$= $1;
			}	
		| simple_expression RELOP simple_expression {
				logFile << "Line " << lCount << " : rel_expression : simple_expression RELOP simple_expression\n"<< endl;
				$$=$1;
				$$->code+=$3->code;
				$$->code+="\tmov ax, " + $1->getName()+"\n";
				$$->code+="\tcmp ax, " + $3->getName()+"\n";
				char *temp=newPTemp();
				char *label1=newLabel();
				char *label2=newLabel();
				if($2->getName()=="<"){
					$$->code+="\tjl " + string(label1)+"\n";
				}
				else if($2->getName()=="<="){
					$$->code+="\tjle " + string(label1)+"\n";
				}
				else if($2->getName()==">"){
					$$->code+="\tjg " + string(label1)+"\n";
				}
				else if($2->getName()==">="){
					$$->code+="\tjge " + string(label1)+"\n";
				}
				else if($2->getName()=="=="){
					$$->code+="\tje " + string(label1)+"\n";
				}
				else if($2->getName()=="!="){
					$$->code+="\tjne " + string(label1)+"\n";
				}
				
				$$->code+="\tmov "+string(temp) +", 0\n";
				$$->code+="\tjmp "+string(label2) +"\n";
				$$->code+=string(label1)+":\n";
				$$->code+= "\tmov "+string(temp)+", 1\n";
				$$->code+=string(label2)+":\n";
				$$->setName(temp);
				delete $3;
			}	
		;
				
simple_expression : term {
				logFile << "Line " << lCount << " : simple_expression : term\n"<< endl;
				$$= $1;
			}
		| simple_expression ADDOP term {
				logFile << "Line " << lCount << " : simple_expression : simple_expression ADDOP term\n"<< endl;
				$$=$1;
				$$->code+=$3->code;
				
				// move one of the operands to a register, perform addition or subtraction with the other operand and move the result in a temporary variable  
				
				if($2->getName()=="+"){
					char* temp = newTemp();
					$$->code += "\tmov ax, " + $1->getName() + "\n";
					$$->code += "\tadd ax, " + $3->getName() + "\n";
					$$->code += "\tmov " + string(temp) +" , ax\n";
					$$->setName(string(temp));
				}
				else if($2->getName() == "-"){
					char* temp = newTemp();
					$$->code += "\tmov ax, " + $1->getName() + "\n";
					$$->code += "\tsub ax, " + $3->getName() + "\n";
					$$->code += "\tmov " + string(temp) +" , ax\n";
					$$->setName(string(temp));
				}
				delete $3;
				cout << endl;
			}
				;
				
term :	unary_expression 
		{
			logFile << "Line " << lCount << " : term : unary_expression\n"<< endl;						
			$$= $1;
		}
	 	|term MULOP unary_expression 
		{
			logFile << "Line " << lCount << " : term : term MULOP unary_expression\n"<< endl;
			$$=$1;
			$$->code += $3->code;
			$$->code += "\tmov ax, "+ $1->getName()+"\n";
			$$->code += "\tmov bx, "+ $3->getName() +"\n";
			char *temp=newTemp();
			if($2->getName()=="*"){
				$$->code += "\tmul bx\n";
				$$->code += "\tmov "+ string(temp) + ", ax\n";
			}
			else if($2->getName()=="/"){
				// clear dx, perform 'div bx' and mov ax to temp
				$$->code += "\txor dx , dx\n";
				$$->code += "\tdiv bx\n";
				$$->code += "\tmov " + string(temp) + " , ax\n";
			}
			else{
			// clear dx, perform 'div bx' and mov dx to temp
				$$->code += "\txor dx , dx\n";
				$$->code += "\tdiv bx\n";
				$$->code += "\tmov " + string(temp) + " , dx\n";
				
			}
			$$->setName(temp);
			cout << endl << $$->code << endl;
			delete $3;
		}
	 	;

unary_expression 	:	ADDOP unary_expression  
					{
						logFile << "Line " << lCount << " : unary_expression : ADDOP unary_expression\n"<< endl;
						
						if($1->getName() == "+"){
							$$=$2;
						}
						else if($1->getName() == "-")
						{
							$$ = $2;
							$$->code += "\tmov ax, " + $2->getName() + "\n";
							$$->code += "\tneg ax\n";
							$$->code += "\tmov " + $2->getName() + " , ax\n";
						}
							// Perform NEG operation if the symbol of ADDOP is '-'
					}
					|	NOT unary_expression 
					{
						logFile << "Line " << lCount << " : unary_expression : NOT unary_expression\n"<< endl;
						$$=$2;
						char *temp=newTemp();
						$$->code="\tmov ax, " + $2->getName() + "\n";
						$$->code+="\tnot ax\n";
						$$->code+="\tmov "+string(temp)+", ax";
					}
					|	factor 
					{
						logFile << "Line " << lCount << " : unary_expression : factor\n"<< endl;
						$$=$1;
					}
					;
	
factor	: variable {
			SymbolInfo * newVar = new SymbolInfo(*$1);
			
			logFile << "Line " << lCount << " : factor : variable\n"<< endl;
			if($$->getType()=="notarray"){
				
			}
			
			else{
				char *temp= newTemp();
				$$->code+="\tmov ax, " + $1->getName() + "[bx]\n";
				$$->code+= "\tmov " + string(temp) + ", ax\n";
				$$->setName(temp);
			}
			}
	| ID LPAREN argument_list RPAREN
	{
		logFile << "Line " << lCount << " : factor : ID LPAREN argument_list RPAREN\n" ;
		logFile << $1->getName() << endl << endl;
		SymbolInfo *temp=new SymbolInfo();
			temp = table.lookUp($1->getName(), "FUNC");
			if(temp == NULL){
				errorFile << "Error at line " << lCount <<" : Function " <<$1->getName() <<" doesn't exist"<<endl << endl;
			}
	}
	| LPAREN expression RPAREN 
	{
		logFile << "Line " << lCount << " : factor : LPAREN expression RPAREN\n"<< endl;
		$$= $2;
	}
	| CONST_INT 
	{
		logFile << "Line " << lCount << " : factor : CONST_INT\n" ;
		logFile << $1->getName() << endl << endl;
		$$= $1;
	}
	| CONST_FLOAT 
	{
		logFile << "Line " << lCount << " : factor : CONST_FLOAT\n" ;
		logFile << $1->getName() << endl << endl;
		$$= $1;
	}
	| variable INCOP 
	{
		logFile << "Line " << lCount << " : factor : variable INCOP\n"<< endl;
		$$=$1;
		$$->code += "\tmov ax , " + $$->getName()+ "\n";
		$$->code += "\tadd ax , 1\n";
		$$->code += "\tmov " + $$->getName() + " , ax\n";
		// perform incop depending on whether the varaible is an array or not
		
	}
	| variable DECOP
	{
		logFile << "Line " << lCount << " : factor : variable DECOP\n"<< endl;
		$$ = $1;
		
		$$->code += "\tmov ax , " + $$->getName()+ "\n";
		$$->code += "\tsub ax , 1\n";
		$$->code += "\tmov " + $$->getName() + " , ax\n";
	}
	;

argument_list	: arguments
				{
					logFile << "Line " << lCount << " : argument_list : arguments\n"<< endl;
				}
				|{}
				;

arguments	:arguments COMMA logic_expression 
			{
				logFile << "Line " << lCount << " : arguments : arguments COMMA logic_expression\n"<< endl;
			}
			|logic_expression
			{
				logFile << "Line " << lCount << " : arguments : logic_expression\n"<< endl;
			}
			;
		
%%


void yyerror(const char *s){
	cout << "Error at line no " << lCount << " : " << s << endl;
}

int main(int argc, char * argv[]){
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	logFile.open("1405045_log.txt");
	errorFile.open("1405045_errors.txt");

	yyin= fin;
	yyparse();
	
	printf("\nTotal Lines: %d\n",--lCount);
	printf("\nTotal Errors: %d\n",errCount);

	logFile.close();
	errorFile.close();
	
	printf("\n");
	return 0;
}
