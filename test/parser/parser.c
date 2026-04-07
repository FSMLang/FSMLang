/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "../../src/parser.y"
         
	/* parser.y

		the FSM Description Language Parser.

	*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cwalk.h>

#if defined (LINUX) || defined (CYGWIN)
#include <strings.h>
#endif

#if defined (LINUX)
#include <unistd.h>
#include <getopt.h>
#endif

#include "revision.h"
#include "fsm_priv.h"
#include "cwalk.h"

#include "fsm_c.h"
#include "fsm_cswitch.h"
#include "fsm_c_event_table.h"
#include "fsm_html.h"
#include "fsm_plantuml.h"
#include "fsm_statistics.h"
#include "fsm_c_pid_xref.h"
#include "fsm_rst.h"
#include "usage.h"
#include "fsm_c_single_switch.h"
#include "fsm_python_transitions.h"

#include "list.h"

int lineno=1;
pLIST id_list = NULL;

//did we parse correctly?
int good=1;

extern char	*yytext;
extern FILE	*yyin, *yyout;

extern int yylex(void);

char *rindex(const char *str,int c);

pMACHINE_INFO               pmachineInfo = NULL;
pFSMOutputGenerator         pfsmog       = NULL;
fpFSMOutputGeneratorFactory fpfsmogf     = NULL;
pSTATE_AND_EVENT_DECLS      psedecls     = NULL;

void yyerror(char *);

/* MinGW/Windows doesn't reliably provide strndup. */
#if defined(_WIN32) && !defined(__CYGWIN__)
static char *fsm_strndup(const char *s, size_t n)
{
    size_t len = 0;
    while (len < n && s[len] != '\0') len++;

    char *out = (char *)malloc(len + 1);
    if (!out) return NULL;

    memcpy(out, s, len);
    out[len] = '\0';
    return out;
}
#define strndup fsm_strndup
#endif


#line 150 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 322 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ON = 3,                         /* ON  */
  YYSYMBOL_NAMESPACE = 4,                  /* NAMESPACE  */
  YYSYMBOL_STATE_KEY = 5,                  /* STATE_KEY  */
  YYSYMBOL_EVENT_KEY = 6,                  /* EVENT_KEY  */
  YYSYMBOL_PROLOGUE_KEY = 7,               /* PROLOGUE_KEY  */
  YYSYMBOL_EPILOGUE_KEY = 8,               /* EPILOGUE_KEY  */
  YYSYMBOL_DATA_KEY = 9,                   /* DATA_KEY  */
  YYSYMBOL_TRANSLATOR_KEY = 10,            /* TRANSLATOR_KEY  */
  YYSYMBOL_MACHINE_KEY = 11,               /* MACHINE_KEY  */
  YYSYMBOL_REENTRANT = 12,                 /* REENTRANT  */
  YYSYMBOL_ACTIONS = 13,                   /* ACTIONS  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_STATES = 15,                    /* STATES  */
  YYSYMBOL_EVENTS = 16,                    /* EVENTS  */
  YYSYMBOL_RETURNS = 17,                   /* RETURNS  */
  YYSYMBOL_EXTERNAL = 18,                  /* EXTERNAL  */
  YYSYMBOL_VOID = 19,                      /* VOID  */
  YYSYMBOL_IMPLEMENTATION_KEY = 20,        /* IMPLEMENTATION_KEY  */
  YYSYMBOL_INHIBITS = 21,                  /* INHIBITS  */
  YYSYMBOL_SUBMACHINES = 22,               /* SUBMACHINES  */
  YYSYMBOL_ALL = 23,                       /* ALL  */
  YYSYMBOL_ENTRY = 24,                     /* ENTRY  */
  YYSYMBOL_EXIT = 25,                      /* EXIT  */
  YYSYMBOL_STRUCT_KEY = 26,                /* STRUCT_KEY  */
  YYSYMBOL_UNION_KEY = 27,                 /* UNION_KEY  */
  YYSYMBOL_START_KEY = 28,                 /* START_KEY  */
  YYSYMBOL_EVENT_SEQ = 29,                 /* EVENT_SEQ  */
  YYSYMBOL_END_KEY = 30,                   /* END_KEY  */
  YYSYMBOL_SEQUENCE_KEY = 31,              /* SEQUENCE_KEY  */
  YYSYMBOL_ACTION_KEY = 32,                /* ACTION_KEY  */
  YYSYMBOL_TRANSITION_KEY = 33,            /* TRANSITION_KEY  */
  YYSYMBOL_GUARD_KEY = 34,                 /* GUARD_KEY  */
  YYSYMBOL_WHEN_KEY = 35,                  /* WHEN_KEY  */
  YYSYMBOL_OTHERWISE_KEY = 36,             /* OTHERWISE_KEY  */
  YYSYMBOL_PARENT = 37,                    /* PARENT  */
  YYSYMBOL_NATIVE_KEY = 38,                /* NATIVE_KEY  */
  YYSYMBOL_NATIVE_BLOCK = 39,              /* NATIVE_BLOCK  */
  YYSYMBOL_MACHINE = 40,                   /* MACHINE  */
  YYSYMBOL_STATE = 41,                     /* STATE  */
  YYSYMBOL_EVENT = 42,                     /* EVENT  */
  YYSYMBOL_ACTION = 43,                    /* ACTION  */
  YYSYMBOL_TRANSITION_FN = 44,             /* TRANSITION_FN  */
  YYSYMBOL_ID = 45,                        /* ID  */
  YYSYMBOL_NUMERIC_STRING = 46,            /* NUMERIC_STRING  */
  YYSYMBOL_TYPE_NAME = 47,                 /* TYPE_NAME  */
  YYSYMBOL_48_ = 48,                       /* '{'  */
  YYSYMBOL_49_ = 49,                       /* '}'  */
  YYSYMBOL_50_ = 50,                       /* ';'  */
  YYSYMBOL_51_ = 51,                       /* ','  */
  YYSYMBOL_52_ = 52,                       /* '['  */
  YYSYMBOL_53_ = 53,                       /* ']'  */
  YYSYMBOL_54_ = 54,                       /* '('  */
  YYSYMBOL_55_ = 55,                       /* ')'  */
  YYSYMBOL_56_ = 56,                       /* '='  */
  YYSYMBOL_57_ = 57,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 58,                  /* $accept  */
  YYSYMBOL_fsmlang = 59,                   /* fsmlang  */
  YYSYMBOL_machine_prefix = 60,            /* machine_prefix  */
  YYSYMBOL_machine = 61,                   /* machine  */
  YYSYMBOL_62_1 = 62,                      /* $@1  */
  YYSYMBOL_machine_qualifier = 63,         /* machine_qualifier  */
  YYSYMBOL_machine_transition_decl = 64,   /* machine_transition_decl  */
  YYSYMBOL_action_return_spec = 65,        /* action_return_spec  */
  YYSYMBOL_machine_modifier = 66,          /* machine_modifier  */
  YYSYMBOL_statement_decl_list = 67,       /* statement_decl_list  */
  YYSYMBOL_sequence_start = 68,            /* sequence_start  */
  YYSYMBOL_sequence_node = 69,             /* sequence_node  */
  YYSYMBOL_sequence_nodes = 70,            /* sequence_nodes  */
  YYSYMBOL_sequence = 71,                  /* sequence  */
  YYSYMBOL_sequences = 72,                 /* sequences  */
  YYSYMBOL_machine_list = 73,              /* machine_list  */
  YYSYMBOL_actions_and_transitions = 74,   /* actions_and_transitions  */
  YYSYMBOL_transition_matrix_list = 75,    /* transition_matrix_list  */
  YYSYMBOL_transition_matrix_start = 76,   /* transition_matrix_start  */
  YYSYMBOL_guard_matrix_start = 77,        /* guard_matrix_start  */
  YYSYMBOL_transition_matrix = 78,         /* transition_matrix  */
  YYSYMBOL_action_decl = 79,               /* action_decl  */
  YYSYMBOL_action_decl_list = 80,          /* action_decl_list  */
  YYSYMBOL_action = 81,                    /* action  */
  YYSYMBOL_transition = 82,                /* transition  */
  YYSYMBOL_action_matrix = 83,             /* action_matrix  */
  YYSYMBOL_matrix = 84,                    /* matrix  */
  YYSYMBOL_state_vector = 85,              /* state_vector  */
  YYSYMBOL_state_comma_list = 86,          /* state_comma_list  */
  YYSYMBOL_event_vector = 87,              /* event_vector  */
  YYSYMBOL_event_comma_list = 88,          /* event_comma_list  */
  YYSYMBOL_state_and_event_decls = 89,     /* state_and_event_decls  */
  YYSYMBOL_state_decl = 90,                /* state_decl  */
  YYSYMBOL_state = 91,                     /* state  */
  YYSYMBOL_state_decl_list = 92,           /* state_decl_list  */
  YYSYMBOL_event_decl = 93,                /* event_decl  */
  YYSYMBOL_parent_namespace = 94,          /* parent_namespace  */
  YYSYMBOL_user_event_data = 95,           /* user_event_data  */
  YYSYMBOL_event_decl_list = 96,           /* event_decl_list  */
  YYSYMBOL_external_designation = 97,      /* external_designation  */
  YYSYMBOL_native = 98,                    /* native  */
  YYSYMBOL_native_prologue = 99,           /* native_prologue  */
  YYSYMBOL_native_epilogue = 100,          /* native_epilogue  */
  YYSYMBOL_native_impl = 101,              /* native_impl  */
  YYSYMBOL_native_impl_prologue = 102,     /* native_impl_prologue  */
  YYSYMBOL_native_impl_epilogue = 103,     /* native_impl_epilogue  */
  YYSYMBOL_machine_data = 104,             /* machine_data  */
  YYSYMBOL_data_block = 105,               /* data_block  */
  YYSYMBOL_data_fields = 106,              /* data_fields  */
  YYSYMBOL_data_type = 107,                /* data_type  */
  YYSYMBOL_data_field = 108,               /* data_field  */
  YYSYMBOL_data_field_dimension = 109,     /* data_field_dimension  */
  YYSYMBOL_namespace = 110,                /* namespace  */
  YYSYMBOL_namespace_event_ref = 111,      /* namespace_event_ref  */
  YYSYMBOL_returns_comma_list = 112,       /* returns_comma_list  */
  YYSYMBOL_action_return_decl = 113,       /* action_return_decl  */
  YYSYMBOL_transition_fn_return_decl = 114, /* transition_fn_return_decl  */
  YYSYMBOL_return_choice_cond_list = 115,  /* return_choice_cond_list  */
  YYSYMBOL_return_choice_cond = 116        /* return_choice_cond  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   279

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  58
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  152
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  270

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   302


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      54,    55,    57,     2,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    50,
       2,    56,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    52,     2,    53,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   186,   186,   204,   224,   252,   251,   489,   493,   501,
     508,   516,   525,   534,   549,   557,   568,   572,   579,   582,
     588,   600,   612,   628,   645,   652,   659,   667,   676,   689,
     699,   706,   716,   730,   745,   767,   779,   791,   812,   832,
     879,   900,   921,   945,   979,   983,   990,   996,  1018,  1042,
    1064,  1088,  1107,  1126,  1143,  1162,  1181,  1195,  1253,  1296,
    1306,  1320,  1337,  1356,  1373,  1392,  1411,  1436,  1460,  1480,
    1489,  1503,  1519,  1533,  1549,  1558,  1572,  1589,  1607,  1626,
    1635,  1644,  1651,  1661,  1676,  1703,  1717,  1731,  1749,  1763,
    1783,  1798,  1816,  1830,  1842,  1843,  1859,  1873,  1887,  1908,
    1949,  1973,  2015,  2018,  2029,  2032,  2052,  2074,  2082,  2094,
    2106,  2113,  2120,  2130,  2143,  2151,  2161,  2172,  2174,  2185,
    2197,  2209,  2224,  2237,  2250,  2263,  2281,  2303,  2324,  2328,
    2332,  2339,  2340,  2349,  2360,  2370,  2380,  2388,  2399,  2418,
    2437,  2453,  2469,  2488,  2509,  2528,  2543,  2562,  2580,  2588,
    2597,  2611,  2624
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ON", "NAMESPACE",
  "STATE_KEY", "EVENT_KEY", "PROLOGUE_KEY", "EPILOGUE_KEY", "DATA_KEY",
  "TRANSLATOR_KEY", "MACHINE_KEY", "REENTRANT", "ACTIONS", "RETURN",
  "STATES", "EVENTS", "RETURNS", "EXTERNAL", "VOID", "IMPLEMENTATION_KEY",
  "INHIBITS", "SUBMACHINES", "ALL", "ENTRY", "EXIT", "STRUCT_KEY",
  "UNION_KEY", "START_KEY", "EVENT_SEQ", "END_KEY", "SEQUENCE_KEY",
  "ACTION_KEY", "TRANSITION_KEY", "GUARD_KEY", "WHEN_KEY", "OTHERWISE_KEY",
  "PARENT", "NATIVE_KEY", "NATIVE_BLOCK", "MACHINE", "STATE", "EVENT",
  "ACTION", "TRANSITION_FN", "ID", "NUMERIC_STRING", "TYPE_NAME", "'{'",
  "'}'", "';'", "','", "'['", "']'", "'('", "')'", "'='", "'*'", "$accept",
  "fsmlang", "machine_prefix", "machine", "$@1", "machine_qualifier",
  "machine_transition_decl", "action_return_spec", "machine_modifier",
  "statement_decl_list", "sequence_start", "sequence_node",
  "sequence_nodes", "sequence", "sequences", "machine_list",
  "actions_and_transitions", "transition_matrix_list",
  "transition_matrix_start", "guard_matrix_start", "transition_matrix",
  "action_decl", "action_decl_list", "action", "transition",
  "action_matrix", "matrix", "state_vector", "state_comma_list",
  "event_vector", "event_comma_list", "state_and_event_decls",
  "state_decl", "state", "state_decl_list", "event_decl",
  "parent_namespace", "user_event_data", "event_decl_list",
  "external_designation", "native", "native_prologue", "native_epilogue",
  "native_impl", "native_impl_prologue", "native_impl_epilogue",
  "machine_data", "data_block", "data_fields", "data_type", "data_field",
  "data_field_dimension", "namespace", "namespace_event_ref",
  "returns_comma_list", "action_return_decl", "transition_fn_return_decl",
  "return_choice_cond_list", "return_choice_cond", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-145)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-105)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -145,    47,     8,  -145,    28,  -145,  -145,    12,  -145,    13,
      98,  -145,  -145,   123,    61,   120,    12,  -145,  -145,   149,
    -145,  -145,   158,   159,  -145,  -145,    35,   127,    20,   129,
    -145,  -145,   149,  -145,  -145,  -145,  -145,   144,   150,   151,
     152,   160,   164,  -145,   139,  -145,  -145,  -145,  -145,  -145,
    -145,   135,    83,   156,    -7,     5,  -145,   -21,  -145,    64,
     132,  -145,    23,   167,   153,   134,    51,  -145,  -145,  -145,
     -11,  -145,   174,    82,   154,   154,   190,   191,    62,  -145,
      24,    69,  -145,   106,   110,  -145,  -145,   107,   126,  -145,
    -145,  -145,  -145,  -145,   135,  -145,    87,     5,   137,   188,
    -145,   166,   203,   203,   165,   168,  -145,  -145,    -4,    60,
    -145,   173,   182,   189,  -145,    49,  -145,   154,   154,  -145,
     130,  -145,  -145,    93,   177,  -145,     9,   169,  -145,  -145,
    -145,  -145,  -145,   171,   172,   175,   176,   178,  -145,    82,
    -145,  -145,    23,   153,   181,    69,   179,   184,  -145,  -145,
       6,  -145,  -145,    51,    51,  -145,  -145,    79,  -145,  -145,
     186,   192,   193,  -145,   194,  -145,  -145,   108,   121,  -145,
     215,   117,   119,   196,  -145,   197,   122,    99,    72,   199,
     200,   124,   180,   187,   201,    -8,  -145,  -145,  -145,  -145,
    -145,  -145,   203,   203,  -145,  -145,   185,  -145,    38,    41,
    -145,    73,   195,  -145,  -145,   198,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,   128,  -145,  -145,
    -145,   131,   133,   141,  -145,  -145,   138,   155,   142,  -145,
    -145,  -145,  -145,    19,  -145,    40,   202,  -145,  -145,   156,
    -145,  -145,   204,   205,  -145,  -145,  -145,  -145,  -145,  -145,
    -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,  -145,
     206,   208,   209,  -145,  -145,  -145,  -145,  -145,    55,  -145
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     104,   104,     0,     2,    18,     1,     3,     7,    19,     0,
       0,   105,   106,     0,     0,     0,     5,     8,     9,    10,
     110,   111,     0,     0,   107,     4,     0,     0,     0,     0,
      11,    12,    13,   112,   113,   108,   109,     0,     0,     0,
       0,     0,     0,   114,     0,    14,    16,    15,    17,   115,
     116,     0,     0,     0,     0,   104,    79,     0,    80,     0,
       0,    84,    90,     0,   102,     0,     0,   117,    23,     6,
       0,    33,    22,     0,     0,     0,     0,     0,     0,    35,
      37,    20,    39,     0,     0,    46,    38,     0,     0,    81,
      82,    40,    41,    83,     0,    92,     0,   104,     0,     0,
      93,     0,    94,    94,     0,     0,   121,   122,     0,     0,
     119,     0,    27,    24,    29,     0,    34,     0,     0,    57,
      59,    48,    50,     0,     0,    76,     0,     0,    36,    43,
      42,    44,    45,     0,     0,     0,     0,     0,    56,     0,
      47,    49,    91,   102,     0,    21,    86,    88,    85,   103,
       0,    98,    99,     0,     0,   118,   120,     0,   125,    26,
       0,     0,     0,    31,     0,    67,    66,     0,     0,    60,
       0,     0,     0,     0,   131,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,    53,    52,    55,
      54,    58,    94,    94,    87,    89,     0,    97,     0,     0,
     127,     0,     0,    28,    25,     0,    30,    61,    63,    62,
      65,    64,   132,   143,    72,   140,   135,     0,   133,   141,
     134,     0,     0,     0,   152,   145,     0,     0,     0,   147,
     148,    74,    77,     0,    71,     0,     0,   100,   101,    95,
     123,   124,     0,     0,   128,   126,    32,   142,    73,   138,
     137,   139,   136,   151,   150,   144,   146,   149,    78,    75,
       0,     0,     0,    68,    96,   130,   129,    69,     0,    70
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -145,  -145,  -145,     2,  -145,  -145,   216,   226,  -145,  -145,
    -145,    67,  -145,   207,  -145,  -145,   147,   -76,  -145,  -145,
    -145,   -74,  -145,   112,  -145,  -145,   -62,  -145,  -124,  -145,
    -145,   210,   -37,   162,  -145,   -36,   -38,   -99,  -145,   103,
    -145,  -145,  -145,   231,    57,    65,  -145,  -144,    42,  -145,
    -107,  -145,  -145,    75,  -145,   -73,   -72,  -145,    74
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    79,    29,    16,    17,    18,    10,    54,
      70,   114,   115,    71,    72,    80,    81,    82,    83,    84,
      85,    86,    87,   119,   169,   120,    88,   236,   173,   127,
     184,    55,    56,    62,    57,    58,   174,   151,    59,   102,
       4,    11,    12,    19,    20,    21,    60,    67,   108,   109,
     110,   202,   175,   176,   177,    91,    92,   180,   181
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     179,   156,     3,     6,   152,   129,   197,   130,   131,   132,
      51,    52,   121,   122,    65,    13,   196,   111,    89,    90,
      22,    23,   104,   105,    68,    14,    98,    41,    42,    93,
      94,   112,   182,   234,   113,  -104,  -104,    73,    74,    75,
       8,   106,    69,   107,    99,   155,   235,     5,    76,    77,
      15,   183,    24,     7,    66,   165,   166,    78,   144,    43,
      89,    90,  -104,   260,   104,   105,     9,   104,   105,   129,
     258,   130,   131,   132,   259,    27,    33,   104,   105,   162,
      37,   261,   128,   106,    34,   107,   106,   240,   107,    33,
     241,   156,   156,   237,   238,   264,   106,    34,   107,   163,
     164,    73,    74,    75,   125,   157,   248,   223,   224,    25,
     269,   262,    76,    77,    95,    96,   126,   158,   242,   243,
      63,    78,   225,   214,    63,   117,   244,   118,    64,   200,
      63,   201,   143,   170,   171,   172,    63,    51,    52,   170,
      28,   221,    38,    39,    51,    52,    40,   133,    53,   207,
     134,   135,   208,   209,   136,   137,    26,   138,   139,   140,
     141,   146,   147,   167,   168,   210,   211,   213,   214,   215,
     216,   100,   219,   220,   229,   230,   103,    44,   247,   248,
      61,   249,   250,   251,   252,   253,   254,    15,   255,   248,
     223,   224,   256,   257,    45,   198,   199,    35,    36,    49,
      46,    47,    48,    50,    66,    68,    78,   123,   124,   101,
     148,   149,   150,   153,   159,   160,   154,   161,   178,   212,
     185,   186,   187,   193,   194,   188,   189,   203,   190,   195,
     239,   206,    30,   204,   205,   231,   112,   217,   232,   218,
     226,   227,    31,   233,   145,   245,   192,    32,   246,     0,
     268,   191,   222,     0,   228,   263,   142,   265,   266,   214,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,   116
};

static const yytype_int16 yycheck[] =
{
     124,   108,     0,     1,   103,    81,   150,    81,    81,    81,
       5,     6,    74,    75,    52,     3,    10,    28,    55,    55,
       7,     8,    26,    27,    31,    13,     3,     7,     8,    50,
      51,    42,    23,    41,    45,    11,    12,    32,    33,    34,
      12,    45,    49,    47,    21,    49,    54,     0,    43,    44,
      38,    42,    39,    45,    48,   117,   118,    52,    96,    39,
      97,    97,    38,    23,    26,    27,    38,    26,    27,   145,
      51,   145,   145,   145,    55,    14,    19,    26,    27,    30,
      45,    41,    80,    45,    19,    47,    45,    49,    47,    32,
      49,   198,   199,   192,   193,   239,    45,    32,    47,    50,
      51,    32,    33,    34,    42,    45,    51,    35,    36,    11,
      55,   235,    43,    44,    50,    51,    54,    57,    45,    46,
      37,    52,    50,    51,    37,    43,    53,    45,    45,    50,
      37,    52,    45,    40,    41,    42,    37,     5,     6,    40,
      20,    42,    15,    16,     5,     6,    19,    41,     9,    41,
      44,    45,    44,    45,    44,    45,    33,    50,    51,    33,
      34,    24,    25,    33,    34,    44,    45,    50,    51,    50,
      51,     4,    50,    51,    50,    51,    42,    48,    50,    51,
      45,    50,    51,    50,    51,    44,    45,    38,    50,    51,
      35,    36,    50,    51,    50,   153,   154,    39,    39,    39,
      50,    50,    50,    39,    48,    31,    52,    17,    17,    56,
      22,    45,     9,    48,    41,    33,    48,    28,    41,     4,
      51,    50,    50,    42,    45,    50,    50,    41,    50,    45,
      45,   164,    16,    41,    41,    55,    42,    41,    51,    42,
      41,    41,    16,    42,    97,    50,   143,    16,    50,    -1,
      41,   139,   177,    -1,   180,    53,    94,    53,    53,    51,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    59,    60,    61,    98,     0,    61,    45,    12,    38,
      66,    99,   100,     3,    13,    38,    63,    64,    65,   101,
     102,   103,     7,     8,    39,    11,    33,    14,    20,    62,
      64,    65,   101,   102,   103,    39,    39,    45,    15,    16,
      19,     7,     8,    39,    48,    50,    50,    50,    50,    39,
      39,     5,     6,     9,    67,    89,    90,    92,    93,    96,
     104,    45,    91,    37,    45,    94,    48,   105,    31,    49,
      68,    71,    72,    32,    33,    34,    43,    44,    52,    61,
      73,    74,    75,    76,    77,    78,    79,    80,    84,    90,
      93,   113,   114,    50,    51,    50,    51,    89,     3,    21,
       4,    56,    97,    42,    26,    27,    45,    47,   106,   107,
     108,    28,    42,    45,    69,    70,    71,    43,    45,    81,
      83,    84,    84,    17,    17,    42,    54,    87,    61,    75,
      79,   113,   114,    41,    44,    45,    44,    45,    50,    51,
      33,    34,    91,    45,    94,    74,    24,    25,    22,    45,
       9,    95,    95,    48,    48,    49,   108,    45,    57,    41,
      33,    28,    30,    50,    51,    84,    84,    33,    34,    82,
      40,    41,    42,    86,    94,   110,   111,   112,    41,    86,
     115,   116,    23,    42,    88,    51,    50,    50,    50,    50,
      50,    81,    97,    42,    45,    45,    10,   105,   106,   106,
      50,    52,   109,    41,    41,    41,    69,    41,    44,    45,
      44,    45,     4,    50,    51,    50,    51,    41,    42,    50,
      51,    42,   111,    35,    36,    50,    41,    41,   116,    50,
      51,    55,    51,    42,    41,    54,    85,    95,    95,    45,
      49,    49,    45,    46,    53,    50,    50,    50,    51,    50,
      51,    50,    51,    44,    45,    50,    50,    51,    51,    55,
      23,    41,    86,    53,   105,    53,    53,    55,    41,    55
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    58,    59,    59,    60,    62,    61,    63,    63,    63,
      63,    63,    63,    63,    64,    65,    65,    65,    66,    66,
      67,    67,    67,    68,    68,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    73,    73,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    75,    76,    76,    77,
      77,    78,    78,    78,    78,    78,    79,    80,    80,    81,
      81,    82,    82,    82,    82,    82,    83,    83,    84,    85,
      85,    85,    86,    86,    87,    87,    87,    88,    88,    89,
      89,    89,    89,    90,    91,    91,    91,    91,    91,    91,
      92,    92,    93,    94,    95,    95,    95,    95,    96,    96,
      96,    96,    97,    97,    98,    98,    98,    99,    99,   100,
     101,   101,   101,   101,   102,   102,   103,   104,   105,   106,
     106,   107,   107,   107,   107,   107,   108,   108,   109,   109,
     109,   110,   110,   111,   112,   112,   112,   112,   113,   113,
     113,   113,   113,   113,   114,   114,   114,   114,   115,   115,
     116,   116,   116
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     3,     0,     7,     0,     1,     1,
       1,     2,     2,     2,     4,     4,     4,     4,     0,     1,
       2,     3,     2,     1,     2,     4,     3,     1,     3,     1,
       3,     3,     5,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     3,     3,     3,     3,     3,     2,     2,     3,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     5,     3,
       4,     1,     2,     3,     3,     4,     1,     2,     3,     1,
       1,     2,     2,     2,     1,     3,     3,     4,     3,     4,
       2,     3,     2,     2,     0,     3,     4,     2,     4,     4,
       5,     5,     0,     2,     0,     2,     2,     2,     3,     3,
       1,     1,     2,     2,     3,     4,     4,     2,     3,     1,
       2,     1,     1,     4,     4,     2,     4,     3,     2,     3,
       3,     1,     2,     2,     2,     2,     3,     3,     5,     5,
       4,     4,     5,     4,     5,     4,     5,     4,     2,     3,
       3,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* fsmlang: machine  */
