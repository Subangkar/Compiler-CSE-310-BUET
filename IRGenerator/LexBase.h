//
// Created by subangkar on 5/2/18.
//


#ifndef LEXICALANALYZER_LEXBASE_H
#define LEXICALANALYZER_LEXBASE_H

//#define SYMBOL_TABLE_SIZE 73





#include "DataStructure.h" // SymbolInfo Defined Here
#include "y.tab.h" // SymbolInfo Used Here
#include "Utils.h"
#include <locale>


extern ofstream logFile, errorFile;
extern char *yytext;
int line_count = 1;

int err_count_lex = 0;


#define REPLACE_NEWLINES(string_literal) StringUtils::replaceAll(string_literal, "\\\r\n", ""); // CRLF \
StringUtils::replaceAll(string_literal, "\\\n", ""); // LF



void printLexError(const string &msg, std::ofstream &out) {
	out << " >> LEXICAL Error @ Line no " << line_count << ": " << msg.data() << ": <" << yytext << "> >>" << endl << endl;

	err_count_lex++;

	line_count += StringUtils::occCount(yytext, '\n');
}

void printLexError(const string& msg) {
	printLexError(msg,errorFile);
}


void assignSymbol(const string& name,const string& type) {
	yylval.symbolValue = new SymbolInfo(name,type);
}

void assignSymbol(const string& type) {
	assignSymbol(yytext, type);
}


void addToken_identifier() {
	string token_name = "ID";
	assignSymbol(token_name);
}

void addToken_string() {
	string token_name = "STRING";

	string string_literal = StringParser::parse(yytext);
	StringUtils::replaceFirst(string_literal, "\"", "");
	StringUtils::replaceLast(string_literal, "\"", "");
	REPLACE_NEWLINES(string_literal);

//	string_literal=StringParser::parse(string_literal);

	line_count += StringUtils::occCount(yytext, '\n');

	assignSymbol(string_literal,token_name);
}


void addToken_const_int() {
	string token_name = "CONST_INT";
	assignSymbol(token_name);
}

void addToken_const_float() {
	string token_name = "CONST_FLOAT";
	assignSymbol(token_name);
}

void addToken_const_char() {
	string token_name = "CONST_CHAR";
	string char_literal = StringParser::parse(yytext);
	StringUtils::replaceFirst(char_literal, "\'", "");
	StringUtils::replaceLast(char_literal, "\'", "");
	assignSymbol(char_literal,token_name);
}

void addToken_operator(const string &token_name) {
	assignSymbol(token_name);
}

void comment() {

	string cmnt = string(yytext);

	if (cmnt[1] == '/') {
		StringUtils::replaceFirst(cmnt, "//", "");
		REPLACE_NEWLINES(cmnt);
	} else {
		StringUtils::replaceFirst(cmnt, "/*", "");
		StringUtils::replaceFirst(cmnt, "*/", "");
	}
	line_count += StringUtils::occCount(yytext, '\n');
}

#endif //LEXICALANALYZER_LEXBASE_H
