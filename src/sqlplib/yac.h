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


//---------------------------------------------------------------------------
// type - declarations
//---------------------------------------------------------------------------
#include <vector>
namespace sqlp
{

	class CTerm;
	class CTable;
	class CStatement;
	typedef std::vector<const char *> CNameList;
} // namespace
using namespace sqlp;
//---------------------------------------------------------------------------
// YYSTYPE Type
//---------------------------------------------------------------------------
#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 71 ".\\yac.y"
typedef union YYSTYPE {
	double number;
	const char * string;
	long int integer;
	int token;
	time_t datetime;
	sqlp::CTerm * term;
	sqlp::CTable * table;
	sqlp::CNameList * name;
} YYSTYPE;
/* Line 1447 of yacc.c.  */
#line 184 "yac.hpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
//---------------------------------------------------------------------------
// yyfunc - prototypes
//---------------------------------------------------------------------------
int    yylex (void);
int    yyexit();
void   yyerror (const char*);
int    yyinput (char * buff, int size);
double yynumber();
time_t yydatetime();
const char * yystring();
void  yyinit (CStatement*,FILE*);
int   yyparse();
void  yyfprintf (FILE*,char const *,...);
//---------------------------------------------------------------------------
// END
//---------------------------------------------------------------------------



