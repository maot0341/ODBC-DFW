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

#ifndef INDEX_H
#define INDEX_H

#include <windows.h>
#include <vector>
//#include "sqlp.h"

using namespace std;
namespace idx {
//---------------------------------------------------------------------------
ULONG rows (ULONG length, const ULONG * index);
ULONG rows (const vector<ULONG> & raIndex);
ULONG item (const vector<ULONG> & raIndex, ULONG i);
vector<ULONG> & invert (vector<ULONG> & raIndex, ULONG nLast);
vector<ULONG> & operator &= (vector<ULONG> & i1, const vector<ULONG> & i2);
vector<ULONG> & operator |= (vector<ULONG> & i1, const vector<ULONG> & i2);
vector<ULONG> & append (vector<ULONG> & raIndex, ULONG i, ULONG n=1);
vector<ULONG> & remove (vector<ULONG> & raIndex, ULONG i, ULONG n=1);
vector<ULONG> & insert (vector<ULONG> & raIndex, ULONG i, ULONG n=1);

vector<ULONG> & assigne (vector<ULONG> & raIndex, ULONG i, ULONG n);
vector<ULONG> & assigne (vector<ULONG> & raIndex, ULONG i);

vector<ULONG> any2sub (const vector<ULONG> &);
vector<ULONG> idx2sub (const vector<ULONG> &);
vector<ULONG> sub2idx (const vector<ULONG> &);
//---------------------------------------------------------------------------
class iterator
{
public:
	iterator();
	iterator (const iterator&);
	iterator (const vector<ULONG> &);
	void operator= (const vector<ULONG> & idx) { assign (idx); }
	void assign (const vector<ULONG> &);
	ULONG count() const;
	ULONG operator++();
	ULONG operator--();
	ULONG operator++(int);
	ULONG operator--(int);
	ULONG operator()() const { return m_iRecord; }
	ULONG get() const { return m_iRecord; }
	bool valid() const;
	void reset();
	bool seek (ULONG);
	ULONG operator[](int i) const { return m_iRange[i]; }
protected:
	const vector<ULONG> * m_pIndex;
	vector<ULONG>::const_iterator m_iRange;
	vector<ULONG>::const_iterator m_eRange;
	ULONG m_iRecord;
};
//---------------------------------------------------------------------------

} // namespace
#endif
