/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "../src/parser.y" /* yacc.c:339  */

	/* parser.y

		the FSM Description Language Parser.

	*/

#include <stdio.h>
#include <string.h>


#if defined (LINUX) || defined (CYGWIN)
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#endif

#include "revision.h"
#include "fsm_priv.h"

#include "fsm_c.h"
#include "fsm_html.h"

int lineno=1;

//did we parse correctly?
int good=1;

extern char	*yytext;
extern FILE	*yyin, *yyout;

MACHINE_INFO  machineInfo;

pFSMOutputGenerator	pfsmog;

void yyerror(char *);


#line 106 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
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
#line 41 "../src/parser.y" /* yacc.c:355  */

	pID_INFO				pid_info;
	pACTION_SE_INFO	se_info;
	pACTION_INFO		action_info;
	pMATRIX_INFO		matrix_info;
	char *					charData;

#line 206 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 223 "y.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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


#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   111

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  123

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      33,    34,     2,     2,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    29,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,    28,     2,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    83,    83,   105,   128,   323,   330,   331,   332,   333,
     337,   344,   352,   359,   368,   369,   375,   378,   379,   382,
     383,   384,   385,   388,   396,   441,   488,   528,   551,   577,
     591,   601,   615,   632,   651,   663,   689,   715,   734,   760,
     782,   802,   824,   850,   872,   892,   914,   915,   918,   919,
     922,   945,   964,   981,  1004,  1019,  1037,  1040,  1051,  1054,
    1065,  1068,  1080,  1102,  1127,  1149
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MACHINE_KEY", "TRANSITION_KEY",
  "STATE_KEY", "EVENT_KEY", "ACTION_KEY", "ON", "REENTRANT", "ACTIONS",
  "RETURN", "STATES", "EVENTS", "RETURNS", "EXTERNAL", "EQUALS", "VOID",
  "NATIVE_KEY", "DATA_KEY", "DOC_COMMENT", "MACHINE", "STATE", "EVENT",
  "ACTION", "TRANSITION_FN", "ID", "'{'", "'}'", "';'", "','", "'['",
  "']'", "'('", "')'", "$accept", "fsmlang", "machine",
  "machine_qualifier", "machine_transition_decl", "action_return_spec",
  "machine_modifier", "statement_decl_list", "actions_and_transitions",
  "actions_or_transitions", "transition_matrix_list", "transition_matrix",
  "action_decl", "action_decl_list", "action", "transition",
  "action_matrix", "matrix", "state_vector", "state_comma_list",
  "event_vector", "event_comma_list", "state_and_event_decls",
  "state_or_event_decl", "state_decl", "state_decl_list", "event_decl",
  "event_decl_list", "native", "data", "doccmnt", "action_return_decl",
  "transition_fn_return_decl", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   123,   125,    59,
      44,    91,    93,    40,    41
};
# endif

#define YYPACT_NINF -44

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-44)))

