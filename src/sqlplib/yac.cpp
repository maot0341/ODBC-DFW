
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 ".\\yac.y"

/****************************************************************************
parser.y
Parser for a simple calculator. The parser is implemented using a C++ class.
This is specified by selecting the -Tcpp option in AYACC (the "C++"
selection from the Target Language combo box in the AYACC Options dialog
box).

The parser implements the usual floating point arithmetic using the
addition, subtraction, multiplication and division operators, and unary
minus. Expressions can be grouped and simple error recovery is supported. In
addition it is now posssible to assign values to variables, and to use such
values in expressions.
****************************************************************************/

#pragma warning(disable : 4786)

#define YYDEBUG 1

#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string>
#include "sqlp.h"
#include "yac.h"

//#define PRSER_NAME yy

namespace sqlp {


const char * join (int head, const char * pLeft, const char * pRight, CTerm * pCond);
const char * table (const char * szCatalog, const char * szSchema, const char * szTable, const char * szAlias);

CTerm* column (const char * szSchema, const char * szTable, const char * szColumn);
CTerm* number (double dValue);
CTerm* number (long int nValue);
CTerm* value (const char * szValue);
CTerm* value (short type, double dValue);
CTerm* time (const char * szValue);
CTerm* time (double dValue);
CTerm* func (int nHead, ...);
CTerm* unary (int nHead, CTerm * pTerm);
CTerm* aggregate (int nHead, CTerm * pTerm);
CTerm * param();
CTerm * termlist(CTerm* t1, CTerm* t2);

CTermList* termlist(CTermList* pList, CTerm * pTerm);
CNameList* namelist(CNameList* pList, const char * pName);

void select (CTerm * pTerm, const char * szName=0);
CTerm* where (CTerm * pTerm);
void group (CTerm * t);
void having (CTerm * pTerm);
void order (CTerm * t, int head=0);
void distinct (bool bSwitch);

std::string exec (const char * szCmd);
void update (const char * szTable, CTerm* pList);

void crosstab_query (CTerm * pData, CTerm * pCols, CFunction * pList);

extern CStatement * m_stmt;
} // namespace
using namespace sqlp;



/* Line 189 of yacc.c  */
#line 145 "yac.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     lCMD = 258,
     lNAME = 259,
     lSTRING = 260,
     lNUMBER = 261,
     lINTEGER = 262,
     lBOOLEAN = 263,
     lDATETIME = 264,
     lAVG = 265,
     lSUM = 266,
     lCOUNT = 267,
     lMIN = 268,
     lMAX = 269,
     lFIRST = 270,
     lLAST = 271,
     lSELECT = 272,
     lDISTINCT = 273,
     lALL = 274,
     lAS = 275,
     lFROM = 276,
     lWHERE = 277,
     lGROUP = 278,
     lBY = 279,
     lHAVING = 280,
     lJOIN = 281,
     lON = 282,
     lLEFT = 283,
     lRIGHT = 284,
     lFULL = 285,
     lINNER = 286,
     lOUTER = 287,
     lORDER = 288,
     lASC = 289,
     lDESC = 290,
     lOJ = 291,
     lDATE = 292,
     lTIME = 293,
     lTIMESTAMP = 294,
     lGUID = 295,
     lESCAPE = 296,
     lCALL = 297,
     lFN = 298,
     lABS = 299,
     lSIGN = 300,
     lMOD = 301,
     lDIV = 302,
     lEXP = 303,
     lLOG = 304,
     lSQRT = 305,
     lFMT = 306,
     lNULL = 307,
     lIS = 308,
     lLIKE = 309,
     lIN = 310,
     lBETWEEN = 311,
     lINSERT = 312,
     lINTO = 313,
     lVALUES = 314,
     lUPDATE = 315,
     lSET = 316,
     lDELETE = 317,
     lNEQ = 318,
     lLEQ = 319,
     lGEQ = 320,
     lLEN = 321,
     lNVL = 322,
     lTRANSFORM = 323,
     lPIVOT = 324,
     lOR = 325,
     lAND = 326,
     lNOT = 327,
     lUMINUS = 328
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 73 ".\\yac.y"

	double number;
	const char * string;
	long int integer;
	int token;
	time_t datetime;
	sqlp::CTerm * term;
	sqlp::CTable * table;
	sqlp::CNameList * name;



