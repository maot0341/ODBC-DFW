%{
/****************************************************************************
lexer.l
Lexical analyser for a simple calculator. The lexical analyser is
implemented using a C++ class. This is specified by selecting the -Tcpp
option in ALex (the "C++" selection from the Target Language combo box in
the ALex Options dialog box).
****************************************************************************/

#pragma warning(disable : 4786)
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <errno.h>

#include <string>
#include "sqlp.h"

//---------------------------------------------------------------------------
// Lexer Declarations
//---------------------------------------------------------------------------
#include "yac.h"
namespace sqlp 
{
	CStatement * m_stmt;
}
//using namespace sqlp;
//---------------------------------------------------------------------------
// Lexer Defines
//---------------------------------------------------------------------------
//#define YY_DECL int yylex (YYSTYPE & yylval)
//#define YY_INPUT(b,r,s) (r = yyinput(b,s))


%}
 //---------------------------------------------------------------------------
 // LEX - Defintions
 //---------------------------------------------------------------------------

%option noyywrap

 /* macros */
exponent	([Ee][+-]?[0-9]+)
%s SQLESC

 //---------------------------------------------------------------------------
 // LEX - Rules
 //---------------------------------------------------------------------------
%%

 /*************************************
  LITERALS KEYWORDS TOKENS
  ************************************/
ALL			{ return lALL; }
AND			{ return lAND; }
AS  		{ return lAS; }
ASC			{ return lASC; }
BETWEEN		{ return lBETWEEN; }
BY			{ return lBY; }
DELETE		{ return lDELETE; }
DESC		{ return lDESC; }
DISTINCT	{ return lDISTINCT; }
FROM		{ return lFROM; }
GROUP		{ return lGROUP; }
HAVING		{ return lHAVING; }
IN			{ return lIN; }
INSERT		{ return lINSERT; }
INTO		{ return lINTO; }
IS			{ return lIS; }
LIKE        { return lLIKE; }
NOT			{ return lNOT; }
NULL		{ return lNULL; }
OR			{ return lOR; }
ORDER 		{ return lORDER; }
SELECT		{ return lSELECT; }
SET			{ return lSET; }
UPDATE		{ return lUPDATE; }
VALUES		{ return lVALUES; }
WHERE		{ return lWHERE; }

JOIN		{ return lJOIN; }
INNER		{ return lINNER; }
OUTER		{ return lOUTER; }
FULL		{ return lFULL; }
LEFT		{ return lLEFT; }
RIGHT		{ return lRIGHT; }
ON			{ return lON; }

ABS			{ return lABS; }
SIGN        { return lSIGN; }
MOD			{ return lMOD; }
DIV			{ return lDIV; }
SQRT		{ return lSQRT; }
EXP         { return lEXP; }
LOG			{ return lLOG; }
POWER		{ return lEXP; }

FMT			{ return lFMT; }

SUM			{ return lSUM; }
AVG			{ return lAVG; }
COUNT		{ return lCOUNT; }
MIN			{ return lMIN; }
MAX			{ return lMAX; }
FIRST		{ return lFIRST; }
LAST		{ return lLAST; }
LEN			{ return lLEN; }

TRANSFORM   { return lTRANSFORM; }
PIVOT		{ return lPIVOT; }

".quit"		{ return yyexit(); }
^\..+		{  
			yylval.string = m_stmt->atom (yytext+1);
			return lCMD;
			}			

\{				{ BEGIN SQLESC; return '{'; }
<SQLESC>\}		{ BEGIN 0; return '}'; }
<SQLESC>D		{ return lDATE; }
<SQLESC>T		{ return lTIME; }
<SQLESC>TS		{ return lTIMESTAMP; }
<SQLESC>GUID	{ return lGUID; }
<SQLESC>ESCAPE	{ return lESCAPE; }
<SQLESC>OJ		{ return lOJ; }
<SQLESC>CALL	{ return lCALL; }
<SQLESC>FN		{ return lFN; }


 /*************************************
  COMPARISON
  *************************************/
"<>" 	{ return lNEQ; }
"<="	{ return lLEQ; }
">="	{ return lGEQ; }

 /*************************************
  PUNCTUATION
  *************************************/
[-+*/:(),.;?{}=<>]	{
			return yytext[0];
			}

 /*************************************
  white space
  *************************************/
[ \t\n\r]						{ /* do nothing */ }

 /*************************************
  BOOL
  *************************************/
"true"|"TRUE"					{ yylval.number = 1; return lBOOLEAN; }
"false"|"FALSE"					{ yylval.number = 0; return lBOOLEAN; }

 /*************************************
  INTEGER
  *************************************/
[0-9]+		{ yylval.number = yynumber(); return lINTEGER; }

 /*************************************
  NUMBER
  *************************************/