#define YYTABLE_NINF -17

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      19,   -44,     5,    22,   -44,    22,   -44,   -44,   -44,   -44,
      23,    35,   -44,    33,    48,    51,    36,    54,    57,    40,
      -1,    49,   -44,   -44,    38,    41,    42,    44,   -44,    22,
     -44,   -44,   -44,   -44,    46,    22,   -44,   -44,    15,   -44,
      17,    43,   -44,   -10,   -44,   -44,   -44,    21,   -44,    -4,
     -44,   -44,   -44,    50,   -44,    52,    53,    55,   -44,     0,
     -44,   -11,    56,   -11,    58,    61,   -44,   -44,   -44,   -44,
     -44,    56,    56,   -44,    65,   -14,     8,   -44,    59,    62,
     -44,   -44,    13,   -44,   -44,    60,    47,    63,    64,    66,
      24,    67,    26,    69,   -44,   -44,   -44,    68,    71,   -16,
     -44,   -44,   -44,   -44,   -44,    28,   -44,   -44,    30,    -3,
     -44,    74,    70,   -44,   -44,   -44,   -44,   -44,    73,    75,
     -44,     6,   -44
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      56,    57,    56,    60,     1,    60,    61,     2,    14,     3,
       0,     0,    15,     5,     0,     0,     0,     7,     6,     0,
       0,    58,     8,     9,     0,     0,     0,     0,    59,    60,
      10,    12,    11,    13,     0,    60,    46,    48,     0,    49,
       0,     0,     4,    60,    17,    20,    19,     0,    47,     0,
      21,    22,    50,     0,    53,     0,     0,     0,    18,     0,
      27,     0,     0,     0,     0,     0,    23,    52,    55,    51,
      54,     0,     0,    29,    30,     0,     0,    28,     0,     0,
      36,    35,     0,    31,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    32,    34,    33,     0,     0,     0,
      24,    26,    25,    63,    44,     0,    65,    40,     0,     0,
      39,     0,     0,    62,    45,    64,    41,    42,     0,     0,
      37,     0,    38
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -44,   -44,    80,   -44,    81,    72,   -44,   -44,   -44,    37,
     -44,   -44,   -44,   -44,    25,   -44,   -44,   -43,   -44,   -25,
     -44,    16,   -44,    76,   -44,   -44,   -44,   -44,    98,   -44,
     -21,   -44,   -44
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     7,    16,    17,    18,    10,    34,    43,    44,
      45,    66,    46,    47,    73,    83,    74,    76,   112,    93,
      86,    91,    35,    36,    37,    38,    39,    40,     3,    29,
       8,    50,    51
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      62,    56,    57,    63,    62,     4,   110,    63,    41,    84,
       6,    25,    26,    71,    49,    72,    27,   111,   -16,    85,
      64,    65,    59,     1,    64,    65,    11,   114,    80,    81,
      87,   117,    12,    88,    89,    94,   116,     1,    95,    96,
     122,    14,     6,    15,    52,    53,    54,    55,    56,    57,
      60,    61,    19,   103,   104,   106,   107,   113,   114,   115,
     116,    13,    20,    21,    15,    14,    24,    30,    28,    82,
      31,    32,    78,    33,    42,    79,    67,    99,    68,    69,
      58,    70,    90,    97,    92,     9,   119,    75,    77,    22,
     105,   108,   100,   101,   109,   102,   118,   121,   104,    23,
       5,    98,   120,   107,     0,     0,     0,     0,     0,     0,
       0,    48
};

