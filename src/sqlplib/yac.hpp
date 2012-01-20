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
#line 190 "yac.hpp"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



