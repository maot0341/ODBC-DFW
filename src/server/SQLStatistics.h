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

#ifndef __SQLSTATISTICS_H__
#define __SQLSTATISTICS_H__

#include "CTableImpl.h"
#include <stdx/debug.h>


//---------------------------------------------------------------------------
// SQLTables QUERY
//---------------------------------------------------------------------------
class CSQLStatistics: public CTableImpl
{
public:
	CSQLStatistics 
	( const char * catalog
	, const char * schema
	, const char * table
	, short nUnique
	, short nReserved);

	virtual ~CSQLStatistics();

	virtual void clear (CRecord &) const {};
	virtual bool read (ULONG row, CRecord &) const { return false; }
	virtual const char* col_name (ULONG i) const { return 0; }

	
	bool read (ULONG iRow, ULONG nRow, idl::typRecord &) const;

	ULONG rows() const;

	string m_strCatalog;
	string m_strSchema;
	string m_strTable;
	short m_nUnique;
	short m_nReserved;

//	static CSQLStatistics ms_aInstance;
};
//---------------------------------------------------------------------------
#endif