static const yytype_int8 yycheck[] =
{
       4,     5,     6,     7,     4,     0,    22,     7,    29,    23,
      20,    12,    13,    24,    35,    26,    17,    33,    28,    33,
      24,    25,    43,    18,    24,    25,     3,    30,    71,    72,
      22,    34,     9,    25,    26,    22,    30,    18,    25,    26,
      34,     8,    20,    10,    29,    30,    29,    30,     5,     6,
      29,    30,     4,    29,    30,    29,    30,    29,    30,    29,
      30,    26,    11,    27,    10,     8,    26,    29,    19,     4,
      29,    29,    14,    29,    28,    14,    26,    30,    26,    26,
      43,    26,    23,    23,    22,     5,   111,    31,    63,    17,
      23,    22,    29,    29,    23,    29,    22,    22,    30,    18,
       2,    85,    32,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    18,    36,    63,     0,    63,    20,    37,    65,    37,
      41,     3,     9,    26,     8,    10,    38,    39,    40,     4,
      11,    27,    40,    39,    26,    12,    13,    17,    19,    64,
      29,    29,    29,    29,    42,    57,    58,    59,    60,    61,
      62,    65,    28,    43,    44,    45,    47,    48,    58,    65,
      66,    67,    29,    30,    29,    30,     5,     6,    44,    65,
      29,    30,     4,     7,    24,    25,    46,    26,    26,    26,
      26,    24,    26,    49,    51,    31,    52,    49,    14,    14,
      52,    52,     4,    50,    23,    33,    55,    22,    25,    26,
      23,    56,    22,    54,    22,    25,    26,    23,    56,    30,
      29,    29,    29,    29,    30,    23,    29,    30,    22,    23,
      22,    33,    53,    29,    30,    29,    30,    34,    22,    54,
      32,    22,    34
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    36,    37,    38,    38,    38,    38,    38,
      39,    40,    40,    40,    41,    41,    42,    43,    43,    44,
      44,    44,    44,    45,    46,    46,    46,    47,    48,    48,
      49,    49,    50,    50,    50,    51,    51,    52,    53,    53,
      54,    54,    55,    55,    56,    56,    57,    57,    58,    58,
      59,    60,    60,    61,    62,    62,    63,    63,    64,    64,
      65,    65,    66,    66,    67,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     3,     9,     0,     1,     1,     2,     2,
       4,     4,     4,     4,     0,     2,     2,     1,     2,     1,
       1,     1,     1,     2,     4,     4,     4,     2,     3,     3,
       1,     2,     2,     2,     2,     2,     2,     5,     4,     1,
       2,     3,     4,     1,     2,     3,     1,     2,     1,     1,
       2,     3,     3,     2,     3,     3,     0,     1,     0,     1,
       0,     1,     6,     5,     6,     5
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
| yyreduce -- Do a reduction.  |
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
        case 2:
#line 84 "../src/parser.y" /* yacc.c:1646  */
    { 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a single machine\n"); 

						#else

						/* grab the native language stuff */
						machineInfo.native = (yyvsp[-1].charData);

						/* write the machine */
						(*pfsmog->writeMachine)(&machineInfo);

						#endif

						/* get ready for the next machine */
						freeMachineInfo(&machineInfo);

						free_ids();

					}
#line 1409 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 106 "../src/parser.y" /* yacc.c:1646  */
    { 

						#ifdef PARSER_DEBUG
						fprintf(yyout,"making a list of machines\n"); 
						#else

						/* grab the native language stuff */
						machineInfo.native = (yyvsp[-1].charData);

						/* write the machine */
						(*pfsmog->writeMachine)(&machineInfo);

						#endif

						/* get ready for the next machine */
						freeMachineInfo(&machineInfo);

						free_ids();

					}
#line 1434 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 129 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						pID_INFO	pid_info;
						pACTION_SE_INFO	pase_info;
						pACTION_INFO	pai;

						fprintf(yyout,"found a machine\n");
						fprintf(yyout
								,"\twith %d events and %d states\n"
								,machineInfo.event_count
								,machineInfo.state_count
								);

						if (machineInfo.modFlags & mfReentrant) {

							fprintf(yyout,"The machine is reentrant\n");

						}

						if (machineInfo.modFlags & mfActionsReturnStates) {

							fprintf(yyout,"Actions return states\n");

						}
						else if (machineInfo.modFlags & mfActionsReturnVoid) {

							fprintf(yyout,"Actions return void\n");

						}
						else {

							fprintf(yyout,"Actions return events\n");

						}

           if (machineInfo.machineTransition)
           {
               fprintf(yyout
                       ,"on transition: %s\n"
                       ,machineInfo.machineTransition->name
                       );
           }

						fprintf(yyout,"The states :\n");
						for (pid_info = machineInfo.state_list;
									pid_info;
									pid_info = pid_info->nextState) {

								fprintf(yyout
										,"\t%d:\t%s\n%s\n"
										,pid_info->seOrder
										,pid_info->name
										,pid_info->docCmnt ? pid_info->docCmnt : ""
										);

						}

						fprintf(yyout,"The events :\n");
						for (pid_info = machineInfo.event_list;
									pid_info;
									pid_info = pid_info->nextEvent) {

								fprintf(yyout
										,"\t%d:\t%s\n%s\n"
										,pid_info->seOrder
										,pid_info->name
										,pid_info->docCmnt ? pid_info->docCmnt : ""
										);

						}

						fprintf(yyout,"The actions :\n");
						for (pid_info = machineInfo.action_list;
									pid_info;
									pid_info = pid_info->nextAction) {

								fprintf(yyout
										,"\t%s\n"
										,strlen(pid_info->name) ? pid_info->name : "noAction"
										);

								for (pai = pid_info->actionInfo;
											pai;
											pai = pai->nextAction) {

									fprintf(yyout,"\t\twhich occurs in these events\n");
									for (pase_info = pai->matrix->event_list;
											pase_info;
											pase_info = pase_info->next) {
	
										fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									}
	
									fprintf(yyout,"\t\tand states\n");
									for (pase_info = pai->matrix->state_list;
											pase_info;
											pase_info = pase_info->next) {
	
										fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									}
	
									if (pai->transition)
                 {
                     switch (pai->transition->type)
                     {
                         case STATE:
										         fprintf(yyout,"\t\tand transitions to state %s\n"
                            ,pai->transition->name);
                            break;
                         case TRANSITION_FN:
										         fprintf(yyout,"\t\tand transitions using function %s\n"
                            ,pai->transition->name);
                            break;
                     }
                 }
	
								}

								if (pid_info->action_returns_decl)
               {
                 fprintf(yyout,"\t\tand returns\n");

									for (pase_info = pid_info->action_returns_decl;
											 pase_info;
											 pase_info = pase_info->next) {
	
										   fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									}
               }
	
								if (pid_info->docCmnt)
									fprintf(yyout,"Doc Comments:\n%s\n"
												,pid_info->docCmnt);

						}

           fprintf(yyout,"\nThe %d transitions :\n"
                   , machineInfo.transition_count
                   );
           for (pid_info = machineInfo.transition_list;
                pid_info;
                pid_info = pid_info->nextTransition)
           {
             fprintf(yyout,"\t%s\n"
                     , pid_info->name
                    );
           }

           if (machineInfo.transition_fn_count)
           {
		           fprintf(yyout,"\nThe %d transition functions :\n"
                      , machineInfo.transition_fn_count
                      );
		           for (pid_info = machineInfo.transition_fn_list;
					          pid_info;
					          pid_info = pid_info->nextTransitionFn)
                  {
                    fprintf(yyout,"\t%s\n"
                            , pid_info->name
                            );

								     if (pid_info->transition_fn_returns_decl)
                    {
                        fprintf(yyout,"\t\twhich returns\n");

									       for (pase_info = pid_info->transition_fn_returns_decl;
											       pase_info;
											       pase_info = pase_info->next) {
	
										         fprintf(yyout,"\t\t\t%s\n",pase_info->se->name);
	
									       }
                    }
	
                  }

           }

						fprintf(yyout,"\n");

						#endif

						(yyvsp[-5].pid_info)->docCmnt = (yyvsp[-8].charData);
						machineInfo.name = (yyvsp[-5].pid_info);
						machineInfo.data = (yyvsp[-2].charData);

					}
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 323 "../src/parser.y" /* yacc.c:1646  */
    {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(EVENT,"noEvent",&pid_info);
        }
