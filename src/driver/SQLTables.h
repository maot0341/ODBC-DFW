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

#ifndef __SQLTABLES_H__
#define __SQLTABLES_H__

#include "CTableImpl.h"
#include <sqlplib/index.h>
#include <stdx/debug.h>


class CDatabase;
//---------------------------------------------------------------------------
// SQLTablesData
//---------------------------------------------------------------------------
class CSQLTablesData
{
public:
	struct record_t
	{
		string catalog;
		string schema;
		string tablename;
		string tabletype;
		string remarks;
	};

	CSQLTablesData();

	void record (const char * catalog
	, const char * schema
	, const char * table
	, const char * type
	, const char * remakrs);

	record_t & record (ULONG i)                 { return m_aData[i]; }
	const record_t & record (ULONG i) const     { return m_aData[i]; }

	ULONG rows() const                          { return m_aData.size(); }

	std::vector<record_t> m_aData;
};
//---------------------------------------------------------------------------
// SQLTables QUERY
//---------------------------------------------------------------------------
class CSQLTables: public CTableImpl
{
public:
	typedef CSQLTablesData::record_t record_t;

	CSQLTables (CSQLTablesData *
	, const char * catalog=0
	, const char * schema=0
	, const char * table=0
	, const char * type=0);

	virtual ~CSQLTables();

	bool read (ULONG row, CRecord &) const;

	const record_t & record (ULONG i) const;

	ULONG rows() const;

	string m_strCatalog;
	string m_strSchema;
	string m_strTable;
	string m_strType;

	CSQLTablesData * m_pData;
	idx::iterator * m_pIter;
	vector<ULONG> m_aFilter;

	mutable ULONG m_nRows;
};
//---------------------------------------------------------------------------
#endif