#line 187 "../../src/parser.y"
                                        { 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a single machine\n"); 

						#else

						/* write the machine */
            (*pfsmog->writeMachine)(pfsmog,(yyvsp[0].pmachineInfo));
						#endif

						/* get ready for the next machine */
						freeMachineInfo((yyvsp[0].pmachineInfo));

						free_ids(id_list);

					}
#line 1636 "y.tab.c"
    break;

  case 3: /* fsmlang: fsmlang machine  */
#line 205 "../../src/parser.y"
                                        { 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"making a list of machines\n"); 
						#else

						/* write the machine */
            (*pfsmog->writeMachine)(pfsmog,(yyvsp[0].pmachineInfo));

						#endif

						/* get ready for the next machine */
						freeMachineInfo((yyvsp[0].pmachineInfo));

						free_ids(id_list);

					}
#line 1658 "y.tab.c"
    break;

  case 4: /* machine_prefix: native machine_modifier MACHINE_KEY  */
#line 225 "../../src/parser.y"
   {

				if (((yyval.pmachine_prefix) = (pMACHINE_PREFIX)calloc(1,sizeof(MACHINE_PREFIX))) == NULL)
						yyerror("out of memory");

				if (((yyval.pmachine_prefix)->pmachineInfo = (pMACHINE_INFO)calloc(1,sizeof(MACHINE_INFO))) == NULL)
						yyerror("out of memory");

				/* grab any native language stuff */
				if ((yyvsp[-2].pnative_info))
				{
					(yyval.pmachine_prefix)->pmachineInfo->native_prologue = (yyvsp[-2].pnative_info)->prologue;
					(yyval.pmachine_prefix)->pmachineInfo->native_epilogue = (yyvsp[-2].pnative_info)->epilogue;
				}

       /* grab any modifiers */
 			 (yyval.pmachine_prefix)->pmachineInfo->modFlags = (yyvsp[-1].mod_flags);

       id_list = (yyval.pmachine_prefix)->pmachineInfo->id_list = init_list();

        (yyval.pmachine_prefix)->pmachineInfo->parent = pmachineInfo;
				pmachineInfo             = (yyval.pmachine_prefix)->pmachineInfo;

   }
#line 1687 "y.tab.c"
    break;

  case 5: /* $@1: %empty  */
#line 252 "../../src/parser.y"
         {
            if (!((yyvsp[0].pmachine_qualifier)->modFlags & ACTIONS_RETURN_FLAGS))
            {
   						pID_INFO pid_event;
							/* note that this is not added to the machine event list;
								 it is here only to be found as an event id for return
								 decls.
              */
   						add_id(id_list, EVENT,"noEvent",&pid_event);
              pid_event->powningMachine = pmachineInfo;
							pid_event->order          = NO_EVENT; // This makes it easier to detect.
            }

						/* as with 'noEvent', we need this to support return decls */
						pID_INFO pid_state;
						add_id(id_list, STATE, "noTransition",&pid_state);
						pid_state->powningMachine = pmachineInfo;
						pid_state->order          = NO_TRANSITION;  // This makes it easier to detect.

         }
#line 1712 "y.tab.c"
    break;

  case 6: /* machine: machine_prefix ID machine_qualifier $@1 '{' statement_decl_list '}'  */
