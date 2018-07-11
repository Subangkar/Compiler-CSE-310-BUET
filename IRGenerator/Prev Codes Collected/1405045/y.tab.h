/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    COMMENT = 258,
    IF = 259,
    ELSE = 260,
    FOR = 261,
    WHILE = 262,
    DO = 263,
    BREAK = 264,
    CONTINUE = 265,
    INT = 266,
    FLOAT = 267,
    CHAR = 268,
    DOUBLE = 269,
    VOID = 270,
    RETURN = 271,
    SWITCH = 272,
    CASE = 273,
    DEFAULT = 274,
    INCOP = 275,
    DECOP = 276,
    ASSIGNOP = 277,
    LPAREN = 278,
    RPAREN = 279,
    LCURL = 280,
    RCURL = 281,
    LTHIRD = 282,
    RTHIRD = 283,
    SEMICOLON = 284,
    COMMA = 285,
    STRING = 286,
    NOT = 287,
    PRINTLN = 288,
    ID = 289,
    CONST_INT = 290,
    CONST_FLOAT = 291,
    CONST_CHAR = 292,
    ADDOP = 293,
    MULOP = 294,
    LOGICOP = 295,
    RELOP = 296,
    THEN = 297
  };
#endif
/* Tokens.  */
#define COMMENT 258
#define IF 259
#define ELSE 260
#define FOR 261
#define WHILE 262
#define DO 263
#define BREAK 264
#define CONTINUE 265
#define INT 266
#define FLOAT 267
#define CHAR 268
#define DOUBLE 269
#define VOID 270
#define RETURN 271
#define SWITCH 272
#define CASE 273
#define DEFAULT 274
#define INCOP 275
#define DECOP 276
#define ASSIGNOP 277
#define LPAREN 278
#define RPAREN 279
#define LCURL 280
#define RCURL 281
#define LTHIRD 282
#define RTHIRD 283
#define SEMICOLON 284
#define COMMA 285
#define STRING 286
#define NOT 287
#define PRINTLN 288
#define ID 289
#define CONST_INT 290
#define CONST_FLOAT 291
#define CONST_CHAR 292
#define ADDOP 293
#define MULOP 294
#define LOGICOP 295
#define RELOP 296
#define THEN 297

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 85 "parser.y" /* yacc.c:1909  */

SymbolInfo* symVal;

#line 142 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
