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
const char * yystring();
void  yyinit (CStatement*,FILE*);
int   yyparse();
void  yyfprintf (FILE*,char const *,...);
//---------------------------------------------------------------------------
// END
//---------------------------------------------------------------------------



