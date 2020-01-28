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
    MACHINE_KEY = 258,
    TRANSITION_KEY = 259,
    STATE_KEY = 260,
    EVENT_KEY = 261,
    ACTION_KEY = 262,
    ON = 263,
    REENTRANT = 264,
    ACTIONS = 265,
    RETURN = 266,
    STATES = 267,
    EVENTS = 268,
    RETURNS = 269,
    EXTERNAL = 270,
    EQUALS = 271,
    VOID = 272,
    NATIVE_KEY = 273,
    DATA_KEY = 274,
    DOC_COMMENT = 275,
    MACHINE = 276,
    STATE = 277,
    EVENT = 278,
    ACTION = 279,
    TRANSITION_FN = 280,
    ID = 281
  };
#endif
/* Tokens.  */
#define MACHINE_KEY 258
#define TRANSITION_KEY 259
#define STATE_KEY 260
#define EVENT_KEY 261
#define ACTION_KEY 262
#define ON 263
#define REENTRANT 264
#define ACTIONS 265
#define RETURN 266
#define STATES 267
#define EVENTS 268
#define RETURNS 269
#define EXTERNAL 270
#define EQUALS 271
#define VOID 272
#define NATIVE_KEY 273
#define DATA_KEY 274
#define DOC_COMMENT 275
#define MACHINE 276
#define STATE 277
#define EVENT 278
#define ACTION 279
#define TRANSITION_FN 280
#define ID 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 41 "../src/parser.y" /* yacc.c:1909  */

	pID_INFO				pid_info;
	pACTION_SE_INFO	se_info;
	pACTION_INFO		action_info;
	pMATRIX_INFO		matrix_info;
	char *					charData;

#line 114 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
