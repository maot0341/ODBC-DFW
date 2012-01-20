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

#ifndef __SQLCOLUMNS_H__
#define __SQLCOLUMNS_H__

#include <stdx/debug.h>
#include <vector>
#include <string>

#include "CTableImpl.h"
//---------------------------------------------------------------------------
// CSQLColumns QUERY
//---------------------------------------------------------------------------
class CSQLColumns: public CTableImpl
{
public:
	struct record_t
	{
		string catalog;
		string schema;
		string table;
		string column;
		short  datatype;
		string type_name;
		long   size;
		long   length;
		short  digits;
		short  num_prec_radix;
		short  nullable;
		string remarks;
		string column_def;
		short  sql_data_type;
		short  sql_datetime_sub;
		long   char_octet_length;
		long   ordinal_position;
		const char * is_nullable;
	};

	CSQLColumns (const char * catalog
	, const char * schema
	, const char *table
	, const char *column
	);
	~CSQLColumns();

	void record (const char * catalog
	, const char * schema
	, const char *table
	, const char *column
	, short  datatype
	, const char *  type_name
	, long size
	, long lentgh
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
	);

	void clear (CRecord &) const {};
	bool read (ULONG row, CRecord &) const { return false; }

	bool read (ULONG iRow, ULONG nRow, idl::typRecord &) const;
	ULONG rows() const;

	string m_strCatalog;
	string m_strSchema;
	string m_strTable;
	string m_strColumn;

	std::vector<record_t> m_aData;

};
//---------------------------------------------------------------------------
#endif
