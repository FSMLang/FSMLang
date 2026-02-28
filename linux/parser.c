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
#line 1 "../src/parser.y"
         
	/* parser.y

		the FSM Description Language Parser.

	*/

#include <stdio.h>
#include <string.h>

#include <cwalk.h>

#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
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


#line 134 "y.tab.c"

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
    PARENT = 290,                  /* PARENT  */
    NATIVE_KEY = 291,              /* NATIVE_KEY  */
    NATIVE_BLOCK = 292,            /* NATIVE_BLOCK  */
    MACHINE = 293,                 /* MACHINE  */
    STATE = 294,                   /* STATE  */
    EVENT = 295,                   /* EVENT  */
    ACTION = 296,                  /* ACTION  */
    TRANSITION_FN = 297,           /* TRANSITION_FN  */
    ID = 298,                      /* ID  */
    NUMERIC_STRING = 299,          /* NUMERIC_STRING  */
    TYPE_NAME = 300                /* TYPE_NAME  */
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
#define PARENT 290
#define NATIVE_KEY 291
#define NATIVE_BLOCK 292
#define MACHINE 293
#define STATE 294
#define EVENT 295
#define ACTION 296
#define TRANSITION_FN 297
#define ID 298
#define NUMERIC_STRING 299
#define TYPE_NAME 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 64 "../src/parser.y"

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

