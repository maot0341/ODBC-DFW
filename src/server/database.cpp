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
#include <time.h>
#include <set>
#include <string>

#include <sqlplib/yac.h>
#include <stdx/utils.h>
#include "database.h"
#include "CTableTST.h"
#include "CTableERG.h"


using namespace std;
using namespace idx;
using namespace sqlp;


//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
auto_ptr<CDatabase> CDatabase::ms_pInstance;
//---------------------------------------------------------------------------
CDatabase *
CDatabase::Instance()
{
	CDatabase * pDB = ms_pInstance.get();
	if (pDB)
		return pDB;
	pDB = new CDatabase();
	ms_pInstance = auto_ptr<CDatabase> (pDB);
	return pDB;
}
//---------------------------------------------------------------------------
CDatabase::CDatabase()
{
//	assert (paEnv);
	m_strPath = "D:\\jvater\\odbc\\data";
//	m_strPath = "D:\\gow\\hasy\\hasyjv\\prj\\ems\\ProgMod\\wrk";
}
//---------------------------------------------------------------------------
CDatabase::~CDatabase()
{
}
//---------------------------------------------------------------------------
CTable * 
CDatabase::table (const char * szCatalog, const char * szSchema, const char * szName)
{
	const char * szPath = path();
	CTable * pTable = 0;
	if (strlen(szName) == 1)
		pTable = new CTableTST(szName);
	else
	if (stricmp (szName, "erg") == 0)
		pTable = new CTableERG (szPath);
	else
		throw EXC("42S02", 900, "Base table or view not found [%s]", szName);
	ASSUME (pTable);
	return pTable;
}
//---------------------------------------------------------------------------
CSQLTables *
CDatabase::SQLTables (const char * catalog, const char * schema, const char * table, const char * type)
{
	return new CSQLTables (&m_aSQLTables, catalog, schema, table, type);
}
//---------------------------------------------------------------------------
CSQLGetTypeInfo *
CDatabase::SQLGetTypeInfo (short type)
{
	return new CSQLGetTypeInfo (type);
}
//---------------------------------------------------------------------------
