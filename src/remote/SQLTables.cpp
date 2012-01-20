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
#include "CStmtHandle.h"
#include "driver.h"

using namespace stdx;
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLTables (SQLHSTMT hstmt
, SQLCHAR *pCatalog, SQLSMALLINT nCatalog
, SQLCHAR *pSchema, SQLSMALLINT nSchema
, SQLCHAR *pTable, SQLSMALLINT nTable
, SQLCHAR *pType, SQLSMALLINT nType)
{
	TRY ("SQLTables");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	pStmt->prepare();
	CORBA::String_var strCatalog = SQLString (pCatalog, nCatalog);
	CORBA::String_var strSchema = SQLString (pSchema, nSchema);
	CORBA::String_var strTable = SQLString (pTable, nTable);
	CORBA::String_var strType = SQLString (pType, nType);
	idl::RETN nRetn = (*pStmt)->SQLTables (strCatalog, strSchema, strTable, strType);
	return nRetn;
	EXCEPTION(hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