#line 300 "y.tab.c"

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
  YYSYMBOL_PARENT = 35,                    /* PARENT  */
  YYSYMBOL_NATIVE_KEY = 36,                /* NATIVE_KEY  */
  YYSYMBOL_NATIVE_BLOCK = 37,              /* NATIVE_BLOCK  */
  YYSYMBOL_MACHINE = 38,                   /* MACHINE  */
  YYSYMBOL_STATE = 39,                     /* STATE  */
  YYSYMBOL_EVENT = 40,                     /* EVENT  */
  YYSYMBOL_ACTION = 41,                    /* ACTION  */
  YYSYMBOL_TRANSITION_FN = 42,             /* TRANSITION_FN  */
  YYSYMBOL_ID = 43,                        /* ID  */
  YYSYMBOL_NUMERIC_STRING = 44,            /* NUMERIC_STRING  */
  YYSYMBOL_TYPE_NAME = 45,                 /* TYPE_NAME  */
  YYSYMBOL_46_ = 46,                       /* '{'  */
  YYSYMBOL_47_ = 47,                       /* '}'  */
  YYSYMBOL_48_ = 48,                       /* ';'  */
  YYSYMBOL_49_ = 49,                       /* ','  */
  YYSYMBOL_50_ = 50,                       /* '['  */
  YYSYMBOL_51_ = 51,                       /* ']'  */
  YYSYMBOL_52_ = 52,                       /* '('  */
  YYSYMBOL_53_ = 53,                       /* ')'  */
  YYSYMBOL_54_ = 54,                       /* '='  */
  YYSYMBOL_55_ = 55,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 56,                  /* $accept  */
  YYSYMBOL_fsmlang = 57,                   /* fsmlang  */
  YYSYMBOL_machine_prefix = 58,            /* machine_prefix  */
  YYSYMBOL_machine = 59,                   /* machine  */
  YYSYMBOL_60_1 = 60,                      /* $@1  */
  YYSYMBOL_machine_qualifier = 61,         /* machine_qualifier  */
  YYSYMBOL_machine_transition_decl = 62,   /* machine_transition_decl  */
  YYSYMBOL_action_return_spec = 63,        /* action_return_spec  */
  YYSYMBOL_machine_modifier = 64,          /* machine_modifier  */
  YYSYMBOL_statement_decl_list = 65,       /* statement_decl_list  */
  YYSYMBOL_sequence_start = 66,            /* sequence_start  */
  YYSYMBOL_sequence_node = 67,             /* sequence_node  */
  YYSYMBOL_sequence_nodes = 68,            /* sequence_nodes  */
  YYSYMBOL_sequence = 69,                  /* sequence  */
  YYSYMBOL_sequences = 70,                 /* sequences  */
  YYSYMBOL_machine_list = 71,              /* machine_list  */
  YYSYMBOL_actions_and_transitions = 72,   /* actions_and_transitions  */
  YYSYMBOL_transition_matrix_list = 73,    /* transition_matrix_list  */
  YYSYMBOL_transition_matrix_start = 74,   /* transition_matrix_start  */
  YYSYMBOL_transition_matrix = 75,         /* transition_matrix  */
  YYSYMBOL_action_decl = 76,               /* action_decl  */
  YYSYMBOL_action_decl_list = 77,          /* action_decl_list  */
  YYSYMBOL_action = 78,                    /* action  */
  YYSYMBOL_transition = 79,                /* transition  */
  YYSYMBOL_action_matrix = 80,             /* action_matrix  */
  YYSYMBOL_matrix = 81,                    /* matrix  */
  YYSYMBOL_state_vector = 82,              /* state_vector  */
  YYSYMBOL_state_comma_list = 83,          /* state_comma_list  */
  YYSYMBOL_event_vector = 84,              /* event_vector  */
  YYSYMBOL_event_comma_list = 85,          /* event_comma_list  */
  YYSYMBOL_state_and_event_decls = 86,     /* state_and_event_decls  */
  YYSYMBOL_state_decl = 87,                /* state_decl  */
  YYSYMBOL_state = 88,                     /* state  */
  YYSYMBOL_state_decl_list = 89,           /* state_decl_list  */
  YYSYMBOL_event_decl = 90,                /* event_decl  */
  YYSYMBOL_parent_namespace = 91,          /* parent_namespace  */
  YYSYMBOL_user_event_data = 92,           /* user_event_data  */
  YYSYMBOL_event_decl_list = 93,           /* event_decl_list  */
  YYSYMBOL_external_designation = 94,      /* external_designation  */
  YYSYMBOL_native = 95,                    /* native  */
  YYSYMBOL_native_prologue = 96,           /* native_prologue  */
  YYSYMBOL_native_epilogue = 97,           /* native_epilogue  */
  YYSYMBOL_native_impl = 98,               /* native_impl  */
  YYSYMBOL_native_impl_prologue = 99,      /* native_impl_prologue  */
  YYSYMBOL_native_impl_epilogue = 100,     /* native_impl_epilogue  */
  YYSYMBOL_machine_data = 101,             /* machine_data  */
  YYSYMBOL_data_block = 102,               /* data_block  */
  YYSYMBOL_data_fields = 103,              /* data_fields  */
  YYSYMBOL_data_type = 104,                /* data_type  */
  YYSYMBOL_data_field = 105,               /* data_field  */
  YYSYMBOL_data_field_dimension = 106,     /* data_field_dimension  */
  YYSYMBOL_namespace = 107,                /* namespace  */
  YYSYMBOL_namespace_event_ref = 108,      /* namespace_event_ref  */
  YYSYMBOL_returns_comma_list = 109,       /* returns_comma_list  */
  YYSYMBOL_action_return_decl = 110,       /* action_return_decl  */
  YYSYMBOL_transition_fn_return_decl = 111 /* transition_fn_return_decl  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYLAST   257

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  253

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


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
      52,    53,    55,     2,    49,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    48,
       2,    54,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    50,     2,    51,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
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
      45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   163,   163,   181,   201,   229,   228,   466,   470,   478,
     485,   493,   502,   511,   526,   534,   545,   549,   556,   559,
     565,   577,   589,   605,   622,   629,   636,   644,   653,   666,
     676,   683,   693,   707,   722,   744,   756,   768,   789,   809,
     850,   871,   892,   910,   938,   942,   949,   955,   977,   999,
    1021,  1045,  1057,  1069,  1082,  1096,  1148,  1185,  1195,  1209,
    1219,  1230,  1240,  1251,  1263,  1288,  1312,  1332,  1341,  1355,
    1371,  1385,  1401,  1410,  1424,  1441,  1459,  1478,  1487,  1496,
    1503,  1513,  1528,  1555,  1569,  1583,  1601,  1615,  1635,  1650,
    1668,  1682,  1694,  1695,  1711,  1725,  1739,  1760,  1801,  1825,
    1867,  1870,  1881,  1884,  1904,  1926,  1934,  1946,  1958,  1965,
    1972,  1982,  1995,  2003,  2013,  2024,  2026,  2037,  2049,  2061,
    2076,  2089,  2102,  2115,  2133,  2155,  2176,  2180,  2184,  2191,
    2192,  2201,  2212,  2222,  2232,  2240,  2251,  2270,  2289,  2305,
    2321,  2340,  2361,  2380
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
  "ACTION_KEY", "TRANSITION_KEY", "GUARD_KEY", "PARENT", "NATIVE_KEY",
  "NATIVE_BLOCK", "MACHINE", "STATE", "EVENT", "ACTION", "TRANSITION_FN",
  "ID", "NUMERIC_STRING", "TYPE_NAME", "'{'", "'}'", "';'", "','", "'['",
  "']'", "'('", "')'", "'='", "'*'", "$accept", "fsmlang",
  "machine_prefix", "machine", "$@1", "machine_qualifier",
  "machine_transition_decl", "action_return_spec", "machine_modifier",
  "statement_decl_list", "sequence_start", "sequence_node",
  "sequence_nodes", "sequence", "sequences", "machine_list",
  "actions_and_transitions", "transition_matrix_list",
  "transition_matrix_start", "transition_matrix", "action_decl",
  "action_decl_list", "action", "transition", "action_matrix", "matrix",
  "state_vector", "state_comma_list", "event_vector", "event_comma_list",
  "state_and_event_decls", "state_decl", "state", "state_decl_list",
  "event_decl", "parent_namespace", "user_event_data", "event_decl_list",
  "external_designation", "native", "native_prologue", "native_epilogue",
  "native_impl", "native_impl_prologue", "native_impl_epilogue",
  "machine_data", "data_block", "data_fields", "data_type", "data_field",
  "data_field_dimension", "namespace", "namespace_event_ref",
  "returns_comma_list", "action_return_decl", "transition_fn_return_decl", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-143)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-103)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -143,    29,    35,  -143,    13,  -143,  -143,    14,  -143,    11,
      60,  -143,  -143,    92,   103,   144,    14,  -143,  -143,   106,
    -143,  -143,   145,   146,  -143,  -143,   137,   115,    23,   121,
    -143,  -143,   106,  -143,  -143,  -143,  -143,   133,   136,   138,
     139,   148,   151,  -143,   127,  -143,  -143,  -143,  -143,  -143,
    -143,   147,    -9,   149,    33,     5,  -143,    62,  -143,    98,
     143,  -143,    40,   185,   140,   152,    49,  -143,  -143,  -143,
      72,  -143,   160,    85,   150,   150,   176,   179,    25,  -143,
       9,    63,  -143,    -7,  -143,  -143,   102,   119,  -143,  -143,
    -143,  -143,  -143,   147,  -143,    31,     5,   130,   175,  -143,
     155,   190,   190,   156,   157,  -143,  -143,    -3,    65,  -143,
     162,   171,   177,  -143,     3,  -143,   150,   150,  -143,   123,
    -143,  -143,    83,   167,  -143,    67,   158,  -143,  -143,  -143,
    -143,  -143,   161,   163,   164,  -143,    85,  -143,  -143,    40,
     140,   168,    63,   170,   172,  -143,  -143,    12,  -143,  -143,
      49,    49,  -143,  -143,    95,  -143,  -143,   178,   180,   181,
    -143,   174,  -143,  -143,    96,   116,  -143,   206,   112,   114,
     182,  -143,   183,   117,    89,   120,   186,   165,   173,   184,
     -11,  -143,  -143,  -143,  -143,   190,   190,  -143,  -143,   187,
    -143,    36,    46,  -143,    58,   188,  -143,  -143,   189,  -143,
    -143,  -143,  -143,  -143,  -143,  -143,  -143,  -143,  -143,  -143,
     122,  -143,  -143,  -143,   124,   126,  -143,   128,  -143,  -143,
      88,  -143,    75,   191,  -143,  -143,   149,  -143,  -143,   192,
     193,  -143,  -143,  -143,  -143,  -143,  -143,  -143,  -143,  -143,
    -143,  -143,  -143,   194,   196,   195,  -143,  -143,  -143,  -143,
    -143,    91,  -143
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     102,   102,     0,     2,    18,     1,     3,     7,    19,     0,
       0,   103,   104,     0,     0,     0,     5,     8,     9,    10,
     108,   109,     0,     0,   105,     4,     0,     0,     0,     0,
      11,    12,    13,   110,   111,   106,   107,     0,     0,     0,
       0,     0,     0,   112,     0,    14,    16,    15,    17,   113,
     114,     0,     0,     0,     0,   102,    77,     0,    78,     0,
       0,    82,    88,     0,   100,     0,     0,   115,    23,     6,
       0,    33,    22,     0,     0,     0,     0,     0,     0,    35,
      37,    20,    39,     0,    46,    38,     0,     0,    79,    80,
      40,    41,    81,     0,    90,     0,   102,     0,     0,    91,
       0,    92,    92,     0,     0,   119,   120,     0,     0,   117,
       0,    27,    24,    29,     0,    34,     0,     0,    55,    57,
      48,    50,     0,     0,    74,     0,     0,    36,    43,    42,
      44,    45,     0,     0,     0,    54,     0,    47,    49,    89,
     100,     0,    21,    84,    86,    83,   101,     0,    96,    97,
       0,     0,   116,   118,     0,   123,    26,     0,     0,     0,
      31,     0,    65,    64,     0,     0,    58,     0,     0,     0,
       0,   129,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    53,    52,    56,    92,    92,    85,    87,     0,
      95,     0,     0,   125,     0,     0,    28,    25,     0,    30,
      59,    61,    60,    63,    62,   130,   141,    70,   138,   133,
       0,   131,   139,   132,     0,     0,   143,     0,    72,    75,
       0,    69,     0,     0,    98,    99,    93,   121,   122,     0,
       0,   126,   124,    32,   140,    71,   136,   135,   137,   134,
     142,    76,    73,     0,     0,     0,    66,    94,   128,   127,
      67,     0,    68
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -143,  -143,  -143,     2,  -143,  -143,   200,   210,  -143,  -143,
    -143,    66,  -143,   159,  -143,  -143,   132,   -75,  -143,  -143,
     -74,  -143,    93,  -143,  -143,   -60,  -143,  -123,  -143,  -143,
     197,   -43,   142,  -143,   -42,   -36,   -98,  -143,    99,  -143,
    -143,  -143,   216,    84,    87,  -143,  -142,    28,  -143,  -106,
    -143,  -143,    59,  -143,   -73,   -72
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,    79,    29,    16,    17,    18,    10,    54,
      70,   113,   114,    71,    72,    80,    81,    82,    83,    84,
      85,    86,   118,   166,   119,    87,   223,   170,   126,   179,
      55,    56,    62,    57,    58,   171,   148,    59,   101,     4,
      11,    12,    19,    20,    21,    60,    67,   107,   108,   109,
     195,   172,   173,   174,    90,    91
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     176,   153,     3,     6,   149,   190,   128,   129,   130,   131,
      51,    52,    88,    89,   120,   121,    65,    13,    22,    23,
    -102,  -102,   189,   103,   104,     8,    63,    14,   221,     5,
      41,    42,   132,   159,    64,   133,   134,    73,    74,    75,
     105,   222,   106,    97,   152,  -102,    76,    77,    24,     9,
      15,   160,   161,    88,    89,    78,   162,   163,    66,   141,
      43,    98,   103,   104,    68,   124,    63,   128,   129,   130,
     131,    25,   103,   104,   140,   103,   104,   125,     7,   105,
      69,   106,   127,   227,   247,   153,   153,   224,   225,   105,
     177,   106,   105,   228,   106,    73,    74,    75,   243,   245,
     110,   229,   230,    33,    76,    77,    34,   178,   154,   231,
      92,    93,   111,    78,   244,   112,    33,    27,    63,    34,
     155,   167,   168,   169,    63,    26,   116,   167,   117,   214,
      38,    39,    51,    52,    40,   200,    53,   241,   201,   202,
     235,   242,    15,   193,   252,   194,    94,    95,    51,    52,
     135,   136,   137,   138,   143,   144,   164,   165,   203,   204,
     206,   207,   208,   209,    28,   212,   213,    44,   216,   207,
     234,   235,   236,   237,   238,   239,   240,   235,   191,   192,
      37,    45,    35,    36,    46,    49,    47,    48,    50,    99,
      61,    68,   102,   122,   100,    66,   123,   145,   146,   147,
      78,   156,   150,   151,   157,   158,   175,   180,   186,   181,
     205,   182,   183,   187,   111,   188,    30,   196,   218,   197,
     198,   210,   219,   211,   220,   217,    31,   199,   142,   184,
     226,   115,    32,   215,   251,   139,   232,   233,     0,   185,
       0,     0,   246,   248,   249,   207,     0,   250,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96
};

static const yytype_int16 yycheck[] =
{
     123,   107,     0,     1,   102,   147,    81,    81,    81,    81,
       5,     6,    55,    55,    74,    75,    52,     3,     7,     8,
      11,    12,    10,    26,    27,    12,    35,    13,    39,     0,
       7,     8,    39,    30,    43,    42,    43,    32,    33,    34,
      43,    52,    45,     3,    47,    36,    41,    42,    37,    36,
      36,    48,    49,    96,    96,    50,   116,   117,    46,    95,
      37,    21,    26,    27,    31,    40,    35,   142,   142,   142,
     142,    11,    26,    27,    43,    26,    27,    52,    43,    43,
      47,    45,    80,    47,   226,   191,   192,   185,   186,    43,
      23,    45,    43,    47,    45,    32,    33,    34,    23,   222,
      28,    43,    44,    19,    41,    42,    19,    40,    43,    51,
      48,    49,    40,    50,    39,    43,    32,    14,    35,    32,
      55,    38,    39,    40,    35,    33,    41,    38,    43,    40,
      15,    16,     5,     6,    19,    39,     9,    49,    42,    43,
      49,    53,    36,    48,    53,    50,    48,    49,     5,     6,
      48,    49,    33,    34,    24,    25,    33,    34,    42,    43,
      48,    49,    48,    49,    20,    48,    49,    46,    48,    49,
      48,    49,    48,    49,    48,    49,    48,    49,   150,   151,
      43,    48,    37,    37,    48,    37,    48,    48,    37,     4,
      43,    31,    40,    17,    54,    46,    17,    22,    43,     9,
      50,    39,    46,    46,    33,    28,    39,    49,    40,    48,
       4,    48,    48,    43,    40,    43,    16,    39,    53,    39,
      39,    39,    49,    40,    40,    39,    16,   161,    96,   136,
      43,    72,    16,   174,    39,    93,    48,    48,    -1,   140,
      -1,    -1,    51,    51,    51,    49,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    57,    58,    59,    95,     0,    59,    43,    12,    36,
      64,    96,    97,     3,    13,    36,    61,    62,    63,    98,
      99,   100,     7,     8,    37,    11,    33,    14,    20,    60,
      62,    63,    98,    99,   100,    37,    37,    43,    15,    16,
      19,     7,     8,    37,    46,    48,    48,    48,    48,    37,
      37,     5,     6,     9,    65,    86,    87,    89,    90,    93,
     101,    43,    88,    35,    43,    91,    46,   102,    31,    47,
      66,    69,    70,    32,    33,    34,    41,    42,    50,    59,
      71,    72,    73,    74,    75,    76,    77,    81,    87,    90,
     110,   111,    48,    49,    48,    49,    86,     3,    21,     4,
      54,    94,    40,    26,    27,    43,    45,   103,   104,   105,
      28,    40,    43,    67,    68,    69,    41,    43,    78,    80,
      81,    81,    17,    17,    40,    52,    84,    59,    73,    76,
     110,   111,    39,    42,    43,    48,    49,    33,    34,    88,
      43,    91,    72,    24,    25,    22,    43,     9,    92,    92,
      46,    46,    47,   105,    43,    55,    39,    33,    28,    30,
      48,    49,    81,    81,    33,    34,    79,    38,    39,    40,
      83,    91,   107,   108,   109,    39,    83,    23,    40,    85,
      49,    48,    48,    48,    78,    94,    40,    43,    43,    10,
     102,   103,   103,    48,    50,   106,    39,    39,    39,    67,
      39,    42,    43,    42,    43,     4,    48,    49,    48,    49,
      39,    40,    48,    49,    40,   108,    48,    39,    53,    49,
      40,    39,    52,    82,    92,    92,    43,    47,    47,    43,
      44,    51,    48,    48,    48,    49,    48,    49,    48,    49,
      48,    49,    53,    23,    39,    83,    51,   102,    51,    51,
      53,    39,    53
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    56,    57,    57,    58,    60,    59,    61,    61,    61,
      61,    61,    61,    61,    62,    63,    63,    63,    64,    64,
      65,    65,    65,    66,    66,    66,    66,    67,    67,    68,
      68,    69,    69,    70,    70,    71,    71,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    73,    74,    74,    74,
      74,    75,    75,    75,    76,    77,    77,    78,    78,    79,
      79,    79,    79,    79,    80,    80,    81,    82,    82,    82,
      83,    83,    84,    84,    84,    85,    85,    86,    86,    86,
      86,    87,    88,    88,    88,    88,    88,    88,    89,    89,
      90,    91,    92,    92,    92,    92,    93,    93,    93,    93,
      94,    94,    95,    95,    95,    96,    96,    97,    98,    98,
      98,    98,    99,    99,   100,   101,   102,   103,   103,   104,
     104,   104,   104,   104,   105,   105,   106,   106,   106,   107,
     107,   108,   109,   109,   109,   109,   110,   110,   110,   110,
     110,   110,   111,   111
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     3,     0,     7,     0,     1,     1,
       1,     2,     2,     2,     4,     4,     4,     4,     0,     1,
       2,     3,     2,     1,     2,     4,     3,     1,     3,     1,
       3,     3,     5,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     3,     3,     3,     2,     2,     3,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     5,     3,     4,     1,
       2,     3,     3,     4,     1,     2,     3,     1,     1,     2,
       2,     2,     1,     3,     3,     4,     3,     4,     2,     3,
       2,     2,     0,     3,     4,     2,     4,     4,     5,     5,
       0,     2,     0,     2,     2,     2,     3,     3,     1,     1,
       2,     2,     3,     4,     4,     2,     3,     1,     2,     1,
       1,     4,     4,     2,     4,     3,     2,     3,     3,     1,
       2,     2,     2,     2,     3,     3,     5,     5,     4,     4,
       5,     4,     5,     4
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
#line 164 "../src/parser.y"
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
#line 1597 "y.tab.c"
    break;

  case 3: /* fsmlang: fsmlang machine  */
