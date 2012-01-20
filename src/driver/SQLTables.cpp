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
#include <stdx/regexpr.h>
#include <assert.h>

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
bool match (const string & s1, const char * szExpr)
{
	if (!szExpr || *szExpr == 0)
		return true;
	CRegExpr aExpr(szExpr);
	return aExpr.match (s1.c_str());
	return true;
}

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

	CRegExpr aExprCatalog (szCatalog, "%", "_");
	CRegExpr aExprSchema (szSchema, "%", "_");
	CRegExpr aExprTable (szTable, "%", "_");
	CRegExpr aExprType (szType, "%", "_");

	const ULONG nRows = m_pData->rows();
	ULONG i;
	if (szCatalog && !szCatalog[0])
		szCatalog = 0;
	if (szSchema && !szSchema[0])
		szSchema = 0;
	if (szTable && !szTable[0])
		szTable = 0;
	if (szType && !szType[0])
		szType = 0;
	if (szCatalog)
		m_strCatalog = szCatalog;
	if (szSchema)
		m_strSchema = szSchema;
	if (szTable)
		m_strTable = szTable;
	if (szType)
		m_strType = szType;
	if (!szCatalog && !szSchema && !szTable && !szType)
	{
		m_aFilter.push_back(0);
		m_aFilter.push_back(nRows);
	}
	else
	for (i=0; i<nRows; i++)
	{
		const CSQLTablesData::record_t & aRecord = m_pData->record(i);
		if (aExprCatalog.match (aRecord.catalog))
		if (aExprSchema.match (aRecord.schema))
		if (aExprTable.match (aRecord.tablename))
		//if (aExprType.match (aRecord.tabletype))
		if (!szType || strstr (szType, aRecord.tabletype.c_str()))
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
bool
CSQLTables::read (ULONG iRow, CRecord & raRecord) const
{
	const ULONG nRows = rows();
	const ULONG nCols = 5;
	if (iRow >= nRows)
		return false;
	assert (nCols <= raRecord.size());
	const record_t & aRecord = record (iRow);
	raRecord[0] = aRecord.catalog;
	raRecord[1] = aRecord.schema;
	raRecord[2] = aRecord.tablename;
	raRecord[3] = aRecord.tabletype;
	raRecord[4] = aRecord.remarks;
	return true;
}
//---------------------------------------------------------------------------
