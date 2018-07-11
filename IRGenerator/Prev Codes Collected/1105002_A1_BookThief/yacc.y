%{
#include "symboltable.h"
#define YYSTYPE SymbolInfo*


using namespace std;
int yyparse(void);
int yylex(void);
string wholecode;
string varlist;
extern FILE *yyin;
extern int count;
int tempcount=0;
int levelcount=0;

FILE *fp,*fasmp;

void yyerror(char *s)
{
	fprintf(stdout,"At Line %d, ERROR-> %s\n",count+1,s);
	return;
}

string num_to_str(double num)
{
    ostringstream oss;
    oss<<num;
    return oss.str();
}
void generateTemp(string &temp)
{   temp=temp+num_to_str(tempcount); 
	tempcount++;
	
}

void generateLabel(string &level)
{
	level=level+num_to_str(levelcount); 
	levelcount++;
}

SymbolTable st;

%}

%token INTEGER REAL FUNCTION PROCEDURE DOTDOT COLON VAR ARRAY IF THEN ELSE DO WHILE WRITE PROGRAM OF ID SEMICOLON COMMA BEGIN1 END NUM ASSIGNOP  RELOP ADDOP NOT MULOP PAREN1 PAREN2 BRACKET1 BRACKET2 OR AND CURLEY1 CURLEY2

%left ADDOP1
%left ADDOP2
%left MULOP
%left MULOP1
%left MULOP2

%nonassoc IFX
%nonassoc ELSE


%%
program:    PROGRAM ID PAREN1 identifier_list PAREN2 SEMICOLON declarations subprogram_declarations compound_statement
            {
            printf("program:    PROGRAM ID PAREN1 identifier_list PAREN2 SEMICOLON declarations subprogram_declarations compound_statement\n");
//            string asmcode=$2->code+$4->code+$5->code+$6->code+$7->code+$8->code+$9->code;
            string s=($4->symbol+" dw 0\n");
            int x=varlist.find(s);
            varlist.erase(x,s.length());
            wholecode=".model small\n.stack 100H\n.data\n"+varlist+"\n.code\n\nmain proc\n\nMOV AX,@data ;\nMOV ds,AX ;\nMOV es,AX ;\n"+$9->code+"\n\nmain endp"+"\nend main";
            }
            ;
identifier_list:    ID
            {$$=$1;
             varlist+=$1->symbol+" dw 0\n";
             printf("identifier_list--> ID\n");
            }
            |
            identifier_list COMMA ID
            {
            
            varlist+=$3->symbol+" dw 0\n";
              
            printf("identifier_list-->COMMA  ID\n");
            
            }
            ;
declarations: 	       
            {
            printf("declarations-->    \n");
            }
            |
            declarations VAR identifier_list COLON type SEMICOLON
            {
            $$=$3;
            printf("declarations-->   declarations VAR identifier_list COLON type SEMICOLON \n");
            
            }
            ;
type:       standard_type
            {$$=$1;
            printf("type--->standard_type\n");
            }
            |
            ARRAY BRACKET1 NUM DOTDOT NUM BRACKET2 OF standard_type
            {
            printf("type-->ARRAY BRACKET1 NUM DOTDOT NUM BRACKET2 OF standard_type\n");
            }
            ;
standard_type:  INTEGER
            {
            $$=$1;
            printf("standard_type-->  INTEGER\n");
            }
            |
            REAL
            {
            $$=$1;
            printf("standard_type-->  REAL\n");
            }
            ;
subprogram_declarations:	           
            {
            printf("subprogram_declarations--> \n");
            }
            |
            subprogram_declarations subprogram_declaration SEMICOLON
            {
            printf("subprogram_declarations-->subprogram_declarations subprogram_declaration SEMICOLON\n");
            }
            ;
subprogram_declaration:	subprogram_head declarations compound_statement
            {
            printf("subprogram_declaration-->subprogram_head declarations compound_statement\n");
            }
            ;
subprogram_head:    FUNCTION ID arguments COLON standard_type SEMICOLON
            {
            printf("subprogram_head-->FUNCTION ID arguments COLON standard_type SEMICOLON\n");
            }
            |
            PROCEDURE ID arguments SEMICOLON
            {
            printf("subprogram_head-->PROCEDURE ID arguments SEMICOLON\n");
            }
            ;
arguments:	        
            {
            printf("arguments--> \n");
            }
            |
            PAREN1 parameter_list PAREN2
            {
            printf("arguments --> PAREN1 parameter_list PAREN2\n");
            }
            ;