#line 182 "../src/parser.y"
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
#line 1619 "y.tab.c"
    break;

  case 4: /* machine_prefix: native machine_modifier MACHINE_KEY  */
#line 202 "../src/parser.y"
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
#line 1648 "y.tab.c"
    break;

  case 5: /* $@1: %empty  */
#line 229 "../src/parser.y"
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
#line 1673 "y.tab.c"
    break;

  case 6: /* machine: machine_prefix ID machine_qualifier $@1 '{' statement_decl_list '}'  */
#line 250 "../src/parser.y"
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
#line 1891 "y.tab.c"
    break;

  case 7: /* machine_qualifier: %empty  */
#line 466 "../src/parser.y"
        {
    					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
    						yyerror("Out of memory");
        }
#line 1900 "y.tab.c"
    break;

  case 8: /* machine_qualifier: machine_transition_decl  */
#line 471 "../src/parser.y"
                     {
 					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					(yyval.pmachine_qualifier)->machineTransition = (yyvsp[0].pid_info);
 				
		     }
#line 1912 "y.tab.c"
    break;

  case 9: /* machine_qualifier: action_return_spec  */
#line 479 "../src/parser.y"
                     {
 					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					(yyval.pmachine_qualifier)->modFlags = (yyvsp[0].mod_flags);
		     }
#line 1923 "y.tab.c"
    break;

  case 10: /* machine_qualifier: native_impl  */
