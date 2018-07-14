%{
#ifndef SYMTABLE
#define SYMTABLE
#include "SynBase.h"
#endif // SYMTABLE

%}
%union{
SymbolInfo* symbolValue;
}

%token IF FOR DO INT FLOAT VOID SWITCH DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE
%token INCOP DECOP ASSIGNOP NOT
%token LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON
%token PRINTLN
%token STRING

%token <symbolValue>ID
%token <symbolValue>CONST_INT
%token <symbolValue>CONST_FLOAT
%token <symbolValue>CONST_CHAR
%token <symbolValue>ADDOP
%token <symbolValue>MULOP
%token <symbolValue>LOGICOP
%token <symbolValue>RELOP

%token <symbolValue>BITOP

%type <symbolValue>type_specifier expression logic_expression rel_expression simple_expression term unary_expression factor variable program unit var_declaration func_declaration func_definition parameter_list compound_statement declaration_list statements statement expression_statement argument_list arguments

%nonassoc second_prec
%nonassoc ELSE
%error-verbose


%%


start: program
		{
				if(!syntaxErrors && !err_count_lex){
						addCode($1->code);
						writeASM();
				}
				delete $1;
		}
	;

program: program unit
		{
			$$ = new SymbolInfo("",TEMPORARY);
			$$->code = $1->code + $2->code;
			delete $1,$2;
		}
	| unit
	;

unit: var_declaration
     | func_declaration
     | func_definition
     ;

func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
			{
				insertFunc($2,$1);
				clearFunctionArgs();
			}
		| type_specifier ID LPAREN RPAREN SEMICOLON	{	insertFunc($2,$1);	}
		| type_specifier ID LPAREN parameter_list RPAREN error {	clearFunctionArgs();	}
		| type_specifier ID LPAREN RPAREN error{	printErrorLog("; missing");	}
		;

func_definition: type_specifier ID LPAREN parameter_list RPAREN{	addFuncDef($2,$1);} compound_statement
			{
				$$ = new SymbolInfo("",TEMPORARY);
				$$->code = funcBodyCode($2,$7) + NEWLINE_ASM;
				currentFunc = nullptr;
			}
		| type_specifier ID LPAREN RPAREN {	addFuncDef($2,$1);} compound_statement
			{
				$$ = new SymbolInfo("",TEMPORARY);
				$$->code = funcBodyCode($2,$6) + NEWLINE_ASM;
				currentFunc = nullptr;
			}
 		;


parameter_list: parameter_list COMMA type_specifier ID	{	addIDenArgtoParamList($4);}
		| parameter_list COMMA type_specifier	{	addTypeArgtoParamList(variableType);}
 		| type_specifier ID	{	addIDenArgtoParamList($2);}
		| type_specifier	{	addTypeArgtoParamList(variableType);}
		| parameter_list COMMA error ID {	printErrorLog("type specifier missing in parameters");}
		| error ID {	}
		;


compound_statement: LCURL {	enterFuncScope();} statements RCURL	{	$$ = $3;
					exitFuncScope();	}
 		  | LCURL {	enterFuncScope();} RCURL	{	exitFuncScope();	}
 		  ;


var_declaration: type_specifier declaration_list SEMICOLON
			| type_specifier declaration_list error{
				printErrorLog("; missing");}
		 ;

type_specifier: INT	{	$$ = getType(INT_TYPE);	}
 		| FLOAT	{	$$ = getType(FLOAT_TYPE);	}
 		| VOID	{	$$ = getType(VOID_TYPE);	}
 		;

declaration_list: declaration_list COMMA ID	{	insertVar($3);	}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD	{	insertArr($3,$5);	}
 		  | ID	{	insertVar($1);	}
 		  | ID LTHIRD CONST_INT RTHIRD	{	insertArr($1,$3);	}
			| ID LTHIRD error RTHIRD{
				printErrorLog("Array size must be provided with constant integer");	}
			| declaration_list COMMA ID LTHIRD error RTHIRD	{
				printErrorLog("Array size must be provided with constant integer");	}
 		  ;

statements: statement	{	$1->code += NEWLINE_ASM;	}
	   | statements statement {
			 	$$ = new SymbolInfo("",TEMPORARY);
			 	$$->code = $1->code + $2->code;
				$$->code += NEWLINE_ASM;
		 	}
	   ;

