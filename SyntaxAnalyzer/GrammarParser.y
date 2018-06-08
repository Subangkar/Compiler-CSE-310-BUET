%{
#ifndef SYMTABLE
#define SYMTABLE
#include "SynBase.h"
#endif // SYMTABLE

string buf;
string relBuf;
int i=0;

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
		printLog("program");
	};

program: program unit
		{
			printLog("program unit");
		}
	| unit
		{
			printLog("unit");
		}
	;

unit: var_declaration
     | func_declaration
     | func_definition
     ;

func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
		| type_specifier ID LPAREN RPAREN SEMICOLON
		;

func_definition: type_specifier ID LPAREN parameter_list RPAREN compound_statement
		| type_specifier ID LPAREN RPAREN compound_statement
			{
				//logFile<<"Function Def"<<endl;
			}
 		;


parameter_list: parameter_list COMMA type_specifier ID
		| parameter_list COMMA type_specifier
 		| type_specifier ID
		| type_specifier
 		;


compound_statement: LCURL statements RCURL
 		    | LCURL RCURL
 		    ;

var_declaration: type_specifier declaration_list SEMICOLON
 		 ;

type_specifier: INT
 		| FLOAT
 		| VOID
 		;

declaration_list: declaration_list COMMA ID
				{
					logFile << buf << "," << $3->getName() <<endl;
					buf = buf + "," + $3->getName();
				}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
 		  | ID
				{
					buf=$1->getName();
				}
 		  | ID LTHIRD CONST_INT RTHIRD
 		  ;

statements: statement {printLog("statement");}
	   | statements statement {printLog("statements statement");}
	   ;

statement: var_declaration
	  | expression_statement {printLog("expression_statement");}
	  | compound_statement
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
	  | IF LPAREN expression RPAREN statement
	  | IF LPAREN expression RPAREN statement ELSE statement
	  | WHILE LPAREN expression RPAREN statement
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
	  | RETURN expression SEMICOLON
	  ;

expression_statement: SEMICOLON
			| expression SEMICOLON {printLog("expression SEMICOLON");}
			;

variable: ID
	 | ID LTHIRD expression RTHIRD
	 ;

expression: logic_expression
	   | variable ASSIGNOP logic_expression {printLog("variable ASSIGNOP logic_expression");}
	   ;

logic_expression: rel_expression
		 | rel_expression LOGICOP rel_expression
		 ;

rel_expression: simple_expression
		| simple_expression RELOP simple_expression
			{
				string s2=simpEx.top();
				simpEx.pop();
				string s1=simpEx.top();
				simpEx.pop();
				relEx.push(s1+"=="+s2);
				logFile << relEx.top() << endl;
				printLog(s1+"=="+s2);
			}
		;

simple_expression: term
				{
					simpEx.push(termV.top());termV.pop();
				}
		  | simple_expression ADDOP term
		  ;

term:	unary_expression
				{
					++i;
					string x = "2";
					x[0]=i+'0';
					termV.push(x);
				}
     |  term MULOP unary_expression
     ;

unary_expression: ADDOP unary_expression
		 | NOT unary_expression
		 | factor
		 ;

factor: variable
	| ID LPAREN argument_list RPAREN
	| LPAREN expression RPAREN
	| CONST_INT
	| CONST_FLOAT
	| variable INCOP
	| variable DECOP
	;

argument_list: arguments
			  |
			  ;

arguments: arguments COMMA logic_expression
	      | logic_expression
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

	printLog("start");
	yyparse();

	logFile << "Total Lines : " << line_count << std::endl << std::endl;
	logFile << "Total Errors : " << semErrors << std::endl;
	logFile.close();
	errorFile.close();
	parserFile.close();

	return 0;
}