#line 486 "../src/parser.y"
                     {
 					if (NULL == ((yyval.pmachine_qualifier) = (pMACHINE_QUALIFIER)calloc(1, sizeof(MACHINE_QUALIFIER))))
 						yyerror("Out of memory");

 					(yyval.pmachine_qualifier)->native_impl_prologue = (yyvsp[0].pnative_info)->prologue;
 					(yyval.pmachine_qualifier)->native_impl_epilogue = (yyvsp[0].pnative_info)->epilogue;
		     }
#line 1935 "y.tab.c"
    break;

  case 11: /* machine_qualifier: machine_qualifier machine_transition_decl  */
#line 494 "../src/parser.y"
                     {
           if ((yyvsp[-1].pmachine_qualifier)->machineTransition)
             yyerror("only one machine transition declaration allowed per machine");

 					(yyvsp[-1].pmachine_qualifier)->machineTransition = (yyvsp[0].pid_info);

           (yyval.pmachine_qualifier) = (yyvsp[-1].pmachine_qualifier);
		     }
#line 1948 "y.tab.c"
    break;

  case 12: /* machine_qualifier: machine_qualifier action_return_spec  */
#line 503 "../src/parser.y"
                     {
           if ((yyvsp[-1].pmachine_qualifier)->modFlags != mfNone)
             yyerror("only one action return spec allowed per machine");

 					(yyvsp[-1].pmachine_qualifier)->modFlags          = (yyvsp[0].mod_flags);

           (yyval.pmachine_qualifier) = (yyvsp[-1].pmachine_qualifier);
		     }
#line 1961 "y.tab.c"
    break;

  case 13: /* machine_qualifier: machine_qualifier native_impl  */
#line 512 "../src/parser.y"
                     {
           if ((yyvsp[-1].pmachine_qualifier)->native_impl_prologue)
             yyerror("only one native_prologue implementation allowed per machine");

           if ((yyvsp[-1].pmachine_qualifier)->native_impl_epilogue)
             yyerror("only one native_epilogue implementation allowed per machine");

           (yyvsp[-1].pmachine_qualifier)->native_impl_prologue = (yyvsp[0].pnative_info)->prologue;
           (yyvsp[-1].pmachine_qualifier)->native_impl_epilogue = (yyvsp[0].pnative_info)->epilogue;

           (yyval.pmachine_qualifier) = (yyvsp[-1].pmachine_qualifier);
		     }
#line 1978 "y.tab.c"
    break;

  case 14: /* machine_transition_decl: ON TRANSITION_KEY ID ';'  */
#line 527 "../src/parser.y"
    {
 	 		(yyval.pid_info)          = (yyvsp[-1].pid_info);
 			(yyvsp[-1].pid_info)->docCmnt = (yyvsp[-2].charData);
    }
#line 1987 "y.tab.c"
    break;

  case 15: /* action_return_spec: ACTIONS RETURN EVENTS ';'  */
#line 535 "../src/parser.y"
        {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(id_list, EVENT,"noEvent",&pid_info);
           pid_info->powningMachine = pmachineInfo;

 					(yyval.mod_flags) = 0;
        }
#line 2002 "y.tab.c"
    break;

  case 16: /* action_return_spec: ACTIONS RETURN STATES ';'  */
#line 546 "../src/parser.y"
                                        {
 					(yyval.mod_flags) = mfActionsReturnStates;
					}
#line 2010 "y.tab.c"
    break;

  case 17: /* action_return_spec: ACTIONS RETURN VOID ';'  */
#line 550 "../src/parser.y"
                                        {
						(yyval.mod_flags) = mfActionsReturnVoid;
					}
#line 2018 "y.tab.c"
    break;

  case 18: /* machine_modifier: %empty  */
#line 556 "../src/parser.y"
        {
 		(yyval.mod_flags) = 0;
	}
#line 2026 "y.tab.c"
    break;

  case 19: /* machine_modifier: REENTRANT  */
#line 560 "../src/parser.y"
                                        {
						(yyval.mod_flags) = mfReentrant;
					}
#line 2034 "y.tab.c"
    break;

  case 20: /* statement_decl_list: state_and_event_decls actions_and_transitions  */
#line 566 "../src/parser.y"
        {
    	if (NULL == ((yyval.pstatement_decl_list) = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
    		yyerror("Out of memory");
   
    	(yyval.pstatement_decl_list)->pstate_and_event_decls   = (yyvsp[-1].pstate_and_event_decls);
    	(yyval.pstatement_decl_list)->pactions_and_transitions = (yyvsp[0].pactions_and_transitions);

			/* This allows the event sequence parser to set the proper default for the initial state. */
			pmachineInfo->state_list = (yyvsp[-1].pstate_and_event_decls)->state_decls;
   
   	}
#line 2050 "y.tab.c"
    break;

  case 21: /* statement_decl_list: machine_data state_and_event_decls actions_and_transitions  */
#line 578 "../src/parser.y"
        {
    	if (NULL == ((yyval.pstatement_decl_list) = (pSTATEMENT_DECL_LIST)calloc(1, sizeof(STATEMENT_DECL_LIST))))
    		yyerror("Out of memory");
   
      (yyval.pstatement_decl_list)->data                     = (yyvsp[-2].plist);
    	(yyval.pstatement_decl_list)->pstate_and_event_decls   = (yyvsp[-1].pstate_and_event_decls);
    	(yyval.pstatement_decl_list)->pactions_and_transitions = (yyvsp[0].pactions_and_transitions);
   
			/* This allows the event sequence parser to set the proper default for the initial state. */
			pmachineInfo->state_list = (yyvsp[-1].pstate_and_event_decls)->state_decls;
   	}
#line 2066 "y.tab.c"
    break;

  case 22: /* statement_decl_list: statement_decl_list sequences  */
#line 590 "../src/parser.y"
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
#line 2084 "y.tab.c"
    break;

  case 23: /* sequence_start: SEQUENCE_KEY  */
#line 606 "../src/parser.y"
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
#line 2105 "y.tab.c"
    break;

  case 24: /* sequence_start: sequence_start ID  */
#line 623 "../src/parser.y"
        {
		(yyval.psequence) = (yyvsp[-1].psequence);
		set_id_type((yyvsp[0].pid_info),EVENT_SEQ);
		(yyval.psequence)->name = (yyvsp[0].pid_info);
		(yyval.psequence)->initial_state = statePidByIndex(pmachineInfo, 0);
	}
#line 2116 "y.tab.c"
    break;

  case 25: /* sequence_start: sequence_start ID START_KEY STATE  */
#line 630 "../src/parser.y"
        {
		(yyval.psequence) = (yyvsp[-3].psequence);
		set_id_type((yyvsp[-2].pid_info),EVENT_SEQ);
		(yyval.psequence)->name = (yyvsp[-2].pid_info);
		(yyval.psequence)->initial_state = (yyvsp[0].pid_info);
	}
#line 2127 "y.tab.c"
    break;

  case 26: /* sequence_start: sequence_start START_KEY STATE  */
#line 637 "../src/parser.y"
        {
		(yyval.psequence) = (yyvsp[-2].psequence);
		(yyval.psequence)->initial_state = (yyvsp[0].pid_info);
	}
#line 2136 "y.tab.c"
    break;

  case 27: /* sequence_node: EVENT  */
#line 645 "../src/parser.y"
  {
    if (((yyval.pevent_sequence_node) = (pEVENT_SEQUENCE_NODE) calloc(1, sizeof(EVENT_SEQUENCE_NODE))) == NULL)
    {
      yyerror("out of memory");
    }

    (yyval.pevent_sequence_node)->pevent = (yyvsp[0].pid_info);
  }
#line 2149 "y.tab.c"
    break;

  case 28: /* sequence_node: EVENT TRANSITION_KEY STATE  */
#line 654 "../src/parser.y"
  {
    if (((yyval.pevent_sequence_node) = (pEVENT_SEQUENCE_NODE) calloc(1, sizeof(EVENT_SEQUENCE_NODE))) == NULL)
    {
      yyerror("out of memory");
    }

    (yyval.pevent_sequence_node)->pevent = (yyvsp[-2].pid_info);
    (yyval.pevent_sequence_node)->pnew_state = (yyvsp[0].pid_info);
  }
#line 2163 "y.tab.c"
    break;

  case 29: /* sequence_nodes: sequence_node  */
#line 667 "../src/parser.y"
  {
    if (((yyval.plist) = init_list()) == NULL)
    {
      yyerror("out of memory");
    }

    add_to_list((yyval.plist), (yyvsp[0].pevent_sequence_node));

  }
#line 2177 "y.tab.c"
    break;

  case 30: /* sequence_nodes: sequence_nodes ',' sequence_node  */
#line 677 "../src/parser.y"
  {
    add_to_list((yyvsp[-2].plist), (yyvsp[0].pevent_sequence_node));
    (yyval.plist) = (yyvsp[-2].plist);
  }
#line 2186 "y.tab.c"
    break;

  case 31: /* sequence: sequence_start sequence_nodes ';'  */
#line 684 "../src/parser.y"
        {
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found an event sequence\n");
		#endif

		(yyval.psequence) = (yyvsp[-2].psequence);
		(yyval.psequence)->sequence = (yyvsp[-1].plist);

	}
#line 2200 "y.tab.c"
    break;

  case 32: /* sequence: sequence_start sequence_nodes END_KEY STATE ';'  */
#line 694 "../src/parser.y"
        {
		#ifdef PARSER_DEBUG
		fprintf(yyout, "Found an event sequence indicating an end state\n");
		#endif

		(yyval.psequence) = (yyvsp[-4].psequence);
		(yyval.psequence)->sequence    = (yyvsp[-3].plist);
		(yyval.psequence)->final_state = (yyvsp[-1].pid_info);

	}
#line 2215 "y.tab.c"
    break;

  case 33: /* sequences: sequence  */
#line 708 "../src/parser.y"
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
#line 2234 "y.tab.c"
    break;

  case 34: /* sequences: sequences sequence  */
#line 723 "../src/parser.y"
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
#line 2256 "y.tab.c"
    break;

  case 35: /* machine_list: machine  */
#line 745 "../src/parser.y"
    {
        if (NULL == ((yyval.plist) = init_list()))
            yyerror("out of memory");

        if (NULL == add_to_list((yyval.plist),(yyvsp[0].pmachineInfo)))
            yyerror("out of memory");

				/* Bump the depth count. The function is a no-op if we're at the top. */
				increase_sub_machine_depth((yyvsp[0].pmachineInfo)->parent);

    }
#line 2272 "y.tab.c"
    break;

  case 36: /* machine_list: machine_list machine  */
#line 757 "../src/parser.y"
    {

        (yyval.plist) = (yyvsp[-1].plist);

        if (NULL == add_to_list((yyval.plist),(yyvsp[0].pmachineInfo)))
            yyerror("out of memory");

    }
#line 2285 "y.tab.c"
    break;

  case 37: /* actions_and_transitions: machine_list  */
#line 769 "../src/parser.y"
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
#line 2310 "y.tab.c"
    break;

  case 38: /* actions_and_transitions: action_decl  */
#line 790 "../src/parser.y"
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
#line 2334 "y.tab.c"
    break;

  case 39: /* actions_and_transitions: transition_matrix_list  */
#line 810 "../src/parser.y"
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
				switch ((yyvsp[0].action_info)->transition->type)
				{
 				case STATE:
						if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->transition_list, (yyvsp[0].action_info)->transition))
 						yyerror("out of memory");
 					break;

 				case TRANSITION_FN:
						if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->transition_fn_list, (yyvsp[0].action_info)->transition))
 						yyerror("out of memory");
 					break;

					//there are no other choices
				}
			}
	  }
