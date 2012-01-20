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
#include "table.h"
#include "sqlplib/index.h"

using namespace std;

//---------------------------------------------------------------------------
// Subset
//---------------------------------------------------------------------------
CTableSubset::CTableSubset (ITable * pData)
{
	assert (pData);
	m_pData = pData;
	m_nRows = pData->rows();
	m_aIndex.reserve (m_nRows);
	m_aIndex.push_back (0);
	m_aIndex.push_back (m_nRows);
}
//---------------------------------------------------------------------------
size_t 
CTableSubset::rows() const
{
	return idx::rows (m_aIndex);
}
//---------------------------------------------------------------------------
void 
CTableSubset::fetch (size_t i, CRecord & raRecord) const
{
	assert (i < m_nRows);
	i = idx::item (m_aIndex, i);
	m_pData->fetch (i, raRecord);
}
//---------------------------------------------------------------------------
// Cursor
//---------------------------------------------------------------------------
SQLRETURN 
CCursor::fetch()
{
	if (m_iRecord >= m_nRecord)
		return SQL_NO_DATA;
	return fetch (m_iRecord++);
}
//---------------------------------------------------------------------------
SQLRETURN 
CTableCursor::fetch (size_t i)
{
	if (i >= m_nRecord)
		return SQL_NO_DATA;
	m_pData->fetch (i, m_aRecord);
	m_iRecord = i;
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
// Join
//---------------------------------------------------------------------------
CTableJoin::CTableJoin()
{
	m_iRecord = 0;
	m_nRecord = 0;
}
//---------------------------------------------------------------------------
size_t 
CTableJoin::rows() const
{
	size_t nTable = m_aTable.size();
	size_t nIndex = m_aIndex.size();
	if (!nTable)
		return 0;
	return nIndex / nTable;
}
//---------------------------------------------------------------------------
size_t
CTableJoin::cols() const
{
	if (m_aFields.empty())
		desc (m_aFields);
	return m_aFields.size();
}
//---------------------------------------------------------------------------
void
CTableJoin::add (ITable * pTable)
{
	if (!pTable)
		return;
	m_aFields.clear();
	CTableCursor aCursor(pTable);
	m_aTable.push_back (aCursor);
}
//---------------------------------------------------------------------------
template<class T>
vector<T> &
operator+= (vector<T> & dst, const vector<T> & src)
{
	vector<T>::iterator i = dst.end();
	vector<T>::const_iterator a = src.begin();
	vector<T>::const_iterator b = src.end();
	dst.insert (i, a, b);
	return dst;
}
//---------------------------------------------------------------------------
void
CTableJoin::desc (CTableDef & aDef) const
{
	aDef = defs();
}
//---------------------------------------------------------------------------
const CTableDef &
CTableJoin::defs() const
{ 
	const int n = m_aTable.size();
	int i;
	if (m_aFields.empty())
	for (i=0; i<n; i++)
	{
		ITable * pTable = m_aTable[i].m_pData;
		assert (pTable);
		const CTableDef & aItem = pTable->defs();
		int m = aItem.size();
		m_aFields += aItem;
	}
	return m_aFields; 
}
//---------------------------------------------------------------------------
SQLRETURN
CTableJoin::fetch (size_t i)
{
	size_t nRows = rows();
	if (i >= nRows)
		return SQL_NO_DATA;
	m_iRecord = i;
	size_t n = m_aTable.size();
	assert (n > 0);
	vector<size_t>::const_iterator iRecord = m_aIndex.begin() + (n * i);
	for (i=0; i<n; i++)
	{
		CCursor & aTable = m_aTable[i];
		size_t j = *(iRecord + i);
		aTable.fetch (j);
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
