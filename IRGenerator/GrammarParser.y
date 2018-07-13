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
						/* printDebug($1->code); */
				}
				pushVal(start,popVal(program));
				printRuleLog(start,"program");
		}
	;

program: program unit
		{
			$$ = new SymbolInfo("","");
			$$->code = $1->code + $2->code;
			delete $1,$2;
			/* printDebug($2->code); */

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
				clearFunctionArgs();

				pushVal(func_declaration,popVal(type_specifier)+$2->getName()+"("+popVal(parameter_list)+")"+";");
				printRuleLog(func_declaration,"type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
			}
		| type_specifier ID LPAREN RPAREN SEMICOLON
			{
				insertFunc($2,$1);

				pushVal(func_declaration,popVal(type_specifier)+$2->getName()+"("+")"+";");
				printRuleLog(func_declaration,"type_specifier ID LPAREN RPAREN SEMICOLON");
			}
		|type_specifier ID LPAREN parameter_list RPAREN error {
			clearFunctionArgs();

			pushVal(func_declaration,popVal(type_specifier)+$2->getName()+"("+popVal(parameter_list)+")"+"");
			printErrorRuleLog("; missing",func_declaration,"type_specifier ID LPAREN parameter_list RPAREN error");}
		|type_specifier ID LPAREN RPAREN error{
			pushVal(func_declaration,popVal(type_specifier)+$2->getName()+"("")"+"");
			printErrorRuleLog("; missing",func_declaration,"type_specifier ID LPAREN RPAREN error");}
		;

func_definition: type_specifier ID LPAREN parameter_list RPAREN{addFuncDef($2,$1);} compound_statement
			{
				$$ = new SymbolInfo("",TEMPORARY);
				$$->code = funcBodyCode($2,$7) + NEWLINE_ASM;
				currentFunc = nullptr;

				pushVal(func_definition,popVal(type_specifier)+$2->getName()+"("+popVal(parameter_list)+")"+popVal(compound_statement));
				printRuleLog(func_definition,"type_specifier ID LPAREN parameter_list RPAREN compound_statement");
			}
		| type_specifier ID LPAREN RPAREN {addFuncDef($2,$1);} compound_statement
			{
				$$ = new SymbolInfo("",TEMPORARY);
				$$->code = funcBodyCode($2,$6) + NEWLINE_ASM;
				currentFunc = nullptr;

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
					addTypeArgtoParamList(variableType);

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
					addTypeArgtoParamList(variableType);

		   		pushVal(parameter_list,popVal(type_specifier));
					printRuleLog(parameter_list,"type_specifier");
			}
		| parameter_list COMMA error ID {
				pushVal(parameter_list,popVal(parameter_list)+","+ERROR_VAL+$4->getName());
				printErrorRuleLog("type specifier missing in parameters",statement,"parameter_list COMMA error ID");
			}
		| error ID{
		   	pushVal(parameter_list,ERROR_VAL+$2->getName());
				printRuleLog(parameter_list,"error ID");
			}
		;


compound_statement: LCURL {enterFuncScope();} statements RCURL
			{
					$$ = $3;
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
			   	pushVal(var_declaration,popVal(type_specifier)+" "+popVal(declaration_list)+";");
					printRuleLog(var_declaration,"type_specifier declaration_list SEMICOLON");
			}
			|type_specifier declaration_list error{
				pushVal(var_declaration,popVal(type_specifier)+" "+popVal(declaration_list)+"");
				printErrorRuleLog("; missing",var_declaration,"type_specifier declaration_list error");}
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
			| ID LTHIRD error RTHIRD{
				pushVal(declaration_list,$1->getName()+"["+ERROR_VAL+"]");
				printErrorRuleLog("Array size must be provided with constant integer",declaration_list,"ID LTHIRD error RTHIRD");
			}
			| declaration_list COMMA ID LTHIRD error RTHIRD	{
				pushVal(declaration_list,popVal(declaration_list)+","+$3->getName()+"["+ERROR_VAL+"]");
				printErrorRuleLog("Array size must be provided with constant integer",declaration_list,"declaration_list COMMA ID LTHIRD error RTHIRD");
			}
 		  ;

statements: statement
			{
				$1->code += NEWLINE_ASM;

				pushVal(statements,popVal(statement));
				printRuleLog(statements,"statement");
			}
	   | statements statement {
			 	$$ = new SymbolInfo("","");
			 	$$->code = $1->code + $2->code;
				$$->code += NEWLINE_ASM;

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
				$$->code = forLoopCode($3,$4,$5,$7);
				pushVal(statement,(string("for")+"("+popVal(expression_statement)+popVal(expression_statement)+popVal(expression)+")"+popVal(statement)));
				printRuleLog(statement,"FOR LPAREN expression_statement expression_statement expression RPAREN statement");
			}
	  | IF LPAREN expression RPAREN statement %prec second_prec
			{
				$$->code = ifElseCode($3,$5);
				pushVal(statement,(string("if")+"("+popVal(expression)+")"+popVal(statement)));
				printRuleLog(statement,"IF LPAREN expression RPAREN statement");
			}
	  | IF LPAREN expression RPAREN statement ELSE statement
			{
				$$->code = ifElseCode($3,$5,$7);
				pushVal(statement,(string("if")+"("+popVal(expression)+")"+popVal(statement)+"else"+popVal(statement)));
				printRuleLog(statement,"IF LPAREN expression RPAREN statement ELSE statement");
			}
	  | WHILE LPAREN expression RPAREN statement
			{
				$$->code = whlLoopCode($3,$5);
				pushVal(statement,(string("while")+"("+popVal(expression)+")"+popVal(statement)));
				printRuleLog(statement,"WHILE LPAREN expression RPAREN statement");
			}
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
			{
				$$->code += printVarValue($3);
				pushVal(statement,"("+$3->getName()+")"+";");
				printRuleLog(statement,"PRINTLN LPAREN ID RPAREN SEMICOLON");
			}
		| RETURN expression SEMICOLON			{
				checkReturnType($2);
				$$->code += returnExpCode($2);

				pushVal(statement,"return"+popVal(expression)+";");
				printRuleLog(statement,"RETURN expression SEMICOLON");
			}
		| RETURN expression error		{
			pushVal(statement,"return"+popVal(expression)+"");
			printErrorRuleLog("; missing before return",statement,"RETURN expression error");
		}
		| error ELSE {
			printErrorLog("else without if");} statement{
			pushVal(statement,string(ERROR_VAL)+" else"+popVal(statement));
			printErrorRuleLog("",statement,"error ELSE statement");
		}
		| IF invalid_condition statement %prec second_prec
		{
			pushVal(statement,(string("if")+"("+ERROR_VAL+")"+popVal(statement)));
			printErrorRuleLog("",statement,"IF LPAREN error RPAREN statement");
		}
		| IF invalid_condition statement ELSE statement{
			pushVal(statement,("else"+popVal(statement)));
			printErrorRuleLog("",statement,"IF LPAREN error RPAREN statement ELSE statement");
		}
		/* | IF LPAREN expression RPAREN statement ELSE LPAREN expression RPAREN statement %prec third_precedence */
	  ;
invalid_condition: LPAREN error RPAREN {printErrorLog("invalid conditional expression");};

expression_statement: SEMICOLON
				{
					$$ = new SymbolInfo(";");
					pushVal(expression_statement,";");
					printRuleLog(expression_statement,"SEMICOLON");
				}
			| expression SEMICOLON {
					tempCount = 0;
					pTempCount = 0;
					$$ = new SymbolInfo(*$1);
					$$->setType(TEMPORARY);
					$$->code = $1->code + NEWLINE_ASM;
					deleteTemp($1);

					pushVal(expression_statement,popVal(expression)+";");
					printRuleLog(expression_statement,"expression SEMICOLON");
				}
			| expression error {
				pushVal(expression_statement,popVal(expression)+ERROR_VAL);
				printErrorRuleLog("; missing after expression",expression_statement,"expression error");
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
					$$->code += NEWLINE_ASM;

					pushVal(logic_expression,popVal(rel_expression));
					printRuleLog(logic_expression,"rel_expression");
				}
		 | rel_expression LOGICOP rel_expression
		 		{
					$$ = getLogicOpVal($1,$3,$2);
					$$->code += NEWLINE_ASM;

					string r2 = popVal(rel_expression);
					string r1 = popVal(rel_expression);
					pushVal(logic_expression,r1+$2->getName()+r2);
					printRuleLog(logic_expression,"rel_expression LOGICOP rel_expression");
				}
		 ;

rel_expression: simple_expression
			{
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
					pushVal(simple_expression,popVal(term));
					printRuleLog(simple_expression,"term");
				}
		  | simple_expression ADDOP term
				{
					$$ = getAddtnOpVal($1,$3,$2);

					pushVal(simple_expression,popVal(simple_expression)+$2->getName()+popVal(term));
					printRuleLog(simple_expression,"simple_expression ADDOP term");
				}
		  ;

term:	unary_expression
				{
					pushVal(term,popVal(unary_expression));
					printRuleLog(term,"unary_expression");
				}
     | term MULOP unary_expression
		 		{
					SymbolInfo* t = getMultpOpVal($1,$3,$2);
					if(t!=nullptr)$$ = t;

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
			$$=getFuncCallValue($1);

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
			$$ = getIncOpVal($1,"++");

			pushVal(factor,popVal(variable)+"++");
			printRuleLog(factor,"variable INCOP");
		}
	| variable DECOP
		{
			$$ = getIncOpVal($1,"--");

			pushVal(factor,popVal(variable)+"--");
			printRuleLog(factor,"variable DECOP");
		}
	| ID LPAREN argument_list error {
		pushVal(factor,$1->getName()+"("+popVal(argument_list)+"");
		printErrorRuleLog("right parentheses missing",factor,"ID LPAREN argument_list error"+ERROR_VAL);clearFunctionArgs();}
	| LPAREN expression error {
		pushVal(factor,"("+popVal(expression)+"");
		printErrorRuleLog("right parentheses missing",factor,"LPAREN expression error");}
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
						argsType.push_back($3->getVarType());

						pushVal(arguments,popVal(arguments)+","+popVal(logic_expression));
						printRuleLog(arguments,"arguments COMMA logic_expression");
					}
	      | logic_expression
					{
						argsFunc.push_back(*$1);
						argsType.push_back($1->getVarType());

						pushVal(arguments,popVal(logic_expression));
						printRuleLog(arguments,"logic_expression");
					}
				| arguments COMMA error {
					pushVal(arguments,popVal(arguments)+","+ERROR_VAL);
					printErrorRuleLog("arguments list not terminated",arguments,"arguments COMMA error");
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

	asmFile.open("code.asm");

	yyparse();
	logFile << "Total Lines : " << line_count << std::endl << std::endl;
	errorFile << "Total Syntax/Semantic Errors : " << syntaxErrors << std::endl;
	errorFile << "Total Lexical Errors : " << err_count_lex << std::endl;
	errorFile << "Total Warning : " << warnings << std::endl;

	table.printAllScope(logFile);



	logFile.close();
	errorFile.close();
	parserFile.close();

	/* table.printAllScope(); */

	return 0;
}
