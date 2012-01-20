/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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
     lAVG = 264,
     lSUM = 265,
     lCOUNT = 266,
     lMIN = 267,
     lMAX = 268,
     lFIRST = 269,
     lLAST = 270,
     lSELECT = 271,
     lDISTINCT = 272,
     lALL = 273,
     lAS = 274,
     lFROM = 275,
     lWHERE = 276,
     lGROUP = 277,
     lBY = 278,
     lHAVING = 279,
     lJOIN = 280,
     lON = 281,
     lLEFT = 282,
     lRIGHT = 283,
     lFULL = 284,
     lINNER = 285,
     lOUTER = 286,
     lORDER = 287,
     lASC = 288,
     lDESC = 289,
     lOJ = 290,
     lDATE = 291,
     lTIME = 292,
     lTIMESTAMP = 293,
     lGUID = 294,
     lESCAPE = 295,
     lCALL = 296,
     lFN = 297,
     lABS = 298,
     lSIGN = 299,
     lMOD = 300,
     lDIV = 301,
     lEXP = 302,
     lLOG = 303,
     lSQRT = 304,
     lFMT = 305,
     lNULL = 306,
     lIS = 307,
     lLIKE = 308,
     lIN = 309,
     lBETWEEN = 310,
     lINSERT = 311,
     lINTO = 312,
     lVALUES = 313,
     lUPDATE = 314,
     lSET = 315,
     lDELETE = 316,
     lNEQ = 317,
     lLEQ = 318,
     lGEQ = 319,
     lLEN = 320,
     lTRANSFORM = 321,
     lPIVOT = 322,
     lOR = 323,
     lAND = 324,
     lNOT = 325,
     lUMINUS = 326
   };
#endif
/* Tokens.  */
#define lCMD 258
#define lNAME 259
#define lSTRING 260
#define lNUMBER 261
#define lINTEGER 262
#define lBOOLEAN 263
#define lAVG 264
#define lSUM 265
#define lCOUNT 266
#define lMIN 267
#define lMAX 268
#define lFIRST 269
#define lLAST 270
#define lSELECT 271
#define lDISTINCT 272
#define lALL 273
#define lAS 274
#define lFROM 275
#define lWHERE 276
#define lGROUP 277
#define lBY 278
#define lHAVING 279
#define lJOIN 280
#define lON 281
#define lLEFT 282
#define lRIGHT 283
#define lFULL 284
#define lINNER 285
#define lOUTER 286
#define lORDER 287
#define lASC 288
#define lDESC 289
#define lOJ 290
#define lDATE 291
#define lTIME 292
#define lTIMESTAMP 293
#define lGUID 294
#define lESCAPE 295
#define lCALL 296
#define lFN 297
#define lABS 298
#define lSIGN 299
#define lMOD 300
#define lDIV 301
#define lEXP 302
#define lLOG 303
#define lSQRT 304
#define lFMT 305
#define lNULL 306
#define lIS 307
#define lLIKE 308
#define lIN 309
#define lBETWEEN 310
#define lINSERT 311
#define lINTO 312
#define lVALUES 313
#define lUPDATE 314
#define lSET 315
#define lDELETE 316
#define lNEQ 317
#define lLEQ 318
#define lGEQ 319
#define lLEN 320
#define lTRANSFORM 321
#define lPIVOT 322
#define lOR 323
#define lAND 324
#define lNOT 325
#define lUMINUS 326




