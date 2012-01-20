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
#include "CHandle.h"


using namespace std;
using namespace sqlp;
//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
class CDatabase : virtual public CHandle, public IDatabase
{
public:
	CDatabase (CEnvironment * penv);
	virtual ~CDatabase();
	virtual CTable * table (const char * catalog, const char * schema, const char * name);

	CSQLTables * SQLTables (const char * catalog, const char * schema, const char * table, const char * type);
	CSQLGetTypeInfo * SQLGetTypeInfo (short type);

	const char * path() const { return m_strPath.c_str(); }
	void setPath (const char * path) { m_strPath = path;}
	void close();

public:
	string m_strPath;
	CSQLTablesData m_aSQLTables;
};
//---------------------------------------------------------------------------
#endif