#line 1642 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 338 "../src/parser.y" /* yacc.c:1646  */
    {
        machineInfo.machineTransition = (yyvsp[-1].pid_info);
    }
#line 1650 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 345 "../src/parser.y" /* yacc.c:1646  */
    {
						pID_INFO pid_info;
           /* note that this is not added to the machine event list; it is here only to be
           found as an event id for return decls.
           */
						add_id(EVENT,"noEvent",&pid_info);
        }
#line 1662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 353 "../src/parser.y" /* yacc.c:1646  */
    {
             if (machineInfo.modFlags & mfActionsReturnVoid)
                yyerror("actions return void directive already seen");

						machineInfo.modFlags |= mfActionsReturnStates;
					}
#line 1673 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 360 "../src/parser.y" /* yacc.c:1646  */
    {
             if (machineInfo.modFlags & mfActionsReturnStates)
                yyerror("actions return states directive already seen");

						machineInfo.modFlags |= mfActionsReturnVoid;
					}
#line 1684 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 370 "../src/parser.y" /* yacc.c:1646  */
    {
						machineInfo.modFlags |= mfReentrant;
					}
#line 1692 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 389 "../src/parser.y" /* yacc.c:1646  */
    {
						if ((yyvsp[-1].charData))
							(yyvsp[0].action_info)->action->docCmnt = (yyvsp[-1].charData);
						(yyval.action_info) = (yyvsp[0].action_info);
					}
#line 1702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 397 "../src/parser.y" /* yacc.c:1646  */
    {

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix\n");
						#endif

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(ACTION,"",&pid_info);

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->action     = pid_info;
						(yyval.action_info)->matrix     = (yyvsp[-2].matrix_info);
						(yyval.action_info)->transition = (yyvsp[-1].pid_info);

						(yyval.action_info)->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = (yyval.action_info);

						addToActionList(&machineInfo,(yyval.action_info)->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,(yyval.action_info));

           if (!(yyvsp[-1].pid_info)->nextTransition && !machineInfo.transition_count)
           {
              (yyvsp[-1].pid_info)->nextTransition          = machineInfo.transition_list;
              machineInfo.transition_list = (yyvsp[-1].pid_info);
              (yyvsp[-1].pid_info)->tOrder                  = machineInfo.transition_count++;
           }

					}
#line 1751 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 442 "../src/parser.y" /* yacc.c:1646  */
    {

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with new transition function\n");
						#endif

           set_id_type((yyvsp[-1].pid_info),TRANSITION_FN);
           (yyvsp[-1].pid_info)->nextTransitionFn           = machineInfo.transition_fn_list;
           machineInfo.transition_fn_list = (yyvsp[-1].pid_info);
           (yyvsp[-1].pid_info)->seOrder                    = machineInfo.transition_fn_count++;

           (yyvsp[-1].pid_info)->nextTransition          = machineInfo.transition_list;
           machineInfo.transition_list = (yyvsp[-1].pid_info);
           (yyvsp[-1].pid_info)->tOrder                  = machineInfo.transition_count++;

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(ACTION,"",&pid_info);

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->action     = pid_info;
						(yyval.action_info)->matrix     = (yyvsp[-2].matrix_info);
						(yyval.action_info)->transition = (yyvsp[-1].pid_info);

						(yyval.action_info)->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = (yyval.action_info);

						addToActionList(&machineInfo,(yyval.action_info)->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,(yyval.action_info));

					}
