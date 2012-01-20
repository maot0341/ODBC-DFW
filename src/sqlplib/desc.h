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

#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <vector>
#include "sqlp.h"

using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
class CDesc
{
public:
	CDesc (short type, const char * name, unsigned size, short digits, short nullable);
	CDesc (const CDesc &);

	CDesc & operator= (const CDesc &);

	short type() const { return m_SQL_DESC_CONCISE_TYPE; }
	const char * name() const { return m_SQL_DESC_NAME.c_str(); }
	short nullable() const { return m_SQL_DESC_NULLABLE; }
	ULONG size() const { return m_nSize; }
	short digits() const { return m_nDigits; }

	short aSQL_DESC_UNNAMED() const;
	const char * aSQL_DESC_NAME() const                   { return m_SQL_DESC_NAME.c_str(); }
	short aSQL_DESC_NULLABLE() const                      { return m_SQL_DESC_NULLABLE; }
	short aSQL_DESC_LENGTH() const                        { return m_SQL_DESC_LENGTH; }
	short aSQL_DESC_PRECISION() const                     { return m_SQL_DESC_PRECISION; }
	short aSQL_DESC_NUM_PREC_RADIX() const                { return m_SQL_DESC_NUM_PREC_RADIX; }
	short aSQL_DESC_TYPE() const                          { return m_SQL_DESC_TYPE; }

//protected:

	ULONG m_nSize;
	short m_nDigits;

	short m_SQL_DESC_AUTO_UNIQUE_VALUE;
	string m_SQL_DESC_BASE_COLUMN_NAME;
	string m_SQL_DESC_BASE_TABLE_NAME;
	short m_SQL_DESC_CASE_SENSITIVE;
	string m_SQL_DESC_CATALOG_NAME;
	short m_SQL_DESC_CONCISE_TYPE;
	void* m_SQL_DESC_DATA_PTR;
	short m_SQL_DESC_DATETIME_INTERVAL_CODE;
	long m_SQL_DESC_DATETIME_INTERVAL_PRECISION;
	long m_SQL_DESC_DISPLAY_SIZE;
	short m_SQL_DESC_FIXED_PREC_SCALE;
	long* m_SQL_DESC_INDICATOR_PTR;
	string m_SQL_DESC_LABEL;
	ULONG m_SQL_DESC_LENGTH;
	string m_SQL_DESC_LITERAL_PREFIX;
	string m_SQL_DESC_LITERAL_SUFFIX;
	string m_SQL_DESC_LOCAL_TYPE_NAME;
	string m_SQL_DESC_NAME;
	short m_SQL_DESC_NULLABLE;
	short m_SQL_DESC_NUM_PREC_RADIX;
	ULONG m_SQL_DESC_OCTET_LENGTH;
	long* m_SQL_DESC_OCTET_LENGTH_PTR;
	short m_SQL_DESC_PARAMETER_TYPE;
	short m_SQL_DESC_PRECISION;
	short m_SQL_DESC_ROWVER;
	short m_SQL_DESC_SCALE;
	string m_SQL_DESC_SCHEMA_NAME;
	short m_SQL_DESC_SEARCHABLE;
	string m_SQL_DESC_TABLE_NAME;
	short m_SQL_DESC_TYPE;
	string m_SQL_DESC_TYPE_NAME;
	short m_SQL_DESC_UNSIGNED;
	short m_SQL_DESC_UPDATABLE;
};
//---------------------------------------------------------------------------
} // namespace
#endif