[0-9]+"."[0-9]*{exponent}?	|
"."[0-9]+{exponent}?		|
[0-9]+{exponent}			{ yylval.number = yynumber(); return lNUMBER; }


 /*************************************
  NAMES
  *************************************/
[A-Za-z][A-Za-z0-9_]*	{
				std::string & aSQL = m_stmt->m_strSQL;
				if (aSQL.empty())
					yylval.string = yystring();
				else
					yylval.string = yystring();
					//yylval.string = m_stmt->sql (strlen(yytext), false);
				return lNAME;
			}

 /*************************************
  NAMES ([] quotetd)
  *************************************/
\"[^\"\n]+\"	{
				std::string & aSQL = m_stmt->m_strSQL;
				if (!aSQL.empty())
				{
					yylval.string = m_stmt->atom (yytext+1, -1);
					return lNAME;
				}
				yylval.string = yystring();
				return lNAME;
		}			

 /*************************************
  NAMES (unterminated)
  *************************************/
\"[^\"\n]*$	{ 
			yyerror( "Unterminated string" ); 
		}

 /*************************************
  STRINGS (single quotes)
  NOTE: I recommend appending a space on the end of your statement because I
        have found that STRINGs will not get picked up if the last char in the
        statement is the closing quote of said STRING. - PAH
  *************************************/
'[^'\n]*' 	{
			char * szBuff = strdup (yytext+1);
			const int n = strlen(szBuff);
			if (n > 0)
				szBuff[n - 1] = 0;
			const int c = yyinput();
			unput (c);
			if (c != '\'' && n > 1)
			{
				assert (m_stmt);
				yylval.string = szBuff;
				m_stmt->m_aAtoms.push_back (szBuff);
				//yylval.string = m_stmt->atom (szBuff);
				return lSTRING;
			}
			else
			{
				free (szBuff);
				yymore();
			}
		}
		
 /*************************************
  STRINGS (unterminated)
  *************************************/
'[^'\n]*$	{ 
			yyerror( "Unterminated string" ); 
		}

 /*************************************
  others
  *************************************/
.			{ printf("invalid character '0x%02x'\n", (unsigned int)yytext[0]); }

<<EOF>>     { return yyexit(); }

%%

//namespace sqlp {
//---------------------------------------------------------------------------
// Lexer commands
//---------------------------------------------------------------------------
int yyinputc (char * buff, int size)
{
#if 1
	int i,c;
	for (i=0; i<size; i++)
	{
		c = getc (yyin);
		if (c == EOF)
			return YY_NULL;
		buff[i] = c;
		if (c == '\n')
			break;
	}
	return i + 1;
#else
	int c = getchar();
	if (c == EOF)
		return YY_NULL;
	buff[0] = c;
	return 1;
#endif
}
//---------------------------------------------------------------------------
int yyinput (char * buff, int size)
{
	assert (m_stmt);
	std::string & aSQL = m_stmt->m_strSQL;
	ULONG & i = m_stmt->m_iSQL;
	if (aSQL.empty())
		return yyinputc (buff, size);
	if (i >= aSQL.length())
		return YY_NULL;
	const char * szSQL = aSQL.c_str();
	int n = strlen (szSQL);
	if (n > size)
		n = size;
	memcpy (buff, szSQL, n);
	i += n;
	return n;
}
//---------------------------------------------------------------------------
double yynumber()
{
	errno = 0;		// clear error flag
	char* endp;
	double d = strtod(yytext, &endp);
	if ((d == +HUGE_VAL || d == -HUGE_VAL) && errno == ERANGE) {
		printf("number too big\n");
	}
	return d;
}
//---------------------------------------------------------------------------
const char * yystring()
{
	assert (m_stmt);
	return m_stmt->atom (yytext);
}
//---------------------------------------------------------------------------
void yyinit (CStatement * pStmt, FILE* pFile)
{
	assert (pStmt);
	m_stmt = pStmt;
	const char * szSQL = pStmt->m_strSQL.c_str();
	yy_delete_buffer (yy_current_buffer);
	if (!pFile)
		yy_switch_to_buffer (yy_scan_string (szSQL));
	else
	{
		yyin = pFile;
		yy_switch_to_buffer (yy_create_buffer( yyin, YY_BUF_SIZE));
	}
}
//---------------------------------------------------------------------------
int yyexit()
{
	yy_delete_buffer (yy_current_buffer); 
	yy_current_buffer = 0;
	yy_init = 1;
	return YY_NULL;
}
//---------------------------------------------------------------------------
void yyerror (char const * err)
{
	fputs (err, stderr);
}
//---------------------------------------------------------------------------
//} // namespace