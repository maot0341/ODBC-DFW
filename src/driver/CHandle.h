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

#ifndef CHANDLE_H
#define CHANDLE_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include <sqlplib/sqlp.h>
#include <list>
#include "SQLError.h"

class CDriver;
class CEnvironment;
class CDatabase;
class CStmtHandle;
class CLink;

using namespace sqlp;
//---------------------------------------------------------------------------
// HANDLE
//---------------------------------------------------------------------------
class CHandle
{
public:
	CHandle (CHandle * parent);
	virtual ~CHandle();
	void clear();
	void error (const CSQLException & exc) { m_aError.set (exc); }
	CHandle * parent() { return m_paParent; }

	CHandle * m_paParent;
	std::list<CHandle*> m_aChildList;
//	std::vector<typError> m_aError;
	CSQLError m_aError;
	int m_iError;
};
//---------------------------------------------------------------------------
// DRIVER
//---------------------------------------------------------------------------
class CDriver : CHandle
{
	friend CEnvironment;
public:
	virtual ~CDriver();
	static const char * driver_odbc_ver();
	static const SQLUSMALLINT ms_aSuportedFunc[];
	static CDriver * Instance();

protected:
	CDriver();

	static auto_ptr<CDriver> ms_aInstance;
	char * m_argv[99];
	int m_argc;
};
//---------------------------------------------------------------------------
// ENVIRONMENT
//---------------------------------------------------------------------------
class CEnvironment : public CHandle
{
	friend CDatabase;
public:
	CEnvironment (CDriver *);
	virtual ~CEnvironment();

};
//---------------------------------------------------------------------------
// CLink
//---------------------------------------------------------------------------
class CLink
{
public:
	CLink (SQLSMALLINT typ=SQL_C_DEFAULT
	, SQLPOINTER val=0
	, SQLINTEGER len=0
	, SQLINTEGER* ind=0)
	: nTyp(typ), pVal(val), nLen(len), pInd(ind) {}

	SQLSMALLINT nTyp;
	SQLPOINTER pVal;
	SQLINTEGER nLen;
	SQLINTEGER * pInd;
};
//---------------------------------------------------------------------------
#endif
