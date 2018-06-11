%{
#ifndef SYMTABLE
#define SYMTABLE
#include "SynBase.h"
#endif // SYMTABLE

%}
%union{
SymbolInfo* symVal;
}

/* %token COMMENT IF ELSE FOR WHILE DO BREAK CONTINUE INT FLOAT CHAR DOUBLE VOID RETURN SWITCH CASE DEFAULT */
/* %token INCOP DECOP ASSIGNOP LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD SEMICOLON COMMA STRING NOT PRINTLN */
%token IF FOR DO INT FLOAT VOID SWITCH DEFAULT ELSE WHILE BREAK CHAR DOUBLE RETURN CASE CONTINUE
/* %token ADDOP INCOP MULOP RELOP LOGICOP BITOP ASSIGNOP NOT */
%token INCOP DECOP ASSIGNOP NOT
%token LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON
%token PRINTLN
%token STRING

%token <symVal>ID
%token <symVal>CONST_INT
%token <symVal>CONST_FLOAT
%token <symVal>CONST_CHAR
%token <symVal>ADDOP
%token <symVal>MULOP
%token <symVal>LOGICOP
%token <symVal>RELOP

%token <symVal>BITOP

%type <symVal>type_specifier expression logic_expression rel_expression simple_expression term unary_expression factor variable

%nonassoc second_precedence
%nonassoc ELSE
%error-verbose


%%



start: program
		{
				pushVal(start,popVal(program));
				printRuleLog(start,"program");
		}
	;

program: program unit
		{
			pushVal(program,popVal(program)+popVal(unit));
			printRuleLog(program,"program unit");
		}
	| unit
		{
				pushVal(program,popVal(unit));
				printRuleLog(program,"unit");
		}
	;

unit: var_declaration
		{
				pushVal(unit,popVal(var_declaration));
				printRuleLog(unit,"var_declaration");
		}
     | func_declaration
		{
				pushVal(unit,popVal(func_declaration));
				printRuleLog(unit,"func_declaration");
		}
     | func_definition
		{
				pushVal(unit,popVal(func_definition));
				printRuleLog(unit,"func_definition");
		}
     ;

