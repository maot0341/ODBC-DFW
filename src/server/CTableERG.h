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

#ifndef __CTABLEERG_H__
#define __CTABLEERG_H__


#include "CStorage.h"
#include "CTableImpl.h"

#define TPPRO_VERF 4
time_t UTCTime (size_t);
//---------------------------------------------------------------------------
// CTableERG
//---------------------------------------------------------------------------
class CTableERG: public CTableImpl
{
public:
	struct record_t
	{
		unsigned long id;
		string vchar;
		time_t time;
		long   nval;
		double dval;
	};

	CTableERG (const char * szPath);

	ULONG rows() const;
	bool read (ULONG row, CRecord &) const;
	bool read (ULONG iRow, ULONG nRow, idl::typRecord &) const;

	bool get (vector<ULONG> & raIndex, int nHeader, int nCol, CTerm* pValue) const;

protected:
	ULONG rows (const tppro::CStorage::typItem * pItem) const;

	mutable tppro::CStorage m_aFile;
	mutable ULONG m_nRows;
};
//---------------------------------------------------------------------------
#endif
