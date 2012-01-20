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


#include "SQLGetTypeInfo.h"
#include <stdx/utils.h>

//---------------------------------------------------------------------------
// CSQLGTypeInfo 
//---------------------------------------------------------------------------
CSQLTypeInfo::~CSQLTypeInfo()
{
}
//---------------------------------------------------------------------------
// Typen 
//---------------------------------------------------------------------------
class CSQLTypeInfoGUID: public CSQLTypeInfo
{
public:
	CSQLTypeInfoGUID() : CSQLTypeInfo(SQL_GUID) {}
	const char * TYPE_NAME() const                    { return "GUID"; }
	long  COLUMN_SIZE() const                         { return 36; }
	const char * LITERAL_PREFIX() const               { return "\""; }
	const char * LITERAL_SUFFIX() const               { return "\""; }
	const char * CREATE_PARAMS() const                { return ""; }
	short NULLABLE() const                            { return SQL_NULLABLE; }
	short CASE_SENSITIVE() const                      { return SQL_FALSE; }
	short SEARCHABLE() const                          { return SQL_SEARCHABLE; }
	short UNSIGNED_ATTRIBUTE() const                  { return NULL; }
	short FIXED_PREC_SCALE() const                    { return SQL_FALSE; }
	short AUTO_UNIQUE_VALUE() const                   { return NULL; }
	const char * LOCAL_TYPE_NAME() const              { return ""; }
	short MINIMUM_SCALE() const                       { return NULL; }
	short MAXIMUM_SCALE() const                       { return NULL; }
	short SQL_DATETIME_SUB() const                    { return NULL; }
	long  NUM_PREC_RADIX() const                      { return NULL; }
	short INTERVAL_PRECISION() const                  { return NULL; }

};
//---------------------------------------------------------------------------
class CSQLTypeInfoCHAR: public CSQLTypeInfo
{
public:
	CSQLTypeInfoCHAR (short t) : CSQLTypeInfo(t) {}
	CSQLTypeInfoCHAR() : CSQLTypeInfo(SQL_VARCHAR) {}
	const char * TYPE_NAME() const                    { return "CHAR"; }
	long  COLUMN_SIZE() const                         { return 255; }
	const char * LITERAL_PREFIX() const               { return "\""; }
	const char * LITERAL_SUFFIX() const               { return "\""; }
	const char * CREATE_PARAMS() const                { return "length"; }
	short NULLABLE() const                            { return SQL_NULLABLE; }
	short CASE_SENSITIVE() const                      { return SQL_TRUE; }
	short SEARCHABLE() const                          { return SQL_SEARCHABLE; }
	short UNSIGNED_ATTRIBUTE() const                  { return NULL; }
	short FIXED_PREC_SCALE() const                    { return SQL_TRUE; }
	short AUTO_UNIQUE_VALUE() const                   { return SQL_FALSE; }
	const char * LOCAL_TYPE_NAME() const              { return ""; }
	short MINIMUM_SCALE() const                       { return 0; }
	short MAXIMUM_SCALE() const                       { return 0; }
	short SQL_DATETIME_SUB() const                    { return SQL_INTERVAL_HOUR_TO_MINUTE; }
	long  NUM_PREC_RADIX() const                      { return 0; }
	short INTERVAL_PRECISION() const                  { return NULL; }
};
//---------------------------------------------------------------------------
class CSQLTypeInfoVARCHAR: public CSQLTypeInfoCHAR
{
public:
	CSQLTypeInfoVARCHAR() : CSQLTypeInfoCHAR(SQL_VARCHAR) {}
	const char * TYPE_NAME() const                    { return "VARCHAR";  }
	long  COLUMN_SIZE() const                         { return MAXLONG; }
	static const CSQLTypeInfoVARCHAR Inst;
};
//---------------------------------------------------------------------------
class CSQLTypeInfoNUMERIC: public CSQLTypeInfo
{
public:
	CSQLTypeInfoNUMERIC (short t) : CSQLTypeInfo(t) {}
	CSQLTypeInfoNUMERIC() : CSQLTypeInfo(SQL_NUMERIC) {}
	const char * TYPE_NAME() const                    { return "NUMERIC"; }
	long  COLUMN_SIZE() const                         { return 10; }
	const char * LITERAL_PREFIX() const               { return ""; }
	const char * LITERAL_SUFFIX() const               { return ""; }
	const char * CREATE_PARAMS() const                { return ""; }
	short NULLABLE() const                            { return SQL_NULLABLE; }
	short CASE_SENSITIVE() const                      { return SQL_FALSE; }
	short SEARCHABLE() const                          { return SQL_SEARCHABLE; }
	short UNSIGNED_ATTRIBUTE() const                  { return SQL_TRUE; }
	short FIXED_PREC_SCALE() const                    { return SQL_TRUE; }
	short AUTO_UNIQUE_VALUE() const                   { return SQL_FALSE; }
	const char * LOCAL_TYPE_NAME() const              { return ""; }
	short MINIMUM_SCALE() const                       { return 0; }
	short MAXIMUM_SCALE() const                       { return 0; }
	short SQL_DATETIME_SUB() const                    { return NULL; }
	long  NUM_PREC_RADIX() const                      { return 0; }
	short INTERVAL_PRECISION() const                  { return NULL; }
};
//---------------------------------------------------------------------------
class CSQLTypeInfoDATETIME: public CSQLTypeInfo
{
public:
	CSQLTypeInfoDATETIME (short t) : CSQLTypeInfo(t) {}
	CSQLTypeInfoDATETIME() : CSQLTypeInfo(SQL_NUMERIC) {}
	const char * TYPE_NAME() const                    { return "DATETIME"; }
	long  COLUMN_SIZE() const                         { return 10; }
	const char * LITERAL_PREFIX() const               { return ""; }
	const char * LITERAL_SUFFIX() const               { return ""; }
	const char * CREATE_PARAMS() const                { return ""; }
	short NULLABLE() const                            { return SQL_NULLABLE; }
	short CASE_SENSITIVE() const                      { return SQL_FALSE; }
	short SEARCHABLE() const                          { return SQL_SEARCHABLE; }
	short UNSIGNED_ATTRIBUTE() const                  { return SQL_TRUE; }
	short FIXED_PREC_SCALE() const                    { return SQL_TRUE; }
	short AUTO_UNIQUE_VALUE() const                   { return SQL_FALSE; }
	const char * LOCAL_TYPE_NAME() const              { return ""; }
	short MINIMUM_SCALE() const                       { return 0; }
	short MAXIMUM_SCALE() const                       { return 0; }
	short SQL_DATETIME_SUB() const                    { return NULL; }
	long  NUM_PREC_RADIX() const                      { return 0; }
	short INTERVAL_PRECISION() const                  { return NULL; }
};