func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
			{
				insertFunc($2,$1);

				pushVal(func_declaration,popVal(type_specifier)+$2->getName()+"("+popVal(parameter_list)+")"+";");
				printRuleLog(func_declaration,"type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			}
		| type_specifier ID LPAREN RPAREN SEMICOLON
			{
				insertFunc($2,$1);

				pushVal(func_declaration,popVal(type_specifier)+$2->getName()+"("+")"+";");
				printRuleLog(func_declaration,"type_specifier ID LPAREN RPAREN SEMICOLON");
			}
		|type_specifier ID LPAREN parameter_list RPAREN error
			{
				printErrorLog("; missing");
			}
		|type_specifier ID LPAREN RPAREN error
			{
				printErrorLog("; missing");
			}
		;

func_definition: type_specifier ID LPAREN parameter_list RPAREN{addFuncDef($2,$1);} compound_statement
			{
				pushVal(func_definition,popVal(type_specifier)+$2->getName()+"("+popVal(parameter_list)+")"+popVal(compound_statement));
				printRuleLog(func_definition,"type_specifier ID LPAREN parameter_list RPAREN compound_statement");
			}
		| type_specifier ID LPAREN RPAREN {addFuncDef($2,$1);} compound_statement
			{
				pushVal(func_definition,popVal(type_specifier)+$2->getName()+"("+")"+popVal(compound_statement));
				printRuleLog(func_definition,"type_specifier ID LPAREN RPAREN compound_statement");
			}
 		;


parameter_list: parameter_list COMMA type_specifier ID
			{
					addIDenArgtoParamList($4);

			   	pushVal(parameter_list,popVal(parameter_list)+","+popVal(type_specifier)+$4->getName());
					printRuleLog(parameter_list,"parameter_list COMMA type_specifier ID");
			}
		| parameter_list COMMA type_specifier
			{
					addTypeArgtoParamList(variable_type);

					pushVal(parameter_list,popVal(parameter_list)+","+popVal(type_specifier));
					printRuleLog(statements,"parameter_list COMMA type_specifier");
			}
 		| type_specifier ID
			{
					addIDenArgtoParamList($2);

			   	pushVal(parameter_list,popVal(type_specifier)+$2->getName());
					printRuleLog(parameter_list,"type_specifier ID");
			}
		| type_specifier
			{
					addTypeArgtoParamList(variable_type);

		   		pushVal(parameter_list,popVal(type_specifier));
					printRuleLog(parameter_list,"type_specifier");
			}
 		;


compound_statement: LCURL {enterFuncScope();} statements RCURL
			{
			   	pushVal(compound_statement,"{"+popVal(statements)+"}");
					printRuleLog(compound_statement,"LCURL statements RCURL");

					exitFuncScope();
			}
 		  | LCURL {enterFuncScope();} RCURL
			{
			   	pushVal(compound_statement,"{}");
					printRuleLog(compound_statement,"LCURL RCURL");

					exitFuncScope();
			}
 		  ;

var_declaration: type_specifier declaration_list SEMICOLON
 			{
			   	pushVal(var_declaration,popVal(type_specifier)+popVal(declaration_list)+";");
					printRuleLog(var_declaration,"type_specifier declaration_list SEMICOLON");
			}
			|type_specifier declaration_list error{printErrorLog("; missing");}			
		 ;

type_specifier: INT
			{
				$$ = getType(INT_TYPE);

		   	pushVal(type_specifier,"int");
				printRuleLog(type_specifier,"INT");
			}
 		| FLOAT
			{
				$$ = getType(FLOAT_TYPE);

				pushVal(type_specifier,"float");
				printRuleLog(type_specifier,"FLOAT");
			}
 		| VOID
			{
				$$ = getType(VOID_TYPE);

				pushVal(type_specifier,"void");
				printRuleLog(type_specifier,"VOID");
			}
 		;

declaration_list: declaration_list COMMA ID
			{
				insertVar($3);

		   	pushVal(declaration_list,popVal(declaration_list)+","+$3->getName());
				printRuleLog(declaration_list,"declaration_list");
			}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
			{
				insertArr($3,$5);

				pushVal(declaration_list,popVal(declaration_list)+","+$3->getName()+"["+$5->getName()+"]");
				printRuleLog(declaration_list,"declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
			}
 		  | ID
			{
				insertVar($1);

		   	pushVal(declaration_list,$1->getName());
				printRuleLog(declaration_list,"ID");
			}
 		  | ID LTHIRD CONST_INT RTHIRD
			{
				insertArr($1,$3);

				pushVal(declaration_list,$1->getName()+"["+$3->getName()+"]");
				printRuleLog(declaration_list,"ID LTHIRD CONST_INT RTHIRD");
			}
 		  ;

statements: statement
			{
				pushVal(statements,popVal(statement));
				printRuleLog(statements,"statement");
			}
	   | statements statement {
		   	pushVal(statements,popVal(statements)+popVal(statement));
				printRuleLog(statements,"statements statement");
		 	}
	   ;

statement: var_declaration
			{
				pushVal(statement,popVal(var_declaration));
				printRuleLog(statement,"var_declaration");
			}
	  | expression_statement
			{
				pushVal(statement,popVal(expression_statement));
				printRuleLog(statement,"expression_statement");
			}
	  | compound_statement
			{
				pushVal(statement,popVal(compound_statement));
				printRuleLog(statement,"compound_statement");
			}
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
			{
				pushVal(statement,(string("for")+"("+popVal(expression_statement)+popVal(expression_statement)+popVal(expression)+")"+popVal(statement)));
				printRuleLog(statement,"FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			}
	  | IF LPAREN expression RPAREN statement %prec second_precedence
			{
				pushVal(statement,(string("if")+"("+popVal(expression)+")"+popVal(statement)));
				printRuleLog(statement,"IF LPAREN expression RPAREN statement");
			}
	  | IF LPAREN expression RPAREN statement ELSE statement
			{
				pushVal(statement,(string("if")+"("+popVal(expression)+")"+popVal(statement)+"else"+popVal(statement)));
				printRuleLog(statement,"IF LPAREN expression RPAREN statement ELSE statement");
			}
	  | WHILE LPAREN expression RPAREN statement
			{
				pushVal(statement,(string("while")+"("+popVal(expression)+")"+popVal(statement)));
				printRuleLog(statement,"WHILE LPAREN expression RPAREN statement");
			}
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
			{
				pushVal(statement,"("+$3->getName()+")"+";");
				printRuleLog(statement,"PRINTLN LPAREN ID RPAREN SEMICOLON");
			}
	  | RETURN expression SEMICOLON
			{
				pushVal(statement,"return"+popVal(expression)+";");
				printRuleLog(statement,"RETURN expression SEMICOLON");
			}
	  ;

expression_statement: SEMICOLON
				{
					pushVal(expression_statement,";");
					printRuleLog(expression_statement,"SEMICOLON");
				}
			| expression SEMICOLON {
					pushVal(expression_statement,popVal(expression)+";");
					printRuleLog(expression_statement,"expression SEMICOLON");
				}
			| expression error {
				printErrorLog("SEMICOLON missing");
			}
			;

variable: ID
			{
				$$ = getVariable($1);

				pushVal(variable,$1->getName());
				printRuleLog(variable,"ID");
			}
	 | ID LTHIRD expression RTHIRD
	 		{
				$$ = getArrIndexVar($1,$3);

				pushVal(variable,($1->getName()+"["+popVal(expression)+"]"));
				printRuleLog(variable,"ID LTHIRD expression RTHIRD");
			}
	 ;

expression: logic_expression
				{
					$$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0);

					pushVal(expression,popVal(logic_expression));
					printRuleLog(expression,"logic_expression");
				}
	   | variable ASSIGNOP logic_expression
		 		{
					$$ = getAssignExpVal($1,$3);

					pushVal(expression,popVal(variable)+"="+popVal(logic_expression));
					printRuleLog(expression,"variable ASSIGNOP logic_expression");
				}
	   ;

logic_expression: rel_expression
				{
					$$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0);

					pushVal(logic_expression,popVal(rel_expression));
					printRuleLog(logic_expression,"rel_expression");
				}
		 | rel_expression LOGICOP rel_expression
		 		{
					$$ = getLogicOpVal($1,$3,$2);

					string r2 = popVal(rel_expression);
					string r1 = popVal(rel_expression);
					pushVal(logic_expression,r1+$2->getName()+r2);
					printRuleLog(logic_expression,"rel_expression LOGICOP rel_expression");
				}
		 ;

rel_expression: simple_expression
			{
				$$ = $1;
				$$->ints.push_back(0);
				$$->floats.push_back(0);

				pushVal(rel_expression,popVal(simple_expression));
				printRuleLog(rel_expression,"simple_expression");
			}
		| simple_expression RELOP simple_expression
				{
					$$ = getReltnOpVal($1,$3,$2);

					string s2 = popVal(simple_expression);
					string s1 = popVal(simple_expression);
					pushVal(rel_expression,s1+$2->getName()+s2);
					printRuleLog(rel_expression,"simple_expression RELOP simple_expression");
				}
		;

simple_expression: term
				{
					$$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0);

					pushVal(simple_expression,popVal(term));
					printRuleLog(simple_expression,"term");
				}
		  | simple_expression ADDOP term
				{
					/* if(getAddtnOpVal($1,$3,$2)!=nullptr) */
					$$ = getAddtnOpVal($1,$3,$2);
					/* printDebug(string("term Val: ") + "float: " + to_string($3->floats[0])+ "int: " + to_string($3->ints[0])); */

					pushVal(simple_expression,popVal(simple_expression)+$2->getName()+popVal(term));
					printRuleLog(simple_expression,"simple_expression ADDOP term");
				}
		  ;

term:	unary_expression
				{
					$$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0);

					pushVal(term,popVal(unary_expression));
					printRuleLog(term,"unary_expression");
				}
     | term MULOP unary_expression
		 		{
					/* printDebug($1->getName()+"Term Val : "+to_string($1->getVarType()=="INT" ? $1->ints[0]:$1->floats[0])); */
					SymbolInfo* t = getMultpOpVal($1,$3,$2);
					if(t!=nullptr)
					$$ = t;

					pushVal(term,popVal(term)+$2->getName()+popVal(unary_expression));
					printRuleLog(term,"term MULOP unary_expression");
				}
     ;

unary_expression: ADDOP unary_expression
				{
					$$ = getUniAddOpVal($2,$1);

					pushVal(unary_expression,$1->getName()+popVal(unary_expression));
					printRuleLog(unary_expression,"ADDOP unary_expression");
				}
		 | NOT unary_expression
		 		{
					$$ = getNotOpVal($2);

					pushVal(unary_expression,"!"+popVal(unary_expression));
					printRuleLog(unary_expression,"NOT unary_expression");
				}
		 | factor
		 		{
					/* printDebug($1->getName()+"Fact Val : "+to_string($1->getVarType()=="INT" ? $1->ints[0]:$1->floats[0])); */
					// Not sure why
					/* $$ = $1;
					$$->ints.push_back(0);
					$$->floats.push_back(0); */

					/* printDebug($1->getName()+"Fact Type : "+$1->getVarType()); */

					pushVal(unary_expression,popVal(factor));
					printRuleLog(unary_expression,"factor");
				}
		 ;

factor: variable
		{
			pushVal(factor,popVal(variable));
			printRuleLog(factor,"variable");
		}
	| ID LPAREN argument_list RPAREN
		{
			SymbolInfo* t=getFuncCallValue($1);
			if(t!=nullptr)
			$$ = t;

			pushVal(factor,$1->getName()+"("+popVal(argument_list)+")");
			printRuleLog(factor,"ID LPAREN argument_list RPAREN");
		}
	| LPAREN expression RPAREN
		{
			$$=$2;

			pushVal(factor,"("+popVal(expression)+")");
			printRuleLog(factor,"LPAREN expression RPAREN");
		}
	| CONST_INT
		{
			$$ = getConstVal($1,INT_TYPE);
			/* printDebug($1->getName()+"CONST_INT Val : "+to_string($1->ints[0])); */

			pushVal(factor,$1->getName());
			printRuleLog(factor,"CONST_INT");
		}
	| CONST_FLOAT
		{
			$$ = getConstVal($1,FLOAT_TYPE);

			pushVal(factor,$1->getName());
			printRuleLog(factor,"CONST_FLOAT");
		}
	| variable INCOP
		{
			$$ = getIncOpVal($1);

			pushVal(factor,popVal(variable)+"++");
			printRuleLog(factor,"variable INCOP");
		}
	| variable DECOP
		{
			$$ = getIncOpVal($1);

			pushVal(factor,popVal(variable)+"--");
			printRuleLog(factor,"variable DECOP");
		}
	;

argument_list: arguments
					{
						pushVal(argument_list,popVal(arguments));
						printRuleLog(argument_list,"arguments");
					}
			  |
					{
						pushVal(argument_list,"");
						printRuleLog(argument_list,"");
					}
			  ;

arguments: arguments COMMA logic_expression
					{
						argsFunc.push_back(*$3);
						args.push_back($3->getVarType());

						pushVal(arguments,popVal(arguments)+","+popVal(logic_expression));
						printRuleLog(argument_list,"arguments COMMA logic_expression");
					}
	      | logic_expression
					{
						argsFunc.push_back(*$1);
						args.push_back($1->getVarType());

						pushVal(arguments,popVal(logic_expression));
						printRuleLog(argument_list,"logic_expression");
					}
	      ;

%%

int main(int argc,char *argv[])
{

	if((yyin=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	logFile.open("log.txt");
	errorFile.open("errors.txt");
	parserFile.open("parser.txt");

	yyparse();
	logFile << "Total Lines : " << line_count << std::endl << std::endl;
	errorFile << "Total Errors : " << semErrors << std::endl;
	errorFile << "Total Warning : " << warnings << std::endl;

	table.printAllScope(logFile);



	logFile.close();
	errorFile.close();
	parserFile.close();

	table.printAllScope();

	return 0;
}
