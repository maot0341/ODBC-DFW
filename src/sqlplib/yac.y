%{
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

%}

/* attribute type */
%union {
	double number;
	const char * string;
	long int integer;
	int token;
	time_t datetime;
	sqlp::CTerm * term;
	sqlp::CTable * table;
	sqlp::CNameList * name;
}

/* variable types */
%token <string>   lCMD
%token <string>   lNAME
%token <string>   lSTRING
%token <number>   lNUMBER
%token <number>   lINTEGER
%token <number>   lBOOLEAN
%token <datetime> lDATETIME

/* keywords */
%token lAVG lSUM lCOUNT lMIN lMAX lFIRST lLAST
%token lSELECT lDISTINCT lALL lAS lFROM lWHERE lGROUP lBY lHAVING
%token lJOIN lON lLEFT lRIGHT lFULL lINNER lOUTER
%token lORDER lBY lASC lDESC

/* escape seq */
%token lOJ lDATE lTIME lTIMESTAMP lGUID lESCAPE lCALL lFN

%token lABS lSIGN lMOD lDIV lEXP lLOG lSQRT lFMT
/*
%token lCHAR lVARCHAR lLONG lNUMERIC lDECIMAL lSMALLINT lINTEGER lREAL lFLOAT lDOUBLE
%token lPRECISION lBIT lTINYINT lBIGINT lBINARY lVARBINARY
*/
%token lNULL lIS lLIKE lIN lBETWEEN
%token lTIMESTAMP lDATE lTIME
%token lINSERT lINTO lVALUES lUPDATE lSET lDELETE
%token lNEQ lLEQ lGEQ lMOD lDIV lLEN lNVL

%token lTRANSFORM lPIVOT

%left lOR
%left lAND
%left lNOT
%left lIS lNULL

%left '+' '-'
%left '*' '/'
%nonassoc lUMINUS


/* types */
%type <string> y_table
%type <term> y_column
%type <term> y_term
%type <term> y_bool
%type <term> y_aggr
%type <string> y_join_table
%type <string> y_join
%type <term> y_term_list
%type <name> y_name_list
%type <term> y_update_term
%type <term> y_update_list



%%

y_lines
	: y_lines y_line 
	| /* empty */
	;
	
y_line
	: y_select				{ printf("SQL: ok\n"); }
	| y_insert
	| y_update				{ printf("UPDATE: SQL-Syntax ok.\n"); }
	| y_delete
	| y_crosstab_query
	| lFMT y_term lSTRING	{ m_stmt->setFormat ($2, $3); exec("formats");}
	| lCMD					{ printf("%s\n", exec ($1).c_str()); }
	| error                 { yyexit(); YYABORT; }
	;

y_select
	: lSELECT                                      { m_stmt->clear(); }
	y_select_opt y_select_list lFROM y_table_list  { m_stmt->select(); }
	y_where y_group y_having y_order               { m_stmt->prepare(); }
	;

y_select_opt
	: /* optional */		{ distinct (false); }
	| lALL					{ distinct (false); }
	| lDISTINCT				{ distinct (true); }
	;

y_select_list
	: y_select_item
	| y_select_list ',' y_select_item
	| '*'						{ select (column(0,0,0)); }
	;
	
y_select_item
	: y_term					{ select ($1); }
	| y_term lAS lNAME			{ select ($1, $3); }
	| y_term lAS lSTRING		{ select ($1, $3); }
	| lNAME '.' '*'				{ select (column (0, $1, 0)); }
	;
	
y_join_table
	: y_table					{ $$ = $1; }
	| y_join					{ $$ = $1; }
	| '(' y_join ')'			{ $$ = $2; }
	;
	
y_join
	: y_table lINNER lJOIN y_join_table lON y_bool        { join (lINNER, $1, $4, $6); }
	| y_table lLEFT  y_outer_join y_join_table lON y_bool { join (lLEFT,  $1, $4, $6); }
	| y_table lRIGHT y_outer_join y_join_table lON y_bool { join (lRIGHT, $1, $4, $6); }
	| y_table lFULL  y_outer_join y_join_table lON y_bool { join (lFULL,  $1, $4, $6); }
	;
	
