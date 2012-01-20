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

#include "desc.h"


using namespace std;
namespace sqlp {

//---------------------------------------------------------------------------
CDesc::CDesc(short type, const char * name, unsigned size, short digits, short nullable)
{
	m_SQL_DESC_AUTO_UNIQUE_VALUE = SQL_FALSE;
	m_SQL_DESC_CASE_SENSITIVE = SQL_TRUE;
	m_SQL_DESC_CONCISE_TYPE = type;
	m_SQL_DESC_DATA_PTR = 0;;
	m_SQL_DESC_DATETIME_INTERVAL_CODE = 0;
	m_SQL_DESC_DATETIME_INTERVAL_PRECISION = 0;
	m_SQL_DESC_DISPLAY_SIZE = size;
	m_SQL_DESC_FIXED_PREC_SCALE = SQL_FALSE;
	m_SQL_DESC_INDICATOR_PTR = 0;
	m_SQL_DESC_LABEL = name;
	m_SQL_DESC_LENGTH = size;
	m_SQL_DESC_LITERAL_PREFIX = "";
	m_SQL_DESC_LITERAL_SUFFIX = "";
	m_SQL_DESC_LOCAL_TYPE_NAME = "";
	m_SQL_DESC_NAME = name;
	m_SQL_DESC_NULLABLE = nullable;
	m_SQL_DESC_NUM_PREC_RADIX = 0;
	m_SQL_DESC_OCTET_LENGTH = size + 1;
	m_SQL_DESC_OCTET_LENGTH_PTR = 0;
	m_SQL_DESC_PARAMETER_TYPE = 0;
	m_SQL_DESC_PRECISION = digits;

	m_SQL_DESC_ROWVER = 0;
	m_SQL_DESC_SCALE = 0;
	m_SQL_DESC_SCHEMA_NAME = "";
	m_SQL_DESC_SEARCHABLE = SQL_TRUE;
	m_SQL_DESC_TABLE_NAME = "";
	m_SQL_DESC_TYPE = type;
	m_SQL_DESC_TYPE_NAME = "";
	m_SQL_DESC_UNSIGNED = SQL_FALSE;
	m_SQL_DESC_UPDATABLE = SQL_TRUE;

	m_nSize = size;
	m_nDigits = digits;
}
//---------------------------------------------------------------------------
CDesc::CDesc (const CDesc & aDesc)
{
	m_SQL_DESC_AUTO_UNIQUE_VALUE = aDesc.m_SQL_DESC_AUTO_UNIQUE_VALUE;
	m_SQL_DESC_BASE_COLUMN_NAME = aDesc.m_SQL_DESC_BASE_COLUMN_NAME;
	m_SQL_DESC_BASE_TABLE_NAME = aDesc.m_SQL_DESC_BASE_TABLE_NAME;
	m_SQL_DESC_CASE_SENSITIVE = aDesc.m_SQL_DESC_CASE_SENSITIVE;
	m_SQL_DESC_CATALOG_NAME = aDesc.m_SQL_DESC_CATALOG_NAME;
	m_SQL_DESC_CONCISE_TYPE = aDesc.m_SQL_DESC_CONCISE_TYPE;
	m_SQL_DESC_DATA_PTR = aDesc.m_SQL_DESC_DATA_PTR;
	m_SQL_DESC_DATETIME_INTERVAL_CODE = aDesc.m_SQL_DESC_DATETIME_INTERVAL_CODE;
	m_SQL_DESC_DATETIME_INTERVAL_PRECISION = aDesc.m_SQL_DESC_DATETIME_INTERVAL_PRECISION;
	m_SQL_DESC_DISPLAY_SIZE = aDesc.m_SQL_DESC_DISPLAY_SIZE;
	m_SQL_DESC_FIXED_PREC_SCALE = aDesc.m_SQL_DESC_FIXED_PREC_SCALE;
	m_SQL_DESC_INDICATOR_PTR = aDesc.m_SQL_DESC_INDICATOR_PTR;
	m_SQL_DESC_LABEL = aDesc.m_SQL_DESC_LABEL;
	m_SQL_DESC_LENGTH = aDesc.m_SQL_DESC_LENGTH;
	m_SQL_DESC_LITERAL_PREFIX = aDesc.m_SQL_DESC_LITERAL_PREFIX;
	m_SQL_DESC_LITERAL_SUFFIX = aDesc.m_SQL_DESC_LITERAL_SUFFIX;
	m_SQL_DESC_LOCAL_TYPE_NAME = aDesc.m_SQL_DESC_LOCAL_TYPE_NAME;
	m_SQL_DESC_NAME = aDesc.m_SQL_DESC_NAME;
	m_SQL_DESC_NULLABLE = aDesc.m_SQL_DESC_NULLABLE;
	m_SQL_DESC_NUM_PREC_RADIX = aDesc.m_SQL_DESC_NUM_PREC_RADIX;
	m_SQL_DESC_OCTET_LENGTH = aDesc.m_SQL_DESC_OCTET_LENGTH;
	m_SQL_DESC_OCTET_LENGTH_PTR = aDesc.m_SQL_DESC_OCTET_LENGTH_PTR;
	m_SQL_DESC_PARAMETER_TYPE = aDesc.m_SQL_DESC_PARAMETER_TYPE;
	m_SQL_DESC_PRECISION = aDesc.m_SQL_DESC_PRECISION;
	m_SQL_DESC_ROWVER = aDesc.m_SQL_DESC_ROWVER;
	m_SQL_DESC_SCALE = aDesc.m_SQL_DESC_SCALE;
	m_SQL_DESC_SCHEMA_NAME = aDesc.m_SQL_DESC_SCHEMA_NAME;
	m_SQL_DESC_SEARCHABLE = aDesc.m_SQL_DESC_SEARCHABLE;
	m_SQL_DESC_TABLE_NAME = aDesc.m_SQL_DESC_TABLE_NAME;
	m_SQL_DESC_TYPE = aDesc.m_SQL_DESC_TYPE;
	m_SQL_DESC_TYPE_NAME = aDesc.m_SQL_DESC_TYPE_NAME;
	m_SQL_DESC_UNSIGNED = aDesc.m_SQL_DESC_UNSIGNED;
	m_SQL_DESC_UPDATABLE = aDesc.m_SQL_DESC_UPDATABLE;

	m_nSize = aDesc.m_nSize;
	m_nDigits = aDesc.m_nDigits;
}
//---------------------------------------------------------------------------
short
CDesc::aSQL_DESC_UNNAMED() const
{
	const char * szName = aSQL_DESC_NAME();
	return szName && *szName ? SQL_NAMED : SQL_UNNAMED;
}
//---------------------------------------------------------------------------
CDesc & 
CDesc::operator= (const CDesc & aDesc)
{
	m_SQL_DESC_AUTO_UNIQUE_VALUE = aDesc.m_SQL_DESC_AUTO_UNIQUE_VALUE;
	m_SQL_DESC_BASE_COLUMN_NAME = aDesc.m_SQL_DESC_BASE_COLUMN_NAME;
	m_SQL_DESC_BASE_TABLE_NAME = aDesc.m_SQL_DESC_BASE_TABLE_NAME;
	m_SQL_DESC_CASE_SENSITIVE = aDesc.m_SQL_DESC_CASE_SENSITIVE;
	m_SQL_DESC_CATALOG_NAME = aDesc.m_SQL_DESC_CATALOG_NAME;
	m_SQL_DESC_CONCISE_TYPE = aDesc.m_SQL_DESC_CONCISE_TYPE;
	m_SQL_DESC_DATA_PTR = aDesc.m_SQL_DESC_DATA_PTR;
	m_SQL_DESC_DATETIME_INTERVAL_CODE = aDesc.m_SQL_DESC_DATETIME_INTERVAL_CODE;
	m_SQL_DESC_DATETIME_INTERVAL_PRECISION = aDesc.m_SQL_DESC_DATETIME_INTERVAL_PRECISION;
	m_SQL_DESC_DISPLAY_SIZE = aDesc.m_SQL_DESC_DISPLAY_SIZE;
	m_SQL_DESC_FIXED_PREC_SCALE = aDesc.m_SQL_DESC_FIXED_PREC_SCALE;
	m_SQL_DESC_INDICATOR_PTR = aDesc.m_SQL_DESC_INDICATOR_PTR;
	m_SQL_DESC_LABEL = aDesc.m_SQL_DESC_LABEL;
	m_SQL_DESC_LENGTH = aDesc.m_SQL_DESC_LENGTH;
	m_SQL_DESC_LITERAL_PREFIX = aDesc.m_SQL_DESC_LITERAL_PREFIX;
	m_SQL_DESC_LITERAL_SUFFIX = aDesc.m_SQL_DESC_LITERAL_SUFFIX;
	m_SQL_DESC_LOCAL_TYPE_NAME = aDesc.m_SQL_DESC_LOCAL_TYPE_NAME;
	m_SQL_DESC_NAME = aDesc.m_SQL_DESC_NAME;
	m_SQL_DESC_NULLABLE = aDesc.m_SQL_DESC_NULLABLE;
	m_SQL_DESC_NUM_PREC_RADIX = aDesc.m_SQL_DESC_NUM_PREC_RADIX;
	m_SQL_DESC_OCTET_LENGTH = aDesc.m_SQL_DESC_OCTET_LENGTH;
	m_SQL_DESC_OCTET_LENGTH_PTR = aDesc.m_SQL_DESC_OCTET_LENGTH_PTR;
	m_SQL_DESC_PARAMETER_TYPE = aDesc.m_SQL_DESC_PARAMETER_TYPE;
	m_SQL_DESC_PRECISION = aDesc.m_SQL_DESC_PRECISION;
	m_SQL_DESC_ROWVER = aDesc.m_SQL_DESC_ROWVER;
	m_SQL_DESC_SCALE = aDesc.m_SQL_DESC_SCALE;
	m_SQL_DESC_SCHEMA_NAME = aDesc.m_SQL_DESC_SCHEMA_NAME;
	m_SQL_DESC_SEARCHABLE = aDesc.m_SQL_DESC_SEARCHABLE;
	m_SQL_DESC_TABLE_NAME = aDesc.m_SQL_DESC_TABLE_NAME;
	m_SQL_DESC_TYPE = aDesc.m_SQL_DESC_TYPE;
	m_SQL_DESC_TYPE_NAME = aDesc.m_SQL_DESC_TYPE_NAME;
	m_SQL_DESC_UNSIGNED = aDesc.m_SQL_DESC_UNSIGNED;
	m_SQL_DESC_UPDATABLE = aDesc.m_SQL_DESC_UPDATABLE;

	m_nSize = aDesc.m_nSize;
	m_nDigits = aDesc.m_nDigits;
	return *this;
}
//---------------------------------------------------------------------------
} // namespace