#line 1802 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 489 "../src/parser.y" /* yacc.c:1646  */
    {

						pID_INFO pid_info;

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition matrix with known transition function\n");
						#endif

						//first, we have to add an id_info struct to the id list
						//we treat it as a "null action"
						add_id(ACTION,"",&pid_info);

						//second, we grab a struct to hold the info
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->action     = pid_info;
						(yyval.action_info)->matrix     = (yyvsp[-2].matrix_info);
						(yyval.action_info)->transition = (yyvsp[-1].pid_info);

						(yyval.action_info)->nextAction = pid_info->actionInfo;
						pid_info->actionInfo = (yyval.action_info);

						addToActionList(&machineInfo,(yyval.action_info)->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,(yyval.action_info));
          }
#line 1843 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 529 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the actions */
						pID_INFO	pid;

						fprintf(yyout,"The actions so far(1):\n");
						for (pid = machineInfo.action_list;
									pid;
									pid = pid->nextAction) {

								fprintf(yyout,"\t%s\n",pid->name);

						}

						#endif

						(yyval.action_info) = (yyvsp[-1].action_info);

					}
#line 1868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 552 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"started an action declaration\n");
						#endif

						addToActionList(&machineInfo,(yyvsp[0].action_info)->action);

						if (!machineInfo.actionArray) {

							if (allocateActionArray(&machineInfo))

								yyerror("out of memory");

						}

						addToActionArray(&machineInfo,(yyvsp[0].action_info));

						if ((yyvsp[-2].charData))
							(yyvsp[0].action_info)->action->docCmnt = (yyvsp[-2].charData);

						(yyval.action_info) = (yyvsp[0].action_info);

					}
#line 1897 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 578 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another action declarations\n");
						#endif

						addToActionList(&machineInfo,(yyvsp[0].action_info)->action);

						(yyval.action_info) = (yyvsp[0].action_info);

					}
#line 1913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 592 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action without transition\n");
						#endif

						(yyval.action_info) = (yyvsp[0].action_info);

					}
#line 1927 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 602 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an action with transition\n");
						#endif

						(yyvsp[-1].action_info)->transition = (yyvsp[0].pid_info);

						(yyval.action_info) = (yyvsp[-1].action_info);

					}
#line 1943 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 616 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition to known state\n");
						#endif

           if (!(yyvsp[0].pid_info)->nextTransition && !machineInfo.transition_count)
           {
              (yyvsp[0].pid_info)->nextTransition          = machineInfo.transition_list;
              machineInfo.transition_list = (yyvsp[0].pid_info);
              (yyvsp[0].pid_info)->tOrder                  = machineInfo.transition_count++;
           }

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 1964 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 633 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with new function\n");
						#endif

           set_id_type((yyvsp[0].pid_info),TRANSITION_FN);
           (yyvsp[0].pid_info)->nextTransitionFn           = machineInfo.transition_fn_list;
           machineInfo.transition_fn_list = (yyvsp[0].pid_info);
           (yyvsp[0].pid_info)->seOrder                    = machineInfo.transition_fn_count++;

           (yyvsp[0].pid_info)->nextTransition          = machineInfo.transition_list;
           machineInfo.transition_list = (yyvsp[0].pid_info);
           (yyvsp[0].pid_info)->tOrder                  = machineInfo.transition_count++;

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 1987 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 652 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a transition with known function\n");
						#endif

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2001 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 664 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						volatile pID_INFO pid_info = (yyvsp[-1].pid_info);

						fprintf(yyout,"found an action matrix\n");
						#endif

           set_id_type((yyvsp[-1].pid_info),ACTION);

						/* 
							grab an ACTION_INFO struct
						*/
						if (((yyval.action_info) = (pACTION_INFO)calloc(1,sizeof(ACTION_INFO))) == NULL)

							yyerror("out of memory");

						(yyval.action_info)->action     = (yyvsp[-1].pid_info);
						(yyval.action_info)->matrix     = (yyvsp[0].matrix_info);
						(yyval.action_info)->transition = NULL;

						(yyval.action_info)->nextAction = (yyvsp[-1].pid_info)->actionInfo;
						(yyvsp[-1].pid_info)->actionInfo = (yyval.action_info);

					}