//---------------------------------------------------------------------------
CSQLGetTypeInfo::CSQLGetTypeInfo (short nType)
{
	column (SQL_VARCHAR,  "TYPE_NAME",          128, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "DATA_TYPE",           30, 0, SQL_NO_NULLS);
	column (SQL_INTEGER,  "COLUMN_SIZE",         30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "LITERAL_PREFIX",      30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "LITERAL_SUFFIX",      30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "CREATE_PARAMS",       30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "NULLABLE",            30, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "CASE_SENSITIVE",      30, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "SEARCHABLE",          30, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "UNSIGNED_ATTRIBUTE",  30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "FIXED_PREC_SCALE",    30, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "AUTO_UNIQUE_VALUE",   30, 0, SQL_NULLABLE);
	column (SQL_VARCHAR,  "LOCAL_TYPE_NAME",     30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "MINIMUM_SCALE",       30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "MAXIMUM_SCALE",       30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "SQL_DATA_TYPE",       30, 0, SQL_NO_NULLS);
	column (SQL_SMALLINT, "SQL_DATETIME_SUB",    30, 0, SQL_NULLABLE);
	column (SQL_INTEGER,  "NUM_PREC_RADIX",      30, 0, SQL_NULLABLE);
	column (SQL_SMALLINT, "INTERVAL_PRECISION",  30, 0, SQL_NULLABLE);

	//SQL_PRED_BASIC  SQL_SEARCHABLE 
	m_aData[SQL_CHAR]     = new CSQLTypeInfoCHAR (SQL_CHAR);
	m_aData[SQL_VARCHAR]  = new CSQLTypeInfoCHAR (SQL_VARCHAR);
	m_aData[SQL_INTEGER]  = new CSQLTypeInfoNUMERIC (SQL_INTEGER);
	m_aData[SQL_FLOAT]    = new CSQLTypeInfoNUMERIC (SQL_FLOAT);
	m_aData[SQL_REAL]     = new CSQLTypeInfoNUMERIC (SQL_REAL);
	m_aData[SQL_DOUBLE]   = new CSQLTypeInfoNUMERIC (SQL_DOUBLE);
	m_aData[SQL_DATETIME] = new CSQLTypeInfoDATETIME (SQL_DATETIME);

	m_aKeys = stdx::keys (m_aData);
}
//---------------------------------------------------------------------------
CSQLGetTypeInfo::~CSQLGetTypeInfo()
{
	std::map<short,CSQLTypeInfo*>::iterator i = m_aData.begin();
	std::map<short,CSQLTypeInfo*>::iterator e = m_aData.end();
	for (; i != e; i++)
	{
		delete i->second;
		const CSQLTypeInfo * pInfo = i->second;
	}
}
//---------------------------------------------------------------------------
ULONG
CSQLGetTypeInfo::rows() const
{
	return m_aData.size();
}
//---------------------------------------------------------------------------
