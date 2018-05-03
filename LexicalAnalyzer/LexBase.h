//
// Created by subangkar on 5/2/18.
//

#ifndef LEXICALANALYZER_LEXBASE_H
#define LEXICALANALYZER_LEXBASE_H

#include "DataStructure.h"
#include "Utils.h"

SymbolTable hashTable(7);
FILE *logout, *tokenout;
int line_count = 1;
int keyword_count = 0;
int err_count = 0;



#define TOKEN_PRINT_KEY "<%s> "
#define TOKEN_PRINT_SYMBOL "<%s, %s> "
#define LOG_TOKEN_PRINT "\nLine no %d: TOKEN <%s> Lexeme %s found\n"
#define LOG_ERROR_PRINT "\n<< Error @ Line no %d: %s: %s >>\n"
#define LOG_COMMENT_PRINT "\nLine no %d: TOKEN <COMMENT> Lexeme <%s> found\n"



void printLog(int lineNo, string tokenName, string lexemeName) {
	fprintf(logout, LOG_TOKEN_PRINT, line_count, tokenName.data(), lexemeName.data());
}


void addToken_keyword(string token_name) {
	fprintf(tokenout, TOKEN_PRINT_KEY, token_name.data());
	printLog(line_count, token_name, yytext);
	keyword_count++;
}

void addToken_identifier() {
	string token_name = "ID";
	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), yytext);
	printLog(line_count, token_name, yytext);
	hashTable.insert(yytext, token_name);
}

void addToken_string() {
	string token_name = "STRING";

	string string_literal = StringParser::parse(yytext);
	StringUtils::replaceFirst(string_literal, "\"", "");
	StringUtils::replaceLast(string_literal, "\"", "");
	StringUtils::replaceAll(string_literal, "\\\n", "");
//	string_literal=StringParser::parse(string_literal);
	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), string_literal.data());
	printLog(line_count, token_name, yytext);

	line_count += StringUtils::occCount(yytext, '\n');

//	hashTable.insert(string_literal,token_name);
}


void addToken_const_int() {
	string token_name = "CONST_INT";
	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), yytext);
	printLog(line_count, token_name, yytext);

	hashTable.insert(yytext, token_name);
}

void addToken_const_float() {
	string token_name = "CONST_FLOAT";
	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), yytext);
	printLog(line_count, token_name, yytext);

	hashTable.insert(yytext, token_name);
}

void addToken_const_char() {
	string token_name = "CONST_CHAR";
	string char_literal = StringParser::parse(yytext);
	StringUtils::replaceFirst(char_literal, "\'", "");
	StringUtils::replaceLast(char_literal, "\'", "");

	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), char_literal.data());
	printLog(line_count, token_name, yytext);

	hashTable.insert(yytext, token_name);
}

void addToken_operator(string token_name) {
	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), yytext);
	printLog(line_count, token_name, yytext);

	hashTable.insert(yytext, token_name);
}

void addToken_operatorNotStore(string token_name) {
	fprintf(tokenout, TOKEN_PRINT_SYMBOL, token_name.data(), yytext);
	printLog(line_count, token_name, yytext);
}


void printError(string msg) {
	fprintf(logout, LOG_ERROR_PRINT, line_count, msg.data(),yytext);

	err_count++;

	line_count += StringUtils::occCount(yytext, '\n');
}

void comment() {

	string cmnt = string(yytext);

	if(cmnt[1]=='/'){
		StringUtils::replaceFirst(cmnt,"//","");
		StringUtils::replaceAll(cmnt,"\\\n","");
	} else{
		StringUtils::replaceFirst(cmnt,"/*","");
		StringUtils::replaceFirst(cmnt,"*/","");
	}

	fprintf(logout, LOG_COMMENT_PRINT, line_count, cmnt.data());

//	string s(yytext);
	line_count += StringUtils::occCount(yytext, '\n');
}

#endif //LEXICALANALYZER_LEXBASE_H