#line 2031 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 690 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						volatile pID_INFO pid_info = (yyvsp[-1].pid_info);

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

						(yyval.action_info)->nextAction = (yyvsp[-1].pid_info)->actionInfo;
						(yyvsp[-1].pid_info)->actionInfo = (yyval.action_info);

					}
#line 2059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 716 "../src/parser.y" /* yacc.c:1646  */
    {
         #ifdef PARSER_DEBUG
         fprintf(yyout,"found a matrix\n");
         #endif

         /*
            grab a MATRIX_INFO struct
         */
						if (((yyval.matrix_info) = (pMATRIX_INFO)malloc(sizeof(MATRIX_INFO))) == NULL)

							yyerror("out of memory");

            (yyval.matrix_info)->event_list = (yyvsp[-3].se_info);
            (yyval.matrix_info)->state_list = (yyvsp[-1].se_info);

     }
#line 2080 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 735 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						pACTION_SE_INFO	pse_info;
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->next = (yyvsp[-2].se_info);
						(yyval.se_info)->se = (yyvsp[-1].pid_info);

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state vector\n");
						for (pse_info = (yyval.se_info); pse_info; pse_info = pse_info->next)
							fprintf(yyout,"\t%s\n",pse_info->se->name); 
						#endif

					}
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 761 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a state scalar\n");
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->se = (yyvsp[0].pid_info);
						(yyval.se_info)->next = NULL;

					}
#line 2134 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 783 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the beginning of a state comma list\n");
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->se = (yyvsp[-1].pid_info);
						(yyval.se_info)->next = NULL;

					}
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 803 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of a state comma list\n");
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->next = (yyvsp[-2].se_info);
						(yyval.se_info)->se = (yyvsp[-1].pid_info);

					}
#line 2182 "y.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 825 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						pACTION_SE_INFO	pse_info;
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->next = (yyvsp[-2].se_info);
						(yyval.se_info)->se = (yyvsp[-1].pid_info);

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found an event vector\n");
						for (pse_info = (yyval.se_info); pse_info; pse_info = pse_info->next)
							fprintf(yyout,"\t%s\n",pse_info->se->name); 
						#endif

					}
#line 2212 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 851 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found a scalar event : %s\n",(yyvsp[0].pid_info)->name);
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->se = (yyvsp[0].pid_info);
						(yyval.se_info)->next = NULL;

					}
#line 2236 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 873 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the begining of an event comma list: %s\n",(yyvsp[-1].pid_info)->name);
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->se = (yyvsp[-1].pid_info);
						(yyval.se_info)->next = NULL;

					}
#line 2260 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 893 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the continuation of an event comma list. adding %s\n",(yyvsp[-1].pid_info)->name);
						#endif

						/* grab a state/event info record */
						if (((yyval.se_info) = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

							yyerror("out of memory");

						#ifdef MEM_DEBUG
						fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",(yyval.se_info));
						#endif

						(yyval.se_info)->next = (yyvsp[-2].se_info);
						(yyval.se_info)->se = (yyvsp[-1].pid_info);

					}
#line 2284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 923 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the states */
						pID_INFO	pid_info;

						fprintf(yyout,"The %d states so far :\n",machineInfo.state_count);
						for (pid_info = machineInfo.state_list;
									pid_info;
									pid_info = pid_info->nextState) {

								fprintf(yyout,"\t%s",pid_info->name);
								fprintf(yyout,"\n");

						}
						#endif

						(yyval.pid_info) = (yyvsp[-1].pid_info);

					}
#line 2309 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 946 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"found the start of a state declaration list\n");
						#endif

           set_id_type((yyvsp[0].pid_info),STATE);

						(yyvsp[0].pid_info)->nextState = machineInfo.state_list;
						machineInfo.state_list = (yyvsp[0].pid_info);

						(yyvsp[0].pid_info)->seOrder = machineInfo.state_count++;

						(yyvsp[0].pid_info)->docCmnt             = (yyvsp[-2].charData);

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2332 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 965 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the state declaration list\n");
						#endif

           set_id_type((yyvsp[0].pid_info),STATE);

						(yyvsp[0].pid_info)->nextState = machineInfo.state_list;
						machineInfo.state_list = (yyvsp[0].pid_info);
						(yyvsp[0].pid_info)->seOrder = machineInfo.state_count++;
						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2351 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 982 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						/* we should now be able to print a list of the events */
						pID_INFO	pid_info;

						fprintf(yyout,"The %d events so far :\n",machineInfo.event_count);
						for (pid_info = machineInfo.event_list;
									pid_info;
									pid_info = pid_info->nextEvent) {

								fprintf(yyout,"\t%s",pid_info->name);
								fprintf(yyout,"\n");

						}
						#endif

						(yyval.pid_info) = (yyvsp[-1].pid_info);

					}
