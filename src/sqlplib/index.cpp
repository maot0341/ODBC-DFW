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

#include <assert.h>
#include "yac.h"
#include "sqlp.h"
#include "index.h"

using namespace std;
namespace idx {

//---------------------------------------------------------------------------
// index operations
//---------------------------------------------------------------------------
ULONG rows (ULONG nSize, const ULONG * aIndex)
{
	assert (nSize % 2 == 0);
	ULONG nRows = 0;
	const ULONG * r;
	const ULONG * e = aIndex + nSize; 
	for (r=aIndex; r<e; r+=2)
	{
		nRows += r[1] - r[0];
	}
	return nRows;
}
//---------------------------------------------------------------------------
ULONG rows (const vector<ULONG> & raIndex)
{
	assert (raIndex.size() % 2 == 0);
	ULONG nRows = 0;
	const ULONG * r;
	for (r=raIndex.begin(); r<raIndex.end(); r+=2)
	{
		nRows += r[1] - r[0];
	}
	return nRows;
}
//---------------------------------------------------------------------------
ULONG item (const vector<ULONG> & raIndex, ULONG i)
{
	assert (raIndex.size() % 2 == 0);
	ULONG nRows = 0;
	const ULONG * r;
	for (r=raIndex.begin(); r<raIndex.end(); r+=2)
	{
		const ULONG n = r[1] - r[0];
		if (i < n)
			return r[0] + i;
		i -= n;
	}
	return nRows;
}
//---------------------------------------------------------------------------
vector<ULONG> & invert (vector<ULONG> & raIndex, ULONG nLast)
{
	ULONG i;
	if (!raIndex.size() || raIndex[0])
		raIndex.insert (raIndex.begin(), 0);
	else
		raIndex.erase (raIndex.begin());
	raIndex.push_back (nLast);
	const ULONG n = raIndex.size();
	for (i=2; i<n; i+=2)
		raIndex[i]++;
	return raIndex;
}
//---------------------------------------------------------------------------
vector<ULONG> & operator &= (vector<ULONG> & i1, const vector<ULONG> & i2)
{

	ULONG r[2];
	vector<ULONG> aIndex;
	const ULONG * a = i1.begin();
	const ULONG * b = i2.begin();
	vector<ULONG>::const_iterator e1 = i1.end();
	vector<ULONG>::const_iterator e2 = i2.end();
	while (a<e1 && b<e2)
	{
		//// intersection
		r[0] = a[0] < b[0] ? b[0] : a[0]; // max!
		r[1] = a[1] < b[1] ? a[1] : b[1]; // min!
		if (r[0] < r[1])
		{
			aIndex.push_back (r[0]);
			aIndex.push_back (r[1]);
		}
		if (a[1] < b[1])
			a += 2;
		else
			b += 2;
	}
	i1 = aIndex;
	return i1;
}
//---------------------------------------------------------------------------
vector<ULONG> & operator |= (vector<ULONG> & i1, const vector<ULONG> & i2)
{
	ULONG r[2];
	vector<ULONG> aIndex;
	const ULONG * a = i1.begin();
	const ULONG * b = i2.begin();
	vector<ULONG>::const_iterator e1 = i1.end();
	vector<ULONG>::const_iterator e2 = i2.end();
	while (a < e1 && b < e2)
	{
		//// intersection
		r[0] = a[0] < b[0] ? b[0] : a[0]; // max!
		r[1] = a[1] < b[1] ? a[1] : b[1]; // min!
		if (r[0] <= r[1])
		{
			r[0] = a[0] < b[0] ? a[0] : b[0]; // min!
			r[1] = a[1] < b[1] ? b[1] : a[1]; // max!
			aIndex.push_back (r[0]);
			aIndex.push_back (r[1]);
			a += 2;
			b += 2;
		}
		else if (a[1] < b[0])
		{
			aIndex.push_back (a[0]);
			aIndex.push_back (a[1]);
			a += 2;
		}
		else if (b[1] < a[0])
		{
			aIndex.push_back (b[0]);
			aIndex.push_back (b[1]);
			b += 2;
		}
	}
	if (a < i1.end())
		aIndex.insert (aIndex.end(), a, i1.end());
	if (b < i2.end())
		aIndex.insert (aIndex.end(), b, i2.end());
	i1 = aIndex;
	return i1;
}
//---------------------------------------------------------------------------
vector<ULONG> & append (vector<ULONG> & raIndex, ULONG i, ULONG n)
{
	const ULONG j = i + n;
	if (raIndex.size())
	if (i <= raIndex.back())
	{
		ULONG & raBack = raIndex.back();
		if(raBack < j)
			raBack = j;
		return raIndex;
	}
	raIndex.push_back (i);
	raIndex.push_back (j);
	return raIndex;
}
//---------------------------------------------------------------------------
vector<ULONG> & remove (vector<ULONG> & raIndex, ULONG i, ULONG n)
{
	if (!raIndex.size())
		return raIndex;
	if (n < 1)
		return raIndex;
	ULONG j = i + n + 1;
	ULONG e = raIndex.back();
	if (e <= j)
		e = j + 1;
	//// a = inverse (i,i+n)
	vector<ULONG> a(4);
	a[0] = 0;
	a[1] = i;
	a[2] = j;
	a[3] = e;
	raIndex &= a;
	return raIndex;
}
//---------------------------------------------------------------------------
vector<ULONG> & insert (vector<ULONG> & raIndex, ULONG i, ULONG n)
{
	if (n < 1)
		return raIndex;
	vector<ULONG> a(2);
	a[0] = i;
	a[1] = i + n;
	raIndex |= a;
	return raIndex;
}
//---------------------------------------------------------------------------
// convert functions
//---------------------------------------------------------------------------
vector<ULONG> any2sub (const vector<ULONG> & aSrc)
{
	vector<ULONG> aDst;
	ULONG i, j, n = aSrc.size();
	for (i=0; i<n; i++)
	{
		j = aSrc[i];
		insert (aDst, j);
	}
	return aDst;
}
//---------------------------------------------------------------------------
vector<ULONG> idx2sub (const vector<ULONG> & aSrc)
{
	vector<ULONG> aDst;
	ULONG i, j, n = aSrc.size();
	for (i=0; i<n; i++)
	{
		j = aSrc[i];
		append (aDst, j);
	}
	return aDst;
}
//---------------------------------------------------------------------------
vector<ULONG> sub2idx (const vector<ULONG> & aSrc)
{
	vector<ULONG> aDst;
	vector<ULONG>::const_iterator r = aSrc.begin();
	vector<ULONG>::const_iterator e = aSrc.end();
	if (r == e)
		return aDst;
	ULONG i, n = aSrc.back();
	aDst.reserve (n);
	for (; r != e; r+=2)
	{
		i = r[0];
		n = r[1];
		for (; i<n; i++)
		{
			aDst.push_back(i);
		}
	}
	return aDst;
}
//---------------------------------------------------------------------------
// iterator
//---------------------------------------------------------------------------
iterator::iterator()
{
	m_pIndex = 0;
	m_iRecord = 0;
}
//---------------------------------------------------------------------------
iterator::iterator (const iterator & aIterator)
{
	m_pIndex = aIterator.m_pIndex;
	m_iRecord = aIterator.m_iRecord;
	m_iRange = aIterator.m_iRange;
	m_eRange = aIterator.m_eRange;
}
//---------------------------------------------------------------------------
iterator::iterator (const vector<ULONG> & aIndex)
{
	m_pIndex = &aIndex;
	m_iRange = aIndex.begin();
	m_eRange = aIndex.end();
	m_iRecord = 0;
	reset();
}
//---------------------------------------------------------------------------
void
iterator::assign (const vector<ULONG> & aIndex)
{
	ULONG n = aIndex.size();
	assert ((n % 2) == 0);
	m_pIndex = &aIndex;
	m_iRange = aIndex.begin();
	m_eRange = aIndex.end();
	m_iRecord = 0;
	reset();
}
//---------------------------------------------------------------------------
void
iterator::reset()
{
	m_iRecord = 0;
	if (!m_pIndex)
		return;
	m_iRange = m_pIndex->begin();
	if (m_iRange == m_eRange)
		return;
	m_iRecord = m_pIndex->front();
}
//---------------------------------------------------------------------------
ULONG
iterator::count() const
{
	if (!m_pIndex)
		return 0;
	return rows (*m_pIndex);
}
//---------------------------------------------------------------------------
ULONG
iterator::operator++()
{
	assert (m_pIndex);
	vector<ULONG>::const_iterator e = m_eRange; 
	vector<ULONG>::const_iterator & a = m_iRange;
	assert (a && e);
	if (a == e)
		return 0;
	ULONG i = m_iRecord + 1;
	if (i < a[1])
		return m_iRecord = i;
	a += 2;
	if (a == e)
		return 0;
	m_iRecord = a[0];
	return m_iRecord;
}
//---------------------------------------------------------------------------
ULONG
iterator::operator--()
{
	assert (m_pIndex);
	vector<ULONG>::const_iterator e = m_eRange; 
	vector<ULONG>::const_iterator & a = m_iRange;
	assert (a && e);
	if (a == e)
		return 0;
	if (m_iRecord == 0)
		return 0;
	ULONG i = m_iRecord - 1;
	if (i > a[0])
		return --m_iRecord;
	a -= 2;
	if (a == e)
		return 0;
	m_iRecord = a[1];
	return m_iRecord;
}
//---------------------------------------------------------------------------
ULONG
iterator::operator++(int)
{
	ULONG i = m_iRecord;
	operator++();
	return i;
}
//---------------------------------------------------------------------------
ULONG
iterator::operator--(int)
{
	ULONG i = m_iRecord;
	operator--();
	return i;
}
//---------------------------------------------------------------------------
bool
iterator::valid() const
{
	if (!m_pIndex)
		return false;
	if (m_iRange == m_eRange)
		return false;
	return true;
	const vector<ULONG>::const_iterator & a = m_iRange;
	const ULONG i = m_iRecord;
	return (a[0] <= i && i < a[1]);
}
//---------------------------------------------------------------------------
bool
iterator::seek (ULONG i)
{
	if (!m_pIndex)
		return false;
	vector<ULONG>::const_iterator e = m_eRange; 
	vector<ULONG>::const_iterator & a = m_iRange;
	a = m_pIndex->begin();
	for (; a!=e; a+=2)
	if (a[0] <= i && i < a[1])
		break;
	if (a == e)
		return false;
	m_iRecord = i;
	return true;
}
//---------------------------------------------------------------------------
} // namespace