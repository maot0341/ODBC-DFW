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


#include "SQLStatistics.h"
#include "idlcpy.h"
#include <stdx/regexpr.h>

using namespace stdx;
//---------------------------------------------------------------------------
//CSQLStatistics CSQLStatistics::ms_aInstance;
//---------------------------------------------------------------------------
CSQLStatistics::CSQLStatistics 
( const char * szCatalog
, const char * szSchema
, const char * szTable
, short nScope
, short nNullabele)
{
	column (SQL_VARCHAR,  "TABLE_CAT",        254, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "TABLE_SCHEM",      254, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "TABLE_NAME",       254, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "NON_UNIQUE",        10, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "INDEX_QUALIFIER",  254, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "INDEX_NAME",       254, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "TYPE",              10, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "ORDINAL_POSITION",  10, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "COLUMN_NAME",      254, 0, SQL_NULLABLE);
	column (SQL_CHAR,     "ASC_OR_DESC",        1, 0, SQL_NULLABLE);
	column (SQL_INTEGER,  "CARDINALITY",       10, 0, SQL_NULLABLE);
	column (SQL_INTEGER,  "PAGES",             10, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "FILTER_CONDITION", 254, 0, SQL_NULLABLE);
}
//---------------------------------------------------------------------------
CSQLStatistics::~CSQLStatistics()
{
}
//---------------------------------------------------------------------------
bool
CSQLStatistics::read (ULONG iRow, ULONG nRow, idl::typRecord & raRecord) const
{
	const ULONG nRecord = rows();
	if (iRow >= nRecord)
		return false;
#if 0
	const record_t & aRecord = record (i);
	raRecord.length (5);
	idlcpy (raRecord[0], aRecord.catalog);
	idlcpy (raRecord[1], aRecord.schema);
	idlcpy (raRecord[2], aRecord.tablename);
	idlcpy (raRecord[3], aRecord.tabletype);
	idlcpy (raRecord[4], aRecord.remarks);
#endif
	return true;
}
//---------------------------------------------------------------------------
ULONG
CSQLStatistics::rows() const
{
	return 0;
}
//---------------------------------------------------------------------------
