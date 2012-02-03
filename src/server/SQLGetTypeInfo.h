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

#ifndef __SQLGETTYPEINFO_H__
#define __SQLGETTYPEINFO_H__

#include "CTableImpl.h"
#include <stdx/debug.h>

//---------------------------------------------------------------------------
class CSQLTypeInfo
{
public:
	typedef std::map<short,CSQLTypeInfo*>::iterator iterator;

	CSQLTypeInfo (short t) : m_DATA_TYPE(t)                                  { }
	virtual ~CSQLTypeInfo();
	virtual const char * TYPE_NAME() const = 0;
	virtual short DATA_TYPE() const                                          { return m_DATA_TYPE; }
	virtual long  COLUMN_SIZE() const = 0;
	virtual const char * LITERAL_PREFIX() const = 0;
	virtual const char * LITERAL_SUFFIX() const = 0;
	virtual const char * CREATE_PARAMS() const = 0;
	virtual short NULLABLE() const = 0;
	virtual short CASE_SENSITIVE() const = 0;
	virtual short SEARCHABLE() const = 0;
	virtual short UNSIGNED_ATTRIBUTE() const = 0;
	virtual short FIXED_PREC_SCALE() const = 0;
	virtual short AUTO_UNIQUE_VALUE() const = 0;
	virtual const char * LOCAL_TYPE_NAME() const = 0;
	virtual short MINIMUM_SCALE() const = 0;
	virtual short MAXIMUM_SCALE() const = 0;
	virtual short SQL_DATA_TYPE() const                                      { return m_DATA_TYPE; };
	virtual short SQL_DATETIME_SUB() const = 0;
	virtual long  NUM_PREC_RADIX() const = 0;
	virtual short INTERVAL_PRECISION() const = 0;

protected:
	short m_DATA_TYPE;
};
//---------------------------------------------------------------------------
// SQLGetTypeInfo QUERY
//---------------------------------------------------------------------------
class CSQLGetTypeInfo: public CTableImpl
{
public:

	CSQLGetTypeInfo (short type);
	virtual ~CSQLGetTypeInfo();

	void clear (CRecord &) const {};
	bool read (ULONG row, CRecord &) const { return false; }

	bool read (ULONG iRow, ULONG nRow, idl::typRecord &) const;
	ULONG rows() const;

	CSQLTypeInfo * get (short t)                      { return m_aData[t]; }
	CSQLTypeInfo * operator[](short t)                { return m_aData[t]; }

protected:
	std::map<short,CSQLTypeInfo*> m_aData;
	vector<short> m_aKeys;
};
//---------------------------------------------------------------------------
#endif
