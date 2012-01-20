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

#include <OB/CORBA.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdx/debug.h>
#include <vector>

#include "CStmtHandle.h"
//#include <vld.h>

using namespace stdx;
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLGetTypeInfo (SQLHSTMT hstmt, SQLSMALLINT nDataType)
{
	TRACELN ("SQLGetTypeInfo");
//	return SQL_NO_DATA;

	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	pStmt->prepare();
	idl::RETN nRetn = (*pStmt)->SQLGetTypeInfo (nDataType);
	return nRetn;

}
//---------------------------------------------------------------------------