parameter_list:	identifier_list COLON type
            {
            printf("parameter_list--->identifier_list COLON type\n");
            }
            |
            parameter_list SEMICOLON identifier_list COLON type
            {
            printf("parameter_list ---> parameter_list SEMICOLON identifier_list COLON type\n");
            }
            ;

compound_statement:	BEGIN1 optional_statements END
            {
            $$=$2;
            printf("compound_statement-->	BEGIN optional_statements END\n");
            }
            ;
optional_statements:	      
            {
            printf("optional_statements-->	      \n");
            }
            |
            statement_list
            {$$=$1;
            printf("optional_statements--> statement_list\n");
            
            }
            ;
statement_list: statement
            {$$=$1;
            printf("statement_list--> statement\n");
            }
            |
            statement_list SEMICOLON statement
            {
            $$->code=$1->code+"\n"+$3->code+"\n";
            printf("statement_list--> statement_list SEMICOLON statement\n");
            }
            ;
statement:  variable ASSIGNOP expression
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo* n=st.uplook(temp);
            $1->value=$3->value;
            n->code=$3->code+"\n";
            n->code+="MOV AX, "+$3->symbol+"\n";
            n->code+="MOV "+$1->symbol+" , AX;\n";
            
            $$=n;
            varlist+=$$->symbol+" dw 0\n";
            printf("statement--> variable ASSIGNOP expression\n");
            }
            |
            procedure_statement
            {$$=$1;
            printf("statement--> procedure_statement\n");
            }
            |
            compound_statement
            {$$=$1;
            printf("statement--> compound_statement\n");
            
            }
            |
            IF expression THEN statement %prec IFX
            {
            string temp="temp";
           generateTemp(temp);
        st.insert(temp,"temp");
        SymbolInfo *n=st.uplook(temp);
        SymbolInfo *b=$2;
        n->code = b->code + b->TrueLabel + ":\n" + $4->code + "\n" + b->FalseLabel +":\n";
        $$ = n;
            varlist+=$$->symbol+" dw 0\n";
        
        
            printf("statement-->IF expression THEN statement\n");
            }
            |
            IF expression THEN statement ELSE statement
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo *n=st.uplook(temp);
            SymbolInfo *b=$2;
            
            string tempLabel="label";
            generateLabel(tempLabel);
            
            n->code = b->code + b->TrueLabel + ":\n" + $4->code + "\ngoto " + tempLabel + ";\n" + b->FalseLabel +":\n" + $6->code + "\n" + tempLabel +":\n";
        $$ = n;
        varlist+=$$->symbol+" dw 0\n";
        
            printf("statement-->IF expression THEN statement ELSE statement\n");
            
            }
            |
            WHILE expression DO statement
            {
            string beginLabel="label";
            generateLabel(beginLabel);
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo *n=st.uplook(temp);
            SymbolInfo *b=$2;
            
            n->code=beginLabel+":\n"+$2->code+$2->TrueLabel+":\n"+$4->code+"goto " + beginLabel +";\n"+$2->FalseLabel;
                       $$ = n;
        varlist+=$$->symbol+" dw 0\n";
            
            printf("statement--> WHILE expression DO statement\n");
            
            }
            |
            WRITE PAREN1 ID PAREN2
            {
            printf("statement-->WRITE PAREN1 ID PAREN2");
            }
            ;
variable:   ID
            {$$=$1;
            printf("variable-->   ID\n");
            }
            |
            ID BRACKET1 expression BRACKET2
            {
            printf("variable--> ID BRACKET1 expression BRACKET2\n");
            }
            ;
procedure_statement:    ID
            {$$=$1;
            printf("procedure_statement --> ID \n");
            }
            |
            ID PAREN1 expression_list PAREN2
            {
            printf("procedure_statement --> ID PAREN1 expression_list PAREN2\n");
            }
            ;
expression_list:	expression
            { $$=$1;
            printf("expression_list:	expression\n");
            }
            |
            expression_list COMMA expression
            {
            printf("expression_list: expression_list COMMA expression\n");
            
            }
            ;