y_outer_join
	: lOUTER lJOIN
	| lJOIN
	;
		
y_table_list
	: y_table					    {}
	| y_table_list ',' y_table
	| y_join                        {}
	| '{' lOJ y_join '}'			{}
	;

y_table
	: lNAME									{ $$ = table ("", "", $1,  0); }
	| lNAME lNAME							{ $$ = table ("", "", $1, $2); }
	| lNAME lAS lNAME						{ $$ = table ("", "", $1, $3); }
	| lNAME '.' lNAME						{ $$ = table ("", $1, $3,  0); } 
	| lNAME '.' lNAME lNAME					{ $$ = table ("", $1, $3, $4); }
	| lNAME '.' lNAME lAS lNAME				{ $$ = table ("", $1, $3, $5); }
	| lNAME '.' lNAME '.' lNAME				{ $$ = table ($1, $3, $5,  0); }
	| lNAME '.' lNAME '.' lNAME lNAME		{ $$ = table ($1, $3, $5, $6); }
	| lNAME '.' lNAME '.' lNAME lAS lNAME	{ $$ = table ($1, $3, $5, $7); }
	;
		
y_where
	: /* optional */
	| lWHERE y_bool				    { where ($2); }
	;

y_group
	: /* optional */
	| lGROUP lBY y_group_list
	;
	
y_group_list
	: y_term						{ group($1); }
	| y_group_list ',' y_term		{ group($3); }
	;

y_having
	: /* optional */
	| lHAVING y_bool				{ having($2); }
	;

y_order
	: /* optional */
	| lORDER lBY y_order_list
	;

y_order_list
	: y_order_term
	| y_order_list ',' y_order_term
	;

y_order_term
	: y_term					    { order($1); }
	| y_term lASC				    { order($1,lASC); }
	| y_term lDESC				    { order($1,lDESC); }
	;
	
y_column
	: lNAME							{ $$ = column ( 0,  0, $1); }
	| lNAME '.' lNAME				{ $$ = column ( 0, $1, $3); }
	| lNAME '.' lNAME '.' lNAME 	{ $$ = column ($1, $3, $5); }
	;
		
y_bool
	: y_bool lAND y_bool			{ $$ = func(lAND, $1, $3, 0); } 
	| y_bool lOR y_bool				{ $$ = func(lOR, $1, $3, 0); } 
	| y_term '=' y_term				{ $$ = func('=', $1, $3, 0); }
	| y_term '<' y_term				{ $$ = func('<', $1, $3, 0); }
	| y_term '>' y_term				{ $$ = func('>', $1, $3, 0); }
	| y_term lNEQ y_term			{ $$ = func(lNEQ, $1, $3, 0); }
	| y_term lGEQ y_term			{ $$ = func(lGEQ, $1, $3, 0); }
	| y_term lLEQ y_term			{ $$ = func(lLEQ, $1, $3, 0); }
	| y_term lLIKE lSTRING			{ $$ = func(lLIKE, $1, value($3), 0); }
	| y_term lIS lNOT lNULL			{ $$ = unary(lNOT, unary(lNULL, $1)); }
	| y_term lIS lNULL				{ $$ = unary(lNULL, $1); }
	| lNOT y_bool					{ $$ = unary(lNOT, $2); }
	| '(' y_bool ')'				{ $$ = $2; }
	| y_term lIN '(' y_term_list ')' {$$ = func(lIN, $1, $4, 0); }
	| y_term lBETWEEN y_term lAND y_term {$$ = func(lBETWEEN, $1, $3, $5, 0); }
	;

y_aggr
	: lCOUNT '(' '*' ')'			        { $$ = aggregate (lCOUNT, (CTerm*)0); }
	| lCOUNT '(' y_term ')'			        { $$ = aggregate (lCOUNT, $3); }
	| lAVG '(' y_term ')'			        { $$ = aggregate (lAVG, $3); } 
	| lSUM '(' y_term ')'			        { $$ = aggregate (lSUM, $3); } 
	| lMIN '(' y_term ')'			        { $$ = aggregate (lMIN, $3); } 
	| lMAX '(' y_term ')'			        { $$ = aggregate (lMAX, $3); } 
	| lFIRST '(' y_term ')'			        { $$ = aggregate (lFIRST, $3); } 
	| lLAST '(' y_term ')'			        { $$ = aggregate (lLAST, $3); }
	;
	