#line 2379 "y.tab.c"
    break;

  case 40: /* actions_and_transitions: action_return_decl  */
#line 851 "../src/parser.y"
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
#line 2404 "y.tab.c"
    break;

  case 41: /* actions_and_transitions: transition_fn_return_decl  */
#line 872 "../src/parser.y"
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
#line 2429 "y.tab.c"
    break;

  case 42: /* actions_and_transitions: actions_and_transitions action_decl  */
#line 893 "../src/parser.y"
          {
 		(yyval.pactions_and_transitions) = (yyvsp[-1].pactions_and_transitions);

 		move_list_unique((yyvsp[-1].pactions_and_transitions)->action_list, (yyvsp[0].paction_decl)->action_list);
 		free_list((yyvsp[0].paction_decl)->action_list);

 		move_list((yyvsp[-1].pactions_and_transitions)->action_info_list, (yyvsp[0].paction_decl)->action_info_list);
 		free((yyvsp[0].paction_decl)->action_info_list);

 		move_list_unique((yyvsp[-1].pactions_and_transitions)->transition_list, (yyvsp[0].paction_decl)->transition_list);
 		free_list((yyvsp[0].paction_decl)->transition_list);

 		move_list_unique((yyvsp[-1].pactions_and_transitions)->transition_fn_list, (yyvsp[0].paction_decl)->transition_fn_list);
 		free_list((yyvsp[0].paction_decl)->transition_fn_list);

 		free((yyvsp[0].paction_decl));
	  }
#line 2451 "y.tab.c"
    break;

  case 43: /* actions_and_transitions: actions_and_transitions transition_matrix_list  */
#line 911 "../src/parser.y"
          {
 		(yyval.pactions_and_transitions)=(yyvsp[-1].pactions_and_transitions);

			if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->action_list, (yyvsp[0].action_info)->action))
 				yyerror("out of memory");

			if (NULL == add_to_list((yyval.pactions_and_transitions)->action_info_list, (yyvsp[0].action_info)))
 			yyerror("out of memory");

			if ((yyvsp[0].action_info)->transition)
			{
				switch ((yyvsp[0].action_info)->transition->type)
				{
 				case STATE:
						if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->transition_list, (yyvsp[0].action_info)->transition))
 						yyerror("out of memory");
 					break;

 				case TRANSITION_FN:
						if (NULL == add_unique_to_list((yyval.pactions_and_transitions)->transition_fn_list, (yyvsp[0].action_info)->transition))
 						yyerror("out of memory");
 					break;

					//there are no other choices
				}
			}
	  }
#line 2483 "y.tab.c"
    break;

  case 44: /* actions_and_transitions: actions_and_transitions action_return_decl  */
#line 939 "../src/parser.y"
          {
			(yyval.pactions_and_transitions) = (yyvsp[-1].pactions_and_transitions);
	  }
#line 2491 "y.tab.c"
    break;

  case 45: /* actions_and_transitions: actions_and_transitions transition_fn_return_decl  */
#line 943 "../src/parser.y"
          {
			(yyval.pactions_and_transitions) = (yyvsp[-1].pactions_and_transitions);
	  }
#line 2499 "y.tab.c"
    break;

  case 46: /* transition_matrix_list: transition_matrix  */
#line 950 "../src/parser.y"
                                        {
						(yyval.action_info) = (yyvsp[0].action_info);
					}
#line 2507 "y.tab.c"
    break;

  case 47: /* transition_matrix_start: matrix TRANSITION_KEY  */
#line 956 "../src/parser.y"
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
#line 2533 "y.tab.c"
    break;

  case 48: /* transition_matrix_start: TRANSITION_KEY matrix  */
#line 978 "../src/parser.y"
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
#line 2559 "y.tab.c"
    break;

  case 49: /* transition_matrix_start: matrix GUARD_KEY  */
#line 1000 "../src/parser.y"
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
#line 2585 "y.tab.c"
    break;

  case 50: /* transition_matrix_start: GUARD_KEY matrix  */
#line 1022 "../src/parser.y"
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
#line 2611 "y.tab.c"
    break;

  case 51: /* transition_matrix: transition_matrix_start STATE ';'  */
#line 1046 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix\n");
						#endif

           (yyval.action_info) = (yyvsp[-2].action_info);

						(yyval.action_info)->transition = (yyvsp[-1].pid_info);

					}
#line 2627 "y.tab.c"
    break;

  case 52: /* transition_matrix: transition_matrix_start ID ';'  */
#line 1058 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type((yyvsp[-1].pid_info),TRANSITION_FN);

						(yyval.action_info)->transition = (yyvsp[-1].pid_info);

					}
#line 2643 "y.tab.c"
    break;

  case 53: /* transition_matrix: transition_matrix_start TRANSITION_FN ';'  */
