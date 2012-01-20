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

#ifndef QUERY_H
#define QUERY_H

//---------------------------------------------------------------------------
class CDriver;
class CEnvironment;
class CDriverDB;
class CStmtHandle;
class CStmtData;
class CField;

using namespace std;
class CStmtHandle;
//---------------------------------------------------------------------------
class CQuery
{
public:
	CQuery (CStmtHandle*h=0) : m_pHandle(h) {}
	virtual ~CQuery() {};
	virtual short fields() = 0;
	virtual CField & fields (short index) = 0;

	virtual SQLRETURN getData (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	) = 0; // { ASSUME(false); return SQL_ERROR; }

	virtual SQLRETURN bind (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	) = 0;

	SQLRETURN unbind();

	virtual SQLRETURN close() = 0;
	virtual SQLRETURN fetch() = 0;
	virtual SQLRETURN exec() { return SQL_SUCCESS; }

	
	CStmtHandle * m_pHandle;
};
//---------------------------------------------------------------------------
#endif
