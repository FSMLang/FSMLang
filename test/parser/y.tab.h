/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ON = 258,                      /* ON  */
    NAMESPACE = 259,               /* NAMESPACE  */
    STATE_KEY = 260,               /* STATE_KEY  */
    EVENT_KEY = 261,               /* EVENT_KEY  */
    PROLOGUE_KEY = 262,            /* PROLOGUE_KEY  */
    EPILOGUE_KEY = 263,            /* EPILOGUE_KEY  */
    DATA_KEY = 264,                /* DATA_KEY  */
    TRANSLATOR_KEY = 265,          /* TRANSLATOR_KEY  */
    MACHINE_KEY = 266,             /* MACHINE_KEY  */
    REENTRANT = 267,               /* REENTRANT  */
    ACTIONS = 268,                 /* ACTIONS  */
    RETURN = 269,                  /* RETURN  */
    STATES = 270,                  /* STATES  */
    EVENTS = 271,                  /* EVENTS  */
    RETURNS = 272,                 /* RETURNS  */
    EXTERNAL = 273,                /* EXTERNAL  */
    VOID = 274,                    /* VOID  */
    IMPLEMENTATION_KEY = 275,      /* IMPLEMENTATION_KEY  */
    INHIBITS = 276,                /* INHIBITS  */
    SUBMACHINES = 277,             /* SUBMACHINES  */
    ALL = 278,                     /* ALL  */
    ENTRY = 279,                   /* ENTRY  */
    EXIT = 280,                    /* EXIT  */
    STRUCT_KEY = 281,              /* STRUCT_KEY  */
    UNION_KEY = 282,               /* UNION_KEY  */
    START_KEY = 283,               /* START_KEY  */
    EVENT_SEQ = 284,               /* EVENT_SEQ  */
    END_KEY = 285,                 /* END_KEY  */
    SEQUENCE_KEY = 286,            /* SEQUENCE_KEY  */
    ACTION_KEY = 287,              /* ACTION_KEY  */
    TRANSITION_KEY = 288,          /* TRANSITION_KEY  */
    GUARD_KEY = 289,               /* GUARD_KEY  */
    WHEN_KEY = 290,                /* WHEN_KEY  */
    OTHERWISE_KEY = 291,           /* OTHERWISE_KEY  */
    PARENT = 292,                  /* PARENT  */
    NATIVE_KEY = 293,              /* NATIVE_KEY  */
    NATIVE_BLOCK = 294,            /* NATIVE_BLOCK  */
    MACHINE = 295,                 /* MACHINE  */
    STATE = 296,                   /* STATE  */
    EVENT = 297,                   /* EVENT  */
    ACTION = 298,                  /* ACTION  */
    TRANSITION_FN = 299,           /* TRANSITION_FN  */
    ID = 300,                      /* ID  */
    NUMERIC_STRING = 301,          /* NUMERIC_STRING  */
    TYPE_NAME = 302                /* TYPE_NAME  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ON 258
#define NAMESPACE 259
#define STATE_KEY 260
#define EVENT_KEY 261
#define PROLOGUE_KEY 262
#define EPILOGUE_KEY 263
#define DATA_KEY 264
#define TRANSLATOR_KEY 265
#define MACHINE_KEY 266
#define REENTRANT 267
#define ACTIONS 268
#define RETURN 269
#define STATES 270
#define EVENTS 271
#define RETURNS 272
#define EXTERNAL 273
#define VOID 274
#define IMPLEMENTATION_KEY 275
#define INHIBITS 276
#define SUBMACHINES 277
#define ALL 278
#define ENTRY 279
#define EXIT 280
#define STRUCT_KEY 281
#define UNION_KEY 282
#define START_KEY 283
#define EVENT_SEQ 284
#define END_KEY 285
#define SEQUENCE_KEY 286
#define ACTION_KEY 287
#define TRANSITION_KEY 288
#define GUARD_KEY 289
#define WHEN_KEY 290
#define OTHERWISE_KEY 291
#define PARENT 292
#define NATIVE_KEY 293
#define NATIVE_BLOCK 294
#define MACHINE 295
#define STATE 296
#define EVENT 297
#define ACTION 298
#define TRANSITION_FN 299
#define ID 300
#define NUMERIC_STRING 301
#define TYPE_NAME 302

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 80 "../../src/parser.y"

	pID_INFO				        pid_info;
	pACTION_SE_INFO	        se_info;
	pACTION_INFO		        action_info;
	pMATRIX_INFO		        matrix_info;
	char *					        charData;
 MOD_FLAGS                mod_flags;
 pMACHINE_INFO            pmachineInfo;
 pLIST                    plist;
 pSTATE_AND_EVENT_DECLS   pstate_and_event_decls;
 pSTATEMENT_DECL_LIST     pstatement_decl_list;
 pACTIONS_AND_TRANSITIONS pactions_and_transitions;
 pACTION_DECL						  paction_decl;
 pMACHINE_QUALIFIER       pmachine_qualifier;
 pMACHINE_PREFIX          pmachine_prefix;
 pDATA_FIELD              pdata_field;
 pDATA_TYPE_STRUCT        pdata_type_struct;
 pUSER_EVENT_DATA         puser_event_data;
 pNATIVE_INFO             pnative_info;
 pEVENT_SEQUENCE          psequence;
 pEVENT_SEQUENCE_NODE     pevent_sequence_node;
 pTRANSITION_DATA         ptransition;
 pRETURN_CHOICE_DATA      preturn_choice;

#line 186 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
