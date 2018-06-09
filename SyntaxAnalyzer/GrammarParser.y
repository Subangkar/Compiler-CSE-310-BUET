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
		;

func_definition: type_specifier ID LPAREN parameter_list RPAREN compound_statement
			{
				pushVal(func_definition,popVal(type_specifier)+$2->getName()+"("+popVal(parameter_list)+")"+popVal(compound_statement));
				printRuleLog(func_definition,"type_specifier ID LPAREN parameter_list RPAREN compound_statement");
			}
		| type_specifier ID LPAREN RPAREN compound_statement
			{
				pushVal(func_definition,popVal(type_specifier)+$2->getName()+"("+")"+popVal(compound_statement));
				printRuleLog(func_definition,"type_specifier ID LPAREN RPAREN compound_statement");
			}
 		;


parameter_list: parameter_list COMMA type_specifier ID
			{
			   	pushVal(parameter_list,popVal(parameter_list)+","+popVal(type_specifier)+$4->getName());
					printRuleLog(parameter_list,"parameter_list COMMA type_specifier ID");
			}
		| parameter_list COMMA type_specifier
			{
				pushVal(parameter_list,popVal(parameter_list)+","+popVal(type_specifier));
				printRuleLog(statements,"parameter_list COMMA type_specifier");
			}
 		| type_specifier ID
			{
			   	pushVal(parameter_list,popVal(type_specifier)+$2->getName());
					printRuleLog(parameter_list,"type_specifier ID");
			}
		| type_specifier
			{
		   		pushVal(parameter_list,popVal(type_specifier));
					printRuleLog(parameter_list,"type_specifier");
			}
 		;


compound_statement: LCURL statements RCURL
			{
			   	pushVal(compound_statement,"{"+popVal(statements)+"}");
					printRuleLog(compound_statement,"LCURL statements RCURL");
			}
 		  | LCURL RCURL
			{
			   	pushVal(compound_statement,"{}");
					printRuleLog(compound_statement,"LCURL RCURL");
			}
 		  ;

var_declaration: type_specifier declaration_list SEMICOLON
 			{
			   	pushVal(var_declaration,popVal(type_specifier)+popVal(declaration_list)+";");
					printRuleLog(var_declaration,"type_specifier declaration_list SEMICOLON");
			}
		 ;

type_specifier: INT
			{
		   	pushVal(type_specifier,"int");
				printRuleLog(type_specifier,"INT");
			}
 		| FLOAT
			{
				pushVal(type_specifier,"float");
				printRuleLog(type_specifier,"FLOAT");
			}
 		| VOID
			{
				pushVal(type_specifier,"void");
				printRuleLog(type_specifier,"VOID");
			}
 		;

declaration_list: declaration_list COMMA ID
			{
		   	pushVal(declaration_list,popVal(declaration_list)+","+$3->getName());
				printRuleLog(declaration_list,"declaration_list");
			}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
			{
				pushVal(declaration_list,popVal(declaration_list)+","+$3->getName()+"["+$5->getName()+"]");
				printRuleLog(declaration_list,"declaration_list COMMA ID LTHIRD CONST_INT RTHIRD");
			}
 		  | ID
			{
		   	pushVal(declaration_list,$1->getName());
				printRuleLog(declaration_list,"ID");
			}
 		  | ID LTHIRD CONST_INT RTHIRD
			{
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
	  | IF LPAREN expression RPAREN statement
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
			;

variable: ID
			{
				pushVal(variable,$1->getName());
				printRuleLog(variable,"ID");
			}
	 | ID LTHIRD expression RTHIRD
	 		{
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
					pushVal(expression,popVal(variable)+"="+popVal(logic_expression));
					printRuleLog(expression,"variable ASSIGNOP logic_expression");
				}
	   ;

logic_expression: rel_expression
				{
					pushVal(logic_expression,popVal(rel_expression));
					printRuleLog(logic_expression,"rel_expression");
				}
		 | rel_expression LOGICOP rel_expression
		 		{
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
					pushVal(term,popVal(term)+$2->getName()+popVal(unary_expression));
					printRuleLog(term,"term MULOP unary_expression");
				}
     ;

unary_expression: ADDOP unary_expression
				{
					pushVal(unary_expression,$1->getName()+popVal(unary_expression));
					printRuleLog(unary_expression,"ADDOP unary_expression");
				}
		 | NOT unary_expression
		 		{
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
	| LPAREN expression RPAREN
		{
			pushVal(factor,"("+popVal(expression)+")");
			printRuleLog(factor,"LPAREN expression RPAREN");
		}
	| CONST_INT
		{
			pushVal(factor,$1->getName());
			printRuleLog(factor,"CONST_INT");
		}
	| CONST_FLOAT
		{
			pushVal(factor,$1->getName());
			printRuleLog(factor,"CONST_FLOAT");
		}
	| variable INCOP
		{
			pushVal(factor,popVal(variable)+"++");
			printRuleLog(factor,"variable INCOP");
		}
	| variable DECOP
		{
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
						pushVal(arguments,popVal(arguments)+","+popVal(logic_expression));
						printRuleLog(argument_list,"arguments COMMA logic_expression");
					}
	      | logic_expression
					{
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
	logFile << "Total Errors : " << semErrors << std::endl;

	table.printAllScope(logFile);



	logFile.close();
	errorFile.close();
	parserFile.close();

	table.printAllScope();

	return 0;
}