#line 273 "../../src/parser.y"
                                        {

						(yyval.pmachineInfo)                     = (yyvsp[-6].pmachine_prefix)->pmachineInfo;

				    (yyval.pmachineInfo)->name               = (yyvsp[-5].pid_info);
 			      (yyval.pmachineInfo)->modFlags          |= (yyvsp[-4].pmachine_qualifier)->modFlags;
 			      (yyval.pmachineInfo)->machineTransition  = (yyvsp[-4].pmachine_qualifier)->machineTransition;
            (yyval.pmachineInfo)->native_impl_prologue = (yyvsp[-4].pmachine_qualifier)->native_impl_prologue;
            (yyval.pmachineInfo)->native_impl_epilogue = (yyvsp[-4].pmachine_qualifier)->native_impl_epilogue;


						/* harvest the lists */
 					(yyval.pmachineInfo)->data               = (yyvsp[-1].pstatement_decl_list)->data;
 					(yyval.pmachineInfo)->state_list         = (yyvsp[-1].pstatement_decl_list)->pstate_and_event_decls->state_decls;
 					(yyval.pmachineInfo)->event_list         = (yyvsp[-1].pstatement_decl_list)->pstate_and_event_decls->event_decls;
 					(yyval.pmachineInfo)->action_list        = (yyvsp[-1].pstatement_decl_list)->pactions_and_transitions->action_list;
 					(yyval.pmachineInfo)->action_info_list   = (yyvsp[-1].pstatement_decl_list)->pactions_and_transitions->action_info_list;
 					(yyval.pmachineInfo)->transition_list    = (yyvsp[-1].pstatement_decl_list)->pactions_and_transitions->transition_list;
 					(yyval.pmachineInfo)->transition_fn_list = (yyvsp[-1].pstatement_decl_list)->pactions_and_transitions->transition_fn_list;
 					(yyval.pmachineInfo)->machine_list       = (yyvsp[-1].pstatement_decl_list)->pactions_and_transitions->machine_list;
					(yyval.pmachineInfo)->sequences          = (yyvsp[-1].pstatement_decl_list)->sequences;

						count_external_declarations     ((yyval.pmachineInfo)->event_list,&((yyval.pmachineInfo)->external_event_designation_count));
						count_parent_event_referenced   ((yyval.pmachineInfo)->event_list,&((yyval.pmachineInfo)->parent_event_reference_count));
						count_shared_events             ((yyval.pmachineInfo)->event_list,&((yyval.pmachineInfo)->shared_event_count));
						count_event_user_data_attributes((yyval.pmachineInfo)->event_list
																						 ,&((yyval.pmachineInfo)->data_translator_count)
																						 ,&((yyval.pmachineInfo)->data_block_count)
																						 );

 					/* sanity checks */
 					if ((yyval.pmachineInfo)->parent && (yyval.pmachineInfo)->data_block_count && !output_generated_file_names_only)
					{
 					   yyerror("event user data not allowed in sub-machines");
					}


					count_external_declarations((yyval.pmachineInfo)->state_list
																				,&((yyval.pmachineInfo)->external_state_designation_count)
																				);
 					count_states_with_entry_exit_fns((yyval.pmachineInfo)->state_list
																					 ,&((yyval.pmachineInfo)->states_with_entry_fns_count)
																					 ,&((yyval.pmachineInfo)->states_with_exit_fns_count)
																					 );

          (yyval.pmachineInfo)->executes_fns_on_state_transitions = (
                                                   (((yyval.pmachineInfo)->states_with_entry_fns_count + (yyval.pmachineInfo)->states_with_exit_fns_count) > 0)
                                                   || ((yyval.pmachineInfo)->machineTransition != NULL)
                                                   );
 					if ((yyval.pmachineInfo)->machine_list)
						{
						    count_sub_machine_inhibitors  ((yyval.pmachineInfo)->state_list,&((yyval.pmachineInfo)->submachine_inhibitor_count));
						}

						if (allocateActionArray((yyval.pmachineInfo)))
 						yyerror("out of memory");

 					enumerate_pid_list((yyval.pmachineInfo)->state_list);
 					enumerate_pid_list((yyval.pmachineInfo)->event_list);

						if (populate_action_array((yyval.pmachineInfo), yyout))
 						yyerror("Action array population failed");

					 count_states_with_zero_events((yyval.pmachineInfo)->state_list
																				 , &((yyval.pmachineInfo)->states_with_zero_events)
																				 );
					 count_states_with_one_event((yyval.pmachineInfo)->state_list
																			 , &((yyval.pmachineInfo)->states_with_one_event)
																			 );
					 count_states_with_no_way_in((yyval.pmachineInfo)->state_list
																			 , &((yyval.pmachineInfo)->states_with_no_way_in)
																			 );
					 count_states_with_no_way_out((yyval.pmachineInfo)->state_list
																			 , &((yyval.pmachineInfo)->states_with_no_way_out)
																			 );
					 count_events_with_zero_handlers((yyval.pmachineInfo)->event_list
																					 , &((yyval.pmachineInfo)->events_with_zero_handlers)
																					 );
					 count_events_with_one_handler((yyval.pmachineInfo)->event_list
																				 , &((yyval.pmachineInfo)->events_with_one_handler)
																				 );

					 compute_event_and_state_density_pct((yyval.pmachineInfo));

           free((yyvsp[-6].pmachine_prefix));

           (yyvsp[-5].pid_info)->powningMachine = pmachineInfo;

           /* reset context */
           pmachineInfo = (yyval.pmachineInfo)->parent;
           if ((yyval.pmachineInfo)->parent)
           {
            pID_INFO pid;

            id_list = (yyval.pmachineInfo)->parent->id_list;

            add_id(id_list,MACHINE,(yyvsp[-5].pid_info)->name,&pid);
            pid->powningMachine = (yyval.pmachineInfo);

           }

						#ifdef PARSER_DEBUG

						fprintf(yyout
                   ,"found a machine named %s\n"
                   , (yyval.pmachineInfo)->name->name
                   );
						fprintf(yyout
								,"\twith %d events and %d states\n"
								,(yyval.pmachineInfo)->event_list->count
								,(yyval.pmachineInfo)->state_list->count
								);

						if ((yyval.pmachineInfo)->modFlags & mfReentrant) {

							fprintf(yyout,"The machine is reentrant\n");

						}

						if ((yyval.pmachineInfo)->modFlags & mfActionsReturnStates) {

							fprintf(yyout,"Actions return states\n");

						}
						else if ((yyval.pmachineInfo)->modFlags & mfActionsReturnVoid) {

							fprintf(yyout,"Actions return void\n");

						}
						else {

							fprintf(yyout,"Actions return events\n");

						}

           if ((yyval.pmachineInfo)->machineTransition)
           {
               fprintf(yyout
                       ,"on transition: %s\n"
                       ,(yyval.pmachineInfo)->machineTransition->name
                       );
           }

						fprintf(yyout,"The states :\n");
 					parser_debug_print_state_list((yyval.pmachineInfo)->state_list,yyout);

						fprintf(yyout,"The events :\n");
 					parser_debug_print_event_list((yyval.pmachineInfo)->event_list,yyout);

           if ((yyval.pmachineInfo)->parent_event_reference_count > 0)
           {
						   fprintf(yyout
                      ,"%d events reference the parent machine.\n"
                      , (yyval.pmachineInfo)->parent_event_reference_count);
           }

					 if ((yyval.pmachineInfo)->sequences)
					 {
						 fprintf(yyout
										 , "There %s %d event sequence%s given:\n"
										 , (yyval.pmachineInfo)->sequences->count == 1 ? "is" : "are"
										 , (yyval.pmachineInfo)->sequences->count
										 , (yyval.pmachineInfo)->sequences->count == 1 ? "" : "s"
										 );

						 parser_debug_print_event_sequences((yyval.pmachineInfo), yyout);

					 }

						fprintf(yyout,"The actions :\n");
 					parser_debug_print_action_list_deep((yyval.pmachineInfo)->action_list,(yyval.pmachineInfo),yyout);

           fprintf(yyout,"\nThe %d transitions :\n"
                   , (yyval.pmachineInfo)->transition_list->count
                   );
					  parser_debug_print_transition_list((yyval.pmachineInfo)->transition_list,yyout);

           if ((yyval.pmachineInfo)->transition_fn_list->count)
           {
		           fprintf(yyout,"\nThe %d transition functions :\n"
                      , (yyval.pmachineInfo)->transition_fn_list->count
                      );

								parser_debug_print_transition_fn_list((yyval.pmachineInfo)->transition_fn_list,yyout);
           }

           if ((yyval.pmachineInfo)->data)
           {
             fprintf(yyout
                     , "this machine has data\n"
                     );

             parser_debug_print_data_block((yyval.pmachineInfo)->data, yyout);

           }

            if ((yyval.pmachineInfo)->machine_list)
            {
                fprintf(yyout
                     ,"this machine has %u sub-machines\n"
                     ,(yyval.pmachineInfo)->machine_list->count
                     );

								fprintf(yyout
												, "the sub-machine depth is %u\n"
												, (yyval.pmachineInfo)->sub_machine_depth
												);
            }

						fprintf(yyout,"\n");
						#endif

					}
#line 1930 "y.tab.c"
    break;

  case 7: /* machine_qualifier: %empty  */
#line 489 "../../src/parser.y"
        {
    					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
    						yyerror("Out of memory");
        }
#line 1939 "y.tab.c"
    break;

  case 8: /* machine_qualifier: machine_transition_decl  */
#line 494 "../../src/parser.y"
                     {
 					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					(yyval.pmachine_qualifier)->machineTransition = (yyvsp[0].pid_info);
 				
		     }
#line 1951 "y.tab.c"
    break;

  case 9: /* machine_qualifier: action_return_spec  */
#line 502 "../../src/parser.y"
                     {
 					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					(yyval.pmachine_qualifier)->modFlags = (yyvsp[0].mod_flags);
		     }
#line 1962 "y.tab.c"
    break;

  case 10: /* machine_qualifier: native_impl  */
#line 509 "../../src/parser.y"
                     {
 					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					(yyval.pmachine_qualifier)->native_impl_prologue = (yyvsp[0].pnative_info)->prologue;
 					(yyval.pmachine_qualifier)->native_impl_epilogue = (yyvsp[0].pnative_info)->epilogue;
		     }
#line 1974 "y.tab.c"
    break;

  case 11: /* machine_qualifier: machine_qualifier machine_transition_decl  */
#line 517 "../../src/parser.y"
                     {
           if ((yyvsp[-1].pmachine_qualifier)->machineTransition)
             yyerror("only one machine transition declaration allowed per machine");

 					(yyvsp[-1].pmachine_qualifier)->machineTransition = (yyvsp[0].pid_info);

           (yyval.pmachine_qualifier) = (yyvsp[-1].pmachine_qualifier);
		     }
#line 1987 "y.tab.c"
    break;

  case 12: /* machine_qualifier: machine_qualifier action_return_spec  */
#line 526 "../../src/parser.y"
                     {
           if ((yyvsp[-1].pmachine_qualifier)->modFlags != mfNone)
             yyerror("only one action return spec allowed per machine");

 					(yyvsp[-1].pmachine_qualifier)->modFlags          = (yyvsp[0].mod_flags);

           (yyval.pmachine_qualifier) = (yyvsp[-1].pmachine_qualifier);
		     }
#line 2000 "y.tab.c"
    break;

  case 13: /* machine_qualifier: machine_qualifier native_impl  */
#line 535 "../../src/parser.y"
                     {
           if ((yyvsp[-1].pmachine_qualifier)->native_impl_prologue)
             yyerror("only one native_prologue implementation allowed per machine");

           if ((yyvsp[-1].pmachine_qualifier)->native_impl_epilogue)
             yyerror("only one native_epilogue implementation allowed per machine");

           (yyvsp[-1].pmachine_qualifier)->native_impl_prologue = (yyvsp[0].pnative_info)->prologue;
           (yyvsp[-1].pmachine_qualifier)->native_impl_epilogue = (yyvsp[0].pnative_info)->epilogue;

           (yyval.pmachine_qualifier) = (yyvsp[-1].pmachine_qualifier);
		     }
#line 2017 "y.tab.c"
    break;

  case 14: /* machine_transition_decl: ON TRANSITION_KEY ID ';'  */
#line 550 "../../src/parser.y"
    {
 	 		(yyval.pid_info)          = (yyvsp[-1].pid_info);
 			(yyvsp[-1].pid_info)->docCmnt = (yyvsp[-2].charData);
    }
#line 2026 "y.tab.c"
    break;

  case 15: /* action_return_spec: ACTIONS RETURN EVENTS ';'  */
#line 558 "../../src/parser.y"
        {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(id_list, EVENT,"noEvent",&pid_info);
           pid_info->powningMachine = pmachineInfo;

 					(yyval.mod_flags) = 0;
        }
#line 2041 "y.tab.c"
    break;

  case 16: /* action_return_spec: ACTIONS RETURN STATES ';'  */
#line 569 "../../src/parser.y"
                                        {
 					(yyval.mod_flags) = mfActionsReturnStates;
					}
#line 2049 "y.tab.c"
    break;

  case 17: /* action_return_spec: ACTIONS RETURN VOID ';'  */
#line 573 "../../src/parser.y"
                                        {
						(yyval.mod_flags) = mfActionsReturnVoid;
					}
#line 2057 "y.tab.c"
    break;

  case 18: /* machine_modifier: %empty  */
#line 579 "../../src/parser.y"
        {
 		(yyval.mod_flags) = 0;
	}
#line 2065 "y.tab.c"
    break;

  case 19: /* machine_modifier: REENTRANT  */
#line 583 "../../src/parser.y"
                                        {
						(yyval.mod_flags) = mfReentrant;
					}
#line 2073 "y.tab.c"
    break;

  case 20: /* statement_decl_list: state_and_event_decls actions_and_transitions  */
