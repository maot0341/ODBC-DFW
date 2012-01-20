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

#ifndef __DATABASE_H__
#define __DATABASE_H__


#include <memory>
#include <string>
#include "SQLTables.h"
#include "SQLGetTypeInfo.h"

using namespace std;
using namespace sqlp;
//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
class CDatabase : public IDatabase
{
public:
	CDatabase ();
	virtual ~CDatabase();
	virtual CTable * table (const char * catalog, const char * schema, const char * name);


	//virtual ITable* SQLTables (const char * catalog, const char * schema, const char * table, const char * type);

	CSQLTables * SQLTables (const char * catalog, const char * schema, const char * table, const char * type);
	CSQLGetTypeInfo * SQLGetTypeInfo (short type);

	const char * path() const { return m_strPath.c_str(); }
	void setPath (const char * path) { m_strPath = path;}
	static CDatabase * Instance();

public:
	string m_strPath;
	static auto_ptr<CDatabase> ms_pInstance;
	CSQLTablesData m_aSQLTables;
};
//---------------------------------------------------------------------------
#endif
