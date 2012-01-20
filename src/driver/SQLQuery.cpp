/***************************************************************************************

    Projekt:    Hasy                                                     (C) 2000 PSI AG
    Komponente: tppro
    Modul:      tpprodump.cpp

    Dump der Prognose-Online-Dateien

    Systeme: Alpha, Linux, NT 4

    $Author: hasyjv $
    $Date: 2005/08/19 10:43:18 $
    $Revision: 1.4 $
    $State: Exp $

    Änderung  Datum     Autor  Bemerkung
    ------------------------------------------------------------------------------------
    @1        ??.??.00  ??
***************************************************************************************/
#pragma warning (disable:4786)

#include "SQLQuery.h"
#include "database.h"
#include "sqlcpy.h"

#include <sqlplib/sqlp.h>
#include <sqlplib/tableset.h>
#include <stdx/debug.h>
#include <assert.h>

using namespace stdx;
//---------------------------------------------------------------------------
extern int yyparse();
extern void yyinit (CStatement *, FILE*);
//---------------------------------------------------------------------------
extern int yydebug;
CSQLQuery::CSQLQuery (CDatabase * pDatabase, const char * szSQL)
: m_aStmt (pDatabase)
{
	CStatement * pStmt = &m_aStmt;
	if (szSQL)
		pStmt->setSQL (szSQL);
	yyinit (pStmt,0);
//		yydebug = 1;
	int n = yyparse();
	if (n != 0)
		throw EXC("42000", 900, "Syntax error or access violation");
	pStmt->prepare();
	const int nCol = pStmt->m_aSelection.size();
	int i;
	if (n == 0)
	for (i=0; i<nCol; i++)
	{
		CTerm * pTerm = pStmt->m_aSelection[i];
		assert (pTerm);
		string strTerm = id(pTerm);
		const char * szName = strTerm.c_str();
		int nType = pTerm->type();
		assert (nType != SQL_UNKNOWN_TYPE);
		column (nType, szName);
	}
}
//---------------------------------------------------------------------------
void
CSQLQuery::open()
{
	CStatement * pStmt = &m_aStmt;
	ASSUME (pStmt);
	pStmt->execute();
}
//---------------------------------------------------------------------------
ULONG
CSQLQuery::rows() const
{
	const CStatement * pStmt = &m_aStmt;
	ASSUME (pStmt);
	return pStmt->rows();
	return 0;
}
//---------------------------------------------------------------------------
void
CSQLQuery::assign (const vector<CParamImpl> & aAPD)
{
	short nParam = aAPD.size();
	vector<CParam*> & aIPD = m_aStmt.m_aParam;
	assert (aIPD.size() == nParam);
	short i;
	for (i=0; i<nParam; i++)
		sqlcpy (aIPD[i], aAPD[i]);
}
//---------------------------------------------------------------------------
bool
CSQLQuery::read (ULONG iRow, CRecord & raRecord) const
{
	const ULONG nRecord = rows();
	CStatement * pStmt = const_cast<CStatement *>(&m_aStmt);
	ASSUME (pStmt);
	
	const vector<CTerm*> & aSelection = pStmt->m_aSelection;
	const ULONG nCols = aSelection.size();
	assert (raRecord.size() >=  nCols);
	ULONG i;
	if (!pStmt->move (iRow))
		return false;
	for (i=0; i<nCols; i++)
	{
		raRecord[i] = *aSelection[i];
	}
	return true;
}
//---------------------------------------------------------------------------