#line 589 "../../src/parser.y"
        {
    	if (NULL == ((yyval.pstatement_decl_list) = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
    		yyerror("Out of memory");
   
    	(yyval.pstatement_decl_list)->pstate_and_event_decls   = (yyvsp[-1].pstate_and_event_decls);
    	(yyval.pstatement_decl_list)->pactions_and_transitions = (yyvsp[0].pactions_and_transitions);

			/* This allows the event sequence parser to set the proper default for the initial state. */
			pmachineInfo->state_list = (yyvsp[-1].pstate_and_event_decls)->state_decls;
   
   	}
#line 2089 "y.tab.c"
    break;

  case 21: /* statement_decl_list: machine_data state_and_event_decls actions_and_transitions  */
#line 601 "../../src/parser.y"
        {
    	if (NULL == ((yyval.pstatement_decl_list) = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
    		yyerror("Out of memory");
   
      (yyval.pstatement_decl_list)->data                     = (yyvsp[-2].plist);
    	(yyval.pstatement_decl_list)->pstate_and_event_decls   = (yyvsp[-1].pstate_and_event_decls);
    	(yyval.pstatement_decl_list)->pactions_and_transitions = (yyvsp[0].pactions_and_transitions);
   
			/* This allows the event sequence parser to set the proper default for the initial state. */
			pmachineInfo->state_list = (yyvsp[-1].pstate_and_event_decls)->state_decls;
   	}
#line 2105 "y.tab.c"
    break;

  case 22: /* statement_decl_list: statement_decl_list sequences  */
#line 613 "../../src/parser.y"
 {
	#ifdef PARSER_DEBUG
	fprintf(yyout
					,"Found %d sequence%s.\n"
					, (yyvsp[0].plist)->count
					, (yyvsp[0].plist)->count == 1 ? "" : "s"
					);
	#endif

	(yyval.pstatement_decl_list) = (yyvsp[-1].pstatement_decl_list);
	(yyval.pstatement_decl_list)->sequences = (yyvsp[0].plist);

 }
#line 2123 "y.tab.c"
    break;

  case 23: /* sequence_start: SEQUENCE_KEY  */
#line 629 "../../src/parser.y"
        {
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Starting an event sequence\n");
		if ((yyval.psequence))
		{
			fprintf(yyout, "Comment: %s\n", (yyvsp[0].charData));
		}
		#endif

    if (NULL == ((yyval.psequence) = (pEVENT_SEQUENCE)calloc(1, sizeof(EVENT_SEQUENCE))))
    	yyerror("Out of memory");

		(yyval.psequence)->initial_state = statePidByIndex(pmachineInfo, 0);
		(yyval.psequence)->docCmt = (yyvsp[0].charData);

	}
#line 2144 "y.tab.c"
    break;

  case 24: /* sequence_start: sequence_start ID  */
#line 646 "../../src/parser.y"
        {
		(yyval.psequence) = (yyvsp[-1].psequence);
		set_id_type((yyvsp[0].pid_info),EVENT_SEQ);
		(yyval.psequence)->name = (yyvsp[0].pid_info);
		(yyval.psequence)->initial_state = statePidByIndex(pmachineInfo, 0);
	}
#line 2155 "y.tab.c"
    break;

  case 25: /* sequence_start: sequence_start ID START_KEY STATE  */
#line 653 "../../src/parser.y"
        {
		(yyval.psequence) = (yyvsp[-3].psequence);
		set_id_type((yyvsp[-2].pid_info),EVENT_SEQ);
		(yyval.psequence)->name = (yyvsp[-2].pid_info);
		(yyval.psequence)->initial_state = (yyvsp[0].pid_info);
	}
#line 2166 "y.tab.c"
    break;

  case 26: /* sequence_start: sequence_start START_KEY STATE  */
#line 660 "../../src/parser.y"
        {
		(yyval.psequence) = (yyvsp[-2].psequence);
		(yyval.psequence)->initial_state = (yyvsp[0].pid_info);
	}
#line 2175 "y.tab.c"
    break;

  case 27: /* sequence_node: EVENT  */
#line 668 "../../src/parser.y"
  {
    if (((yyval.pevent_sequence_node) = (pEVENT_SEQUENCE_NODE) calloc(1, sizeof(EVENT_SEQUENCE_NODE))) == NULL)
    {
      yyerror("out of memory");
    }

    (yyval.pevent_sequence_node)->pevent = (yyvsp[0].pid_info);
  }
#line 2188 "y.tab.c"
    break;

  case 28: /* sequence_node: EVENT TRANSITION_KEY STATE  */
#line 677 "../../src/parser.y"
  {
    if (((yyval.pevent_sequence_node) = (pEVENT_SEQUENCE_NODE) calloc(1, sizeof(EVENT_SEQUENCE_NODE))) == NULL)
    {
      yyerror("out of memory");
    }

    (yyval.pevent_sequence_node)->pevent = (yyvsp[-2].pid_info);
    (yyval.pevent_sequence_node)->pnew_state = (yyvsp[0].pid_info);
  }
#line 2202 "y.tab.c"
    break;

  case 29: /* sequence_nodes: sequence_node  */
#line 690 "../../src/parser.y"
  {
    if (((yyval.plist) = init_list()) == NULL)
    {
      yyerror("out of memory");
    }

    add_to_list((yyval.plist), (yyvsp[0].pevent_sequence_node));

  }
#line 2216 "y.tab.c"
    break;

  case 30: /* sequence_nodes: sequence_nodes ',' sequence_node  */
#line 700 "../../src/parser.y"
  {
    add_to_list((yyvsp[-2].plist), (yyvsp[0].pevent_sequence_node));
    (yyval.plist) = (yyvsp[-2].plist);
  }
#line 2225 "y.tab.c"
    break;

  case 31: /* sequence: sequence_start sequence_nodes ';'  */
#line 707 "../../src/parser.y"
        {
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found an event sequence\n");
		#endif

		(yyval.psequence) = (yyvsp[-2].psequence);
		(yyval.psequence)->sequence = (yyvsp[-1].plist);

	}
#line 2239 "y.tab.c"
    break;

  case 32: /* sequence: sequence_start sequence_nodes END_KEY STATE ';'  */
#line 717 "../../src/parser.y"
        {
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found an event sequence indicating an end state\n");
		#endif

		(yyval.psequence) = (yyvsp[-4].psequence);
		(yyval.psequence)->sequence    = (yyvsp[-3].plist);
		(yyval.psequence)->final_state = (yyvsp[-1].pid_info);

	}
#line 2254 "y.tab.c"
    break;

  case 33: /* sequences: sequence  */
#line 731 "../../src/parser.y"
        {
		if (((yyval.plist) = init_list()) == NULL)
    		yyerror("Out of memory");

		add_to_list((yyval.plist), (yyvsp[0].psequence));

		if ((yyvsp[0].psequence)->name == NULL)
		{
				//this is the first sequence in the list
				char *sn;
				add_id(id_list, ID,sn = create_sequence_name(0),&(yyvsp[0].psequence)->name);
				CHECK_AND_FREE(sn);
		}
	}
#line 2273 "y.tab.c"
    break;

  case 34: /* sequences: sequences sequence  */
#line 746 "../../src/parser.y"
        {
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found another event sequence\n");
		#endif

		add_to_list((yyvsp[-1].plist), (yyvsp[0].psequence));

		if ((yyvsp[0].psequence)->name == NULL)
		{
				char *sn;
				add_id(id_list, ID,sn = create_sequence_name((yyvsp[-1].plist)->count-1),&(yyvsp[0].psequence)->name);
				CHECK_AND_FREE(sn);
		}

		(yyval.plist) = (yyvsp[-1].plist);

	}
#line 2295 "y.tab.c"
    break;

  case 35: /* machine_list: machine  */
#line 768 "../../src/parser.y"
    {
        if (NULL == ((yyval.plist) = init_list()))
            yyerror("out of memory");

        if (NULL == add_to_list((yyval.plist),(yyvsp[0].pmachineInfo)))
            yyerror("out of memory");

				/* Bump the depth count. The function is a no-op if we're at the top. */
				increase_sub_machine_depth((yyvsp[0].pmachineInfo)->parent);

    }
#line 2311 "y.tab.c"
    break;

  case 36: /* machine_list: machine_list machine  */
#line 780 "../../src/parser.y"
    {

        (yyval.plist) = (yyvsp[-1].plist);

        if (NULL == add_to_list((yyval.plist),(yyvsp[0].pmachineInfo)))
            yyerror("out of memory");

    }
#line 2324 "y.tab.c"
    break;

  case 37: /* actions_and_transitions: machine_list  */
#line 792 "../../src/parser.y"
                {
 		if (NULL == ((yyval.pactions_and_transitions) = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");


 		if (NULL == ((yyval.pactions_and_transitions)->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_fn_list = init_list()))
 			yyerror("out of memory");

    (yyval.pactions_and_transitions)->machine_list = (yyvsp[0].plist);

		}
#line 2349 "y.tab.c"
    break;

  case 38: /* actions_and_transitions: action_decl  */
#line 813 "../../src/parser.y"
                {
 		if (NULL == ((yyval.pactions_and_transitions) = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

			(yyval.pactions_and_transitions)->action_list        = (yyvsp[0].paction_decl)->action_list        ? (yyvsp[0].paction_decl)->action_list        : init_list();
			(yyval.pactions_and_transitions)->action_info_list   = (yyvsp[0].paction_decl)->action_info_list   ? (yyvsp[0].paction_decl)->action_info_list   : init_list();
			(yyval.pactions_and_transitions)->transition_list    = (yyvsp[0].paction_decl)->transition_list    ? (yyvsp[0].paction_decl)->transition_list    : init_list();
			(yyval.pactions_and_transitions)->transition_fn_list = (yyvsp[0].paction_decl)->transition_fn_list ? (yyvsp[0].paction_decl)->transition_fn_list : init_list();

			if (
 				   (NULL == (yyval.pactions_and_transitions)->action_list)
 				|| (NULL == (yyval.pactions_and_transitions)->action_info_list)
 				|| (NULL == (yyval.pactions_and_transitions)->transition_list)
 				|| (NULL == (yyval.pactions_and_transitions)->transition_fn_list)
					)
 				yyerror("out of memory");

 		free((yyvsp[0].paction_decl));
		}
#line 2373 "y.tab.c"
    break;

  case 39: /* actions_and_transitions: transition_matrix_list  */
#line 833 "../../src/parser.y"
          {
 		if (NULL == ((yyval.pactions_and_transitions) = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_fn_list = init_list()))
 			yyerror("out of memory");

			if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->action_list, (yyvsp[0].action_info)->action))
 				yyerror("out of memory");

			if (NULL == add_to_list((yyval.pactions_and_transitions)->action_info_list, (yyvsp[0].action_info)))
 			yyerror("out of memory");

			if ((yyvsp[0].action_info)->transition)
			{
				switch ((yyvsp[0].action_info)->transition->name->type)
				{
 				case STATE:
						if (NULL == add_unique_to_list_with_test((yyval.pactions_and_transitions)->transition_list
                                                     , (yyvsp[0].action_info)->transition
                                                     , match_transition
                                                     ))
 						yyerror("out of memory");
 					break;

 				case TRANSITION_FN:
						if (NULL == add_unique_to_list_with_test((yyval.pactions_and_transitions)->transition_fn_list
                                                     , (yyvsp[0].action_info)->transition
                                                     , match_transition
                                                     ))
 						yyerror("out of memory");
 					break;

					//there are no other choices
				}
			}
	  }
#line 2424 "y.tab.c"
    break;

  case 40: /* actions_and_transitions: action_return_decl  */
#line 880 "../../src/parser.y"
          {
 		if (NULL == ((yyval.pactions_and_transitions) = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_fn_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->machine_list = init_list()))
 			yyerror("out of memory");

   }
#line 2449 "y.tab.c"
    break;

  case 41: /* actions_and_transitions: transition_fn_return_decl  */
#line 901 "../../src/parser.y"
   {
 		if (NULL == ((yyval.pactions_and_transitions) = (pACTIONS_AND_TRANSITIONS)calloc(1,sizeof(ACTIONS_AND_TRANSITIONS))))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_list = init_list()))
	 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->action_info_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->transition_fn_list = init_list()))
 			yyerror("out of memory");

 		if (NULL == ((yyval.pactions_and_transitions)->machine_list = init_list()))
 			yyerror("out of memory");

   }
#line 2474 "y.tab.c"
    break;

  case 42: /* actions_and_transitions: actions_and_transitions action_decl  */
#line 922 "../../src/parser.y"
          {
 		(yyval.pactions_and_transitions) = (yyvsp[-1].pactions_and_transitions);

 		move_list_unique((yyvsp[-1].pactions_and_transitions)->action_list, (yyvsp[0].paction_decl)->action_list);
 		free_list((yyvsp[0].paction_decl)->action_list);

 		move_list((yyvsp[-1].pactions_and_transitions)->action_info_list, (yyvsp[0].paction_decl)->action_info_list);
 		free((yyvsp[0].paction_decl)->action_info_list);

 		move_list_unique_with_test((yyvsp[-1].pactions_and_transitions)->transition_list
                               , (yyvsp[0].paction_decl)->transition_list
                               , match_transition
                               );
 		free_list((yyvsp[0].paction_decl)->transition_list);

 		move_list_unique_with_test((yyvsp[-1].pactions_and_transitions)->transition_fn_list
                               , (yyvsp[0].paction_decl)->transition_fn_list
                               , match_transition
                               );
 		free_list((yyvsp[0].paction_decl)->transition_fn_list);

 		free((yyvsp[0].paction_decl));
	  }
#line 2502 "y.tab.c"
    break;

  case 43: /* actions_and_transitions: actions_and_transitions transition_matrix_list  */
#line 946 "../../src/parser.y"
          {
 		(yyval.pactions_and_transitions)=(yyvsp[-1].pactions_and_transitions);

			if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->action_list, (yyvsp[0].action_info)->action))
 				yyerror("out of memory");

			if (NULL == add_to_list((yyval.pactions_and_transitions)->action_info_list, (yyvsp[0].action_info)))
 			yyerror("out of memory");

			if ((yyvsp[0].action_info)->transition)
			{
				switch ((yyvsp[0].action_info)->transition->name->type)
				{
 				case STATE:
						if (NULL == add_unique_to_list_with_test((yyval.pactions_and_transitions)->transition_list
                                                     , (yyvsp[0].action_info)->transition
                                                     , match_transition
                                                     ))
 						yyerror("out of memory");
 					break;

 				case TRANSITION_FN:
						if (NULL == add_unique_to_list_with_test((yyval.pactions_and_transitions)->transition_fn_list
                                                     , (yyvsp[0].action_info)->transition
                                                     , match_transition
                                                     ))
 						yyerror("out of memory");
 					break;

					//there are no other choices
				}
			}
	  }
#line 2540 "y.tab.c"
    break;

  case 44: /* actions_and_transitions: actions_and_transitions action_return_decl  */
#line 980 "../../src/parser.y"
          {
			(yyval.pactions_and_transitions) = (yyvsp[-1].pactions_and_transitions);
	  }
#line 2548 "y.tab.c"
    break;

  case 45: /* actions_and_transitions: actions_and_transitions transition_fn_return_decl  */
#line 984 "../../src/parser.y"
          {
			(yyval.pactions_and_transitions) = (yyvsp[-1].pactions_and_transitions);
	  }
#line 2556 "y.tab.c"
    break;

  case 46: /* transition_matrix_list: transition_matrix  */
#line 991 "../../src/parser.y"
                                        {
						(yyval.action_info) = (yyvsp[0].action_info);
					}
#line 2564 "y.tab.c"
    break;

  case 47: /* transition_matrix_start: matrix TRANSITION_KEY  */
#line 997 "../../src/parser.y"
        {
						pID_INFO pid_info;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->matrix     = (yyvsp[-1].matrix_info);
						(yyval.action_info)->action     = pid_info;
 					(yyval.action_info)->docCmnt    = (yyvsp[0].charData);

						(yyval.action_info)->nextAction = pid_info->type_data.action_data.actionInfo;
						pid_info->type_data.action_data.actionInfo = (yyval.action_info);

        }
#line 2590 "y.tab.c"
    break;

  case 48: /* transition_matrix_start: TRANSITION_KEY matrix  */
#line 1019 "../../src/parser.y"
        {
						pID_INFO pid_info;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->matrix     = (yyvsp[0].matrix_info);
						(yyval.action_info)->action     = pid_info;
 					(yyval.action_info)->docCmnt    = (yyvsp[-1].charData);

						(yyval.action_info)->nextAction = pid_info->type_data.action_data.actionInfo;
						pid_info->type_data.action_data.actionInfo = (yyval.action_info);

        }
#line 2616 "y.tab.c"
    break;

  case 49: /* guard_matrix_start: matrix GUARD_KEY  */
#line 1043 "../../src/parser.y"
        {
						pID_INFO pid_info;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->matrix     = (yyvsp[-1].matrix_info);
						(yyval.action_info)->action     = pid_info;
 					(yyval.action_info)->docCmnt    = (yyvsp[0].charData);

						(yyval.action_info)->nextAction = pid_info->type_data.action_data.actionInfo;
						pid_info->type_data.action_data.actionInfo = (yyval.action_info);

        }
#line 2642 "y.tab.c"
    break;

  case 50: /* guard_matrix_start: GUARD_KEY matrix  */
#line 1065 "../../src/parser.y"
        {
						pID_INFO pid_info;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(id_list, ACTION,"",&pid_info);
           pid_info->powningMachine = pmachineInfo;

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->matrix     = (yyvsp[0].matrix_info);
						(yyval.action_info)->action     = pid_info;
 					(yyval.action_info)->docCmnt    = (yyvsp[-1].charData);

						(yyval.action_info)->nextAction = pid_info->type_data.action_data.actionInfo;
						pid_info->type_data.action_data.actionInfo = (yyval.action_info);

        }
#line 2668 "y.tab.c"
    break;

  case 51: /* transition_matrix: transition_matrix_start STATE ';'  */
