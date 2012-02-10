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


#include "SQLTables.h"
#include "idlcpy.h"
#include <stdx/regexpr.h>
#include <stdx/utils.h>

using namespace stdx;
//---------------------------------------------------------------------------
CSQLTablesData::CSQLTablesData()
{
	record ("", "PRO", "reg",      "TABLE", "Modelle der Regression");
	record ("", "PRO", "erg",      "TABLE", "Relativer Fehler");
	record ("", "ARV", "a",        "TABLE", "Test Tabelle");
	record ("", "ARV", "b",        "TABLE", "Test Tabelle");
	record ("", "ARV", "c",        "TABLE", "Test Tabelle");
}
//---------------------------------------------------------------------------
void
CSQLTablesData::record (const char * szCatalog
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
//CSQLTables CSQLTables::ms_aInstance;
//---------------------------------------------------------------------------
CSQLTables::CSQLTables 
( CSQLTablesData * pData
, const char * szCatalog
, const char * szSchema
, const char * szTable
, const char * szType)
{
	column (SQL_VARCHAR, "TABLE_QUALIFIER", 128, 0, SQL_NULLABLE);
	column (SQL_VARCHAR, "TABLE_OWNER",      30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR, "TABLE_NAME",       30, 0, SQL_NO_NULLS);
	column (SQL_VARCHAR, "TABLE_TYPE",       30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR, "REMARKS",         254, 0, SQL_NULLABLE);

	m_pIter = 0;
	m_nRows = 0;
	m_pData = pData;
	ASSUME (m_pData);

	CRegExp aExprCatalog (szCatalog,"%", "_");
	CRegExp aExprSchema (szSchema,"%", "_");
	CRegExp aExprTable (szTable,"%", "_");

	const ULONG nRows = m_pData->rows();
	ULONG i;
	m_strCatalog = STRNVL(szCatalog);
	m_strSchema = STRNVL(szSchema);
	m_strTable = STRNVL(szTable);
	m_strType = STRNVL(szType);
	vector<string> aType = SQLNameList (szType);
//	if (!STRLEN(szCatalog) && !STRLEN(szSchema) && !STRLEN(szTable) && !aType.size())
	if (aExprCatalog.empty() && aExprSchema.empty() && aExprTable.empty() && aType.empty())
		idx::assign (m_aFilter, 0, nRows);
	else
	for (i=0; i<nRows; i++)
	{
		const CSQLTablesData::record_t & aRecord = m_pData->record(i);
		if (aExprCatalog.match (aRecord.catalog))
		if (aExprSchema.match (aRecord.schema))
		if (aExprTable.match (aRecord.tablename))
//		if (!STRLEN(szType) || stristr (szType, aRecord.tabletype.c_str()))
		if (aType.empty() || CONTAINS (aType, aRecord.tabletype))
			idx::append (m_aFilter, i);
	}
//	m_pIter = new idx::iterator (m_aFilter);
}
//---------------------------------------------------------------------------
CSQLTables::~CSQLTables()
{
	delete m_pIter;
}
//---------------------------------------------------------------------------
bool
CSQLTables::read (ULONG iRow, ULONG nRow, idl::typRecord & raRecord) const
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
		j = i * nCols;
		const record_t & aRecord = record (iRow++);
		idlcpy (raRecord[j + 0], aRecord.catalog);
		idlcpy (raRecord[j + 1], aRecord.schema);
		idlcpy (raRecord[j + 2], aRecord.tablename);
		idlcpy (raRecord[j + 3], aRecord.tabletype);
		idlcpy (raRecord[j + 4], aRecord.remarks);
	}
	raRecord.length (nCols * i);
	return true;
}
//---------------------------------------------------------------------------
const CSQLTables::record_t &
CSQLTables::record (ULONG i) const
{
	ULONG iRow = idx::item (m_aFilter, i);
	return m_pData->record(iRow);
}
//---------------------------------------------------------------------------
ULONG
CSQLTables::rows() const
{
	if (!m_nRows)
		m_nRows = idx::rows (m_aFilter);;
	return m_nRows;
}
//---------------------------------------------------------------------------
