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


#include "SQLSpecialColumns.h"
#include "idlcpy.h"
#include <stdx/regexpr.h>

using namespace stdx;
//---------------------------------------------------------------------------
CSQLSpecialColumnsData::CSQLSpecialColumnsData()
{
	record ("", "", "P_VBR001", "TABLE", "Prognose Ergebnisse");
	record ("", "", "F_VBR001", "TABLE", "Relativer Fehler");
	record ("", "", "P_VBR004", "TABLE", "Prognose Ergebnisse");
	record ("", "", "F_VBR004", "TABLE", "Relativer Fehler");
	record ("", "", "reg",      "TABLE", "Modelle der Regression");
	record ("", "", "erg",      "TABLE", "Relativer Fehler");
	record ("", "", "a",        "TABLE", "Test Tabelle");
	record ("", "", "b",        "TABLE", "Test Tabelle");
}
//---------------------------------------------------------------------------
void
CSQLSpecialColumnsData::record (const char * szCatalog
, const char * szSchema
, const char * szTable
, const char * szType
, const char * szRemakrs)
{
	record_t aRecord;
	if (szCatalog)
		aRecord.catalog = szCatalog;
	if (szSchema)
		aRecord.schema = szSchema;
	if (szTable)
		aRecord.tablename = szTable;
	if (szType)
		aRecord.tabletype = szType;
	if (szRemakrs)
		aRecord.remarks = szRemakrs;
	m_aData.push_back (aRecord);
}
//---------------------------------------------------------------------------




//---------------------------------------------------------------------------
//CSQLSpecialColumns CSQLSpecialColumns::ms_aInstance;
//---------------------------------------------------------------------------
CSQLSpecialColumns::CSQLSpecialColumns 
( CSQLSpecialColumnsData * pData
, const char * szCatalog
, const char * szSchema
, const char * szTable
, short nScope
, short nNullabele)
{
	column (SQL_SMALLINT, "SCOPE",            30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "COLUMN_NAME",     254, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "DATA_TYPE",        30, 0, SQL_NO_NULLS);
	column (SQL_VARCHAR,  "TYPE_NAME",       254, 0, SQL_NO_NULLS);
	column (SQL_INTEGER,  "COLUMN_SIZE",      30, 0, SQL_NULLABLE);
	column (SQL_INTEGER,  "BUFFER_LENGTH",    30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "DECIMAL_DIGITS",   30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "PSEUDO_COLUMN",    30, 0, SQL_NULLABLE);
}
//---------------------------------------------------------------------------
CSQLSpecialColumns::~CSQLSpecialColumns()
{
}
//---------------------------------------------------------------------------
bool
CSQLSpecialColumns::read (ULONG iRow, ULONG nRow, idl::typRecord & raRecord) const
{
	const ULONG nRows = rows();
	const ULONG nCols = 5;
	if (iRow >= nRows)
		return false;
	raRecord.length (nCols * nRow);
	ULONG i,j;
	for (i=0; i<nRow; i++)
	{
		if (iRow >= nRows)
			break;
		const record_t & aRecord = record (iRow++);
		j = i * nCols;
		idlcpy (raRecord[j + 0], aRecord.catalog);
		idlcpy (raRecord[j + 1], aRecord.schema);
		idlcpy (raRecord[j + 2], aRecord.tablename);
		idlcpy (raRecord[j + 3], aRecord.tabletype);
		idlcpy (raRecord[j + 4], aRecord.remarks);
	}
	return true;
}
//---------------------------------------------------------------------------
const CSQLSpecialColumns::record_t &
CSQLSpecialColumns::record (ULONG i) const
{
	ULONG iRow = idx::item (m_aFilter, i);
	return m_pData->record(iRow);
}
//---------------------------------------------------------------------------
ULONG
CSQLSpecialColumns::rows() const
{
	return 0;
	if (!m_nRows)
		m_nRows = idx::rows (m_aFilter);;
	return m_nRows;
}
//---------------------------------------------------------------------------