#line 1089 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix\n");
						#endif

           (yyval.action_info) = (yyvsp[-2].action_info);

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.action_info)->transition = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->transition->name = (yyvsp[-1].pid_info);

					}
#line 2691 "y.tab.c"
    break;

  case 52: /* transition_matrix: transition_matrix_start ID ';'  */
#line 1108 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type((yyvsp[-1].pid_info),TRANSITION_FN);

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.action_info)->transition = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->transition->name = (yyvsp[-1].pid_info);

					}
#line 2714 "y.tab.c"
    break;

  case 53: /* transition_matrix: transition_matrix_start TRANSITION_FN ';'  */
#line 1127 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.action_info)->transition = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->transition->name = (yyvsp[-1].pid_info);

        }
#line 2735 "y.tab.c"
    break;

  case 54: /* transition_matrix: guard_matrix_start ID ';'  */
#line 1144 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type((yyvsp[-1].pid_info),TRANSITION_FN);

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.action_info)->transition = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->transition->name = (yyvsp[-1].pid_info);

					}
#line 2758 "y.tab.c"
    break;

  case 55: /* transition_matrix: guard_matrix_start TRANSITION_FN ';'  */
#line 1163 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.action_info)->transition = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->transition->name = (yyvsp[-1].pid_info);

        }
#line 2779 "y.tab.c"
    break;

  case 56: /* action_decl: action_decl_list ';'  */
#line 1182 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the actions */
						fprintf(yyout,"The actions in this list:\n");
						parser_debug_print_id_list_names((yyvsp[-1].paction_decl)->action_list,pmachineInfo,yyout,"noAction");
						#endif

						(yyval.paction_decl) = (yyvsp[-1].paction_decl);

					}
#line 2795 "y.tab.c"
    break;

  case 57: /* action_decl_list: ACTION_KEY action  */
#line 1196 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"started an action declaration\n");
						#endif

 					if (NULL == ((yyval.paction_decl) = ((pACTION_DECL) calloc(1,sizeof(ACTION_DECL)))))
 						yyerror("out of memory");

 					if (NULL == ((yyval.paction_decl)->action_list = init_list()))
 						yyerror("out of memory");

 					if (NULL == ((yyval.paction_decl)->action_info_list = init_list()))
 						yyerror("out of memory");

 					if (NULL == ((yyval.paction_decl)->transition_list = init_list()))
 						yyerror("out of memory");

 					if (NULL == ((yyval.paction_decl)->transition_fn_list = init_list()))
 						yyerror("out of memory");

						if (NULL == add_unique_to_list((yyval.paction_decl)->action_list, (yyvsp[0].action_info)->action))
 						yyerror("out of memory");

						if (NULL == add_to_list((yyval.paction_decl)->action_info_list, (yyvsp[0].action_info)))
 						yyerror("out of memory");

						if ((yyvsp[0].action_info)->transition)
						{

							switch ((yyvsp[0].action_info)->transition->name->type)
							{
 							case STATE:
						      if (NULL == add_unique_to_list_with_test((yyval.paction_decl)->transition_list
                                                           , (yyvsp[0].action_info)->transition
                                                           , match_transition
                                                           ))
 									yyerror("out of memory");
 								break;

 							case TRANSITION_FN:
						      if (NULL == add_unique_to_list_with_test((yyval.paction_decl)->transition_fn_list
                                                           , (yyvsp[0].action_info)->transition
                                                           , match_transition
                                                           ))
 									yyerror("out of memory");
 								break;

								//there are no other choices
							}

						}

 					(yyvsp[0].action_info)->docCmnt = (yyvsp[-1].charData);

					}
#line 2856 "y.tab.c"
    break;

  case 58: /* action_decl_list: action_decl_list ',' action  */
#line 1254 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another action declarations\n");
						#endif

						(yyval.paction_decl) = (yyvsp[-2].paction_decl);

						if (NULL == add_unique_to_list((yyval.paction_decl)->action_list, (yyvsp[0].action_info)->action))
 						yyerror("out of memory");

						if (NULL == add_to_list((yyval.paction_decl)->action_info_list, (yyvsp[0].action_info)))
 						yyerror("out of memory");

						if ((yyvsp[0].action_info)->transition)
						{
							switch ((yyvsp[0].action_info)->transition->name->type)
							{
 							case STATE:
						      if (NULL == add_unique_to_list_with_test((yyval.paction_decl)->transition_list
                                                           , (yyvsp[0].action_info)->transition
                                                           , match_transition
                                                           ))
 									      yyerror("out of memory");
 								break;

 							case TRANSITION_FN:
						    if (NULL == add_unique_to_list_with_test((yyval.paction_decl)->transition_fn_list
                                                         , (yyvsp[0].action_info)->transition
                                                         , match_transition
                                                         ))
 									yyerror("out of memory");
 								break;

								//there are no other choices
							}

						}

					}
#line 2901 "y.tab.c"
    break;

  case 59: /* action: action_matrix  */
#line 1297 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action without transition\n");
						#endif

						(yyval.action_info) = (yyvsp[0].action_info);

					}
#line 2915 "y.tab.c"
    break;

  case 60: /* action: action_matrix transition  */
#line 1307 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action with transition\n");
						#endif

						(yyvsp[-1].action_info)->transition = (yyvsp[0].ptransition);

						(yyval.action_info) = (yyvsp[-1].action_info);

					}
#line 2931 "y.tab.c"
    break;

  case 61: /* transition: TRANSITION_KEY STATE  */
#line 1321 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition to known state\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.ptransition) = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.ptransition)->name = (yyvsp[0].pid_info);

					}
#line 2952 "y.tab.c"
    break;

  case 62: /* transition: TRANSITION_KEY ID  */
#line 1338 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.ptransition) = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

           set_id_type((yyvsp[0].pid_info),TRANSITION_FN);

						(yyval.ptransition)->name = (yyvsp[0].pid_info);

					}
#line 2975 "y.tab.c"
    break;

  case 63: /* transition: TRANSITION_KEY TRANSITION_FN  */
#line 1357 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.ptransition) = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.ptransition)->name = (yyvsp[0].pid_info);

					}
#line 2996 "y.tab.c"
    break;

  case 64: /* transition: GUARD_KEY ID  */
#line 1374 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.ptransition) = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

           set_id_type((yyvsp[0].pid_info),TRANSITION_FN);
           
						(yyval.ptransition)->name = (yyvsp[0].pid_info);

					}
#line 3019 "y.tab.c"
    break;

  case 65: /* transition: GUARD_KEY TRANSITION_FN  */
#line 1393 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						/* 
							grab an TRANSITION_DATA struct
						*/
						if (((yyval.ptransition) = (pTRANSITION_DATA)calloc(1,sizeof(TRANSITION_DATA))) == NULL)

							yyerror("out of memory");

						(yyval.ptransition)->name = (yyvsp[0].pid_info);

					}
#line 3040 "y.tab.c"
    break;

  case 66: /* action_matrix: ID matrix  */
#line 1412 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action matrix\n");
						#endif

           set_id_type((yyvsp[-1].pid_info),ACTION);
           (yyvsp[-1].pid_info)->powningMachine = pmachineInfo;

						/* 
							grab an ACTION_INFO struct
						*/
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->action     = (yyvsp[-1].pid_info);
						(yyval.action_info)->matrix     = (yyvsp[0].matrix_info);
						(yyval.action_info)->transition = NULL;

						(yyval.action_info)->nextAction = (yyvsp[-1].pid_info)->type_data.action_data.actionInfo;
						(yyvsp[-1].pid_info)->type_data.action_data.actionInfo = (yyval.action_info);

					}
#line 3069 "y.tab.c"
    break;

  case 67: /* action_matrix: ACTION matrix  */
#line 1437 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action matrix\n");
						#endif

						/* 
							grab an ACTION_INFO struct
						*/
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->action     = (yyvsp[-1].pid_info);
						(yyval.action_info)->matrix     = (yyvsp[0].matrix_info);
						(yyval.action_info)->transition = NULL;

						(yyval.action_info)->nextAction = (yyvsp[-1].pid_info)->type_data.action_data.actionInfo;
						(yyvsp[-1].pid_info)->type_data.action_data.actionInfo = (yyval.action_info);

					}
#line 3095 "y.tab.c"
    break;

  case 68: /* matrix: '[' event_vector ',' state_vector ']'  */
#line 1461 "../../src/parser.y"
     {
         #ifdef PARSER_DEBUG
         fprintf(yyout,"found a matrix\n");
         #endif

         /*
            grab a MATRIX_INFO struct
         */
					if (((yyval.matrix_info) = (pMATRIX_INFO)malloc(sizeof(MATRIX_INFO))) == NULL)

						yyerror("out of memory");

         (yyval.matrix_info)->event_list = (yyvsp[-3].plist);
         (yyval.matrix_info)->state_list = (yyvsp[-1].plist);

     }
#line 3116 "y.tab.c"
    break;

  case 69: /* state_vector: '(' ALL ')'  */