#line 1070 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						(yyval.action_info)->transition = (yyvsp[-1].pid_info);

        }
#line 2657 "y.tab.c"
    break;

  case 54: /* action_decl: action_decl_list ';'  */
#line 1083 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the actions */
						fprintf(yyout,"The actions in this list:\n");
						parser_debug_print_id_list_names((yyvsp[-1].paction_decl)->action_list,pmachineInfo,yyout,"noAction");
						#endif

						(yyval.paction_decl) = (yyvsp[-1].paction_decl);

					}
#line 2673 "y.tab.c"
    break;

  case 55: /* action_decl_list: ACTION_KEY action  */
#line 1097 "../src/parser.y"
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

							switch ((yyvsp[0].action_info)->transition->type)
							{
 							case STATE:
									if (NULL == add_unique_to_list((yyval.paction_decl)->transition_list, (yyvsp[0].action_info)->transition))
 									yyerror("out of memory");
 								break;

 							case TRANSITION_FN:
									if (NULL == add_unique_to_list((yyval.paction_decl)->transition_fn_list, (yyvsp[0].action_info)->transition))
 									yyerror("out of memory");
 								break;

								//there are no other choices
							}

						}

 					(yyvsp[0].action_info)->docCmnt = (yyvsp[-1].charData);

					}
#line 2728 "y.tab.c"
    break;

  case 56: /* action_decl_list: action_decl_list ',' action  */
#line 1149 "../src/parser.y"
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
							switch ((yyvsp[0].action_info)->transition->type)
							{
 							case STATE:
									if (NULL == add_unique_to_list((yyval.paction_decl)->transition_list, (yyvsp[0].action_info)->transition))
 									yyerror("out of memory");
 								break;

 							case TRANSITION_FN:
									if (NULL == add_unique_to_list((yyval.paction_decl)->transition_fn_list, (yyvsp[0].action_info)->transition))
 									yyerror("out of memory");
 								break;

								//there are no other choices
							}

						}

					}
#line 2767 "y.tab.c"
    break;

  case 57: /* action: action_matrix  */
#line 1186 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action without transition\n");
						#endif

						(yyval.action_info) = (yyvsp[0].action_info);

					}
#line 2781 "y.tab.c"
    break;

  case 58: /* action: action_matrix transition  */
#line 1196 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action with transition\n");
						#endif

						(yyvsp[-1].action_info)->transition = (yyvsp[0].pid_info);

						(yyval.action_info) = (yyvsp[-1].action_info);

					}
#line 2797 "y.tab.c"
    break;

  case 59: /* transition: TRANSITION_KEY STATE  */
#line 1210 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition to known state\n");
						#endif

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2811 "y.tab.c"
    break;

  case 60: /* transition: TRANSITION_KEY ID  */
#line 1220 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

           set_id_type((yyvsp[0].pid_info),TRANSITION_FN);
						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2826 "y.tab.c"
    break;

  case 61: /* transition: TRANSITION_KEY TRANSITION_FN  */
#line 1231 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2840 "y.tab.c"
    break;

  case 62: /* transition: GUARD_KEY ID  */
#line 1241 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

           set_id_type((yyvsp[0].pid_info),TRANSITION_FN);
						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2855 "y.tab.c"
    break;

  case 63: /* transition: GUARD_KEY TRANSITION_FN  */
#line 1252 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2869 "y.tab.c"
    break;

  case 64: /* action_matrix: ID matrix  */
#line 1264 "../src/parser.y"
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
#line 2898 "y.tab.c"
    break;

  case 65: /* action_matrix: ACTION matrix  */
#line 1289 "../src/parser.y"
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
#line 2924 "y.tab.c"
    break;

  case 66: /* matrix: '[' event_vector ',' state_vector ']'  */
#line 1313 "../src/parser.y"
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
#line 2945 "y.tab.c"
    break;

  case 67: /* state_vector: '(' ALL ')'  */