#line 2376 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1005 "../src/parser.y" /* yacc.c:1646  */
    {

           set_id_type((yyvsp[0].pid_info),EVENT);

						(yyvsp[0].pid_info)->nextEvent = machineInfo.event_list;
						machineInfo.event_list = (yyvsp[0].pid_info);

						(yyvsp[0].pid_info)->seOrder = machineInfo.event_count++;

 					(yyvsp[0].pid_info)->docCmnt             = (yyvsp[-2].charData);

						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2395 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1020 "../src/parser.y" /* yacc.c:1646  */
    {

						#ifdef PARSER_DEBUG
						fprintf(yyout,"added another id to the event declaration list\n");
						#endif

           set_id_type((yyvsp[0].pid_info),EVENT);

						(yyvsp[0].pid_info)->nextEvent = machineInfo.event_list;
						machineInfo.event_list = (yyvsp[0].pid_info);
						(yyvsp[0].pid_info)->seOrder = machineInfo.event_count++;
						(yyval.pid_info) = (yyvsp[0].pid_info);

					}
#line 2414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1037 "../src/parser.y" /* yacc.c:1646  */
    {
						(yyval.charData) = NULL;
			}
#line 2422 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1041 "../src/parser.y" /* yacc.c:1646  */
    {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Native\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 2434 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1051 "../src/parser.y" /* yacc.c:1646  */
    {
						(yyval.charData) = NULL;
			}
#line 2442 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1055 "../src/parser.y" /* yacc.c:1646  */
    {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Data\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 2454 "y.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 1065 "../src/parser.y" /* yacc.c:1646  */
    {
						(yyval.charData) = NULL;
					}
#line 2462 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1069 "../src/parser.y" /* yacc.c:1646  */
    {
						#ifdef PARSER_DEBUG
						fprintf(yyout,"Document Comment\n%s\n",(yyvsp[0].charData));
						#else
						(yyval.charData) = (yyvsp[0].charData);
						#endif
					}
#line 2474 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1081 "../src/parser.y" /* yacc.c:1646  */
    {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

			/* grab a state/event info record */
			if (((yyvsp[-4].pid_info)->action_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: event ACTION_SE_INFO 0x%x\n",(yyvsp[-5].charData)->actionInfo->returns_decl);
			#endif

			(yyvsp[-4].pid_info)->action_returns_decl->se = (yyvsp[-1].pid_info);
			(yyvsp[-4].pid_info)->action_returns_decl->next = (yyvsp[-2].se_info);

     if ((yyvsp[-5].charData) && !(yyvsp[-4].pid_info)->docCmnt)
        (yyvsp[-4].pid_info)->docCmnt = (yyvsp[-5].charData);

  }
#line 2500 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1103 "../src/parser.y" /* yacc.c:1646  */
    {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an action return declaration\n");
    #endif

		 /* grab a state/event info record */
		 if (((yyvsp[-3].pid_info)->action_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyvsp[-4].charData)->action_returns_decl);
			#endif

			(yyvsp[-3].pid_info)->action_returns_decl->se = (yyvsp[-1].pid_info);
			(yyvsp[-3].pid_info)->action_returns_decl->next = NULL;

     if ((yyvsp[-4].charData) && !(yyvsp[-3].pid_info)->docCmnt)
        (yyvsp[-3].pid_info)->docCmnt = (yyvsp[-4].charData);

  }
#line 2526 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1128 "../src/parser.y" /* yacc.c:1646  */
    {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an transition_fn return declaration\n");
    #endif

		 /* grab a state/event info record */
		 if (((yyvsp[-4].pid_info)->transition_fn_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyvsp[-5].charData)->transition_fn_returns_decl);
			#endif

			(yyvsp[-4].pid_info)->transition_fn_returns_decl->se = (yyvsp[-1].pid_info);
			(yyvsp[-4].pid_info)->transition_fn_returns_decl->next = (yyvsp[-2].se_info);

     if ((yyvsp[-5].charData) && !(yyvsp[-4].pid_info)->docCmnt)
        (yyvsp[-4].pid_info)->docCmnt = (yyvsp[-5].charData);

  }
#line 2552 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1150 "../src/parser.y" /* yacc.c:1646  */
    {
    #ifdef PARSER_DEBUG
    fprintf(yyout,"Found an transition_fn return declaration\n");
    #endif

			/* grab a state/event info record */
			if (((yyvsp[-3].pid_info)->transition_fn_returns_decl = (pACTION_SE_INFO) malloc(sizeof(ACTION_SE_INFO))) == NULL) 

				yyerror("out of memory");

			#ifdef MEM_DEBUG
			fprintf(yyout,"Adding: state ACTION_SE_INFO 0x%x\n",(yyvsp[-4].charData)->transition_fn_returns_decl);
			#endif

			(yyvsp[-3].pid_info)->transition_fn_returns_decl->se = (yyvsp[-1].pid_info);
			(yyvsp[-3].pid_info)->transition_fn_returns_decl->next = NULL;

     if ((yyvsp[-4].charData) && !(yyvsp[-3].pid_info)->docCmnt)
        (yyvsp[-3].pid_info)->docCmnt = (yyvsp[-4].charData);

  }
#line 2578 "y.tab.c" /* yacc.c:1646  */
    break;


#line 2582 "y.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
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
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1173 "../src/parser.y" /* yacc.c:1906  */


#if defined(CYGWIN) || defined (LINUX)
#include <unistd.h>
#include <string.h>
#endif

#ifdef VS
#include "vsstd.h"
#include "vsstring.h"
#endif

void usage(void);
char *dotfsm = ".fsm";

int main(int argc, char **argv)
{

	int		c;
	char	*cp,*cp1;
	char  *outFileBase = 0;

	me = argv[0];

	/* special case the single '?' */
	if (argc == 2 && argv[1][0] == '?') {

		usage();
		return (1);

	}

	/* default to writing a c machine */
	pfsmog = pCMachineWriter;

	while ((c = getopt(argc,argv,"vht:o:i:c")) != -1) {

		switch(c) {

			case 'h':
				usage();
				return (1);

			case 't':

				switch (optarg[0]) {

					case 'c':

						pfsmog = pCMachineWriter;
						break;

					case 'h':

						pfsmog = pHTMLMachineWriter;
						break;

         case 's':
           pfsmog = pCSwitchMachineWriter;
						break;

					default:

						usage();
						return (1);

				}
				break;

     case 'i':
        if ('0' == optarg[0])
        {
            generate_instance = false;
        }
        break;

     case 'c':
        compact_action_array = true;
        break;

			case 'o':

				outFileBase = optarg;
				break;

     case 'v':

        fprintf(stdout,"%s\n",rev_string);
        return (0);

			case '?':
			case ':':

				usage();
				return (1);

		}

	}

	if (argv[optind]) {

		cp = strdup(argv[optind]);

		/* find the extension */
		cp1 = rindex(cp,'.');
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

		if ((yyin = openFile(argv[optind],"r")) == NULL) {

			return 1;

		}

		/* get the base file name */
		if (!outFileBase) {
			/* use the base input file name */
			*cp1 = 0;
			outFileBase = cp;
		}

		memset(&machineInfo,0,sizeof(MACHINE_INFO));

		#ifndef PARSER_DEBUG
		if (!(*pfsmog->initOutput)(outFileBase)) {
		#endif

			yyparse();

		#ifndef PARSER_DEBUG
			(*pfsmog->closeOutput)(good);

		}
		#endif

		fclose(yyin);

		return (!good);

	}
	else {

		usage();
		return (!good);

	}

}

void yyerror(char *s)
{

  fprintf(stderr,"%s: %s\n",me,s);
	fprintf(stderr,"\tline %d : %s\n",lineno,yytext);

	good = 0;

}

void usage(void)
{

	fprintf(stdout,"Usage : %s [-tc|s|h] filename, where filename ends with '.fsm'\n",me);
	fprintf(stdout,"\t and where 'c' gets you c code output based on an event/state table,\n");
	fprintf(stdout,"\t 's' gets you c code output with individual state functions using switch constructions,\n");
	fprintf(stdout,"\t and 'h' gets you html output\n");
 fprintf(stdout,"\t%s -i0 inhibits the creation of a machine instance\n",me);
 fprintf(stdout,"\t\tany other argument to 'i' allows the creation of an instance;\n",me);
 fprintf(stdout,"\t\tthis is the default\n",me);
 fprintf(stdout,"\t%s -c will create a more compact event/state table when -tc is used\n",me);
 fprintf(stdout,"\t\twith machines having actions which return states\n",me);
 fprintf(stdout,"\t%s -v prints the version and exits\n",me);
	
}

