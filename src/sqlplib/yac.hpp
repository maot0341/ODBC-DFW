
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 73 ".\\yac.y"

	double number;
	const char * string;
	long int integer;
	int token;
	time_t datetime;
	sqlp::CTerm * term;
	sqlp::CTable * table;
	sqlp::CNameList * name;



/* Line 1676 of yacc.c  */
#line 138 "yac.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