statement: var_declaration
	  | expression_statement
	  | compound_statement
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement	{	$$->code = forLoopCode($3,$4,$5,$7);deleteTemp($3,$4,$5,$7);}
	  | IF LPAREN expression RPAREN statement %prec second_prec	{	$$->code = ifElseCode($3,$5);}
	  | IF LPAREN expression RPAREN statement ELSE statement	{	$$->code = ifElseCode($3,$5,$7);deleteTemp($3,$5,$7);	}
	  | WHILE LPAREN expression RPAREN statement	{	$$->code = whlLoopCode($3,$5);deleteTemp($3,$5);	}
	  | PRINTLN LPAREN ID RPAREN SEMICOLON	{	$$->code += printVarValue($3);deleteTemp(nullptr);	}
		| RETURN expression SEMICOLON	{
				checkReturnType($2);
				$$->code += returnExpCode($2);
				deleteTemp($2);
			}
		| RETURN expression error	{	printErrorLog("; missing before return");	}
		| error ELSE {	printErrorLog("else without if");} statement	{	}
		| IF invalid_condition statement %prec second_prec {	}
		| IF invalid_condition statement ELSE statement {	}
		;
invalid_condition: LPAREN error RPAREN {	printErrorLog("invalid conditional expression");};

expression_statement: SEMICOLON	{	}
			| expression SEMICOLON {
					tempCount = 0;
					pTempCount = 0;
					$$ = new SymbolInfo(*$1);
					$$->setType(TEMPORARY);
					$$->code = $1->code + NEWLINE_ASM;
					deleteTemp($1);
				}
			| expression error {	printErrorLog("; missing after expression");}
			;

variable: ID	{	$$ = getVariable($1);	}
	 | ID LTHIRD expression RTHIRD	{	$$ = getArrIndexVar($1,$3);	}
	 ;

expression: logic_expression
	   | variable ASSIGNOP logic_expression	{	$$ = getAssignExpVal($1,$3);}
	   ;

logic_expression: rel_expression	{	$$->code += NEWLINE_ASM;}
		 | rel_expression LOGICOP rel_expression
		 		{
					$$ = getLogicOpVal($1,$3,$2);
					$$->code += NEWLINE_ASM;
				}
		 ;

rel_expression: simple_expression
		| simple_expression RELOP simple_expression	{	$$ = getReltnOpVal($1,$3,$2);	}
		;

simple_expression: term
		  | simple_expression ADDOP term {	$$ = getAddtnOpVal($1,$3,$2);	}
		  ;

term:	unary_expression
     | term MULOP unary_expression {	$$ = getMultpOpVal($1,$3,$2);	}
     ;

unary_expression: ADDOP unary_expression	{	$$ = getUniAddOpVal($2,$1);	}
		 | NOT unary_expression	{	$$ = getNotOpVal($2);}
		 | factor
		 ;

factor: variable
	| ID LPAREN argument_list RPAREN {	$$=getFuncCallValue($1,$3);}
	| LPAREN expression RPAREN { $$=$2; }
	| CONST_INT	{	$$ = getConstVal($1,INT_TYPE);}
	| CONST_FLOAT	{	$$ = getConstVal($1,FLOAT_TYPE);}
	| variable INCOP {	$$ = getIncOpVal($1,"++");}
	| variable DECOP	{	$$ = getIncOpVal($1,"--");}
	| ID LPAREN argument_list error {	printErrorLog("right parentheses missing");clearFunctionArgs();}
	| LPAREN expression error {	printErrorLog("right parentheses missing");}
	;

argument_list: arguments {}
			  | {}
			  ;

arguments: arguments COMMA logic_expression
					{
						$$ = new SymbolInfo("");
						$$->code = $1->code + $3->code;
						argsFunc.push_back(*$3);
						argsType.push_back($3->getVarType());
						deleteTemp($1,$3);
					}
	      | logic_expression
					{
						argsFunc.push_back(*$1);
						argsType.push_back($1->getVarType());
					}
				| arguments COMMA error {	printErrorLog("arguments list not terminated");	}
	      ;

%%

int main(int argc,char *argv[])
{
	if((yyin=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	/* logFile.open("log.txt"); */
	errorFile.open("log.txt");

	asmFile.open("code.asm");
	optAsmFile.open("optimized-code.asm");

	yyparse();
	logFile << "Total Lines : " << line_count << std::endl << std::endl;
	errorFile << "Total Syntax/Semantic Errors : " << syntaxErrors << std::endl;
	errorFile << "Total Lexical Errors : " << err_count_lex << std::endl;
	errorFile << "Total Warning : " << warnings << std::endl;

	/* logFile.close(); */
	errorFile.close();

	/* table.printAllScope(); */

	return 0;
}
