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
#include <IRemote_idl.h>

#include "SQLColumns.h"
#include "SQLTables.h"
#include "database.h"
#include "idlcpy.h"
#include <sqlplib/desc.h>



//#include "system.h"
//---------------------------------------------------------------------------
CSQLColumns::CSQLColumns (const char * szCatalog
	, const char * szSchema
	, const char * szTable
	, const char * szColumn)
{
	column (SQL_VARCHAR,  "TABLE_CAT",        128, 0, SQL_NULLABLE); //  1
	column (SQL_VARCHAR,  "TABLE_SCHEM",       30, 0, SQL_NULLABLE); //  2
	column (SQL_VARCHAR,  "TABLE_NAME",        30, 0, SQL_NO_NULLS); //  3
	column (SQL_VARCHAR,  "COLUMN_NAME",       30, 0, SQL_NULLABLE); //  4
	column (SQL_SMALLINT, "DATA_TYPE",          5, 0, SQL_NO_NULLS); //  5
	column (SQL_VARCHAR,  "TYPE_NAME",         30, 0, SQL_NULLABLE); //  6
	column (SQL_INTEGER,  "COLUMN_SIZE",       30, 0, SQL_NULLABLE); //  7
	column (SQL_INTEGER,  "BUFFER_LENGTH",     30, 0, SQL_NULLABLE); //  8
	column (SQL_SMALLINT, "DECIMAL_DIGITS",    30, 0, SQL_NULLABLE); //  9
	column (SQL_SMALLINT, "NUM_PREC_RADIX",    30, 0, SQL_NULLABLE); // 10
	column (SQL_SMALLINT, "NULLABLE",          30, 0, SQL_NO_NULLS); // 11
	column (SQL_VARCHAR,  "REMARKS",           30, 0, SQL_NULLABLE); // 12
	column (SQL_VARCHAR,  "COLUMN_DEF",        30, 0, SQL_NULLABLE); // 13
	column (SQL_SMALLINT, "SQL_DATA_TYPE",     30, 0, SQL_NO_NULLS); // 14
	column (SQL_SMALLINT, "SQL_DATETIME_SUB",  30, 0, SQL_NO_NULLS); // 15
	column (SQL_INTEGER,  "CHAR_OCTET_LENGTH", 30, 0, SQL_NULLABLE); // 16
	column (SQL_INTEGER,  "ORDINAL_POSITION",  30, 0, SQL_NO_NULLS); // 17
	column (SQL_VARCHAR,  "IS_NULLABLE",       30, 0, SQL_NULLABLE); // 18


	ULONG i,j;
	CDatabase * pDatabase = CDatabase::Instance();
	assert (pDatabase);
	auto_ptr<CSQLTables> aTables = auto_ptr<CSQLTables>(pDatabase->SQLTables (szCatalog, szSchema, szTable, "TABLE"));
	const int nRows = aTables->rows();
	auto_ptr<CTable> aTablePtr;
	for (i=0; i<nRows; i++)
	{
		record_t aRecord;
		const CSQLTables::record_t & aTable = aTables->record(i);
		const char * szCatalog = aTable.catalog.c_str();
		const char * szSchema = aTable.schema.c_str();
		const char * szTable = aTable.tablename.c_str();
		CTable * pTable = pDatabase->table (szCatalog, szSchema, szTable);
		if (!pTable)
			continue;
		assert (pTable);
		aTablePtr = auto_ptr<CTable>(pTable);
		aRecord.catalog = aTable.catalog;
		aRecord.schema = aTable.schema;
		aRecord.table = aTable.tablename;
		const int nCols = pTable->cols();
		for (j=0; j<nCols; j++)
		{
			const CDesc * pDesc = pTable->desc(j);
			assert (pDesc);
			aRecord.column = pDesc->name();
			aRecord.datatype = pDesc->type();
//			aRecord.type_name = pTable->desc (SQL_DESC_TYPE, j).asNumber();
			aRecord.size = pDesc->size();
			aRecord.length = pDesc->aSQL_DESC_LENGTH();
			aRecord.digits = pDesc->aSQL_DESC_PRECISION();
			aRecord.num_prec_radix = pDesc->aSQL_DESC_NUM_PREC_RADIX();
			aRecord.nullable = pDesc->aSQL_DESC_NULLABLE();
			aRecord.remarks = "@@@@ debug";

			aRecord.is_nullable = aRecord.nullable ? "YES" : "NO";
			m_aData.push_back (aRecord);
		}
	}
	if (szCatalog)
		m_strCatalog = szCatalog;
	if (szSchema)
		m_strSchema = szSchema;
	if (szTable)
		m_strTable = szTable;
	if (szColumn)
		m_strColumn = szColumn;
}
//---------------------------------------------------------------------------
CSQLColumns::~CSQLColumns ()
{
	OutputDebugString ("~CSQLColumns\n");
}
//---------------------------------------------------------------------------
void
CSQLColumns::record (const char * catalog
, const char * schema
, const char *table
, const char *column
, short  datatype
, const char *  type_name
, long size
, long length
, short digits
, short num_prec_radix
, short nullable
, const char * remarks
, const char * column_def
, short sql_data_type
, short sql_datetime_sub
, long  char_octet_length
, long  ordinal_position
, const char * is_nullable
)
{
	record_t aRecord;
	aRecord.catalog = catalog;
	aRecord.schema = schema;
	aRecord.table = table;
	aRecord.column = column;
	aRecord.datatype = datatype;
	aRecord.type_name = type_name;
	aRecord.size = size;
	aRecord.length = length;
	aRecord.digits = digits;
	aRecord.num_prec_radix = num_prec_radix;
	aRecord.nullable = nullable;
	aRecord.remarks = remarks;
	aRecord.column_def = column_def;
	aRecord.sql_data_type = sql_data_type;
	aRecord.sql_datetime_sub = sql_datetime_sub;
	aRecord.char_octet_length = char_octet_length;
	aRecord.ordinal_position = ordinal_position;
	aRecord.is_nullable = is_nullable;
	m_aData.push_back (aRecord);
}
//---------------------------------------------------------------------------
ULONG
CSQLColumns::rows() const
{
	return m_aData.size();
}
//---------------------------------------------------------------------------
bool
CSQLColumns::read (ULONG iRow, ULONG nRow, idl::typRecord & crbRecord) const
{
	const int nRows = rows();
	const int nCols = cols();
	if (iRow >= nRows)
		return false;
	crbRecord.length (nCols * nRow);
	assert (nCols == 18);
	ULONG i,j;
	for (i=0; i<nRow; i++)
	{
		if (iRow >= nRows)
			break;
		j = i * nCols;
		const record_t & aRecord = m_aData[iRow++];
		idlcpy (crbRecord[j +  0], aRecord.catalog);
		idlcpy (crbRecord[j +  1], aRecord.schema);
		idlcpy (crbRecord[j +  2], aRecord.table);
		idlcpy (crbRecord[j +  3], aRecord.column);
		idlcpy (crbRecord[j +  4], aRecord.datatype);
		idlcpy (crbRecord[j +  5], aRecord.type_name);
		idlcpy (crbRecord[j +  6], aRecord.size);
		idlcpy (crbRecord[j +  7], aRecord.length);
		idlcpy (crbRecord[j +  8], aRecord.digits);
		idlcpy (crbRecord[j +  9], aRecord.num_prec_radix);
		idlcpy (crbRecord[j + 10], aRecord.nullable);
		idlcpy (crbRecord[j + 11], aRecord.remarks);
		idlcpy (crbRecord[j + 12], aRecord.column_def);
		idlcpy (crbRecord[j + 13], aRecord.sql_data_type);
		idlcpy (crbRecord[j + 14], aRecord.sql_datetime_sub);
		idlcpy (crbRecord[j + 15], aRecord.char_octet_length);
		idlcpy (crbRecord[j + 16], aRecord.ordinal_position);
		idlcpy (crbRecord[j + 17], aRecord.is_nullable);
	}
	crbRecord.length (nCols * i);
	return true;
}
//---------------------------------------------------------------------------