expression: simple_expression
            { $$=$1;
            printf("expression: simple_expression\n");
            }
            |
            simple_expression RELOP simple_expression
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo* n=st.uplook(temp);
            
            n->code=$1->code+"\n"+$3->code+"\n";
            string whatsign=$2->symbol;
            string compsignm;
            if(whatsign==">")compsignm="jg";
            else if(whatsign==">=")compsignm="jge";
            else if(whatsign=="<" || whatsign=="<>" )compsignm="jl";
            else if(whatsign=="<=")compsignm="jle";
            else if(whatsign=="==")compsignm="je";
            else if(whatsign=="!=")compsignm="jne";
            
            n->code+="push ax;\n push bx;\n";
            n->code+=("mov ax, " + $1->symbol+";\n");
            n->code+=("mov bx, " + $3->symbol+";\n");
            n->code+=("cmp AX,BX;\n");
            
            
            string l1="label";
            generateLabel(l1);
            n->TrueLabel=l1;
            
            string l2="label";
            generateLabel(l2);
            n->FalseLabel=l2;
            
            n->code+=(compsignm + l1+";\n");
            n->code+=("jmp "+l2+";\n");
            n->code+="pop bx;\n pop ax;\n";
            
            $$=n;
            varlist+=$$->symbol+" dw 0\n";
            
            printf("expression: simple_expression RELOP simple_expression\n");
            }
            ;
simple_expression:  term
            {
            $$=$1;  
            printf("simple_expression -->term\n");
            }
            |
            sign term
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo* n=st.uplook(temp);
            
            n->value=$2->value;
            if($1->symbol=="-")
              {
              n->value*=-1;
              n->code="NEG "+$2->symbol+"\n";
              }
            $$=n;
            varlist+=$$->symbol+" dw 0\n";
            printf("simple_expression --> sign term\n");
            }
            |
            simple_expression ADDOP term
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo* n=st.uplook(temp);
            n->code="\n"+$1->code+"\n"+$3->code+"\n"+"MOV AX,"+$1->symbol+";\n"+"MOV BX,"+$3->symbol+";\n";
            if($2->symbol=="+"){
            n->value=$1->value+$3->value;
            n->code+="ADD AX,BX;\n";
            }
            else if  ($2->symbol=="-"){n->value=$1->value+$3->value;
            n->code+="SUB AX,BX;\n";
            }
            $$=n;
            varlist+=$$->symbol+" dw 0\n";
            printf("simple_expression --> simple_expression ADDOP term \n");
            }
            ;
term:       factor
            {
            $$=$1;
            printf("term-->factor\n");
            }
            |
            term MULOP factor
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            SymbolInfo* n=st.uplook(temp);
            
            n->code="MOV AX, "+$1->symbol+";\n"; 
            if($2->symbol=="*"){n->value=$1->value*$3->value;
            n->code+="MUL "+$3->symbol+";\n";
            }
            else if($2->symbol=="/"){n->value=$1->value/$3->value;
            n->code+="DIV "+$3->symbol+";\n";
            
            }
            else if($2->symbol=="mod"){
            int x=((int)$1->value) %((int) $3->value);
            n->value=x;
            n->code+="DIV "+$3->symbol+";\n";
            
            }
            n->code+="MOV "+n->symbol+", AX;\n";
            $$=n;
            varlist+=$$->symbol+" dw 0\n";
            }
            ;
factor:     ID
            {$$=$1;
            printf("factor-->ID \n");
            }
            |
            ID PAREN1 expression_list PAREN2
            {
            printf("factor--> ID PAREN1 expression_list PAREN2");
            }
            |
            NUM
            {
            $$=$1;
            cout<<$1->value<<endl;
            printf("factor-->NUM\n");
            }
            |
            PAREN1 expression PAREN2
            {
            string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            
            SymbolInfo* n=st.uplook(temp);
            
            n=$2;
            $$=n;
            varlist+=$$->symbol+" dw 0\n";

            printf("factor --> PAREN1 expression PAREN2\n");
            }
            |
            NOT factor
            {string temp="temp";
            generateTemp(temp);
            st.insert(temp,"temp");
            
            SymbolInfo* n=st.uplook(temp);
            
            n->value=~((int)$2->value);
            $$=n;
            varlist+=$$->symbol+" dw 0\n";

            printf("factor-->NOT factor\n");
            }
            ;
sign:       ADDOP
            {
            $$=$1;
            printf("sign--> ADDOP\n");
            }
            
            ;
%%
int main(int argc,char *argv[])
{
	if(argc!=4)
	{
		printf("Usage: ./<programname> <input_file>\n");
		printf("Try Again\n");
		exit(1);
	}

	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}
	
	yyin=fp;
	freopen(argv[2], "w", stdout);
    ofstream fout( argv[3]);     
        
	yyparse();
	st.dump();
	fout<<wholecode<<endl;
     fout.close(); 
	return 0;
}