/* Copy the first part of user declarations.  */
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
CTerm* strval (const char * szValue);
CTerm* time (const char * szValue);
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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 71 ".\\yac.y"
typedef union YYSTYPE {
	double number;
	const char * string;
	long int integer;
	int token;
	sqlp::CTerm * term;
	sqlp::CTable * table;
	sqlp::CNameList * name;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 306 "yac.cpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 318 "yac.cpp"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

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

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   618

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  33
/* YYNRULES -- Number of rules. */
#define YYNRULES  127
/* YYNRULES -- Number of states. */
#define YYNSTATES  303

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   326

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      78,    79,    73,    71,    76,    72,    77,    74,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      83,    82,    84,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    80,     2,    81,     2,     2,     2,     2,
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
      65,    66,    67,    68,    69,    70,    75
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
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
     323,   325,   327,   329,   333,   337,   341,   345,   348,   352,
     357,   362,   369,   376,   381,   386,   391,   398,   405,   410,
     415,   420,   422,   426,   433,   440,   445,   447,   449,   453,
     455,   459,   463,   465,   469,   475,   486,   492
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      87,     0,    -1,    87,    88,    -1,    -1,    89,    -1,   116,
      -1,   115,    -1,   117,    -1,   118,    -1,    50,   110,     5,
      -1,     3,    -1,     1,    -1,    -1,    -1,    16,    90,    92,
      93,    20,    98,    91,   100,   101,   103,   104,    -1,    -1,
      18,    -1,    17,    -1,    94,    -1,    93,    76,    94,    -1,
      73,    -1,   110,    -1,   110,    19,     4,    -1,   110,    19,
       5,    -1,     4,    77,    73,    -1,    99,    -1,    96,    -1,
      78,    96,    79,    -1,    99,    30,    25,    95,    26,   108,
      -1,    99,    27,    97,    95,    26,   108,    -1,    99,    28,
      97,    95,    26,   108,    -1,    99,    29,    97,    95,    26,
     108,    -1,    31,    25,    -1,    25,    -1,    99,    -1,    98,
      76,    99,    -1,    96,    -1,    80,    35,    96,    81,    -1,
       4,    -1,     4,     4,    -1,     4,    19,     4,    -1,     4,
      77,     4,    -1,     4,    77,     4,     4,    -1,     4,    77,
       4,    19,     4,    -1,     4,    77,     4,    77,     4,    -1,
       4,    77,     4,    77,     4,     4,    -1,     4,    77,     4,
      77,     4,    19,     4,    -1,    -1,    21,   108,    -1,    -1,
      22,    23,   102,    -1,   110,    -1,   102,    76,   110,    -1,
      -1,    24,   108,    -1,    -1,    32,    23,   105,    -1,   106,
      -1,   105,    76,   106,    -1,   110,    -1,   110,    33,    -1,
     110,    34,    -1,     4,    -1,     4,    77,     4,    -1,     4,
      77,     4,    77,     4,    -1,   108,    69,   108,    -1,   108,
      68,   108,    -1,   110,    82,   110,    -1,   110,    83,   110,
      -1,   110,    84,   110,    -1,   110,    62,   110,    -1,   110,
      64,   110,    -1,   110,    63,   110,    -1,   110,    53,     5,
      -1,   110,    52,    70,    51,    -1,   110,    52,    51,    -1,
      70,   108,    -1,    78,   108,    79,    -1,   110,    54,    78,
     111,    79,    -1,   110,    55,   110,    69,   110,    -1,    11,
      78,    73,    79,    -1,    11,    78,   110,    79,    -1,     9,
      78,   110,    79,    -1,    10,    78,   110,    79,    -1,    12,
      78,   110,    79,    -1,    13,    78,   110,    79,    -1,    14,
      78,   110,    79,    -1,    15,    78,   110,    79,    -1,     6,
      -1,     7,    -1,     8,    -1,     5,    -1,   107,    -1,   110,
      71,   110,    -1,   110,    72,   110,    -1,   110,    73,   110,
      -1,   110,    74,   110,    -1,    72,   110,    -1,    78,   110,
      79,    -1,    43,    78,   110,    79,    -1,    44,    78,   110,
      79,    -1,    45,    78,   110,    76,   110,    79,    -1,    46,
      78,   110,    76,   110,    79,    -1,    47,    78,   110,    79,
      -1,    48,    78,   110,    79,    -1,    49,    78,   110,    79,
      -1,    47,    78,   110,    76,   110,    79,    -1,    48,    78,
     110,    76,   110,    79,    -1,    80,    38,     5,    81,    -1,
      80,    36,     5,    81,    -1,    80,    37,     5,    81,    -1,
      85,    -1,    78,    89,    79,    -1,    12,    78,   110,    76,
     111,    79,    -1,    13,    78,   110,    76,   111,    79,    -1,
      65,    78,   110,    79,    -1,   109,    -1,   110,    -1,   111,
      76,   110,    -1,     4,    -1,   112,    76,     4,    -1,     4,
      82,   110,    -1,   113,    -1,   114,    76,   113,    -1,    59,
       4,    60,   114,   100,    -1,    56,    57,     4,    78,   112,
      79,    58,    78,   111,    79,    -1,    61,    73,    20,     4,
     100,    -1,    66,   109,    89,    67,   110,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   138,   138,   139,   143,   144,   145,   146,   147,   148,
     149,   150,   154,   155,   154,   160,   161,   162,   166,   167,
     168,   172,   173,   174,   175,   179,   180,   181,   185,   186,
     187,   188,   192,   193,   197,   198,   199,   200,   204,   205,
     206,   207,   208,   209,   210,   211,   212,   215,   217,   220,
     222,   226,   227,   230,   232,   235,   237,   241,   242,   246,
     247,   248,   252,   253,   254,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     276,   277,   278,   279,   280,   281,   282,   283,   287,   288,
     289,   290,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   300,   301,   302,   303,   304,   305,   306,   307,   308,
     309,   310,   311,   312,   313,   314,   315,   319,   320,   324,
     325,   329,   333,   334,   338,   342,   346,   350
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "lCMD", "lNAME", "lSTRING", "lNUMBER",
  "lINTEGER", "lBOOLEAN", "lAVG", "lSUM", "lCOUNT", "lMIN", "lMAX",
  "lFIRST", "lLAST", "lSELECT", "lDISTINCT", "lALL", "lAS", "lFROM",
  "lWHERE", "lGROUP", "lBY", "lHAVING", "lJOIN", "lON", "lLEFT", "lRIGHT",
  "lFULL", "lINNER", "lOUTER", "lORDER", "lASC", "lDESC", "lOJ", "lDATE",
  "lTIME", "lTIMESTAMP", "lGUID", "lESCAPE", "lCALL", "lFN", "lABS",
  "lSIGN", "lMOD", "lDIV", "lEXP", "lLOG", "lSQRT", "lFMT", "lNULL", "lIS",
  "lLIKE", "lIN", "lBETWEEN", "lINSERT", "lINTO", "lVALUES", "lUPDATE",
  "lSET", "lDELETE", "lNEQ", "lLEQ", "lGEQ", "lLEN", "lTRANSFORM",
  "lPIVOT", "lOR", "lAND", "lNOT", "'+'", "'-'", "'*'", "'/'", "lUMINUS",
  "','", "'.'", "'('", "')'", "'{'", "'}'", "'='", "'<'", "'>'", "'?'",
  "$accept", "y_lines", "y_line", "y_select", "@1", "@2", "y_select_opt",
  "y_select_list", "y_select_item", "y_join_table", "y_join",
  "y_outer_join", "y_table_list", "y_table", "y_where", "y_group",
  "y_group_list", "y_having", "y_order", "y_order_list", "y_order_term",
  "y_column", "y_bool", "y_aggr", "y_term", "y_term_list", "y_name_list",
  "y_update_term", "y_update_list", "y_update", "y_insert", "y_delete",
  "y_crosstab_query", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,    43,    45,    42,    47,   326,    44,    46,    40,    41,
     123,   125,    61,    60,    62,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    86,    87,    87,    88,    88,    88,    88,    88,    88,
      88,    88,    90,    91,    89,    92,    92,    92,    93,    93,
      93,    94,    94,    94,    94,    95,    95,    95,    96,    96,
      96,    96,    97,    97,    98,    98,    98,    98,    99,    99,
      99,    99,    99,    99,    99,    99,    99,   100,   100,   101,
     101,   102,   102,   103,   103,   104,   104,   105,   105,   106,
     106,   106,   107,   107,   107,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   108,   108,   108,   108,
     109,   109,   109,   109,   109,   109,   109,   109,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   111,   111,   112,
     112,   113,   114,   114,   115,   116,   117,   118
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
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
       1,     1,     1,     3,     3,     3,     3,     2,     3,     4,
       4,     6,     6,     4,     4,     4,     6,     6,     4,     4,
       4,     1,     3,     6,     6,     4,     1,     1,     3,     1,
       3,     3,     1,     3,     5,    10,     5,     5
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       3,     0,     1,    11,    10,    12,     0,     0,     0,     0,
       0,     2,     4,     6,     5,     7,     8,    15,    62,    91,
      88,    89,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   111,    92,   116,     0,     0,     0,     0,     0,     0,
       0,    17,    16,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     9,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    20,     0,
      18,    21,    63,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   112,
      98,     0,     0,     0,    93,    94,    95,    96,     0,     0,
     122,    47,    47,     0,     0,     0,     0,     0,     0,     0,
       0,    82,    83,    80,    81,     0,    84,     0,    85,    86,
      87,    99,   100,     0,     0,     0,   103,     0,   104,   105,
     115,   109,   110,   108,   119,     0,     0,     0,     0,   124,
     126,   127,    24,    38,     0,    36,    13,    34,    19,    22,
      23,    64,   117,     0,     0,     0,     0,     0,     0,     0,
       0,   121,     0,     0,    48,     0,   123,    39,     0,     0,
       0,     0,    47,     0,     0,     0,     0,     0,   113,   114,
     101,   102,   106,   107,   120,     0,    76,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    40,    41,     0,     0,    35,    49,    33,     0,     0,
       0,     0,     0,   118,     0,    77,    66,    65,    75,     0,
      73,     0,     0,    70,    72,    71,    67,    68,    69,    42,
       0,     0,    37,     0,    53,    32,     0,     0,    26,    25,
       0,     0,     0,     0,    74,     0,     0,    43,    44,     0,
       0,    55,     0,     0,     0,     0,     0,   125,    78,    79,
      45,     0,    50,    51,    54,     0,    14,    27,    29,    30,
      31,    28,    46,     0,     0,    52,    56,    57,    59,     0,
      60,    61,    58
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     1,    11,    71,    17,   192,    53,    89,    90,   257,
     258,   229,   166,   259,   159,   254,   282,   271,   286,   296,
     297,    42,   184,    43,   185,   173,   155,   120,   121,    13,
      14,    15,    16
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -174
static const short int yypact[] =
{
    -174,    67,  -174,  -174,  -174,  -174,   423,   -13,    72,     9,
     192,  -174,  -174,  -174,  -174,  -174,  -174,    13,    21,  -174,
    -174,  -174,  -174,    47,    62,    94,   109,   110,   137,   144,
     149,   152,   154,   155,   156,   181,   196,   211,   423,   233,
     -14,  -174,  -174,  -174,    15,    92,    60,   169,   214,   221,
     208,  -174,  -174,   315,   222,   423,   423,   329,   423,   423,
     423,   423,   423,   423,   423,   423,   423,   423,   423,   423,
    -174,   182,   107,   270,   285,   295,  -174,   423,   423,   423,
     423,   225,   297,   300,   423,   423,   243,   238,  -174,    -7,
    -174,    19,   239,   111,   146,   252,   179,   -31,    36,   183,
     193,   212,   223,   197,   294,    75,    97,   235,   274,  -174,
    -174,   231,   236,   251,   -39,   -39,  -174,  -174,   350,   273,
    -174,    18,   335,   278,   310,   423,     4,     0,   505,   148,
     365,  -174,  -174,  -174,  -174,   423,  -174,   423,  -174,  -174,
    -174,  -174,  -174,   423,   423,   423,  -174,   423,  -174,  -174,
    -174,  -174,  -174,  -174,  -174,   -60,   423,   411,   297,  -174,
    -174,   390,  -174,     7,   336,  -174,   303,   470,  -174,  -174,
    -174,  -174,   390,    35,    37,   325,   368,   372,   406,   381,
     328,   390,   411,   151,   106,   473,  -174,  -174,   386,   387,
     388,   388,   335,   -10,   -10,   -10,   378,   423,  -174,  -174,
    -174,  -174,  -174,  -174,  -174,   327,  -174,   -51,   534,   411,
     411,   -45,   401,   370,   423,   423,   423,   423,   423,   423,
     423,  -174,     8,   369,   470,  -174,   427,  -174,   428,     3,
       3,     3,     3,   390,   423,  -174,   383,  -174,  -174,   414,
    -174,   423,   339,   390,   390,   390,   390,   390,   390,  -174,
     469,   471,  -174,   451,   458,  -174,   388,   460,  -174,   470,
     461,   464,   466,    42,  -174,    48,   423,  -174,    10,   423,
     411,   452,   415,   411,   411,   411,   411,  -174,  -174,   390,
    -174,   489,   426,   390,   106,   481,  -174,  -174,   106,   106,
     106,   106,  -174,   423,   423,   390,   429,  -174,    71,   423,
    -174,  -174,  -174
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -174,  -174,  -174,    45,  -174,  -174,  -174,  -174,   379,   -96,
    -124,    -2,  -174,  -126,  -117,  -174,  -174,  -174,  -174,  -174,
     207,  -174,  -173,   511,    -6,  -135,  -174,   364,  -174,  -174,
    -174,  -174,  -174
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short int yytable[] =
{
      44,   167,   174,   165,   163,   160,   238,   163,    92,   206,
     207,   187,   249,   127,   280,   227,   179,   209,   210,   180,
      76,   228,    73,    74,    75,   239,   188,   250,   235,   281,
      51,    52,    70,    72,    79,    80,   236,   237,   129,   157,
      77,    78,    79,    80,    45,   135,    12,    91,   136,    93,
      94,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   224,   225,   223,     2,     3,   128,
       4,   114,   115,   116,   117,   226,    46,   162,   123,   124,
     164,   256,    47,     5,   189,   251,    77,    78,    79,    80,
      77,    78,    79,    80,   158,    86,    81,   284,    54,   263,
     288,   289,   290,   291,   300,   301,   265,    77,    78,    79,
      80,   197,   137,   197,   198,   138,   199,     6,   197,   161,
      82,   277,    91,     7,   197,    55,     8,   278,     9,   172,
     224,   172,   272,    10,   260,   261,   262,   175,   176,   177,
      56,   178,    77,    78,    79,    80,    77,    78,    79,    80,
     181,   145,   169,   170,   146,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,     5,    77,    78,
      79,    80,    57,   147,   209,   210,   148,   208,    77,    78,
      79,    80,    77,    78,    79,    80,   110,    58,    59,    83,
     131,   233,   230,   231,    30,    31,    32,    33,    34,    35,
      36,    23,    24,    25,    48,    49,    28,    29,   242,   243,
     244,   245,   246,   247,   248,    60,    37,    77,    78,    79,
      80,   182,    61,    38,     5,   132,    92,    62,   172,   183,
      63,    40,    64,    65,    66,   172,    41,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,     5,
      77,    78,    79,    80,    77,    78,    79,    80,   134,    67,
     279,   109,   139,   283,    77,    78,    79,    80,    77,    78,
      79,    80,   140,   143,    68,   111,    30,    31,    32,    33,
      34,    35,    36,    77,    78,    79,    80,   295,   298,    69,
     112,   141,    84,   298,    77,    78,    79,    80,    37,    85,
     113,   119,   142,   118,   122,    38,    77,    78,    79,    80,
     125,    39,   151,    40,   149,   126,   130,   152,    41,    87,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,   133,   153,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    77,    78,    79,    80,    77,
      78,    79,    80,   150,   154,   156,   157,   136,    30,    31,
      32,    33,    34,    35,    36,    77,    78,    79,    80,   171,
     144,   190,    30,    31,    32,    33,    34,    35,    36,   191,
      37,    77,    78,    79,    80,   204,   205,    38,    88,   138,
     221,   222,   163,    39,    37,    40,    77,    78,    79,    80,
      41,    38,    95,   232,   200,   234,   240,    39,   266,    40,
      77,    78,    79,    80,    41,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    77,
      78,    79,    80,    77,    78,    79,    80,   201,   241,   253,
     252,   202,   210,   255,    30,    31,    32,    33,    34,    35,
      36,    77,    78,    79,    80,   264,    30,    31,    32,    33,
      34,    35,    36,   267,   269,   268,    37,    77,    78,    79,
      80,   182,   270,    38,   285,   203,   273,   274,    37,   183,
     275,    40,   276,   292,   287,    38,    41,   193,   194,   195,
     196,    39,   293,    40,   294,   299,   302,   168,    41,    87,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    50,   186,     0,     0,   211,   212,   213,   214,     0,
       0,     0,     0,     0,     0,   215,   216,   217,     0,     0,
       0,     0,     0,     0,    77,    78,    79,    80,    30,    31,
      32,    33,    34,    35,    36,   218,   219,   220,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    38,     0,     0,
       0,     0,     0,    39,     0,    40,   211,   212,   213,   214,
      41,     0,     0,     0,     0,     0,   215,   216,   217,     0,
       0,     0,     0,     0,     0,    77,    78,    79,    80,     0,
       0,     0,     0,   110,     0,     0,   218,   219,   220
};

static const short int yycheck[] =
{
       6,   127,   137,   127,     4,   122,    51,     4,     4,   182,
     183,     4,     4,    20,     4,    25,    76,    68,    69,    79,
       5,    31,    36,    37,    38,    70,    19,    19,    79,    19,
      17,    18,    38,    39,    73,    74,   209,   210,    19,    21,
      71,    72,    73,    74,    57,    76,     1,    53,    79,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,   190,   191,   190,     0,     1,    76,
       3,    77,    78,    79,    80,   192,     4,    73,    84,    85,
      80,    78,    73,    16,    77,    77,    71,    72,    73,    74,
      71,    72,    73,    74,    76,    50,     4,   270,    77,   234,
     273,   274,   275,   276,    33,    34,   241,    71,    72,    73,
      74,    76,    76,    76,    79,    79,    79,    50,    76,   125,
      60,    79,   128,    56,    76,    78,    59,    79,    61,   135,
     256,   137,   256,    66,   230,   231,   232,   143,   144,   145,
      78,   147,    71,    72,    73,    74,    71,    72,    73,    74,
     156,    76,     4,     5,    79,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    71,    72,
      73,    74,    78,    76,    68,    69,    79,   183,    71,    72,
      73,    74,    71,    72,    73,    74,    79,    78,    78,    20,
      79,   197,   194,   195,    43,    44,    45,    46,    47,    48,
      49,     9,    10,    11,    12,    13,    14,    15,   214,   215,
     216,   217,   218,   219,   220,    78,    65,    71,    72,    73,
      74,    70,    78,    72,    16,    79,     4,    78,   234,    78,
      78,    80,    78,    78,    78,   241,    85,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      71,    72,    73,    74,    71,    72,    73,    74,    79,    78,
     266,    79,    79,   269,    71,    72,    73,    74,    71,    72,
      73,    74,    79,    76,    78,     5,    43,    44,    45,    46,
      47,    48,    49,    71,    72,    73,    74,   293,   294,    78,
       5,    79,    78,   299,    71,    72,    73,    74,    65,    78,
       5,     4,    79,    78,     4,    72,    71,    72,    73,    74,
      67,    78,    81,    80,    79,    77,    77,    81,    85,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    79,    81,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    71,    72,    73,    74,    71,
      72,    73,    74,    79,     4,    82,    21,    79,    43,    44,
      45,    46,    47,    48,    49,    71,    72,    73,    74,     4,
      76,    35,    43,    44,    45,    46,    47,    48,    49,    76,
      65,    71,    72,    73,    74,     4,    58,    72,    73,    79,
       4,     4,     4,    78,    65,    80,    71,    72,    73,    74,
      85,    72,    73,    25,    79,    78,     5,    78,    69,    80,
      71,    72,    73,    74,    85,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    71,
      72,    73,    74,    71,    72,    73,    74,    79,    78,    22,
      81,    79,    69,    25,    43,    44,    45,    46,    47,    48,
      49,    71,    72,    73,    74,    51,    43,    44,    45,    46,
      47,    48,    49,     4,    23,     4,    65,    71,    72,    73,
      74,    70,    24,    72,    32,    79,    26,    26,    65,    78,
      26,    80,    26,     4,    79,    72,    85,    27,    28,    29,
      30,    78,    76,    80,    23,    76,   299,   128,    85,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    10,   158,    -1,    -1,    52,    53,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    43,    44,
      45,    46,    47,    48,    49,    82,    83,    84,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    52,    53,    54,    55,
      85,    -1,    -1,    -1,    -1,    -1,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      -1,    -1,    -1,    79,    -1,    -1,    82,    83,    84
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    87,     0,     1,     3,    16,    50,    56,    59,    61,
      66,    88,    89,   115,   116,   117,   118,    90,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      43,    44,    45,    46,    47,    48,    49,    65,    72,    78,
      80,    85,   107,   109,   110,    57,     4,    73,    12,    13,
     109,    17,    18,    92,    77,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
     110,    89,   110,    36,    37,    38,     5,    71,    72,    73,
      74,     4,    60,    20,    78,    78,    89,     4,    73,    93,
      94,   110,     4,   110,   110,    73,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,    79,
      79,     5,     5,     5,   110,   110,   110,   110,    78,     4,
     113,   114,     4,   110,   110,    67,    77,    20,    76,    19,
      77,    79,    79,    79,    79,    76,    79,    76,    79,    79,
      79,    79,    79,    76,    76,    76,    79,    76,    79,    79,
      79,    81,    81,    81,     4,   112,    82,    21,    76,   100,
     100,   110,    73,     4,    80,    96,    98,    99,    94,     4,
       5,     4,   110,   111,   111,   110,   110,   110,   110,    76,
      79,   110,    70,    78,   108,   110,   113,     4,    19,    77,
      35,    76,    91,    27,    28,    29,    30,    76,    79,    79,
      79,    79,    79,    79,     4,    58,   108,   108,   110,    68,
      69,    52,    53,    54,    55,    62,    63,    64,    82,    83,
      84,     4,     4,    96,    99,    99,   100,    25,    31,    97,
      97,    97,    25,   110,    78,    79,   108,   108,    51,    70,
       5,    78,   110,   110,   110,   110,   110,   110,   110,     4,
      19,    77,    81,    22,   101,    25,    78,    95,    96,    99,
      95,    95,    95,   111,    51,   111,    69,     4,     4,    23,
      24,   103,    96,    26,    26,    26,    26,    79,    79,   110,
       4,    19,   102,   110,   108,    32,   104,    79,   108,   108,
     108,   108,     4,    76,    23,   110,   105,   106,   110,    76,
      33,    34,   106
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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
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
    while (0)
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
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
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
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
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
      size_t yyn = 0;
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

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
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
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

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
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


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
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 143 ".\\yac.y"
    { printf("SQL: ok\n"); ;}
    break;

  case 6:
#line 145 ".\\yac.y"
    { printf("UPDATE: SQL-Syntax ok.\n"); ;}
    break;

  case 9:
#line 148 ".\\yac.y"
    { m_stmt->setFormat ((yyvsp[-1].term), (yyvsp[0].string)); exec("formats");;}
    break;

  case 10:
#line 149 ".\\yac.y"
    { printf("%s\n", exec ((yyvsp[0].string)).c_str()); ;}
    break;

  case 11:
#line 150 ".\\yac.y"
    { yyexit(); YYABORT; ;}
    break;

  case 12:
#line 154 ".\\yac.y"
    { m_stmt->clear(); ;}
    break;

  case 13:
#line 155 ".\\yac.y"
    { m_stmt->select(); ;}
    break;

  case 14:
#line 156 ".\\yac.y"
    { m_stmt->prepare(); ;}
    break;

  case 15:
#line 160 ".\\yac.y"
    { distinct (false); ;}
    break;

  case 16:
#line 161 ".\\yac.y"
    { distinct (false); ;}
    break;

  case 17:
#line 162 ".\\yac.y"
    { distinct (true); ;}
    break;

  case 20:
#line 168 ".\\yac.y"
    { select (column(0,0,0)); ;}
    break;

  case 21:
#line 172 ".\\yac.y"
    { select ((yyvsp[0].term)); ;}
    break;

  case 22:
#line 173 ".\\yac.y"
    { select ((yyvsp[-2].term), (yyvsp[0].string)); ;}
    break;

  case 23:
#line 174 ".\\yac.y"
    { select ((yyvsp[-2].term), (yyvsp[0].string)); ;}
    break;

  case 24:
#line 175 ".\\yac.y"
    { select (column (0, (yyvsp[-2].string), 0)); ;}
    break;

  case 25:
#line 179 ".\\yac.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 26:
#line 180 ".\\yac.y"
    { (yyval.string) = (yyvsp[0].string); ;}
    break;

  case 27:
#line 181 ".\\yac.y"
    { (yyval.string) = (yyvsp[-1].string); ;}
    break;

  case 28:
#line 185 ".\\yac.y"
    { join (lINNER, (yyvsp[-5].string), (yyvsp[-2].string), (yyvsp[0].term)); ;}
    break;

  case 29:
#line 186 ".\\yac.y"
    { join (lLEFT,  (yyvsp[-5].string), (yyvsp[-2].string), (yyvsp[0].term)); ;}
    break;

  case 30:
#line 187 ".\\yac.y"
    { join (lRIGHT, (yyvsp[-5].string), (yyvsp[-2].string), (yyvsp[0].term)); ;}
    break;

  case 31:
#line 188 ".\\yac.y"
    { join (lFULL,  (yyvsp[-5].string), (yyvsp[-2].string), (yyvsp[0].term)); ;}
    break;

  case 34:
#line 197 ".\\yac.y"
    {;}
    break;

  case 36:
#line 199 ".\\yac.y"
    {;}
    break;

  case 37:
#line 200 ".\\yac.y"
    {;}
    break;

  case 38:
#line 204 ".\\yac.y"
    { (yyval.string) = table ("", "", (yyvsp[0].string),  0); ;}
    break;

  case 39:
#line 205 ".\\yac.y"
    { (yyval.string) = table ("", "", (yyvsp[-1].string), (yyvsp[0].string)); ;}
    break;

  case 40:
#line 206 ".\\yac.y"
    { (yyval.string) = table ("", "", (yyvsp[-2].string), (yyvsp[0].string)); ;}
    break;

  case 41:
#line 207 ".\\yac.y"
    { (yyval.string) = table ("", (yyvsp[-2].string), (yyvsp[0].string),  0); ;}
    break;

  case 42:
#line 208 ".\\yac.y"
    { (yyval.string) = table ("", (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[0].string)); ;}
    break;

  case 43:
#line 209 ".\\yac.y"
    { (yyval.string) = table ("", (yyvsp[-4].string), (yyvsp[-2].string), (yyvsp[0].string)); ;}
    break;

  case 44:
#line 210 ".\\yac.y"
    { (yyval.string) = table ((yyvsp[-4].string), (yyvsp[-2].string), (yyvsp[0].string),  0); ;}
    break;

  case 45:
#line 211 ".\\yac.y"
    { (yyval.string) = table ((yyvsp[-5].string), (yyvsp[-3].string), (yyvsp[-1].string), (yyvsp[0].string)); ;}
    break;

  case 46:
#line 212 ".\\yac.y"
    { (yyval.string) = table ((yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string), (yyvsp[0].string)); ;}
    break;

  case 48:
#line 217 ".\\yac.y"
    { where ((yyvsp[0].term)); ;}
    break;

  case 51:
#line 226 ".\\yac.y"
    { group((yyvsp[0].term)); ;}
    break;

  case 52:
#line 227 ".\\yac.y"
    { group((yyvsp[0].term)); ;}
    break;

  case 54:
#line 232 ".\\yac.y"
    { having((yyvsp[0].term)); ;}
    break;

  case 59:
#line 246 ".\\yac.y"
    { order((yyvsp[0].term)); ;}
    break;

  case 60:
#line 247 ".\\yac.y"
    { order((yyvsp[-1].term),lASC); ;}
    break;

  case 61:
#line 248 ".\\yac.y"
    { order((yyvsp[-1].term),lDESC); ;}
    break;

  case 62:
#line 252 ".\\yac.y"
    { (yyval.term) = column ( 0,  0, (yyvsp[0].string)); ;}
    break;

  case 63:
#line 253 ".\\yac.y"
    { (yyval.term) = column ( 0, (yyvsp[-2].string), (yyvsp[0].string)); ;}
    break;

  case 64:
#line 254 ".\\yac.y"
    { (yyval.term) = column ((yyvsp[-4].string), (yyvsp[-2].string), (yyvsp[0].string)); ;}
    break;

  case 65:
#line 258 ".\\yac.y"
    { (yyval.term) = func(lAND, (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 66:
#line 259 ".\\yac.y"
    { (yyval.term) = func(lOR, (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 67:
#line 260 ".\\yac.y"
    { (yyval.term) = func('=', (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 68:
#line 261 ".\\yac.y"
    { (yyval.term) = func('<', (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 69:
#line 262 ".\\yac.y"
    { (yyval.term) = func('>', (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 70:
#line 263 ".\\yac.y"
    { (yyval.term) = func(lNEQ, (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 71:
#line 264 ".\\yac.y"
    { (yyval.term) = func(lGEQ, (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 72:
#line 265 ".\\yac.y"
    { (yyval.term) = func(lLEQ, (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 73:
#line 266 ".\\yac.y"
    { (yyval.term) = func(lLIKE, (yyvsp[-2].term), strval((yyvsp[0].string)), 0); ;}
    break;

  case 74:
#line 267 ".\\yac.y"
    { (yyval.term) = unary(lNOT, unary(lNULL, (yyvsp[-3].term))); ;}
    break;

  case 75:
#line 268 ".\\yac.y"
    { (yyval.term) = unary(lNULL, (yyvsp[-2].term)); ;}
    break;

  case 76:
#line 269 ".\\yac.y"
    { (yyval.term) = unary(lNOT, (yyvsp[0].term)); ;}
    break;

  case 77:
#line 270 ".\\yac.y"
    { (yyval.term) = (yyvsp[-1].term); ;}
    break;

  case 78:
#line 271 ".\\yac.y"
    {(yyval.term) = func(lIN, (yyvsp[-4].term), (yyvsp[-1].term), 0); ;}
    break;

  case 79:
#line 272 ".\\yac.y"
    {(yyval.term) = func(lBETWEEN, (yyvsp[-4].term), (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 80:
#line 276 ".\\yac.y"
    { (yyval.term) = aggregate (lCOUNT, (CTerm*)0); ;}
    break;

  case 81:
#line 277 ".\\yac.y"
    { (yyval.term) = aggregate (lCOUNT, (yyvsp[-1].term)); ;}
    break;

  case 82:
#line 278 ".\\yac.y"
    { (yyval.term) = aggregate (lAVG, (yyvsp[-1].term)); ;}
    break;

  case 83:
#line 279 ".\\yac.y"
    { (yyval.term) = aggregate (lSUM, (yyvsp[-1].term)); ;}
    break;

  case 84:
#line 280 ".\\yac.y"
    { (yyval.term) = aggregate (lMIN, (yyvsp[-1].term)); ;}
    break;

  case 85:
#line 281 ".\\yac.y"
    { (yyval.term) = aggregate (lMAX, (yyvsp[-1].term)); ;}
    break;

  case 86:
#line 282 ".\\yac.y"
    { (yyval.term) = aggregate (lFIRST, (yyvsp[-1].term)); ;}
    break;

  case 87:
#line 283 ".\\yac.y"
    { (yyval.term) = aggregate (lLAST, (yyvsp[-1].term)); ;}
    break;

  case 88:
#line 287 ".\\yac.y"
    { (yyval.term) = number((yyvsp[0].number)); ;}
    break;

  case 89:
#line 288 ".\\yac.y"
    { (yyval.term) = number((yyvsp[0].number)); ;}
    break;

  case 90:
#line 289 ".\\yac.y"
    { (yyval.term) = number((yyvsp[0].number)); ;}
    break;

  case 91:
#line 290 ".\\yac.y"
    { (yyval.term) = strval((yyvsp[0].string)); ;}
    break;

  case 92:
#line 291 ".\\yac.y"
    { (yyval.term) = (yyvsp[0].term); ;}
    break;

  case 93:
#line 292 ".\\yac.y"
    { (yyval.term) = func('+', (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 94:
#line 293 ".\\yac.y"
    { (yyval.term) = func('+', (yyvsp[-2].term), unary('-', (yyvsp[0].term)), 0); ;}
    break;

  case 95:
#line 294 ".\\yac.y"
    { (yyval.term) = func('*', (yyvsp[-2].term), (yyvsp[0].term), 0); ;}
    break;

  case 96:
#line 295 ".\\yac.y"
    { (yyval.term) = func('*', (yyvsp[-2].term), unary('/', (yyvsp[0].term)), 0); ;}
    break;

  case 97:
#line 296 ".\\yac.y"
    { (yyval.term) = unary('-', (yyvsp[0].term)); ;}
    break;

  case 98:
#line 297 ".\\yac.y"
    { (yyval.term) = (yyvsp[-1].term); ;}
    break;

  case 99:
#line 298 ".\\yac.y"
    { (yyval.term) = unary(lABS, (yyvsp[-1].term)); ;}
    break;

  case 100:
#line 299 ".\\yac.y"
    { (yyval.term) = unary(lSIGN, (yyvsp[-1].term)); ;}
    break;

  case 101:
#line 300 ".\\yac.y"
    { (yyval.term) = func (lMOD, (yyvsp[-3].term), (yyvsp[-1].term), 0); ;}
    break;

  case 102:
#line 301 ".\\yac.y"
    { (yyval.term) = func (lDIV, (yyvsp[-3].term), (yyvsp[-1].term), 0); ;}
    break;

  case 103:
#line 302 ".\\yac.y"
    { (yyval.term) = unary(lEXP, (yyvsp[-1].term)); ;}
    break;

  case 104:
#line 303 ".\\yac.y"
    { (yyval.term) = unary(lLOG, (yyvsp[-1].term)); ;}
    break;

  case 105:
#line 304 ".\\yac.y"
    { (yyval.term) = unary(lSQRT, (yyvsp[-1].term)); ;}
    break;

  case 106:
#line 305 ".\\yac.y"
    { (yyval.term) = func(lEXP, (yyvsp[-3].term), (yyvsp[-1].term), 0); ;}
    break;

  case 107:
#line 306 ".\\yac.y"
    { (yyval.term) = func(lLOG, (yyvsp[-3].term), (yyvsp[-1].term), 0); ;}
    break;

  case 108:
#line 307 ".\\yac.y"
    { (yyval.term) = time((yyvsp[-1].string)); ;}
    break;

  case 109:
#line 308 ".\\yac.y"
    { (yyval.term) = time((yyvsp[-1].string)); ;}
    break;

  case 110:
#line 309 ".\\yac.y"
    { (yyval.term) = time((yyvsp[-1].string)); ;}
    break;

  case 111:
#line 310 ".\\yac.y"
    { (yyval.term) = param(); ;}
    break;

  case 112:
#line 311 ".\\yac.y"
    { (yyval.term) = param(); ;}
    break;

  case 113:
#line 312 ".\\yac.y"
    { (yyval.term) = func(lMIN, (yyvsp[-3].term), (yyvsp[-1].term), 0); ;}
    break;

  case 114:
#line 313 ".\\yac.y"
    { (yyval.term) = func(lMAX, (yyvsp[-3].term), (yyvsp[-1].term), 0); ;}
    break;

  case 115:
#line 314 ".\\yac.y"
    { (yyval.term) = unary (lLEN, (yyvsp[-1].term)); ;}
    break;

  case 117:
#line 319 ".\\yac.y"
    { (yyval.term) = (yyvsp[0].term); ;}
    break;

  case 118:
#line 320 ".\\yac.y"
    { (yyval.term) = termlist((yyvsp[-2].term), (yyvsp[0].term)); ;}
    break;

  case 119:
#line 324 ".\\yac.y"
    { (yyval.name) = namelist(0 , (yyvsp[0].string)); ;}
    break;

  case 120:
#line 325 ".\\yac.y"
    { (yyval.name) = namelist((yyvsp[-2].name), (yyvsp[0].string)); ;}
    break;

  case 121:
#line 329 ".\\yac.y"
    { (yyval.term) = func('=', column(0,0,(yyvsp[-2].string)), (yyvsp[0].term), 0); ;}
    break;

  case 122:
#line 333 ".\\yac.y"
    { (yyval.term) = (yyvsp[0].term); ;}
    break;

  case 123:
#line 334 ".\\yac.y"
    { (yyval.term) = termlist ((yyvsp[-2].term), (yyvsp[0].term)); ;}
    break;

  case 124:
#line 338 ".\\yac.y"
    { update ((yyvsp[-3].string), (yyvsp[-1].term)); ;}
    break;

  case 127:
#line 350 ".\\yac.y"
    { crosstab_query ((yyvsp[-3].term),(yyvsp[0].term), 0); ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 2162 "yac.cpp"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
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
	  int yychecklim = YYLAST - yyn;
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
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
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
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 353 ".\\yac.y"


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
number (double dValue)
{
	assert (m_stmt);
	return m_stmt->value (dValue);
}
//---------------------------------------------------------------------------
CTerm*
number (long int nValue)
{
	assert (m_stmt);
	return m_stmt->value (nValue);
}
//---------------------------------------------------------------------------
CTerm*
strval (const char * szValue)
{
	assert (m_stmt);
	CTerm * pTerm = m_stmt->value(szValue);
	return pTerm;
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
	std::string mist = id (pTerm);
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

