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

#ifndef __CTABLETST_H__
#define __CTABLETST_H__


#include "CTableImpl.h"
//---------------------------------------------------------------------------
// CTableTST
//---------------------------------------------------------------------------
class CTableTST: public CTableImpl
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

	CTableTST (const char * name);

	ULONG rows() const;
	bool read (ULONG row, CRecord &) const;
	bool read (ULONG iRow, ULONG nRow, idl::typRecord &) const;

	bool get (vector<ULONG> & raIndex, int nHeader, int nCol, CTerm* pValue) const;

//	string m_strCatalog;
	std::vector<record_t> m_aData;
	ULONG m_nRows;
};
//---------------------------------------------------------------------------
#endif