#line 1333 "../src/parser.y"
                      {
 					if (((yyval.plist) = init_list()) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an all state vector\n");
 					#endif
		      }
#line 2958 "y.tab.c"
    break;

  case 68: /* state_vector: '(' state_comma_list STATE ')'  */
#line 1342 "../src/parser.y"
                                        {

						(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state vector\n");
 					parser_debug_print_id_list_names((yyval.plist),pmachineInfo,yyout,"");
						#endif

					}
#line 2976 "y.tab.c"
    break;

  case 69: /* state_vector: STATE  */
#line 1356 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state scalar\n");
						#endif

						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[0].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 2994 "y.tab.c"
    break;

  case 70: /* state_comma_list: STATE ','  */
#line 1372 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the beginning of a state comma list\n");
						#endif

						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3012 "y.tab.c"
    break;

  case 71: /* state_comma_list: state_comma_list STATE ','  */
#line 1386 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of a state comma list\n");
						#endif

						(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3029 "y.tab.c"
    break;

  case 72: /* event_vector: '(' ALL ')'  */
#line 1402 "../src/parser.y"
                      {
 					if (((yyval.plist) = init_list()) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an all event vector\n");
 					#endif
		      }
#line 3042 "y.tab.c"
    break;

  case 73: /* event_vector: '(' event_comma_list EVENT ')'  */
#line 1411 "../src/parser.y"
                                        {

						(yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an event vector\n");
 					parser_debug_print_id_list_names((yyval.plist),pmachineInfo,yyout,"");
						#endif

					}
#line 3060 "y.tab.c"
    break;

  case 74: /* event_vector: EVENT  */
#line 1425 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a scalar event : %s\n",(yyvsp[0].pid_info)->name);
						#endif

						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[0].pid_info)) == NULL)
							yyerror("out of memory");

					}
#line 3078 "y.tab.c"
    break;

  case 75: /* event_comma_list: EVENT ','  */
#line 1442 "../src/parser.y"
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
#line 3100 "y.tab.c"
    break;

  case 76: /* event_comma_list: event_comma_list EVENT ','  */
#line 1460 "../src/parser.y"
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
#line 3120 "y.tab.c"
    break;

  case 77: /* state_and_event_decls: state_decl  */
#line 1479 "../src/parser.y"
                {
 	   if (NULL == ((yyval.pstate_and_event_decls) = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 (yyval.pstate_and_event_decls)->state_decls = (yyvsp[0].plist);
 		 (yyval.pstate_and_event_decls)->event_decls = init_list();
      psedecls = (yyval.pstate_and_event_decls);
		}
#line 3133 "y.tab.c"
    break;

  case 78: /* state_and_event_decls: event_decl  */
#line 1488 "../src/parser.y"
                {
 	   if (NULL == ((yyval.pstate_and_event_decls) = ((pSTATE_AND_EVENT_DECLS)calloc(1,sizeof(STATE_AND_EVENT_DECLS)))))
 		    yyerror("Out of memory");

 		 (yyval.pstate_and_event_decls)->event_decls = (yyvsp[0].plist);
 		 (yyval.pstate_and_event_decls)->state_decls = init_list();
      psedecls = (yyval.pstate_and_event_decls);
		}
#line 3146 "y.tab.c"
    break;

  case 79: /* state_and_event_decls: state_and_event_decls state_decl  */
#line 1497 "../src/parser.y"
          {
 		(yyval.pstate_and_event_decls) = (yyvsp[-1].pstate_and_event_decls);

			(yyval.pstate_and_event_decls)->state_decls = move_list((yyval.pstate_and_event_decls)->state_decls,(yyvsp[0].plist));
      psedecls = (yyval.pstate_and_event_decls);
	  }
#line 3157 "y.tab.c"
    break;

  case 80: /* state_and_event_decls: state_and_event_decls event_decl  */
#line 1504 "../src/parser.y"
          {
 		(yyval.pstate_and_event_decls) = (yyvsp[-1].pstate_and_event_decls);

			(yyval.pstate_and_event_decls)->event_decls = move_list((yyval.pstate_and_event_decls)->event_decls,(yyvsp[0].plist));
      psedecls = (yyval.pstate_and_event_decls);

	  }
#line 3169 "y.tab.c"
    break;

  case 81: /* state_decl: state_decl_list ';'  */
#line 1514 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the states */
						fprintf(yyout,"The %d states in this list :\n",(yyvsp[-1].plist)->count);

 					parser_debug_print_state_list((yyvsp[-1].plist),yyout);
						#endif

						(yyval.plist) = (yyvsp[-1].plist);

					}
#line 3186 "y.tab.c"
    break;

  case 82: /* state: ID  */
#line 1529 "../src/parser.y"
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
#line 3217 "y.tab.c"
    break;

  case 83: /* state: state INHIBITS SUBMACHINES  */
#line 1556 "../src/parser.y"
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
#line 3235 "y.tab.c"
    break;

  case 84: /* state: state ON ENTRY  */
#line 1570 "../src/parser.y"
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
#line 3253 "y.tab.c"
    break;

  case 85: /* state: state ON ENTRY ID  */
#line 1584 "../src/parser.y"
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
#line 3275 "y.tab.c"
    break;

  case 86: /* state: state ON EXIT  */
#line 1602 "../src/parser.y"
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
#line 3293 "y.tab.c"
    break;

  case 87: /* state: state ON EXIT ID  */
#line 1616 "../src/parser.y"
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
#line 3315 "y.tab.c"
    break;

  case 88: /* state_decl_list: STATE_KEY state  */
#line 1636 "../src/parser.y"
                                        {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the start of a state declaration list\n");
						#endif

 					if (NULL == ((yyval.plist) = init_list()))
 						yyerror("Out of memory");


 					if (NULL == (add_to_list((yyval.plist),(yyvsp[0].pid_info))))
 						yyerror("Out of memory");

					}
#line 3334 "y.tab.c"
    break;

  case 89: /* state_decl_list: state_decl_list ',' state  */
#line 1651 "../src/parser.y"
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
#line 3354 "y.tab.c"
    break;

  case 90: /* event_decl: event_decl_list ';'  */
#line 1669 "../src/parser.y"
                                        {

						(yyval.plist) = (yyvsp[-1].plist);

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the events */
						fprintf(yyout,"The %d events in this list:\n",(yyval.plist)->count);
 					parser_debug_print_event_list((yyval.plist),yyout);
						#endif

					}
#line 3370 "y.tab.c"
    break;

  case 91: /* parent_namespace: PARENT NAMESPACE  */
#line 1683 "../src/parser.y"
  {
    if (!pmachineInfo->parent)
        yyerror("parent namespace invoked in top-level machine");

    id_list = pmachineInfo->parent->id_list;

    /* this picks up any doc comment */
 	 (yyval.charData) = (yyvsp[-1].charData);
  }
#line 3384 "y.tab.c"
    break;

  case 92: /* user_event_data: %empty  */
#line 1694 "../src/parser.y"
                 { (yyval.puser_event_data) = NULL; }
#line 3390 "y.tab.c"
    break;

  case 93: /* user_event_data: DATA_KEY TRANSLATOR_KEY ID  */
#line 1696 "../src/parser.y"
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
#line 3410 "y.tab.c"
    break;

  case 94: /* user_event_data: DATA_KEY TRANSLATOR_KEY ID data_block  */
#line 1712 "../src/parser.y"
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
#line 3428 "y.tab.c"
    break;

  case 95: /* user_event_data: DATA_KEY data_block  */
#line 1726 "../src/parser.y"
   {
  		if (NULL == ((yyval.puser_event_data) = ((pUSER_EVENT_DATA) calloc(1, sizeof(USER_EVENT_DATA)))))
  		   yyerror("out of memory");
 
 		 (yyval.puser_event_data)->data_fields = (yyvsp[0].plist);
      
      #ifdef PARSER_DEBUG
      fprintf(yyout,"found data fields\n");
 		 parser_debug_print_data_block((yyvsp[0].plist),yyout);
      #endif
   }
#line 3444 "y.tab.c"
    break;

  case 96: /* event_decl_list: EVENT_KEY ID external_designation user_event_data  */
#line 1740 "../src/parser.y"
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
#line 3469 "y.tab.c"
    break;

  case 97: /* event_decl_list: EVENT_KEY parent_namespace EVENT user_event_data  */
#line 1761 "../src/parser.y"
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
#line 3514 "y.tab.c"
    break;

  case 98: /* event_decl_list: event_decl_list ',' ID external_designation user_event_data  */
#line 1802 "../src/parser.y"
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
#line 3542 "y.tab.c"
    break;

  case 99: /* event_decl_list: event_decl_list ',' parent_namespace EVENT user_event_data  */
#line 1826 "../src/parser.y"
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
#line 3586 "y.tab.c"
    break;

  case 100: /* external_designation: %empty  */
#line 1867 "../src/parser.y"
                       {
           (yyval.pid_info) = NULL;
     }
#line 3594 "y.tab.c"
    break;

  case 101: /* external_designation: '=' ID  */
#line 1871 "../src/parser.y"
 {
           #ifdef PARSER_DEBUG
           fprintf(yyout,"External designation = %s\n",(yyvsp[0].pid_info)->name);
           #endif
            (yyval.pid_info) = (yyvsp[0].pid_info);
 }
#line 3605 "y.tab.c"
    break;

  case 102: /* native: %empty  */
#line 1881 "../src/parser.y"
  {
	(yyval.pnative_info) = NULL;
  }
#line 3613 "y.tab.c"
    break;

  case 103: /* native: native native_prologue  */
#line 1885 "../src/parser.y"
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
#line 3637 "y.tab.c"
    break;

  case 104: /* native: native native_epilogue  */
#line 1905 "../src/parser.y"
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
#line 3660 "y.tab.c"
    break;

  case 105: /* native_prologue: NATIVE_KEY NATIVE_BLOCK  */
#line 1927 "../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3672 "y.tab.c"
    break;

  case 106: /* native_prologue: NATIVE_KEY PROLOGUE_KEY NATIVE_BLOCK  */
#line 1935 "../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native prologue\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3684 "y.tab.c"
    break;

  case 107: /* native_epilogue: NATIVE_KEY EPILOGUE_KEY NATIVE_BLOCK  */
#line 1947 "../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native epilogue\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3696 "y.tab.c"
    break;

  case 108: /* native_impl: native_impl_prologue  */
#line 1959 "../src/parser.y"
        {
		if (((yyval.pnative_info) = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");

		(yyval.pnative_info)->prologue = (yyvsp[0].charData);
	}
#line 3707 "y.tab.c"
    break;

  case 109: /* native_impl: native_impl_epilogue  */
#line 1966 "../src/parser.y"
        {
		if (((yyval.pnative_info) = (pNATIVE_INFO) calloc(1, sizeof(NATIVE_INFO))) == NULL)
			yyerror("out of memory");

		(yyval.pnative_info)->epilogue = (yyvsp[0].charData);
	}
#line 3718 "y.tab.c"
    break;

  case 110: /* native_impl: native_impl native_impl_prologue  */
#line 1973 "../src/parser.y"
        {
		if ((yyvsp[-1].pnative_info)->prologue != NULL)
		{
			yyerror("only one native implementation prologue is allowed");
		}

		(yyval.pnative_info) = (yyvsp[-1].pnative_info);
		(yyval.pnative_info)->prologue = (yyvsp[0].charData);
	}
#line 3732 "y.tab.c"
    break;

  case 111: /* native_impl: native_impl native_impl_epilogue  */
#line 1983 "../src/parser.y"
        {
		if ((yyvsp[-1].pnative_info)->epilogue != NULL)
		{
			yyerror("only one native implementation epilogue is allowed");
		}

		(yyval.pnative_info) = (yyvsp[-1].pnative_info);
		(yyval.pnative_info)->epilogue = (yyvsp[0].charData);
	}
#line 3746 "y.tab.c"
    break;

  case 112: /* native_impl_prologue: NATIVE_KEY IMPLEMENTATION_KEY NATIVE_BLOCK  */
#line 1996 "../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3758 "y.tab.c"
    break;

  case 113: /* native_impl_prologue: NATIVE_KEY IMPLEMENTATION_KEY PROLOGUE_KEY NATIVE_BLOCK  */
#line 2004 "../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation prologue\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3770 "y.tab.c"
    break;

  case 114: /* native_impl_epilogue: NATIVE_KEY IMPLEMENTATION_KEY EPILOGUE_KEY NATIVE_BLOCK  */
#line 2014 "../src/parser.y"
                                        {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native implementation\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 3782 "y.tab.c"
    break;

  case 115: /* machine_data: DATA_KEY data_block  */
#line 2024 "../src/parser.y"
                                  { (yyval.plist) = (yyvsp[0].plist); pmachineInfo->data = (yyval.plist); }
#line 3788 "y.tab.c"
    break;

  case 116: /* data_block: '{' data_fields '}'  */
#line 2027 "../src/parser.y"
        {
			#ifdef PARSER_DEBUG
			fprintf(yyout,"Data block done\n");
			#endif

			(yyval.plist) = (yyvsp[-1].plist);

	}
#line 3801 "y.tab.c"
    break;

  case 117: /* data_fields: data_field  */
#line 2038 "../src/parser.y"
   {
      #ifdef PARSER_DEBUG
    	fprintf(yyout,"Starting data_fields list\n");
    	#endif
   
      if (NULL == ((yyval.plist) = init_list()))
    		yyerror("out of memory");
   
    	add_to_list((yyval.plist), (yyvsp[0].pdata_field));
   
   }
#line 3817 "y.tab.c"
    break;

  case 118: /* data_fields: data_fields data_field  */
#line 2050 "../src/parser.y"
   {
      #ifdef PARSER_DEBUG
    	fprintf(yyout,"Continuing data_fields list\n");
    	#endif
   
 	  (yyval.plist) = (yyvsp[-1].plist);
    	add_to_list((yyval.plist), (yyvsp[0].pdata_field));
   
   }
#line 3831 "y.tab.c"
    break;

  case 119: /* data_type: ID  */
#line 2062 "../src/parser.y"
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
#line 3850 "y.tab.c"
    break;

  case 120: /* data_type: TYPE_NAME  */
#line 2077 "../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found simple data type name\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

 		 (yyval.pdata_type_struct)->dtt      = dtt_simple;
    	 (yyval.pdata_type_struct)->dtu.name = (yyvsp[0].pid_info);

   }
#line 3867 "y.tab.c"
    break;

  case 121: /* data_type: STRUCT_KEY '{' data_fields '}'  */
#line 2090 "../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found struct data type\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

      (yyval.pdata_type_struct)->dtt         = dtt_struct;
 		 (yyval.pdata_type_struct)->dtu.members = (yyvsp[-1].plist);

   }
#line 3884 "y.tab.c"
    break;

  case 122: /* data_type: UNION_KEY '{' data_fields '}'  */
#line 2103 "../src/parser.y"
   {
		   #ifdef PARSER_DEBUG
		   fprintf(yyout, "found union data type\n");
 		 #endif

 		 if (((yyval.pdata_type_struct) = (pDATA_TYPE_STRUCT) calloc(1, sizeof(DATA_TYPE_STRUCT))) == NULL)
 		    yyerror("out of memory");

      (yyval.pdata_type_struct)->dtt         = dtt_union;
 		 (yyval.pdata_type_struct)->dtu.members = (yyvsp[-1].plist);

   }
#line 3901 "y.tab.c"
    break;

  case 123: /* data_type: data_type '*'  */
#line 2116 "../src/parser.y"
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
#line 3920 "y.tab.c"
    break;

  case 124: /* data_field: data_type ID data_field_dimension ';'  */
#line 2134 "../src/parser.y"
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
#line 3946 "y.tab.c"
    break;

  case 125: /* data_field: data_type ID ';'  */
#line 2156 "../src/parser.y"
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
#line 3969 "y.tab.c"
    break;

  case 126: /* data_field_dimension: '[' ']'  */
#line 2177 "../src/parser.y"
   {
       (yyval.charData) = NULL;
   }
#line 3977 "y.tab.c"
    break;

  case 127: /* data_field_dimension: '[' NUMERIC_STRING ']'  */
#line 2181 "../src/parser.y"
   {
       (yyval.charData) = (yyvsp[-1].charData);
   }
#line 3985 "y.tab.c"
    break;

  case 128: /* data_field_dimension: '[' ID ']'  */
#line 2185 "../src/parser.y"
   {
       (yyval.charData) = (yyvsp[-1].pid_info)->name;
   }
#line 3993 "y.tab.c"
    break;

  case 130: /* namespace: MACHINE NAMESPACE  */
#line 2193 "../src/parser.y"
  {
    if (pmachineInfo->parent)
        yyerror("sub-machine namespace invoked in sub-machine");

    id_list = (yyvsp[-1].pid_info)->powningMachine->id_list;
  }
#line 4004 "y.tab.c"
    break;

  case 131: /* namespace_event_ref: namespace EVENT  */
#line 2202 "../src/parser.y"
  {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found a namespace event reference\n");
    #endif

    (yyval.pid_info) = (yyvsp[0].pid_info);
    id_list = pmachineInfo->id_list;
  }
#line 4017 "y.tab.c"
    break;

  case 132: /* returns_comma_list: namespace_event_ref ','  */
#line 2213 "../src/parser.y"
    {
						/* start a list */
						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4031 "y.tab.c"
    break;

  case 133: /* returns_comma_list: EVENT ','  */
#line 2223 "../src/parser.y"
    {
						/* start a list */
						if (((yyval.plist) = init_list()) == NULL) 
							yyerror("out of memory");

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4045 "y.tab.c"
    break;

  case 134: /* returns_comma_list: returns_comma_list namespace_event_ref ','  */
#line 2233 "../src/parser.y"
    {
           (yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4057 "y.tab.c"
    break;

  case 135: /* returns_comma_list: returns_comma_list EVENT ','  */
#line 2241 "../src/parser.y"
    {
           (yyval.plist) = (yyvsp[-2].plist);

 					if (add_to_list((yyval.plist),(yyvsp[-1].pid_info)) == NULL)
							yyerror("out of memory");

    }
#line 4069 "y.tab.c"
    break;

  case 136: /* action_return_decl: ACTION RETURNS returns_comma_list EVENT ';'  */
#line 2252 "../src/parser.y"
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
#line 4092 "y.tab.c"
    break;

  case 137: /* action_return_decl: ACTION RETURNS returns_comma_list namespace_event_ref ';'  */
#line 2271 "../src/parser.y"
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
#line 4115 "y.tab.c"
    break;

  case 138: /* action_return_decl: ACTION RETURNS EVENT ';'  */
#line 2290 "../src/parser.y"
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
#line 4135 "y.tab.c"
    break;

  case 139: /* action_return_decl: ACTION RETURNS namespace_event_ref ';'  */
#line 2306 "../src/parser.y"
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
#line 4155 "y.tab.c"
    break;

  case 140: /* action_return_decl: ACTION RETURNS state_comma_list STATE ';'  */
#line 2322 "../src/parser.y"
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
#line 4178 "y.tab.c"
    break;

  case 141: /* action_return_decl: ACTION RETURNS STATE ';'  */
#line 2341 "../src/parser.y"
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
#line 4200 "y.tab.c"
    break;

  case 142: /* transition_fn_return_decl: TRANSITION_FN RETURNS state_comma_list STATE ';'  */
#line 2362 "../src/parser.y"
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
#line 4223 "y.tab.c"
    break;

  case 143: /* transition_fn_return_decl: TRANSITION_FN RETURNS STATE ';'  */
#line 2381 "../src/parser.y"
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
#line 4242 "y.tab.c"
    break;


#line 4246 "y.tab.c"

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

#line 2397 "../src/parser.y"


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
							fpfsmogf = generatePlantUMLMachineWriter;
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
			/* use the base input file name */
			*cp1 = 0;
			cwk_path_get_basename(inputFileName, (const char**)&outFileBase, NULL);
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

	}

	return (good == 1 ? 0 : 1);

}

void yyerror(char *s)
{
	const char *basename;
	const char *ext;

  fprintf(stderr,"%s%s%s: %s\n"
					, (cwk_path_get_basename(me, &basename, NULL), basename)
					, cwk_path_has_extension(me) ? "." : ""
					, cwk_path_has_extension(me)
						 ? (cwk_path_get_extension(me, &ext, NULL), ext)
						 : ""
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