/* Line 214 of yacc.c  */
#line 267 "yac.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 279 "yac.cpp"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1135

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  88
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  130
/* YYNRULES -- Number of states.  */
#define YYNSTATES  309

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   328

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      80,    81,    75,    73,    78,    74,    79,    76,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      85,    84,    86,    87,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    82,     2,    83,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    77
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,     9,    11,    13,    15,    17,
      21,    23,    25,    26,    27,    39,    40,    42,    44,    46,
      50,    52,    54,    58,    62,    66,    68,    70,    74,    81,
      88,    95,   102,   105,   107,   109,   113,   115,   120,   122,
     125,   129,   133,   138,   144,   150,   157,   165,   166,   169,
     170,   174,   176,   180,   181,   184,   185,   189,   191,   195,
     197,   200,   203,   205,   209,   215,   219,   223,   227,   231,
     235,   239,   243,   247,   251,   256,   260,   263,   267,   273,
     279,   284,   289,   294,   299,   304,   309,   314,   319,   321,
     323,   325,   327,   329,   331,   335,   339,   343,   347,   350,
     354,   359,   364,   371,   378,   383,   388,   393,   400,   407,
     412,   417,   422,   424,   428,   435,   442,   447,   454,   456,
     458,   460,   464,   466,   470,   474,   476,   480,   486,   497,
     503
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      89,     0,    -1,    89,    90,    -1,    -1,    91,    -1,   118,
      -1,   117,    -1,   119,    -1,   120,    -1,    51,   112,     5,
      -1,     3,    -1,     1,    -1,    -1,    -1,    17,    92,    94,
      95,    21,   100,    93,   102,   103,   105,   106,    -1,    -1,
      19,    -1,    18,    -1,    96,    -1,    95,    78,    96,    -1,
      75,    -1,   112,    -1,   112,    20,     4,    -1,   112,    20,
       5,    -1,     4,    79,    75,    -1,   101,    -1,    98,    -1,
      80,    98,    81,    -1,   101,    31,    26,    97,    27,   110,
      -1,   101,    28,    99,    97,    27,   110,    -1,   101,    29,
      99,    97,    27,   110,    -1,   101,    30,    99,    97,    27,
     110,    -1,    32,    26,    -1,    26,    -1,   101,    -1,   100,
      78,   101,    -1,    98,    -1,    82,    36,    98,    83,    -1,
       4,    -1,     4,     4,    -1,     4,    20,     4,    -1,     4,
      79,     4,    -1,     4,    79,     4,     4,    -1,     4,    79,
       4,    20,     4,    -1,     4,    79,     4,    79,     4,    -1,
       4,    79,     4,    79,     4,     4,    -1,     4,    79,     4,
      79,     4,    20,     4,    -1,    -1,    22,   110,    -1,    -1,
      23,    24,   104,    -1,   112,    -1,   104,    78,   112,    -1,
      -1,    25,   110,    -1,    -1,    33,    24,   107,    -1,   108,
      -1,   107,    78,   108,    -1,   112,    -1,   112,    34,    -1,
     112,    35,    -1,     4,    -1,     4,    79,     4,    -1,     4,
      79,     4,    79,     4,    -1,   110,    71,   110,    -1,   110,
      70,   110,    -1,   112,    84,   112,    -1,   112,    85,   112,
      -1,   112,    86,   112,    -1,   112,    63,   112,    -1,   112,
      65,   112,    -1,   112,    64,   112,    -1,   112,    54,     5,
      -1,   112,    53,    72,    52,    -1,   112,    53,    52,    -1,
      72,   110,    -1,    80,   110,    81,    -1,   112,    55,    80,
     113,    81,    -1,   112,    56,   112,    71,   112,    -1,    12,
      80,    75,    81,    -1,    12,    80,   112,    81,    -1,    10,
      80,   112,    81,    -1,    11,    80,   112,    81,    -1,    13,
      80,   112,    81,    -1,    14,    80,   112,    81,    -1,    15,
      80,   112,    81,    -1,    16,    80,   112,    81,    -1,     6,
      -1,     7,    -1,     8,    -1,     5,    -1,     9,    -1,   109,
      -1,   112,    73,   112,    -1,   112,    74,   112,    -1,   112,
      75,   112,    -1,   112,    76,   112,    -1,    74,   112,    -1,
      80,   112,    81,    -1,    44,    80,   112,    81,    -1,    45,
      80,   112,    81,    -1,    46,    80,   112,    78,   112,    81,
      -1,    47,    80,   112,    78,   112,    81,    -1,    48,    80,
     112,    81,    -1,    49,    80,   112,    81,    -1,    50,    80,
     112,    81,    -1,    48,    80,   112,    78,   112,    81,    -1,
      49,    80,   112,    78,   112,    81,    -1,    82,    39,     5,
      83,    -1,    82,    37,     5,    83,    -1,    82,    38,     5,
      83,    -1,    87,    -1,    80,    91,    81,    -1,    13,    80,
     112,    78,   113,    81,    -1,    14,    80,   112,    78,   113,
      81,    -1,    66,    80,   112,    81,    -1,    67,    80,   112,
      78,   112,    81,    -1,   111,    -1,   110,    -1,   112,    -1,
     113,    78,   112,    -1,     4,    -1,   114,    78,     4,    -1,
       4,    84,   112,    -1,   115,    -1,   116,    78,   115,    -1,
      60,     4,    61,   116,   102,    -1,    57,    58,     4,    80,
     114,    81,    59,    80,   113,    81,    -1,    62,    75,    21,
       4,   102,    -1,    68,   111,    91,    69,   112,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   142,   142,   143,   147,   148,   149,   150,   151,   152,
     153,   154,   158,   159,   158,   164,   165,   166,   170,   171,
     172,   176,   177,   178,   179,   183,   184,   185,   189,   190,
     191,   192,   196,   197,   201,   202,   203,   204,   208,   209,
     210,   211,   212,   213,   214,   215,   216,   219,   221,   224,
     226,   230,   231,   234,   236,   239,   241,   245,   246,   250,
     251,   252,   256,   257,   258,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     280,   281,   282,   283,   284,   285,   286,   287,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   306,   307,   308,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318,   319,   320,   321,   322,
     326,   327,   331,   332,   336,   340,   341,   345,   349,   353,
     357
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "lCMD", "lNAME", "lSTRING", "lNUMBER",
  "lINTEGER", "lBOOLEAN", "lDATETIME", "lAVG", "lSUM", "lCOUNT", "lMIN",
  "lMAX", "lFIRST", "lLAST", "lSELECT", "lDISTINCT", "lALL", "lAS",
  "lFROM", "lWHERE", "lGROUP", "lBY", "lHAVING", "lJOIN", "lON", "lLEFT",
  "lRIGHT", "lFULL", "lINNER", "lOUTER", "lORDER", "lASC", "lDESC", "lOJ",
  "lDATE", "lTIME", "lTIMESTAMP", "lGUID", "lESCAPE", "lCALL", "lFN",
  "lABS", "lSIGN", "lMOD", "lDIV", "lEXP", "lLOG", "lSQRT", "lFMT",
  "lNULL", "lIS", "lLIKE", "lIN", "lBETWEEN", "lINSERT", "lINTO",
  "lVALUES", "lUPDATE", "lSET", "lDELETE", "lNEQ", "lLEQ", "lGEQ", "lLEN",
  "lNVL", "lTRANSFORM", "lPIVOT", "lOR", "lAND", "lNOT", "'+'", "'-'",
  "'*'", "'/'", "lUMINUS", "','", "'.'", "'('", "')'", "'{'", "'}'", "'='",
  "'<'", "'>'", "'?'", "$accept", "y_lines", "y_line", "y_select", "$@1",
  "$@2", "y_select_opt", "y_select_list", "y_select_item", "y_join_table",
  "y_join", "y_outer_join", "y_table_list", "y_table", "y_where",
  "y_group", "y_group_list", "y_having", "y_order", "y_order_list",
  "y_order_term", "y_column", "y_bool", "y_aggr", "y_term", "y_term_list",
  "y_name_list", "y_update_term", "y_update_list", "y_update", "y_insert",
  "y_delete", "y_crosstab_query", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,    43,    45,    42,    47,   328,    44,    46,
      40,    41,   123,   125,    61,    60,    62,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    88,    89,    89,    90,    90,    90,    90,    90,    90,
      90,    90,    92,    93,    91,    94,    94,    94,    95,    95,
      95,    96,    96,    96,    96,    97,    97,    97,    98,    98,
      98,    98,    99,    99,   100,   100,   100,   100,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   102,   102,   103,
     103,   104,   104,   105,   105,   106,   106,   107,   107,   108,
     108,   108,   109,   109,   109,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     111,   111,   111,   111,   111,   111,   111,   111,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     113,   113,   114,   114,   115,   116,   116,   117,   118,   119,
     120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     1,     1,     1,     3,
       1,     1,     0,     0,    11,     0,     1,     1,     1,     3,
       1,     1,     3,     3,     3,     1,     1,     3,     6,     6,
       6,     6,     2,     1,     1,     3,     1,     4,     1,     2,
       3,     3,     4,     5,     5,     6,     7,     0,     2,     0,
       3,     1,     3,     0,     2,     0,     3,     1,     3,     1,
       2,     2,     1,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     4,     3,     2,     3,     5,     5,
       4,     4,     4,     4,     4,     4,     4,     4,     1,     1,
       1,     1,     1,     1,     3,     3,     3,     3,     2,     3,
       4,     4,     6,     6,     4,     4,     4,     6,     6,     4,
       4,     4,     1,     3,     6,     6,     4,     6,     1,     1,
       1,     3,     1,     3,     3,     1,     3,     5,    10,     5,
       5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     1,    11,    10,    12,     0,     0,     0,     0,
       0,     2,     4,     6,     5,     7,     8,    15,    62,    91,
      88,    89,    90,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   112,    93,   119,   118,     0,     0,
       0,     0,     0,     0,     0,    17,    16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    76,     0,    98,     0,   119,
       0,     0,     0,     0,     0,     0,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    20,     0,
      18,    21,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,    77,    99,     0,     0,     0,    66,    65,    75,     0,
      73,     0,     0,    70,    72,    71,    94,    95,    96,    97,
      67,    68,    69,     0,     0,   125,    47,    47,     0,     0,
       0,     0,     0,     0,     0,     0,    82,    83,    80,    81,
       0,    84,     0,    85,    86,    87,   100,   101,     0,     0,
       0,   104,     0,   105,   106,   116,     0,   110,   111,   109,
      74,   120,     0,     0,   122,     0,     0,     0,     0,   127,
     129,   130,    24,    38,     0,    36,    13,    34,    19,    22,
      23,    64,     0,     0,     0,     0,     0,     0,     0,     0,
      78,    79,     0,     0,   124,    48,   126,    39,     0,     0,
       0,     0,    47,     0,     0,     0,     0,   114,   115,   102,
     103,   107,   108,   117,   121,   123,     0,    40,    41,     0,
       0,    35,    49,    33,     0,     0,     0,     0,     0,     0,
      42,     0,     0,    37,     0,    53,    32,     0,     0,    26,
      25,     0,     0,     0,     0,    43,    44,     0,     0,    55,
       0,     0,     0,     0,     0,   128,    45,     0,    50,    51,
     119,     0,    14,    27,    29,    30,    31,    28,    46,     0,
       0,    52,    56,    57,    59,     0,    60,    61,    58
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,    17,   232,    57,   109,   110,   268,
     269,   255,   206,   270,   199,   265,   288,   279,   292,   302,
     303,    45,    46,    47,    76,   192,   195,   155,   156,    13,
      14,    15,    16
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -209
static const yytype_int16 yypact[] =
{
    -209,   160,  -209,  -209,  -209,  -209,   366,   -37,    10,   -56,
     236,  -209,  -209,  -209,  -209,  -209,  -209,    14,   -36,  -209,
    -209,  -209,  -209,  -209,    -1,    28,    54,    60,    62,    65,
      66,    70,    71,    72,    73,    75,    79,    82,    85,    87,
     366,   366,   188,    76,  -209,  -209,    35,  -209,    47,    77,
      15,   122,    88,    89,   139,  -209,  -209,   272,   166,   366,
     366,   319,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,  -209,  1025,  1049,    90,   -55,
     625,   170,   173,   174,   366,   366,  -209,   -42,   176,   102,
     366,   366,   366,   366,   366,   366,   366,   366,   366,   366,
     366,   103,   180,   181,   366,   366,   117,   109,  -209,    -9,
    -209,   461,   110,   639,   663,   125,   677,    63,   499,   701,
     715,   739,   753,   777,   793,   533,   567,   819,   833,   857,
    -209,  -209,  -209,   108,   124,   126,   137,  -209,  -209,   158,
    -209,   366,   873,  1025,  1025,  1025,   380,   380,  1049,  1049,
    1025,  1025,  1025,   208,   130,  -209,    -5,   193,   897,   911,
     366,     5,     1,   413,   120,   212,  -209,  -209,  -209,  -209,
     366,  -209,   366,  -209,  -209,  -209,  -209,  -209,   366,   366,
     366,  -209,   366,  -209,  -209,  -209,   366,  -209,  -209,  -209,
    -209,  1025,   -50,   366,  -209,   -44,   366,   366,   180,  -209,
    -209,  1025,  -209,     3,   182,  -209,   141,    11,  -209,  -209,
    -209,  -209,   -34,    -4,   935,   949,   973,   987,  1011,   366,
    -209,  1025,   217,   164,  1025,   601,  -209,  -209,   220,   221,
     222,   222,   193,    -8,    -8,    -8,   201,  -209,  -209,  -209,
    -209,  -209,  -209,  -209,  1025,  -209,   149,  -209,    18,   147,
      11,  -209,   218,  -209,   205,    16,    16,    16,    16,   366,
    -209,   252,   253,  -209,   216,   233,  -209,   222,   232,  -209,
      11,   234,   237,   238,    -3,  -209,     7,   366,   366,   230,
     185,   366,   366,   366,   366,  -209,  -209,   263,   191,  1025,
     451,   248,  -209,  -209,   601,   601,   601,   601,  -209,   366,
     366,  1025,   195,  -209,   475,   366,  -209,  -209,  -209
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -209,  -209,  -209,   -29,  -209,  -209,  -209,  -209,   111,  -208,
    -158,  -105,  -209,  -160,  -128,  -209,  -209,  -209,  -209,  -209,
     -16,  -209,   -39,   280,    -6,  -164,  -209,    93,  -209,  -209,
    -209,  -209,  -209
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -120
static const yytype_int16 yytable[] =
{
      48,    75,   207,    79,   205,   203,   212,   227,   213,   112,
     138,   286,   162,    78,    50,    84,    85,   197,   253,    51,
     203,    49,   260,   228,   254,   106,   131,   287,   219,   200,
     139,   220,    55,    56,   222,    77,    80,   223,   261,   233,
     234,   235,   236,    58,   219,   136,   137,   237,   271,   272,
     273,   111,    86,   113,   114,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   163,
     250,   251,   249,   198,   219,   219,   102,   238,   285,    59,
     202,   101,   229,   204,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   274,   267,   262,   158,   159,
      87,    88,    89,    90,   252,    84,    85,   250,    60,   280,
      91,    92,    93,    81,    82,    83,    87,    88,    89,    90,
      94,    95,    96,    97,   209,   210,    91,    92,    93,   256,
     257,    98,    99,   100,    61,   191,    94,    95,    96,    97,
      62,   170,    63,   103,   171,    64,    65,    98,    99,   100,
      66,    67,    68,    69,   201,    70,     5,   111,   225,    71,
       2,     3,    72,     4,   191,    73,   191,    74,   104,   105,
     112,   130,   214,   215,   216,   133,   217,     5,   134,   135,
     218,   140,   141,   153,   154,   157,   160,   221,   161,   165,
     224,   187,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,     5,   168,   188,    85,   189,
     190,     6,   194,   244,   196,   197,   211,     7,   230,   231,
       8,   245,     9,   246,   247,   248,   203,   258,    10,   259,
     263,   266,    31,    32,    33,    34,    35,    36,    37,   290,
     277,   264,   294,   295,   296,   297,    24,    25,    26,    52,
      53,    29,    30,   191,    38,    39,   275,   276,   278,   281,
      40,   282,    41,   291,   283,   284,   293,   298,    42,   299,
      43,   289,   300,   305,   208,    44,   107,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,   308,
      54,   226,     0,   301,   304,     0,     0,     0,     0,   304,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    31,    32,    33,    34,
      35,    36,    37,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     0,     0,    38,    39,
       0,     0,     0,     0,    40,     0,    41,   108,     0,     0,
       0,     0,    42,     0,    43,     0,     0,     0,     0,    44,
       0,     0,     0,    31,    32,    33,    34,    35,    36,    37,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,     0,     0,    38,    39,     0,     0,     0,
       0,    40,     0,    41,   115,     0,     0,     0,     0,    42,
       0,    43,     0,     0,     0,     0,    44,     0,     0,     0,
      31,    32,    33,    34,    35,    36,    37,   107,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
       0,     0,    38,    39,    88,    89,    90,     0,    40,     0,
      41,     0,     0,    91,    92,    93,    42,     0,    43,     0,
       0,   -54,   -54,    44,   -54,    96,    97,    31,    32,    33,
      34,    35,    36,    37,    98,    99,   100,     0,   -54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
      39,   164,     0,     0,   -54,    40,     0,    41,     0,     0,
       0,     0,     0,    42,     0,    43,     0,     0,     0,     0,
      44,     0,   -54,     0,     0,     0,     0,     0,   -54,   306,
     307,   -54,     0,   -54,    87,    88,    89,    90,     0,   -54,
     -54,    84,    85,     0,    91,    92,    93,     0,    87,    88,
      89,    90,   -54,     0,    94,    95,    96,    97,    91,    92,
      93,     0,     0,     0,     0,    98,    99,   100,    94,    95,
      96,    97,    87,    88,    89,    90,     0,     0,     0,    98,
      99,   100,    91,    92,    93,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,     0,   172,     0,     0,
     173,     0,     0,    98,    99,   100,    87,    88,    89,    90,
       0,     0,     0,     0,     0,     0,    91,    92,    93,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
       0,   180,     0,     0,   181,     0,     0,    98,    99,   100,
      87,    88,    89,    90,     0,     0,     0,     0,     0,     0,
      91,    92,    93,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,     0,   182,     0,     0,   183,     0,
       0,    98,    99,   100,  -119,  -119,  -119,  -119,     0,     0,
       0,     0,     0,     0,  -119,  -119,  -119,     0,     0,     0,
       0,    84,    85,     0,  -119,  -119,  -119,  -119,    87,    88,
      89,    90,     0,     0,     0,  -119,  -119,  -119,    91,    92,
      93,     0,    87,    88,    89,    90,     0,     0,    94,    95,
      96,    97,    91,    92,    93,     0,   132,     0,     0,    98,
      99,   100,    94,    95,    96,    97,    87,    88,    89,    90,
     166,     0,     0,    98,    99,   100,    91,    92,    93,     0,
      87,    88,    89,    90,     0,     0,    94,    95,    96,    97,
      91,    92,    93,     0,   167,     0,     0,    98,    99,   100,
      94,    95,    96,    97,    87,    88,    89,    90,   169,     0,
       0,    98,    99,   100,    91,    92,    93,     0,    87,    88,
      89,    90,     0,     0,    94,    95,    96,    97,    91,    92,
      93,     0,   174,     0,     0,    98,    99,   100,    94,    95,
      96,    97,    87,    88,    89,    90,   175,     0,     0,    98,
      99,   100,    91,    92,    93,     0,    87,    88,    89,    90,
       0,     0,    94,    95,    96,    97,    91,    92,    93,     0,
     176,     0,     0,    98,    99,   100,    94,    95,    96,    97,
      87,    88,    89,    90,   177,     0,     0,    98,    99,   100,
      91,    92,    93,     0,     0,     0,    87,    88,    89,    90,
      94,    95,    96,    97,     0,   178,    91,    92,    93,     0,
       0,    98,    99,   100,     0,     0,    94,    95,    96,    97,
       0,   179,    87,    88,    89,    90,     0,    98,    99,   100,
       0,     0,    91,    92,    93,     0,    87,    88,    89,    90,
       0,     0,    94,    95,    96,    97,    91,    92,    93,     0,
     184,     0,     0,    98,    99,   100,    94,    95,    96,    97,
      87,    88,    89,    90,   185,     0,     0,    98,    99,   100,
      91,    92,    93,     0,     0,     0,    87,    88,    89,    90,
      94,    95,    96,    97,     0,   186,    91,    92,    93,     0,
       0,    98,    99,   100,   193,     0,    94,    95,    96,    97,
      87,    88,    89,    90,     0,     0,     0,    98,    99,   100,
      91,    92,    93,     0,    87,    88,    89,    90,     0,     0,
      94,    95,    96,    97,    91,    92,    93,     0,   171,     0,
       0,    98,    99,   100,    94,    95,    96,    97,    87,    88,
      89,    90,   173,     0,     0,    98,    99,   100,    91,    92,
      93,     0,    87,    88,    89,    90,     0,     0,    94,    95,
      96,    97,    91,    92,    93,     0,   239,     0,     0,    98,
      99,   100,    94,    95,    96,    97,    87,    88,    89,    90,
     240,     0,     0,    98,    99,   100,    91,    92,    93,     0,
      87,    88,    89,    90,     0,     0,    94,    95,    96,    97,
      91,    92,    93,     0,   241,     0,     0,    98,    99,   100,
      94,    95,    96,    97,    87,    88,    89,    90,   242,     0,
       0,    98,    99,   100,    91,    92,    93,     0,    87,    88,
      89,    90,     0,     0,    94,    95,    96,    97,    91,    92,
      93,     0,   243,     0,     0,    98,    99,   100,    94,    95,
      96,    97,     0,    88,    89,    90,     0,     0,     0,    98,
      99,   100,    91,    92,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,    99,   100
};

static const yytype_int16 yycheck[] =
{
       6,    40,   162,    42,   162,     4,   170,     4,   172,     4,
      52,     4,    21,    42,     4,    70,    71,    22,    26,    75,
       4,    58,     4,    20,    32,    54,    81,    20,    78,   157,
      72,    81,    18,    19,    78,    41,    42,    81,    20,    28,
      29,    30,    31,    79,    78,    84,    85,    81,   256,   257,
     258,    57,     5,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    78,
     230,   231,   230,    78,    78,    78,    61,    81,    81,    80,
      75,     4,    79,    82,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   259,    80,    79,   104,   105,
      53,    54,    55,    56,   232,    70,    71,   267,    80,   267,
      63,    64,    65,    37,    38,    39,    53,    54,    55,    56,
      73,    74,    75,    76,     4,     5,    63,    64,    65,   234,
     235,    84,    85,    86,    80,   141,    73,    74,    75,    76,
      80,    78,    80,    21,    81,    80,    80,    84,    85,    86,
      80,    80,    80,    80,   160,    80,    17,   163,   197,    80,
       0,     1,    80,     3,   170,    80,   172,    80,    80,    80,
       4,    81,   178,   179,   180,     5,   182,    17,     5,     5,
     186,     5,    80,    80,     4,     4,    69,   193,    79,    79,
     196,    83,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    81,    83,    71,    83,
      52,    51,     4,   219,    84,    22,     4,    57,    36,    78,
      60,     4,    62,    59,     4,     4,     4,    26,    68,    80,
      83,    26,    44,    45,    46,    47,    48,    49,    50,   278,
      24,    23,   281,   282,   283,   284,    10,    11,    12,    13,
      14,    15,    16,   259,    66,    67,     4,     4,    25,    27,
      72,    27,    74,    33,    27,    27,    81,     4,    80,    78,
      82,   277,    24,    78,   163,    87,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,   305,
      10,   198,    -1,   299,   300,    -1,    -1,    -1,    -1,   305,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,    -1,
      -1,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    44,    45,    46,    47,    48,    49,    50,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    74,    75,    -1,    -1,    -1,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      44,    45,    46,    47,    48,    49,    50,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    66,    67,    54,    55,    56,    -1,    72,    -1,
      74,    -1,    -1,    63,    64,    65,    80,    -1,    82,    -1,
      -1,     0,     1,    87,     3,    75,    76,    44,    45,    46,
      47,    48,    49,    50,    84,    85,    86,    -1,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,
      67,    20,    -1,    -1,    33,    72,    -1,    74,    -1,    -1,
      -1,    -1,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    -1,    51,    -1,    -1,    -1,    -1,    -1,    57,    34,
      35,    60,    -1,    62,    53,    54,    55,    56,    -1,    68,
      69,    70,    71,    -1,    63,    64,    65,    -1,    53,    54,
      55,    56,    81,    -1,    73,    74,    75,    76,    63,    64,
      65,    -1,    -1,    -1,    -1,    84,    85,    86,    73,    74,
      75,    76,    53,    54,    55,    56,    -1,    -1,    -1,    84,
      85,    86,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    74,    75,    76,    -1,    78,    -1,    -1,
      81,    -1,    -1,    84,    85,    86,    53,    54,    55,    56,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    64,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    74,    75,    76,
      -1,    78,    -1,    -1,    81,    -1,    -1,    84,    85,    86,
      53,    54,    55,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    64,    65,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      73,    74,    75,    76,    -1,    78,    -1,    -1,    81,    -1,
      -1,    84,    85,    86,    53,    54,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    65,    -1,    -1,    -1,
      -1,    70,    71,    -1,    73,    74,    75,    76,    53,    54,
      55,    56,    -1,    -1,    -1,    84,    85,    86,    63,    64,
      65,    -1,    53,    54,    55,    56,    -1,    -1,    73,    74,
      75,    76,    63,    64,    65,    -1,    81,    -1,    -1,    84,
      85,    86,    73,    74,    75,    76,    53,    54,    55,    56,
      81,    -1,    -1,    84,    85,    86,    63,    64,    65,    -1,
      53,    54,    55,    56,    -1,    -1,    73,    74,    75,    76,
      63,    64,    65,    -1,    81,    -1,    -1,    84,    85,    86,
      73,    74,    75,    76,    53,    54,    55,    56,    81,    -1,
      -1,    84,    85,    86,    63,    64,    65,    -1,    53,    54,
      55,    56,    -1,    -1,    73,    74,    75,    76,    63,    64,
      65,    -1,    81,    -1,    -1,    84,    85,    86,    73,    74,
      75,    76,    53,    54,    55,    56,    81,    -1,    -1,    84,
      85,    86,    63,    64,    65,    -1,    53,    54,    55,    56,
      -1,    -1,    73,    74,    75,    76,    63,    64,    65,    -1,
      81,    -1,    -1,    84,    85,    86,    73,    74,    75,    76,
      53,    54,    55,    56,    81,    -1,    -1,    84,    85,    86,
      63,    64,    65,    -1,    -1,    -1,    53,    54,    55,    56,
      73,    74,    75,    76,    -1,    78,    63,    64,    65,    -1,
      -1,    84,    85,    86,    -1,    -1,    73,    74,    75,    76,
      -1,    78,    53,    54,    55,    56,    -1,    84,    85,    86,
      -1,    -1,    63,    64,    65,    -1,    53,    54,    55,    56,
      -1,    -1,    73,    74,    75,    76,    63,    64,    65,    -1,
      81,    -1,    -1,    84,    85,    86,    73,    74,    75,    76,
      53,    54,    55,    56,    81,    -1,    -1,    84,    85,    86,
      63,    64,    65,    -1,    -1,    -1,    53,    54,    55,    56,
      73,    74,    75,    76,    -1,    78,    63,    64,    65,    -1,
      -1,    84,    85,    86,    71,    -1,    73,    74,    75,    76,
      53,    54,    55,    56,    -1,    -1,    -1,    84,    85,    86,
      63,    64,    65,    -1,    53,    54,    55,    56,    -1,    -1,
      73,    74,    75,    76,    63,    64,    65,    -1,    81,    -1,
      -1,    84,    85,    86,    73,    74,    75,    76,    53,    54,
      55,    56,    81,    -1,    -1,    84,    85,    86,    63,    64,
      65,    -1,    53,    54,    55,    56,    -1,    -1,    73,    74,
      75,    76,    63,    64,    65,    -1,    81,    -1,    -1,    84,
      85,    86,    73,    74,    75,    76,    53,    54,    55,    56,
      81,    -1,    -1,    84,    85,    86,    63,    64,    65,    -1,
      53,    54,    55,    56,    -1,    -1,    73,    74,    75,    76,
      63,    64,    65,    -1,    81,    -1,    -1,    84,    85,    86,
      73,    74,    75,    76,    53,    54,    55,    56,    81,    -1,
      -1,    84,    85,    86,    63,    64,    65,    -1,    53,    54,
      55,    56,    -1,    -1,    73,    74,    75,    76,    63,    64,
      65,    -1,    81,    -1,    -1,    84,    85,    86,    73,    74,
      75,    76,    -1,    54,    55,    56,    -1,    -1,    -1,    84,
      85,    86,    63,    64,    65,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    84,    85,    86
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    89,     0,     1,     3,    17,    51,    57,    60,    62,
      68,    90,    91,   117,   118,   119,   120,    92,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    44,    45,    46,    47,    48,    49,    50,    66,    67,
      72,    74,    80,    82,    87,   109,   110,   111,   112,    58,
       4,    75,    13,    14,   111,    18,    19,    94,    79,    80,
      80,    80,    80,    80,    80,    80,    80,    80,    80,    80,
      80,    80,    80,    80,    80,   110,   112,   112,    91,   110,
     112,    37,    38,    39,    70,    71,     5,    53,    54,    55,
      56,    63,    64,    65,    73,    74,    75,    76,    84,    85,
      86,     4,    61,    21,    80,    80,    91,     4,    75,    95,
      96,   112,     4,   112,   112,    75,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
      81,    81,    81,     5,     5,     5,   110,   110,    52,    72,
       5,    80,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,    80,     4,   115,   116,     4,   112,   112,
      69,    79,    21,    78,    20,    79,    81,    81,    81,    81,
      78,    81,    78,    81,    81,    81,    81,    81,    78,    78,
      78,    81,    78,    81,    81,    81,    78,    83,    83,    83,
      52,   112,   113,    71,     4,   114,    84,    22,    78,   102,
     102,   112,    75,     4,    82,    98,   100,   101,    96,     4,
       5,     4,   113,   113,   112,   112,   112,   112,   112,    78,
      81,   112,    78,    81,   112,   110,   115,     4,    20,    79,
      36,    78,    93,    28,    29,    30,    31,    81,    81,    81,
      81,    81,    81,    81,   112,     4,    59,     4,     4,    98,
     101,   101,   102,    26,    32,    99,    99,    99,    26,    80,
       4,    20,    79,    83,    23,   103,    26,    80,    97,    98,
     101,    97,    97,    97,   113,     4,     4,    24,    25,   105,
      98,    27,    27,    27,    27,    81,     4,    20,   104,   112,
     110,    33,   106,    81,   110,   110,   110,   110,     4,    78,
      24,   112,   107,   108,   112,    78,    34,    35,   108
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1455 of yacc.c  */
#line 147 ".\\yac.y"
    { printf("SQL: ok\n"); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 149 ".\\yac.y"
    { printf("UPDATE: SQL-Syntax ok.\n"); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 152 ".\\yac.y"
    { m_stmt->setFormat ((yyvsp[(2) - (3)].term), (yyvsp[(3) - (3)].string)); exec("formats");;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 153 ".\\yac.y"
    { printf("%s\n", exec ((yyvsp[(1) - (1)].string)).c_str()); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 154 ".\\yac.y"
    { yyexit(); YYABORT; ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 158 ".\\yac.y"
    { m_stmt->clear(); ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 159 ".\\yac.y"
    { m_stmt->select(); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 160 ".\\yac.y"
    { m_stmt->prepare(); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 164 ".\\yac.y"
    { distinct (false); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 165 ".\\yac.y"
    { distinct (false); ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 166 ".\\yac.y"
    { distinct (true); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 172 ".\\yac.y"
    { select (column(0,0,0)); ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 176 ".\\yac.y"
    { select ((yyvsp[(1) - (1)].term)); ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 177 ".\\yac.y"
    { select ((yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 178 ".\\yac.y"
    { select ((yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 179 ".\\yac.y"
    { select (column (0, (yyvsp[(1) - (3)].string), 0)); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 183 ".\\yac.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 184 ".\\yac.y"
    { (yyval.string) = (yyvsp[(1) - (1)].string); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 185 ".\\yac.y"
    { (yyval.string) = (yyvsp[(2) - (3)].string); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 189 ".\\yac.y"
    { join (lINNER, (yyvsp[(1) - (6)].string), (yyvsp[(4) - (6)].string), (yyvsp[(6) - (6)].term)); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 190 ".\\yac.y"
    { join (lLEFT,  (yyvsp[(1) - (6)].string), (yyvsp[(4) - (6)].string), (yyvsp[(6) - (6)].term)); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 191 ".\\yac.y"
    { join (lRIGHT, (yyvsp[(1) - (6)].string), (yyvsp[(4) - (6)].string), (yyvsp[(6) - (6)].term)); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 192 ".\\yac.y"
    { join (lFULL,  (yyvsp[(1) - (6)].string), (yyvsp[(4) - (6)].string), (yyvsp[(6) - (6)].term)); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 201 ".\\yac.y"
    {;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 203 ".\\yac.y"
    {;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 204 ".\\yac.y"
    {;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 208 ".\\yac.y"
    { (yyval.string) = table ("", "", (yyvsp[(1) - (1)].string),  0); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 209 ".\\yac.y"
    { (yyval.string) = table ("", "", (yyvsp[(1) - (2)].string), (yyvsp[(2) - (2)].string)); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 210 ".\\yac.y"
    { (yyval.string) = table ("", "", (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 211 ".\\yac.y"
    { (yyval.string) = table ("", (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string),  0); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 212 ".\\yac.y"
    { (yyval.string) = table ("", (yyvsp[(1) - (4)].string), (yyvsp[(3) - (4)].string), (yyvsp[(4) - (4)].string)); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 213 ".\\yac.y"
    { (yyval.string) = table ("", (yyvsp[(1) - (5)].string), (yyvsp[(3) - (5)].string), (yyvsp[(5) - (5)].string)); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 214 ".\\yac.y"
    { (yyval.string) = table ((yyvsp[(1) - (5)].string), (yyvsp[(3) - (5)].string), (yyvsp[(5) - (5)].string),  0); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 215 ".\\yac.y"
    { (yyval.string) = table ((yyvsp[(1) - (6)].string), (yyvsp[(3) - (6)].string), (yyvsp[(5) - (6)].string), (yyvsp[(6) - (6)].string)); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 216 ".\\yac.y"
    { (yyval.string) = table ((yyvsp[(1) - (7)].string), (yyvsp[(3) - (7)].string), (yyvsp[(5) - (7)].string), (yyvsp[(7) - (7)].string)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 221 ".\\yac.y"
    { where ((yyvsp[(2) - (2)].term)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 230 ".\\yac.y"
    { group((yyvsp[(1) - (1)].term)); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 231 ".\\yac.y"
    { group((yyvsp[(3) - (3)].term)); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 236 ".\\yac.y"
    { having((yyvsp[(2) - (2)].term)); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 250 ".\\yac.y"
    { order((yyvsp[(1) - (1)].term)); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 251 ".\\yac.y"
    { order((yyvsp[(1) - (2)].term),lASC); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 252 ".\\yac.y"
    { order((yyvsp[(1) - (2)].term),lDESC); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 256 ".\\yac.y"
    { (yyval.term) = column ( 0,  0, (yyvsp[(1) - (1)].string)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 257 ".\\yac.y"
    { (yyval.term) = column ( 0, (yyvsp[(1) - (3)].string), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 258 ".\\yac.y"
    { (yyval.term) = column ((yyvsp[(1) - (5)].string), (yyvsp[(3) - (5)].string), (yyvsp[(5) - (5)].string)); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 262 ".\\yac.y"
    { (yyval.term) = func(lAND, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 263 ".\\yac.y"
    { (yyval.term) = func(lOR, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 264 ".\\yac.y"
    { (yyval.term) = func('=', (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 265 ".\\yac.y"
    { (yyval.term) = func('<', (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 266 ".\\yac.y"
    { (yyval.term) = func('>', (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 267 ".\\yac.y"
    { (yyval.term) = func(lNEQ, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 268 ".\\yac.y"
    { (yyval.term) = func(lGEQ, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 269 ".\\yac.y"
    { (yyval.term) = func(lLEQ, (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 270 ".\\yac.y"
    { (yyval.term) = func(lLIKE, (yyvsp[(1) - (3)].term), value((yyvsp[(3) - (3)].string)), 0); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 271 ".\\yac.y"
    { (yyval.term) = unary(lNOT, unary(lNULL, (yyvsp[(1) - (4)].term))); ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 272 ".\\yac.y"
    { (yyval.term) = unary(lNULL, (yyvsp[(1) - (3)].term)); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 273 ".\\yac.y"
    { (yyval.term) = unary(lNOT, (yyvsp[(2) - (2)].term)); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 274 ".\\yac.y"
    { (yyval.term) = (yyvsp[(2) - (3)].term); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 275 ".\\yac.y"
    {(yyval.term) = func(lIN, (yyvsp[(1) - (5)].term), (yyvsp[(4) - (5)].term), 0); ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 276 ".\\yac.y"
    {(yyval.term) = func(lBETWEEN, (yyvsp[(1) - (5)].term), (yyvsp[(3) - (5)].term), (yyvsp[(5) - (5)].term), 0); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 280 ".\\yac.y"
    { (yyval.term) = aggregate (lCOUNT, (CTerm*)0); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 281 ".\\yac.y"
    { (yyval.term) = aggregate (lCOUNT, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 282 ".\\yac.y"
    { (yyval.term) = aggregate (lAVG, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 283 ".\\yac.y"
    { (yyval.term) = aggregate (lSUM, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 284 ".\\yac.y"
    { (yyval.term) = aggregate (lMIN, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 285 ".\\yac.y"
    { (yyval.term) = aggregate (lMAX, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 286 ".\\yac.y"
    { (yyval.term) = aggregate (lFIRST, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 287 ".\\yac.y"
    { (yyval.term) = aggregate (lLAST, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 291 ".\\yac.y"
    { (yyval.term) = number((yyvsp[(1) - (1)].number)); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 292 ".\\yac.y"
    { (yyval.term) = number((yyvsp[(1) - (1)].number)); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 293 ".\\yac.y"
    { (yyval.term) = number((yyvsp[(1) - (1)].number)); ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 294 ".\\yac.y"
    { (yyval.term) = value((yyvsp[(1) - (1)].string)); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 295 ".\\yac.y"
    { (yyval.term) = time ((yyvsp[(1) - (1)].datetime)); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 296 ".\\yac.y"
    { (yyval.term) = (yyvsp[(1) - (1)].term); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 297 ".\\yac.y"
    { (yyval.term) = func('+', (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 298 ".\\yac.y"
    { (yyval.term) = func('+', (yyvsp[(1) - (3)].term), unary('-', (yyvsp[(3) - (3)].term)), 0); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 299 ".\\yac.y"
    { (yyval.term) = func('*', (yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 300 ".\\yac.y"
    { (yyval.term) = func('*', (yyvsp[(1) - (3)].term), unary('/', (yyvsp[(3) - (3)].term)), 0); ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 301 ".\\yac.y"
    { (yyval.term) = unary('-', (yyvsp[(2) - (2)].term)); ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 302 ".\\yac.y"
    { (yyval.term) = (yyvsp[(2) - (3)].term); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 303 ".\\yac.y"
    { (yyval.term) = unary(lABS, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 304 ".\\yac.y"
    { (yyval.term) = unary(lSIGN, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 305 ".\\yac.y"
    { (yyval.term) = func (lMOD, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 306 ".\\yac.y"
    { (yyval.term) = func (lDIV, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 307 ".\\yac.y"
    { (yyval.term) = unary(lEXP, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 308 ".\\yac.y"
    { (yyval.term) = unary(lLOG, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 309 ".\\yac.y"
    { (yyval.term) = unary(lSQRT, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 310 ".\\yac.y"
    { (yyval.term) = func(lEXP, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 311 ".\\yac.y"
    { (yyval.term) = func(lLOG, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 312 ".\\yac.y"
    { (yyval.term) = time((yyvsp[(3) - (4)].string)); ;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 313 ".\\yac.y"
    { (yyval.term) = time((yyvsp[(3) - (4)].string)); ;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 314 ".\\yac.y"
    { (yyval.term) = time((yyvsp[(3) - (4)].string)); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 315 ".\\yac.y"
    { (yyval.term) = param(); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 316 ".\\yac.y"
    { (yyval.term) = param(); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 317 ".\\yac.y"
    { (yyval.term) = func(lMIN, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 318 ".\\yac.y"
    { (yyval.term) = func(lMAX, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 319 ".\\yac.y"
    { (yyval.term) = unary (lLEN, (yyvsp[(3) - (4)].term)); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 320 ".\\yac.y"
    { (yyval.term) = func(lNVL, (yyvsp[(3) - (6)].term), (yyvsp[(5) - (6)].term), 0); ;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 326 ".\\yac.y"
    { (yyval.term) = (yyvsp[(1) - (1)].term); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 327 ".\\yac.y"
    { (yyval.term) = termlist((yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 331 ".\\yac.y"
    { (yyval.name) = namelist(0 , (yyvsp[(1) - (1)].string)); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 332 ".\\yac.y"
    { (yyval.name) = namelist((yyvsp[(1) - (3)].name), (yyvsp[(3) - (3)].string)); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 336 ".\\yac.y"
    { (yyval.term) = func('=', column(0,0,(yyvsp[(1) - (3)].string)), (yyvsp[(3) - (3)].term), 0); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 340 ".\\yac.y"
    { (yyval.term) = (yyvsp[(1) - (1)].term); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 341 ".\\yac.y"
    { (yyval.term) = termlist ((yyvsp[(1) - (3)].term), (yyvsp[(3) - (3)].term)); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 345 ".\\yac.y"
    { update ((yyvsp[(2) - (5)].string), (yyvsp[(4) - (5)].term)); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 357 ".\\yac.y"
    { crosstab_query ((yyvsp[(2) - (5)].term),(yyvsp[(5) - (5)].term), 0); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2660 "yac.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 360 ".\\yac.y"


/////////////////////////////////////////////////////////////////////////////
#include <stdarg.h>
namespace sqlp {
//---------------------------------------------------------------------------
const char *
join (int head, const char * pLeft, const char * pRight, CTerm * pCond)
{
	assert (m_stmt);
	return m_stmt->join (head, pLeft, pRight, pCond);
}
//---------------------------------------------------------------------------
const char *
table (const char * szCatalog, const char * szSchema, const char * szTable, const char * szAlias)
{
	assert (m_stmt);
	return m_stmt->table (szCatalog, szSchema, szTable, szAlias);
}
//---------------------------------------------------------------------------
CTerm*
column (const char * szSchema, const char * szTable, const char * szColumn)
{
	assert (m_stmt);
	CTerm * pColumn = m_stmt->column (szSchema, szTable, szColumn);
	return pColumn;
}
//---------------------------------------------------------------------------
CTerm*
value (short nType , double dValue)
{
	assert (m_stmt);
	return m_stmt->value (nType, dValue);
}
//---------------------------------------------------------------------------
CTerm*
value (const char * szValue)
{
	assert (m_stmt);
	CTerm * pTerm = m_stmt->value(szValue);
	return pTerm;
}
//---------------------------------------------------------------------------
CTerm*
number (double dValue)
{
	assert (m_stmt);
	return m_stmt->value (SQL_DOUBLE, dValue);
}
//---------------------------------------------------------------------------
CTerm*
number (long int nValue)
{
	assert (m_stmt);
	return m_stmt->value (SQL_INTEGER, nValue);
}
//---------------------------------------------------------------------------
CTerm*
time (double dValue)
{
	assert (m_stmt);
	return m_stmt->time (dValue);
}
//---------------------------------------------------------------------------
CTerm*
time (const char * szValue)
{
	assert (m_stmt);
	return m_stmt->time (szValue);
}
//---------------------------------------------------------------------------
CTerm*
func (int nHead, ...)
{
	assert (m_stmt);
	va_list aArgs;
	va_start (aArgs, nHead);
	CFunction * pTerm = m_stmt->func (nHead, aArgs);
	va_end (aArgs);
	return pTerm;
}
//---------------------------------------------------------------------------
CTerm*
unary (int nHead, CTerm * pTerm)
{
	assert (m_stmt);
	CUnary * pUnary = m_stmt->unary (nHead, pTerm);
	return pUnary;
}
//---------------------------------------------------------------------------
CTerm*
aggregate (int nHead, CTerm * pTerm)
{
	assert (m_stmt);
	CUnary * pUnary = m_stmt->unary (nHead, pTerm);
	m_stmt->m_aAggr.push_back(pUnary);
	return pUnary;
}
//---------------------------------------------------------------------------
void
select (CTerm * pTerm, const char * szName)
{
	assert (m_stmt);
	if (szName)
		m_stmt->m_aNames[szName] = pTerm;
	m_stmt->m_aSelection.push_back(pTerm);
}
//---------------------------------------------------------------------------
CTerm*
where (CTerm * pTerm)
{
	assert (m_stmt);
	assert (pTerm);
	m_stmt->m_pWhere = pTerm;
	return pTerm;
}
//---------------------------------------------------------------------------
void
group (CTerm * t)
{
	assert (m_stmt);
	m_stmt->m_aGroup.push_back(t);
}
//---------------------------------------------------------------------------
void
having (CTerm * pTerm)
{
	assert (m_stmt);
	assert (pTerm);
	m_stmt->m_pHaving = pTerm;
}
//---------------------------------------------------------------------------
void
order (CTerm * t, int head)
{
	assert (m_stmt);
	t = m_stmt->result(t);
	if (head)
		t = m_stmt->unary(head,t);
	m_stmt->m_aOrder.push_back(t);
}
//---------------------------------------------------------------------------
void
distinct (bool bSwitch)
{
	assert (m_stmt);
	m_stmt->m_bDistinct = bSwitch;
}
//---------------------------------------------------------------------------
string
exec (const char * szCmd)
{
	assert (m_stmt);
	return m_stmt->exec (szCmd);
}
//---------------------------------------------------------------------------
CTerm *
param()
{
	assert (m_stmt);
	return m_stmt->param();
}
//---------------------------------------------------------------------------
CTerm *
termlist(CTerm* t1, CTerm* t2)
{
	CFunction * pFunc = dynamic_cast<CFunction*>(t1);
	if (!pFunc)
		return func (lNULL, t1, t2, 0);
	pFunc->append (t2);
	return pFunc;
}
//---------------------------------------------------------------------------
CTermList*
termlist(CTermList* pList, CTerm * pTerm)
{
	if (!pList)
		pList = new CTermList;
	pList->push_back (pTerm);
	return pList;
}
//---------------------------------------------------------------------------
CNameList*
namelist(CNameList* pList, const char * pName)
{
	if (!pList)
		pList = new CNameList;
	pList->push_back (pName);
	return pList;
}
//---------------------------------------------------------------------------
void
update (const char * szTable, CTerm* pList)
{
	assert (szTable);
	assert (pList);
}
//---------------------------------------------------------------------------
void
crosstab_query (CTerm * pData, CTerm * pCols, CFunction * pList)
{
}
//---------------------------------------------------------------------------
} // namespace