#line 1481 "../../src/parser.y"
                      {
 					if (((yyval.plist) = init_list()) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an all state vector\n");
 					#endif
		      }
#line 3129 "y.tab.c"
    break;

  case 70: /* state_vector: '(' state_comma_list STATE ')'  */
#line 1490 "../../src/parser.y"
                                        {

						(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state vector\n");
 					parser_debug_print_id_list_names((yyval.plist),pmachineInfo,yyout,"");
						#endif

					}
#line 3147 "y.tab.c"
    break;

  case 71: /* state_vector: STATE  */
#line 1504 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state scalar\n");
						#endif

						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[0].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3165 "y.tab.c"
    break;

  case 72: /* state_comma_list: STATE ','  */
#line 1520 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the beginning of a state comma list\n");
						#endif

						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3183 "y.tab.c"
    break;

  case 73: /* state_comma_list: state_comma_list STATE ','  */
#line 1534 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of a state comma list\n");
						#endif

						(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3200 "y.tab.c"
    break;

  case 74: /* event_vector: '(' ALL ')'  */
#line 1550 "../../src/parser.y"
                      {
 					if (((yyval.plist) = init_list()) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an all event vector\n");
 					#endif
		      }
#line 3213 "y.tab.c"
    break;

  case 75: /* event_vector: '(' event_comma_list EVENT ')'  */
#line 1559 "../../src/parser.y"
                                        {

						(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an event vector\n");
 					parser_debug_print_id_list_names((yyval.plist),pmachineInfo,yyout,"");
						#endif

					}
#line 3231 "y.tab.c"
    break;

  case 76: /* event_vector: EVENT  */
#line 1573 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a scalar event : %s\n",(yyvsp[0].pid_info)->name);
						#endif

						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[0].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3249 "y.tab.c"
    break;

  case 77: /* event_comma_list: EVENT ','  */
#line 1590 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the begining of an event comma list: %s\n",(yyvsp[-1].pid_info)->name);
						#endif

           if ((yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines)
             yyerror("events which are shared by sub-machines may not be part of a vector");

						/* start a list */
						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3271 "y.tab.c"
    break;

  case 78: /* event_comma_list: event_comma_list EVENT ','  */
#line 1608 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of an event comma list. adding %s\n",(yyvsp[-1].pid_info)->name);
						#endif

           if ((yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines)
             yyerror("events which are shared by sub-machines may not be part of a vector");

 					(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3291 "y.tab.c"
    break;

  case 79: /* state_and_event_decls: state_decl  */
#line 1627 "../../src/parser.y"
                {
 	   if (NULL == ((yyval.pstate_and_event_decls) = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 (yyval.pstate_and_event_decls)->state_decls = (yyvsp[0].plist);
 		 (yyval.pstate_and_event_decls)->event_decls = init_list();
      psedecls = (yyval.pstate_and_event_decls);
		}
#line 3304 "y.tab.c"
    break;

  case 80: /* state_and_event_decls: event_decl  */
#line 1636 "../../src/parser.y"
                {
 	   if (NULL == ((yyval.pstate_and_event_decls) = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 (yyval.pstate_and_event_decls)->event_decls = (yyvsp[0].plist);
 		 (yyval.pstate_and_event_decls)->state_decls = init_list();
      psedecls = (yyval.pstate_and_event_decls);
		}
#line 3317 "y.tab.c"
    break;

  case 81: /* state_and_event_decls: state_and_event_decls state_decl  */
#line 1645 "../../src/parser.y"
          {
 		(yyval.pstate_and_event_decls) = (yyvsp[-1].pstate_and_event_decls);

			(yyval.pstate_and_event_decls)->state_decls = move_list((yyval.pstate_and_event_decls)->state_decls,(yyvsp[0].plist));
      psedecls = (yyval.pstate_and_event_decls);
	  }
#line 3328 "y.tab.c"
    break;

  case 82: /* state_and_event_decls: state_and_event_decls event_decl  */
#line 1652 "../../src/parser.y"
          {
 		(yyval.pstate_and_event_decls) = (yyvsp[-1].pstate_and_event_decls);

			(yyval.pstate_and_event_decls)->event_decls = move_list((yyval.pstate_and_event_decls)->event_decls,(yyvsp[0].plist));
      psedecls = (yyval.pstate_and_event_decls);

	  }
#line 3340 "y.tab.c"
    break;

  case 83: /* state_decl: state_decl_list ';'  */
#line 1662 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the states */
						fprintf(yyout,"The %d states in this list :\n",(yyvsp[-1].plist)->count);

 					parser_debug_print_state_list((yyvsp[-1].plist),yyout);
						#endif

						(yyval.plist) = (yyvsp[-1].plist);

					}
#line 3357 "y.tab.c"
    break;

  case 84: /* state: ID  */
#line 1677 "../../src/parser.y"
         {
 				  (yyval.pid_info) = (yyvsp[0].pid_info);
           set_id_type((yyval.pid_info),STATE);
           (yyval.pid_info)->powningMachine = pmachineInfo;

					 if (NULL == ((yyval.pid_info)->type_data.state_data.pinbound_transitions = init_list()))
					 {
					    yyerror("out of memory");
					 }

					 if (NULL == ((yyval.pid_info)->type_data.state_data.poutbound_transitions = init_list()))
					 {
					    yyerror("out of memory");
					 }

					 if (NULL == ((yyval.pid_info)->type_data.state_data.pevents_handled = init_list()))
					 {
					    yyerror("out of memory");
					 }

					 if (NULL == ((yyval.pid_info)->type_data.state_data.pactions_list = init_list()))
					 {
					    yyerror("out of memory");
					 }

         }
#line 3388 "y.tab.c"
    break;

  case 85: /* state: state INHIBITS SUBMACHINES  */
#line 1704 "../../src/parser.y"
                                        {
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s inhibits submachines\n"
 									, (yyvsp[-2].pid_info)->name
										);
         	#endif

 					(yyval.pid_info) = (yyvsp[-2].pid_info);

 					(yyval.pid_info)->type_data.state_data.state_flags |= sfInibitSubMachines;

		      }
#line 3406 "y.tab.c"
    break;

  case 86: /* state: state ON ENTRY  */
#line 1718 "../../src/parser.y"
                                        {
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has anonymous entry function\n"
 									, (yyvsp[-2].pid_info)->name
										);
         	#endif

 					(yyval.pid_info) = (yyvsp[-2].pid_info);

 					(yyval.pid_info)->type_data.state_data.state_flags |= sfHasEntryFn;

		      }
#line 3424 "y.tab.c"
    break;

  case 87: /* state: state ON ENTRY ID  */
#line 1732 "../../src/parser.y"
                                        {
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has entry function %s\n"
 									, (yyvsp[-3].pid_info)->name
 									, (yyvsp[0].pid_info)->name
										);
         	#endif

 					(yyval.pid_info) = (yyvsp[-3].pid_info);

 					(yyval.pid_info)->type_data.state_data.state_flags |= sfHasEntryFn;
 					(yyval.pid_info)->type_data.state_data.entry_fn     = (yyvsp[0].pid_info);

 					set_id_type((yyvsp[0].pid_info), ENTRY);

		      }
#line 3446 "y.tab.c"
    break;

  case 88: /* state: state ON EXIT  */
#line 1750 "../../src/parser.y"
                                        {
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has anonymous exit function\n"
 									, (yyvsp[-2].pid_info)->name
										);
         	#endif

 					(yyval.pid_info) = (yyvsp[-2].pid_info);

 					(yyval.pid_info)->type_data.state_data.state_flags |= sfHasExitFn;

		      }
#line 3464 "y.tab.c"
    break;

  case 89: /* state: state ON EXIT ID  */
#line 1764 "../../src/parser.y"
                                        {
         	#ifdef PARSER_DEBUG
         	fprintf(yyout
										, "state %s has exit function %s\n"
 									, (yyvsp[-3].pid_info)->name
 									, (yyvsp[0].pid_info)->name
										);
         	#endif

 					(yyval.pid_info) = (yyvsp[-3].pid_info);

 					(yyval.pid_info)->type_data.state_data.state_flags |= sfHasExitFn;
 					(yyval.pid_info)->type_data.state_data.exit_fn     = (yyvsp[0].pid_info);

 					set_id_type((yyvsp[0].pid_info), EXIT);

		      }
#line 3486 "y.tab.c"
    break;

  case 90: /* state_decl_list: STATE_KEY state  */
#line 1784 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the start of a state declaration list\n");
						#endif

 					if (NULL == ((yyval.plist) = init_list()))
 						yyerror("Out of memory");


 					if (NULL == (add_to_list((yyval.plist),(yyvsp[0].pid_info))))
 						yyerror("Out of memory");

					}
#line 3505 "y.tab.c"
    break;

  case 91: /* state_decl_list: state_decl_list ',' state  */
#line 1799 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the state declaration list\n");
						#endif

						(yyval.plist) = (yyvsp[-2].plist);

           set_id_type((yyvsp[0].pid_info),STATE);
           (yyvsp[0].pid_info)->powningMachine = pmachineInfo;

 					if (NULL == (add_to_list((yyval.plist),(yyvsp[0].pid_info))))
 						yyerror("Out of memory");

					}
#line 3525 "y.tab.c"
    break;

  case 92: /* event_decl: event_decl_list ';'  */
#line 1817 "../../src/parser.y"
                                        {

						(yyval.plist) = (yyvsp[-1].plist);

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the events */
						fprintf(yyout,"The %d events in this list:\n",(yyval.plist)->count);
 					parser_debug_print_event_list((yyval.plist),yyout);
						#endif

					}
#line 3541 "y.tab.c"
    break;

  case 93: /* parent_namespace: PARENT NAMESPACE  */
#line 1831 "../../src/parser.y"
  {
    if (!pmachineInfo->parent)
        yyerror("parent namespace invoked in top-level machine");

    id_list = pmachineInfo->parent->id_list;

    /* this picks up any doc comment */
 	 (yyval.charData) = (yyvsp[-1].charData);
  }
#line 3555 "y.tab.c"
    break;

  case 94: /* user_event_data: %empty  */
#line 1842 "../../src/parser.y"
                 { (yyval.puser_event_data) = NULL; }
#line 3561 "y.tab.c"
    break;

  case 95: /* user_event_data: DATA_KEY TRANSLATOR_KEY ID  */
#line 1844 "../../src/parser.y"
   {
		if (pmachineInfo->parent && !pmachineInfo->parent->data) 
		{
			yyerror("data translator declared for sub-machine having parent with no data");
		}

  		if (NULL == ((yyval.puser_event_data) = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
      (yyval.puser_event_data)->translator = (yyvsp[0].pid_info);
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found a data translator: %s\n", (yyvsp[0].pid_info)->name);
      #endif
   }
#line 3581 "y.tab.c"
    break;

  case 96: /* user_event_data: DATA_KEY TRANSLATOR_KEY ID data_block  */
#line 1860 "../../src/parser.y"
   {
  		if (NULL == ((yyval.puser_event_data) = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
      (yyval.puser_event_data)->translator = (yyvsp[-1].pid_info);
 		 (yyval.puser_event_data)->data_fields = (yyvsp[0].plist);
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found a data translator: %s\n", (yyvsp[-1].pid_info)->name);
      fprintf(yyout,"found data fields\n");
 		 parser_debug_print_data_block((yyvsp[0].plist),yyout);
      #endif
   }
#line 3599 "y.tab.c"
    break;

  case 97: /* user_event_data: DATA_KEY data_block  */
#line 1874 "../../src/parser.y"
   {
  		if (NULL == ((yyval.puser_event_data) = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
 		 (yyval.puser_event_data)->data_fields = (yyvsp[0].plist);
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found data fields\n");
 		 parser_debug_print_data_block((yyvsp[0].plist),yyout);
      #endif
   }
#line 3615 "y.tab.c"
    break;

  case 98: /* event_decl_list: EVENT_KEY ID external_designation user_event_data  */
#line 1888 "../../src/parser.y"
                                        {

 					if (NULL == ((yyval.plist) = init_list()))
 						yyerror("Out of memory");

           set_id_type((yyvsp[-2].pid_info),EVENT);
           (yyvsp[-2].pid_info)->type_data.event_data.externalDesignation = (yyvsp[-1].pid_info);
           (yyvsp[-2].pid_info)->type_data.event_data.puser_event_data    = (yyvsp[0].puser_event_data);
           (yyvsp[-2].pid_info)->powningMachine                           = pmachineInfo;

 					if (NULL == ((yyvsp[-2].pid_info)->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == ((yyvsp[-2].pid_info)->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (add_to_list((yyval.plist),(yyvsp[-2].pid_info))))
 						yyerror("Out of memory");

					}
#line 3640 "y.tab.c"
    break;

  case 99: /* event_decl_list: EVENT_KEY parent_namespace EVENT user_event_data  */
#line 1909 "../../src/parser.y"
                                        {
            #ifdef PARSER_DEBUG
            fprintf(yyout,"Found a namespace event reference\n");
            #endif

 					if (NULL == ((yyval.plist) = init_list()))
 						yyerror("Out of memory");

           pID_INFO pid;

            id_list = pmachineInfo->id_list;

           if (!add_unique_id(id_list,EVENT,(yyvsp[-1].pid_info)->name,&pid))
 						yyerror("Cannot reference parent event twice");

           pid->type_data.event_data.puser_event_data   = (yyvsp[0].puser_event_data);
           pid->type_data.event_data.shared_with_parent = true;
           pid->powningMachine                          = pmachineInfo;
 					pid->docCmnt                                 = (yyvsp[-2].charData);

 					if (NULL == (pid->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (pid->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (add_to_list((yyval.plist),pid)))
 						yyerror("Out of memory");

           /* let the parent machine event know about us */
           if (NULL == (yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines)
           {
 					    if (NULL == ((yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines = init_list()))
 						      yyerror("Out of memory");
           }

           if (NULL == add_to_list((yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines, pmachineInfo))
               yyerror("Out of memory");

					}
#line 3685 "y.tab.c"
    break;

  case 100: /* event_decl_list: event_decl_list ',' ID external_designation user_event_data  */
#line 1950 "../../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the event declaration list\n");
						#endif

						(yyval.plist) = (yyvsp[-4].plist);

           set_id_type((yyvsp[-2].pid_info),EVENT);
           (yyvsp[-2].pid_info)->type_data.event_data.externalDesignation = (yyvsp[-1].pid_info);
           (yyvsp[-2].pid_info)->type_data.event_data.puser_event_data    = (yyvsp[0].puser_event_data);
           (yyvsp[-2].pid_info)->powningMachine                           = pmachineInfo;

 					if (NULL == ((yyvsp[-2].pid_info)->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == ((yyvsp[-2].pid_info)->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (add_to_list((yyval.plist),(yyvsp[-2].pid_info))))
 						yyerror("Out of memory");

					}
#line 3713 "y.tab.c"
    break;

  case 101: /* event_decl_list: event_decl_list ',' parent_namespace EVENT user_event_data  */
#line 1974 "../../src/parser.y"
                                        {
            #ifdef PARSER_DEBUG
            fprintf(yyout,"added another namespace event reference to the declaration list\n");
            #endif

            (yyval.plist) = (yyvsp[-4].plist);

           pID_INFO pid;

            id_list = pmachineInfo->id_list;

           if (!add_unique_id(id_list,EVENT,(yyvsp[-1].pid_info)->name,&pid))
 						yyerror("Cannot reference parent event twice");

           pid->type_data.event_data.puser_event_data    = (yyvsp[0].puser_event_data);
           pid->type_data.event_data.shared_with_parent  = true;
           pid->powningMachine                           = pmachineInfo;
 					 pid->docCmnt                                  = (yyvsp[-2].charData);

 					if (NULL == (pid->type_data.event_data.phandling_states = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (pid->type_data.event_data.pactions_list = init_list()))
 						yyerror("Out of memory");

 					if (NULL == (add_to_list((yyval.plist),pid)))
 						yyerror("Out of memory");

           /* let the parent machine event know about us */
           if (NULL == (yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines)
           {
 					    if (NULL == ((yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines = init_list()))
 						      yyerror("Out of memory");
           }

           if (NULL == add_to_list((yyvsp[-1].pid_info)->type_data.event_data.psharing_sub_machines, pmachineInfo))
               yyerror("Out of memory");

					}
#line 3757 "y.tab.c"
    break;

  case 102: /* external_designation: %empty  */
#line 2015 "../../src/parser.y"
                       {
           (yyval.pid_info) = NULL;
     }
#line 3765 "y.tab.c"
    break;

  case 103: /* external_designation: '=' ID  */
#line 2019 "../../src/parser.y"
 {
           #ifdef PARSER_DEBUG
           fprintf(yyout,"External designation = %s\n",(yyvsp[0].pid_info)->name);
           #endif
            (yyval.pid_info) = (yyvsp[0].pid_info);
 }
#line 3776 "y.tab.c"
    break;

  case 104: /* native: %empty  */
#line 2029 "../../src/parser.y"
  {
	(yyval.pnative_info) = NULL;
  }
#line 3784 "y.tab.c"
    break;

  case 105: /* native: native native_prologue  */
#line 2033 "../../src/parser.y"
  {
	if ((yyvsp[-1].pnative_info) == NULL)
	{
		if (((yyval.pnative_info) = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");
	}
	else
	{
		(yyval.pnative_info) = (yyvsp[-1].pnative_info);
	}

	if ((yyval.pnative_info)->prologue)
	{
		yyerror("cannot declare two native prologues");
	}

	(yyval.pnative_info)->prologue = (yyvsp[0].charData);

  }
#line 3808 "y.tab.c"
    break;

  case 106: /* native: native native_epilogue  */
#line 2053 "../../src/parser.y"
  {
	if ((yyvsp[-1].pnative_info) == NULL)
	{
		if (((yyval.pnative_info) = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");
	}
	else
	{
		(yyval.pnative_info) = (yyvsp[-1].pnative_info);
	}

	if ((yyval.pnative_info)->epilogue)
	{
		yyerror("cannot declare two native epilogues");
	}

	(yyval.pnative_info)->epilogue = (yyvsp[0].charData);
  }
#line 3831 "y.tab.c"
    break;

  case 107: /* native_prologue: NATIVE_KEY NATIVE_BLOCK  */
#line 2075 "../../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3843 "y.tab.c"
    break;

  case 108: /* native_prologue: NATIVE_KEY PROLOGUE_KEY NATIVE_BLOCK  */
#line 2083 "../../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native prologue\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3855 "y.tab.c"
    break;

  case 109: /* native_epilogue: NATIVE_KEY EPILOGUE_KEY NATIVE_BLOCK  */
#line 2095 "../../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native epilogue\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3867 "y.tab.c"
    break;

  case 110: /* native_impl: native_impl_prologue  */
#line 2107 "../../src/parser.y"
        {
		if (((yyval.pnative_info) = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");

		(yyval.pnative_info)->prologue = (yyvsp[0].charData);
	}
#line 3878 "y.tab.c"
    break;

  case 111: /* native_impl: native_impl_epilogue  */
#line 2114 "../../src/parser.y"
        {
		if (((yyval.pnative_info) = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");

		(yyval.pnative_info)->epilogue = (yyvsp[0].charData);
	}
#line 3889 "y.tab.c"
    break;

  case 112: /* native_impl: native_impl native_impl_prologue  */
#line 2121 "../../src/parser.y"
        {
		if ((yyvsp[-1].pnative_info)->prologue != NULL)
		{
			yyerror("only one native implementation prologue is allowed");
		}

		(yyval.pnative_info) = (yyvsp[-1].pnative_info);
		(yyval.pnative_info)->prologue = (yyvsp[0].charData);
	}
#line 3903 "y.tab.c"
    break;

  case 113: /* native_impl: native_impl native_impl_epilogue  */
#line 2131 "../../src/parser.y"
        {
		if ((yyvsp[-1].pnative_info)->epilogue != NULL)
		{
			yyerror("only one native implementation epilogue is allowed");
		}

		(yyval.pnative_info) = (yyvsp[-1].pnative_info);
		(yyval.pnative_info)->epilogue = (yyvsp[0].charData);
	}
#line 3917 "y.tab.c"
    break;

  case 114: /* native_impl_prologue: NATIVE_KEY IMPLEMENTATION_KEY NATIVE_BLOCK  */
#line 2144 "../../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3929 "y.tab.c"
    break;

  case 115: /* native_impl_prologue: NATIVE_KEY IMPLEMENTATION_KEY PROLOGUE_KEY NATIVE_BLOCK  */
#line 2152 "../../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation prologue\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3941 "y.tab.c"
    break;

  case 116: /* native_impl_epilogue: NATIVE_KEY IMPLEMENTATION_KEY EPILOGUE_KEY NATIVE_BLOCK  */
#line 2162 "../../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3953 "y.tab.c"
    break;

  case 117: /* machine_data: DATA_KEY data_block  */
#line 2172 "../../src/parser.y"
                                  { (yyval.plist) = (yyvsp[0].plist); pmachineInfo->data = (yyval.plist); }
#line 3959 "y.tab.c"
    break;

  case 118: /* data_block: '{' data_fields '}'  */
#line 2175 "../../src/parser.y"
        {
			#ifdef PARSER_DEBUG
			fprintf(yyout,"Data block done\n");
			#endif

			(yyval.plist) = (yyvsp[-1].plist);

	}
#line 3972 "y.tab.c"
    break;

  case 119: /* data_fields: data_field  */
#line 2186 "../../src/parser.y"
   {
      #ifdef PARSER_DEBUG
    	fprintf(yyout,"Starting data_fields list\n");
    	#endif
   
      if (NULL == ((yyval.plist) = init_list()))
    		yyerror("out of memory");
   
    	add_to_list((yyval.plist), (yyvsp[0].pdata_field));
   
   }
#line 3988 "y.tab.c"
    break;

  case 120: /* data_fields: data_fields data_field  */
#line 2198 "../../src/parser.y"
   {
      #ifdef PARSER_DEBUG
    	fprintf(yyout,"Continuing data_fields list\n");
    	#endif
   
 	  (yyval.plist) = (yyvsp[-1].plist);
    	add_to_list((yyval.plist), (yyvsp[0].pdata_field));
   
   }
#line 4002 "y.tab.c"
    break;

  case 121: /* data_type: ID  */
#line 2210 "../../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found simple data type id\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

    	 set_id_type((yyvsp[0].pid_info), TYPE_NAME);

 		 (yyval.pdata_type_struct)->dtt      = dtt_simple;
    	 (yyval.pdata_type_struct)->dtu.name = (yyvsp[0].pid_info);

   }
#line 4021 "y.tab.c"
    break;

  case 122: /* data_type: TYPE_NAME  */
#line 2225 "../../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found simple data type name\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

 		 (yyval.pdata_type_struct)->dtt      = dtt_simple;
    	 (yyval.pdata_type_struct)->dtu.name = (yyvsp[0].pid_info);

   }
#line 4038 "y.tab.c"
    break;

  case 123: /* data_type: STRUCT_KEY '{' data_fields '}'  */
#line 2238 "../../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found struct data type\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

      (yyval.pdata_type_struct)->dtt         = dtt_struct;
 		 (yyval.pdata_type_struct)->dtu.members = (yyvsp[-1].plist);

   }
#line 4055 "y.tab.c"
    break;

  case 124: /* data_type: UNION_KEY '{' data_fields '}'  */
#line 2251 "../../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found union data type\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

      (yyval.pdata_type_struct)->dtt         = dtt_union;
 		 (yyval.pdata_type_struct)->dtu.members = (yyvsp[-1].plist);

   }
#line 4072 "y.tab.c"
    break;

  case 125: /* data_type: data_type '*'  */
#line 2264 "../../src/parser.y"
   {
    (yyval.pdata_type_struct) = (yyvsp[-1].pdata_type_struct);
    ((yyval.pdata_type_struct)->indirection_level)++;

		 #ifdef PARSER_DEBUG
		 fprintf(yyout
            ,"found pointer data type: TYPE: %s; indirection_level: %u\n"
            , (yyvsp[-1].pdata_type_struct)->dtt == dtt_simple ? "simple"
 					   : (yyvsp[-1].pdata_type_struct)->dtt == dtt_struct ? "struct"
 					     : "union"
            , (yyvsp[-1].pdata_type_struct)->indirection_level
            );
		 #endif
   }
#line 4091 "y.tab.c"
    break;

  case 126: /* data_field: data_type ID data_field_dimension ';'  */
#line 2282 "../../src/parser.y"
  {
		 #ifdef PARSER_DEBUG
		 fprintf(yyout
            ,"found data field: TYPE: %s; NAME: %s; dimension: %s\n"
            , (yyvsp[-3].pdata_type_struct)->dtt == dtt_simple ? "simple"
 					   : (yyvsp[-3].pdata_type_struct)->dtt == dtt_struct ? "struct"
 					     : "union"
            , (yyvsp[-2].pid_info)->name
            , (yyvsp[-1].charData) ? (yyvsp[-1].charData) : "none"
            );
		 #endif

 	 if (((yyval.pdata_field) = calloc(1, sizeof(DATA_FIELD))) == NULL)
 	    yyerror("out of memory");

   (yyval.pdata_field)->pdts            = (yyvsp[-3].pdata_type_struct);
   (yyval.pdata_field)->pdts->is_array  = true;
 	 (yyval.pdata_field)->pdts->dimension = (yyvsp[-1].charData);
 	 (yyval.pdata_field)->data_field_name = (yyvsp[-2].pid_info);

  }
#line 4117 "y.tab.c"
    break;

  case 127: /* data_field: data_type ID ';'  */
#line 2304 "../../src/parser.y"
  {
		 #ifdef PARSER_DEBUG
		 fprintf(yyout
            ,"found data field: TYPE: %s; NAME: %s\n" 
            , (yyvsp[-2].pdata_type_struct)->dtt == dtt_simple ? "simple"
 					   : (yyvsp[-2].pdata_type_struct)->dtt == dtt_struct ? "struct"
 					     : "union"
            , (yyvsp[-1].pid_info)->name
            );
		 #endif

 	 if (((yyval.pdata_field) = calloc(1, sizeof(DATA_FIELD))) == NULL)
 	    yyerror("out of memory");

    (yyval.pdata_field)->pdts            = (yyvsp[-2].pdata_type_struct);
 	 (yyval.pdata_field)->data_field_name = (yyvsp[-1].pid_info);

  }
#line 4140 "y.tab.c"
    break;

  case 128: /* data_field_dimension: '[' ']'  */
#line 2325 "../../src/parser.y"
   {
       (yyval.charData) = NULL;
   }
#line 4148 "y.tab.c"
    break;

  case 129: /* data_field_dimension: '[' NUMERIC_STRING ']'  */
#line 2329 "../../src/parser.y"
   {
       (yyval.charData) = (yyvsp[-1].charData);
   }
#line 4156 "y.tab.c"
    break;

  case 130: /* data_field_dimension: '[' ID ']'  */
#line 2333 "../../src/parser.y"
   {
       (yyval.charData) = (yyvsp[-1].pid_info)->name;
   }
#line 4164 "y.tab.c"
    break;

  case 132: /* namespace: MACHINE NAMESPACE  */
#line 2341 "../../src/parser.y"
  {
    if (pmachineInfo->parent)
        yyerror("sub-machine namespace invoked in sub-machine");

    id_list = (yyvsp[-1].pid_info)->powningMachine->id_list;
  }
#line 4175 "y.tab.c"
    break;

  case 133: /* namespace_event_ref: namespace EVENT  */
#line 2350 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a namespace event reference\n");
    #endif

    (yyval.pid_info) = (yyvsp[0].pid_info);
    id_list = pmachineInfo->id_list;
  }
#line 4188 "y.tab.c"
    break;

  case 134: /* returns_comma_list: namespace_event_ref ','  */
#line 2361 "../../src/parser.y"
    {
						/* start a list */
						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4202 "y.tab.c"
    break;

  case 135: /* returns_comma_list: EVENT ','  */
#line 2371 "../../src/parser.y"
    {
						/* start a list */
						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4216 "y.tab.c"
    break;

  case 136: /* returns_comma_list: returns_comma_list namespace_event_ref ','  */
#line 2381 "../../src/parser.y"
    {
           (yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4228 "y.tab.c"
    break;

  case 137: /* returns_comma_list: returns_comma_list EVENT ','  */
#line 2389 "../../src/parser.y"
    {
           (yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4240 "y.tab.c"
    break;

  case 138: /* action_return_decl: ACTION RETURNS returns_comma_list EVENT ';'  */
#line 2400 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!(yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl)
		 {
		    if (((yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			if (add_to_list((yyvsp[-2].plist),(yyvsp[-1].pid_info)) == NULL)
				 yyerror("out of memory");

			move_list_unique((yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl, (yyvsp[-2].plist));
 		free_list((yyvsp[-2].plist));

  }
#line 4263 "y.tab.c"
    break;

  case 139: /* action_return_decl: ACTION RETURNS returns_comma_list namespace_event_ref ';'  */
#line 2419 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!(yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl)
		 {
		    if (((yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			if (add_to_list((yyvsp[-2].plist),(yyvsp[-1].pid_info)) == NULL)
				 yyerror("out of memory");

			move_list_unique((yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl, (yyvsp[-2].plist));
 		free_list((yyvsp[-2].plist));

  }
#line 4286 "y.tab.c"
    break;

  case 140: /* action_return_decl: ACTION RETURNS EVENT ';'  */
#line 2438 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!(yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl)
		 {
		    if (((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

		 if (add_unique_to_list((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl,(yyvsp[-1].pid_info)) == NULL)
				yyerror("out of memory");

  }
#line 4306 "y.tab.c"
    break;

  case 141: /* action_return_decl: ACTION RETURNS namespace_event_ref ';'  */
#line 2454 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

 	 if (!(yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl)
		 {
		    if (((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

		 if (add_unique_to_list((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl,(yyvsp[-1].pid_info)) == NULL)
				yyerror("out of memory");

  }
#line 4326 "y.tab.c"
    break;

  case 142: /* action_return_decl: ACTION RETURNS state_comma_list STATE ';'  */
#line 2470 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

			if (add_to_list((yyvsp[-2].plist), (yyvsp[-1].pid_info)) == NULL)
				yyerror("out of memory");

 	 if (!(yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl)
		 {
		    if (((yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			move_list_unique((yyvsp[-4].pid_info)->type_data.action_data.action_returns_decl, (yyvsp[-2].plist));
 		free_list((yyvsp[-2].plist));

  }
#line 4349 "y.tab.c"
    break;

  case 143: /* action_return_decl: ACTION RETURNS STATE ';'  */
#line 2489 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

		 if (add_to_list((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl,(yyvsp[-1].pid_info)) == NULL)
				yyerror("out of memory");

 	 if (!(yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl)
		 {
		    if (((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			add_unique_to_list((yyvsp[-3].pid_info)->type_data.action_data.action_returns_decl, (yyvsp[-1].pid_info));

  }
#line 4371 "y.tab.c"
    break;

  case 144: /* transition_fn_return_decl: TRANSITION_FN RETURNS state_comma_list STATE ';'  */
#line 2510 "../../src/parser.y"
  {
			if (add_unique_to_list((yyvsp[-2].plist), (yyvsp[-1].pid_info)) == NULL)
 			yyerror("out of memory");

 	 if (!(yyvsp[-4].pid_info)->transition_fn_returns_decl)
		 {
		    if (((yyvsp[-4].pid_info)->transition_fn_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			move_list_unique((yyvsp[-4].pid_info)->transition_fn_returns_decl, (yyvsp[-2].plist));
 		free_list((yyvsp[-2].plist));

    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a transition_fn return declaration\n");
    #endif

  }
#line 4394 "y.tab.c"
    break;

  case 145: /* transition_fn_return_decl: TRANSITION_FN RETURNS STATE ';'  */
#line 2529 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a transition_fn return declaration\n");
    #endif

 	 if (!(yyvsp[-3].pid_info)->transition_fn_returns_decl)
		 {
		    if (((yyvsp[-3].pid_info)->transition_fn_returns_decl = init_list()) == NULL) 
				   yyerror("out of memory");
		 }

			add_unique_to_list((yyvsp[-3].pid_info)->transition_fn_returns_decl, (yyvsp[-1].pid_info));

  }
#line 4413 "y.tab.c"
    break;

  case 146: /* transition_fn_return_decl: TRANSITION_FN RETURNS return_choice_cond_list return_choice_cond ';'  */
#line 2544 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a Mode B transition_fn return declaration\n");
    #endif

    if (add_to_list((yyvsp[-2].plist), (yyvsp[-1].preturn_choice)) == NULL)
        yyerror("out of memory");

    if (!(yyvsp[-4].pid_info)->transition_fn_returns_cond_decl)
    {
        if (((yyvsp[-4].pid_info)->transition_fn_returns_cond_decl = init_list()) == NULL)
            yyerror("out of memory");
    }

    move_list((yyvsp[-4].pid_info)->transition_fn_returns_cond_decl, (yyvsp[-2].plist));
    free_list((yyvsp[-2].plist));

  }
#line 4436 "y.tab.c"
    break;

  case 147: /* transition_fn_return_decl: TRANSITION_FN RETURNS return_choice_cond ';'  */
#line 2563 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a Mode B transition_fn return declaration (single choice)\n");
    #endif

    if (!(yyvsp[-3].pid_info)->transition_fn_returns_cond_decl)
    {
        if (((yyvsp[-3].pid_info)->transition_fn_returns_cond_decl = init_list()) == NULL)
            yyerror("out of memory");
    }

    if (add_to_list((yyvsp[-3].pid_info)->transition_fn_returns_cond_decl, (yyvsp[-1].preturn_choice)) == NULL)
        yyerror("out of memory");

  }
#line 4456 "y.tab.c"
    break;

  case 148: /* return_choice_cond_list: return_choice_cond ','  */
#line 2581 "../../src/parser.y"
  {
    if (((yyval.plist) = init_list()) == NULL)
        yyerror("out of memory");

    if (add_to_list((yyval.plist), (yyvsp[-1].preturn_choice)) == NULL)
        yyerror("out of memory");
  }
#line 4468 "y.tab.c"
    break;

  case 149: /* return_choice_cond_list: return_choice_cond_list return_choice_cond ','  */
#line 2589 "../../src/parser.y"
  {
    (yyval.plist) = (yyvsp[-2].plist);

    if (add_to_list((yyval.plist), (yyvsp[-1].preturn_choice)) == NULL)
        yyerror("out of memory");
  }
#line 4479 "y.tab.c"
    break;

  case 150: /* return_choice_cond: STATE WHEN_KEY ID  */
#line 2598 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a return choice: state when new_fn\n");
    #endif

    if (((yyval.preturn_choice) = (pRETURN_CHOICE_DATA)calloc(1, sizeof(RETURN_CHOICE_DATA))) == NULL)
        yyerror("out of memory");

    set_id_type((yyvsp[0].pid_info), TRANSITION_FN);
    (yyval.preturn_choice)->state        = (yyvsp[-2].pid_info);
    (yyval.preturn_choice)->condition_fn = (yyvsp[0].pid_info);
    (yyval.preturn_choice)->is_otherwise = false;
  }
#line 4497 "y.tab.c"
    break;

  case 151: /* return_choice_cond: STATE WHEN_KEY TRANSITION_FN  */
#line 2612 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a return choice: state when existing_fn\n");
    #endif

    if (((yyval.preturn_choice) = (pRETURN_CHOICE_DATA)calloc(1, sizeof(RETURN_CHOICE_DATA))) == NULL)
        yyerror("out of memory");

    (yyval.preturn_choice)->state        = (yyvsp[-2].pid_info);
    (yyval.preturn_choice)->condition_fn = (yyvsp[0].pid_info);
    (yyval.preturn_choice)->is_otherwise = false;
  }
#line 4514 "y.tab.c"
    break;

  case 152: /* return_choice_cond: STATE OTHERWISE_KEY  */
#line 2625 "../../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a return choice: state otherwise\n");
    #endif

    if (((yyval.preturn_choice) = (pRETURN_CHOICE_DATA)calloc(1, sizeof(RETURN_CHOICE_DATA))) == NULL)
        yyerror("out of memory");

    (yyval.preturn_choice)->state        = (yyvsp[-1].pid_info);
    (yyval.preturn_choice)->condition_fn = NULL;
    (yyval.preturn_choice)->is_otherwise = true;
  }
#line 4531 "y.tab.c"
    break;


#line 4535 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 2638 "../../src/parser.y"


#if defined(CYGWIN) || defined (LINUX)
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>
#endif

#ifdef VS
#include "vsstd.h"
#include "vsstring.h"
#endif

void usage(void);
char *dotfsm = ".fsm";

/*
  use these as needed for the val param of the long options
  when flag is set to &longval.  Otherwise, set the val param to the
  short option character and set flag to NULL.
*/
typedef enum {
 lo_css_content_filename
 , lo_css_content_internal
 , lo_weak_fns
 , lo_core_logging
 , lo_include_svg_img
	, lo_short_dbg_names
 , lo_force_generation_of_event_passing_actions
 , lo_add_machine_name
 , lo_generate_run_function
 , lo_add_event_cross_reference
 , lo_add_plantuml_title
 , lo_add_plantuml_legend
 , lo_exclude_states_from_plantuml_legend
 , lo_exclude_events_from_plantuml_legend
 , lo_exclude_actions_from_plantuml_legend
 , lo_add_plantuml_prefix_string
 , lo_add_plantuml_prefix_file
 , lo_short_user_fn_names
 , lo_event_cross_ref_only
 , lo_event_cross_ref_format
 , lo_include_state_cross_refs
 , lo_convenience_macro_in_public_header
 , lo_add_profiling_macros
 , lo_profile_sub_fsms
 , lo_empty_cell_fn
 , lo_inhibiting_states_share_events
 , lo_include_uml_objects
 , lo_weak_fn_separate_file
 , lo_doxygen_blocks
 , lo_find_on_sub_machine_depth
 , lo_find_on_top_level_machine_data
 , lo_find_on_event_data
} LONG_OPTIONS;

int longindex = 0;
int longval;
const struct option longopts[] =
{
    {
        .name      = "help"
        , .has_arg = no_argument
        , .flag    = NULL
        , .val     = '?'
    }
    , {
        .name      = "css-content-filename"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_css_content_filename
    }
    , {
        .name      = "css-content-internal"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_css_content_internal
    }
    , {
        .name      = "generate-weak-fns"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_weak_fns
    }
    , {
        .name      = "core-logging-only"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_core_logging
    }
    , {
        .name      = "include-svg-img"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_include_svg_img
    }
    , {
        .name      = "short-debug-names"
        , .has_arg = no_argument
        , .flag    = &longval
        , .val     = lo_short_dbg_names
    }
    , {
        .name      = "force-generation-of-event-passing-actions"
        , .has_arg = no_argument
        , .flag    = &longval
        , .val     = lo_force_generation_of_event_passing_actions
    }
    , {
        .name      = "add-machine-name"
        , .has_arg = no_argument
        , .flag    = &longval
        , .val     = lo_add_machine_name
    }
    , {
        .name      = "generate-run-function"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_generate_run_function
    }
    , {
        .name      = "add-event-cross-reference"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_add_event_cross_reference
    }
    , {
        .name      = "add-plantuml-title"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_title
    }
    , {
        .name      = "add-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_legend
    }
    , {
        .name      = "exclude-events-from-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_exclude_events_from_plantuml_legend
    }
    , {
        .name      = "exclude-states-from-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_exclude_states_from_plantuml_legend
    }
    , {
        .name      = "exclude-actions-from-plantuml-legend"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_exclude_actions_from_plantuml_legend
    }
    , {
        .name      = "add-plantuml-prefix-string"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_prefix_string
    }
    , {
        .name      = "add-plantuml-prefix-file"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_prefix_file
    }
    , {
        .name      = "add-plantuml-prefix-file"
        , .has_arg = required_argument
        , .flag    = &longval
        , .val     = lo_add_plantuml_prefix_file
    }
    , {
        .name      = "short-user-fn-names"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_short_user_fn_names
    }
    , {
        .name      = "event-cross-ref-only"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_event_cross_ref_only
    }
    , {
        .name      = "event-cross-ref-format"
        , .has_arg = required_argument
        , .flag    = &longval
				, .val     = lo_event_cross_ref_format
		}
    , {
        .name      = "include-state-cross-refs"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_include_state_cross_refs
    }
		, {
        .name      = "convenience-macros-in-public-header"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_convenience_macro_in_public_header
    }
		, {
        .name      = "add-profiling-macros"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_add_profiling_macros
    }
		, {
        .name      = "profile-sub-fsms"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_profile_sub_fsms
    }
		, {
        .name      = "empty-cell-fn"
        , .has_arg = required_argument
        , .flag    = &longval
				, .val     = lo_empty_cell_fn
    }
		, {
        .name      = "inhibiting-states-share-events"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_inhibiting_states_share_events
    }
    , {
        .name      = "include-uml-objects"
        , .has_arg = optional_argument
        , .flag    = &longval
        , .val     = lo_include_uml_objects
		}
		, {
        .name      = "weak-fns-separate-file"
        , .has_arg = required_argument
        , .flag    = &longval
				, .val     = lo_weak_fn_separate_file
    }
		, {
        .name      = "add-doxygen-blocks"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_doxygen_blocks
    }
		, {
        .name      = "find-on-sub-machine-depth"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_find_on_sub_machine_depth
    }
		, {
        .name      = "find-on-top-level-machine-data"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_find_on_top_level_machine_data
    }
		, {
        .name      = "find-on-event-data"
        , .has_arg = optional_argument
        , .flag    = &longval
				, .val     = lo_find_on_event_data
    }
    , {0}
};
      
int main(int argc, char **argv)
{

	char	*cp1;
	char    *outFileBase = NULL;

 #ifndef PARSER_DEBUG
 int   c;
 #endif

	me = argv[0];

#ifdef YYDEBUG
// yydebug = 1;
#endif

	/* special case the single '?' */
	if (argc == 2 && argv[1][0] == '?') {

		usage();
		return (1);

	}

#ifndef PARSER_DEBUG

	while ((c = getopt_long(argc,argv,"vh::t:o:i:csM::", longopts, &longindex)) != -1) {

		switch(c) {
  
     case 0:

        switch (longval)
        {
            case lo_weak_fns:
                generate_weak_fns 
                    = !strcmp(optarg,"true") ? true : false;
                    if (!generate_weak_fns && weak_fn_separate_file)
                    {
                      yyerror("Either enable weak function generation or do not indicate a separate file for them.");
                    }
                break;
            case lo_weak_fn_separate_file:
                weak_fn_separate_file 
                  = !strcmp(optarg, "true") ? true : false;
                if (weak_fn_separate_file && !generate_weak_fns)
                {
                      yyerror("Either enable weak function generation or do not indicate a separate file for them.");
                }
                break;
            case lo_doxygen_blocks:
                add_doxygen_blocks
                    = (!optarg || !strcmp(optarg,"true")) ? true : false;
                break;
            case lo_core_logging:
                core_logging_only 
                    = !strcmp(optarg,"true") ? true : false;
                break;
            case lo_include_svg_img:
                if (!optarg || !strcmp(optarg, "true"))
                {
                  include_svg_img = true;
									if (include_uml_objects)
									{
										yyerror("Include either UML objects or SVG images.");
									}
                }
                break;
            case lo_css_content_filename:
                css_content_filename = optarg;
                break;
            case lo_css_content_internal:
                css_content_internal
                    = (!optarg || !strcmp(optarg,"true")) ? true : false;
                break;
            case lo_short_dbg_names:
                short_dbg_names = true;
                break;
            case lo_add_machine_name:
                add_machine_name = true;
                break;
            case lo_force_generation_of_event_passing_actions:
                force_generation_of_event_passing_actions = true;
                break;
            case lo_generate_run_function:
                if (optarg && !strcmp(optarg,"false"))
                    generate_run_function = false;
                break;
 			   case lo_add_event_cross_reference:
 					 if (!optarg || !strcmp(optarg,"true"))
 						add_event_cross_reference = true;
 					 break;
         case lo_event_cross_ref_only:
 					 if (!optarg || !strcmp(optarg,"true"))
						fpfsmogf = generateCEventXRefWriter;
           break;
         case lo_include_state_cross_refs:
 					 if (!optarg || !strcmp(optarg,"true"))
              include_state_cross_refs = true;
           break;
         case lo_event_cross_ref_format:
           if (!check_requested_xref_format(optarg))
             yyerror("invalid event cross reference format");
					 fpfsmogf = generateCEventXRefWriter;
           break;
 			   case lo_add_plantuml_title:
 					 if (!optarg || !strcmp(optarg,"true"))
 						add_plantuml_title = true;
 					 break;
 			   case lo_add_plantuml_legend:
 					 if (!optarg)
						 {
 					    add_plantuml_legend = true;
						 }
 					 else
						 {
								 if (strstr(optarg, "top"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_vertical_placement = vp_top;
   						 }
								 if (strstr(optarg, "bottom"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_vertical_placement = vp_bottom;
   						 }
								 if (strstr(optarg, "center"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_horizontal_placement = hp_center;
   						 }
								 if (strstr(optarg, "left"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_horizontal_placement = hp_left;
   						 }
								 if (strstr(optarg, "right"))
   						 {
    					    add_plantuml_legend = true;
    							plantuml_legend_horizontal_placement = hp_right;
   						 }
						 }

 					 if (!add_plantuml_legend)
						 {
 							 yyerror("unrecognized legend placement option");
 							 usage();
 							 return (good);
						 }
 					 break;
 			   case lo_exclude_events_from_plantuml_legend:
 					 if (!optarg || !strcmp(optarg,"true"))
 						exclude_events_from_plantuml_legend = true;
 					 break;
 			   case lo_exclude_states_from_plantuml_legend:
 					 if (!optarg || !strcmp(optarg,"true"))
 						exclude_states_from_plantuml_legend = true;
 					 break;
 			   case lo_exclude_actions_from_plantuml_legend:
 					 if (!optarg || !strcmp(optarg,"true"))
 						exclude_actions_from_plantuml_legend = true;
 					 break;
 					 case lo_add_plantuml_prefix_string:
 					     if (!pplantuml_prefix_strings_list)
 							    pplantuml_prefix_strings_list = init_list();
 							 add_to_list(pplantuml_prefix_strings_list, optarg);
 							 break;
 					 case lo_add_plantuml_prefix_file:
 					     if (!pplantuml_prefix_files_list)
 							    pplantuml_prefix_files_list = init_list();
 							 add_to_list(pplantuml_prefix_files_list, optarg);
 							 break;
			    case lo_short_user_fn_names:
		            if (!optarg || !strcmp(optarg, "true"))
			            short_user_fn_names=true;
		            break;
						case lo_convenience_macro_in_public_header:
						   if (optarg && !strcmp(optarg, "false"))
							 {
								 convenience_macros_in_public_header = false;
							 }
							 break;
      			case lo_add_profiling_macros:
							if (optarg && !strcmp(optarg, "true"))
							{
								add_profiling_macros = true;
							}
							break;
      			case lo_profile_sub_fsms:
							if (optarg && !strcmp(optarg, "true"))
							{
								profile_sub_fsms = true;
							}
							break;
            case lo_empty_cell_fn:
                empty_cell_fn = optarg;
                break;
			      case lo_inhibiting_states_share_events:
				      if (!optarg || !strcmp(optarg, "true"))
					      inhibiting_states_share_events = true;
				      break;
            case lo_include_uml_objects:
                if (!optarg || !strcmp(optarg, "true"))
                {
                  include_uml_objects = true;
									if (include_svg_img)
									{
										yyerror("Include either UML objects or SVG images.");
									}
                }
                break;
            case lo_find_on_sub_machine_depth:
                find_on_sub_machine_depth = optarg ? atoi(optarg) : 0;
								if (find_on_event_data || find_on_top_level_machine_data)
								{
									yyerror("Only one --find-on... option is allowed");
								}
                break;
			      case lo_find_on_top_level_machine_data:
				      if (!optarg || !strcmp(optarg, "true"))
				      {
					      find_on_top_level_machine_data = true;
					      if (find_on_event_data || (find_on_sub_machine_depth > -1))
					      {
									yyerror("Only one --find-on... option is allowed");
					      }
				      }
				      else if (!strcmp(optarg, "false"))
				      {
					      find_on_top_level_machine_data = false;
				      }
				      break;
			      case lo_find_on_event_data:
				      if (!optarg || !strcmp(optarg, "true"))
				      {
					      find_on_event_data = true;
					      if (find_on_top_level_machine_data || (find_on_sub_machine_depth > -1))
					      {
									yyerror("Only one --find-on... option is allowed");
					      }
				      }
				      else if (!strcmp(optarg, "false"))
				      {
					      find_on_event_data = false;
				      }
				      break;
            default:
                usage();
                return(0);
                break;
        }
        break;

		case 'h':
      if (optarg)
      {
         help_fmt = optarg[0];
      }
			usage();
			return (0);

 		case 's':
 			pfsmog = pMachineStatisticsWriter;
 			break;

			case 't':

        if (optarg)
				{
					switch (optarg[0]) {

						case 'c':
							fpfsmogf = generateCMachineWriter;
							break;

						case 'h':
							fpfsmogf = generateHTMLMachineWriter;
							break;

						case 's':
						  switch (optarg[1])
							{
								case 0:
							    fpfsmogf = generateCSwitchMachineWriter;
								  break;
								case 's':
								  fpfsmogf = generateCSingleSwitchMachineWriter;
								  break;
								default:
						      usage();
						      return (1);
									break;
							}
							break;

						case 'p':
              switch (optarg[1])
              {
                case 0:
							    fpfsmogf = generatePlantUMLMachineWriter;
                  break;
                case 'y':
							    fpfsmogf = generatePyTransitionsWriter;
                  break;
								default:
						      usage();
						      return (1);
									break;
              }
							break;

						case 'e':
							fpfsmogf = generateCEventTableMachineWriter;
							break;

						case 'r':
							fpfsmogf = generateRSTMachineWriter;
            	break;

					  default:
						  usage();
						  return (1);

				  }

				}
				else
				{
					usage();
					return (1);
				}
				break;

		case 'i':
			if ('0' == optarg[0])
			{
				generate_instance = false;
			}
      else
      {
          num_instances = atoi(optarg);
      }
			break;

     case 'c':
        compact_action_array = true;
        break;

			case 'o':

				outFileBase = optarg;
				break;

     case 'v':

        fprintf(stdout,"version %s\n",rev_string);
        return (0);

     case 'M':
		output_generated_file_names_only = true;
		if (optarg)
		{
			switch (optarg[0])
			{
				case 'd':
					output_make_recipe = true;
					break;

				case 'h':
					output_header_files = true;
				break;

				default:
					break;
			}
		}
		break;

     case '?':
     case ':':

        usage();
        return (0);

		}

	}
#endif

	if (optind >= argc)
	{
		fprintf(stdout,"need a file name to work with.\n");
		return (!good);
	}

	for (int fnind = optind; fnind < argc && good; fnind++) {

		inputFileName = strdup(argv[fnind]);

		/* find the extension */
		cp1 = rindex(inputFileName,'.');
		if (!cp1) {

			usage();
			exit(1);

		}
		else {

			if (strcmp(cp1,dotfsm))	{

				usage();
				return (1);

			}

		}

    if (!newfile(argv[fnind]))
    {
      return 1;
    }

		/* get the base file name */
		if (!outFileBase) {

       size_t inputFilePathLen;

			/* use the base input file name */
			*cp1 = 0;
			cwk_path_get_basename(inputFileName, (const char**)&outFileBase, NULL);
      cwk_path_get_dirname(inputFileName, &inputFilePathLen);
      inputFilePath = strndup(inputFileName, inputFilePathLen);
		}

		#ifndef PARSER_DEBUG

		if (NULL == pfsmog)
		{

    	    /* default to writing a c machine */
    	    if (NULL == fpfsmogf)
    	    {
    			fpfsmogf = generateCMachineWriter;
    	    }
   
			pfsmog = fpfsmogf(NULL);
		}

		if (!(*pfsmog->initOutput)(pfsmog,outFileBase)) {

		#endif

			yyparse();

		#ifndef PARSER_DEBUG
			(*pfsmog->closeOutput)(pfsmog,good);

      //prepare for next input file, if any
      outFileBase = NULL;

		}
		#endif


    CHECK_AND_FREE(inputFileName);
    CHECK_AND_FREE(inputFilePath);

	}

	return (good == 1 ? 0 : 1);

}

void yyerror(char *s)
{
	const char *basename;
	const char *ext;

  fprintf(stderr,"%s%s%s: %s.fsm: %s\n"
					, (cwk_path_get_basename(me, &basename, NULL), basename)
					, cwk_path_has_extension(me) ? "." : ""
					, cwk_path_has_extension(me)
						 ? (cwk_path_get_extension(me, &ext, NULL), ext)
						 : ""
          , inputFileName
					,s
					);
  fprintf(stderr,"\tline %d : %s\n",lineno,yytext);

  #ifdef PARSER_DEBUG
	//always return good so that the makefile can pick up stderr
	//the makefile will detect differences between actual and expected outcome;
	good = 1;
	#else
	good = 0;
	#endif

}