y_term
	: lNUMBER						        { $$ = number($1); }
	| lINTEGER						        { $$ = number($1); }
	| lBOOLEAN						        { $$ = number($1); }
	| lSTRING						        { $$ = value($1); }
	| lDATETIME						        { $$ = time ($1); }
	| y_column						        { $$ = $1; }
	| y_term '+' y_term				        { $$ = func('+', $1, $3, 0); }
	| y_term '-' y_term				        { $$ = func('+', $1, unary('-', $3), 0); }
	| y_term '*' y_term				        { $$ = func('*', $1, $3, 0); }
	| y_term '/' y_term				        { $$ = func('*', $1, unary('/', $3), 0); }
	| '-' y_term %prec lUMINUS		        { $$ = unary('-', $2); }
	| '(' y_term ')'				        { $$ = $2; }
	| lABS '(' y_term ')'			        { $$ = unary(lABS, $3); }
	| lSIGN '(' y_term ')'			        { $$ = unary(lSIGN, $3); }
	| lMOD '(' y_term ',' y_term ')'        { $$ = func (lMOD, $3, $5, 0); }
	| lDIV '(' y_term ',' y_term ')'		{ $$ = func (lDIV, $3, $5, 0); }
	| lEXP '(' y_term ')'			        { $$ = unary(lEXP, $3); }
	| lLOG '(' y_term ')'			        { $$ = unary(lLOG, $3); }
	| lSQRT '(' y_term ')'			        { $$ = unary(lSQRT, $3); }
	| lEXP '(' y_term ',' y_term ')'        { $$ = func(lEXP, $3, $5, 0); }
	| lLOG '(' y_term ',' y_term ')'        { $$ = func(lLOG, $3, $5, 0); }
	| '{' lTIMESTAMP lSTRING '}'	        { $$ = time($3); }
	| '{' lDATE lSTRING '}'			        { $$ = time($3); }
	| '{' lTIME lSTRING '}'			        { $$ = time($3); }
	| '?'							        { $$ = param(); }
	| '(' y_select ')'                      { $$ = param(); }
	| lMIN '(' y_term ',' y_term_list ')'	{ $$ = func(lMIN, $3, $5, 0); } 
	| lMAX '(' y_term ',' y_term_list ')'	{ $$ = func(lMAX, $3, $5, 0); } 
	| lLEN '(' y_term  ')'		            { $$ = unary (lLEN, $3); } 
	| lNVL '(' y_term ',' y_term ')'		{ $$ = func(lNVL, $3, $5, 0); } 
	| y_aggr
	| y_bool
	;
	
y_term_list
	: y_term						{ $$ = $1; }
	| y_term_list ',' y_term		{ $$ = termlist($1, $3); }
	;

y_name_list
	: lNAME                         { $$ = namelist(0 , $1); }
	| y_name_list ',' lNAME		    { $$ = namelist($1, $3); }
	;

y_update_term
	: lNAME '=' y_term				{ $$ = func('=', column(0,0,$1), $3, 0); }
	;
	
y_update_list
	: y_update_term						{ $$ = $1; }
	| y_update_list ',' y_update_term	{ $$ = termlist ($1, $3); }
	;
	
y_update
	: lUPDATE lNAME lSET y_update_list	y_where { update ($2, $4); }
	;
	
y_insert
	: lINSERT lINTO lNAME '(' y_name_list ')' lVALUES '(' y_term_list ')'
	;
	
y_delete
	: lDELETE '*' lFROM lNAME y_where
	;

y_crosstab_query
	: lTRANSFORM y_aggr y_select lPIVOT y_term 				           { crosstab_query ($2,$5, 0); }
	;

%%

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
