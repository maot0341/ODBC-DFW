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

#include "sqlp.h"
#include "tableset.h"
#include "index.h"
#include "yac.h"
#include <stdx/utils.h>
#include <assert.h>
#include <algorithm>

using namespace std;
using namespace idx;
using namespace stdx;
namespace sqlp {

//---------------------------------------------------------------------------
enum cond_t {CondFALSE=0, CondTRUE, CondUNDEF, CondTYP};
//---------------------------------------------------------------------------
cond_t Condition (CTerm * pTerm, const CDomain & aDomain)
{
	CValue * pValue = dynamic_cast<CValue*>(pTerm);
	if (pValue)
	{
		return pValue->asBool() ? CondTRUE : CondFALSE;
	}
	CColumn * pColumn = dynamic_cast<CColumn*>(pTerm);
	if (pColumn)
	{
		if (aDomain.empty())
			return CondUNDEF;
		const char * szTable = pColumn->table();
		if (szTable)
		if (aDomain.find (szTable) == aDomain.end())
			return CondUNDEF;
		return pColumn->isTrue() ? CondTRUE : CondFALSE;
	}
	CUnary * pUnary = dynamic_cast<CUnary*>(pTerm);
	if (pUnary)
	{
		short nHead = pUnary->head();
		CTerm * pTerm = pUnary->m_pTerm;
		cond_t eCond = Condition (pTerm, aDomain);
		if (eCond == CondUNDEF)
			return eCond;
		if (nHead == lNOT)
			return pTerm->asBool() ? CondFALSE : CondTRUE;
		if (nHead == lNULL)
			return pTerm->isNull() ? CondTRUE : CondFALSE;
		return pTerm->asBool() ? CondTRUE : CondFALSE;
	}
	CFunction * pFunc = dynamic_cast<CFunction*>(pTerm);
	if (pFunc)
	{
		short nHead = pFunc->head();
		const vector<CTerm*> & aArgs = pFunc->args();
		const int nArgs = aArgs.size();
		cond_t eCond;
		int i;
		if (nHead == lAND)
		{
			for (i=0; i<nArgs; i++)
			{
				CTerm * pTerm = pFunc->arg(i);
				eCond = Condition (pTerm, aDomain);
				if (eCond == CondUNDEF)
					return eCond;
				if (eCond == CondFALSE)
					return eCond;
			}
			return CondTRUE;
		}
		if (nHead == lOR)
		{
			int nFalse = 0;
			for (i=0; i<nArgs; i++)
			{
				CTerm * pTerm = pFunc->arg(i);
				eCond = Condition (pTerm, aDomain);
				if (eCond == CondTRUE)
					return eCond;
				if (eCond == CondFALSE)
					nFalse++;
			}
			return (nFalse == nArgs) ? CondFALSE : CondUNDEF;
		}

		for (i=0; i<nArgs; i++)
		{
			CTerm * pTerm = aArgs[i];
			cond_t eCond = Condition (pTerm, aDomain);
			if (eCond == CondUNDEF)
				return eCond;
		}
		return pFunc->isTrue() ? CondTRUE : CondFALSE;
	}
	return CondUNDEF;
}
//---------------------------------------------------------------------------
inline
const char *
column1 (const char * szTable, CTerm * pTerm)
{
	assert (szTable);
	CColumn * pColumn = dynamic_cast<CColumn*>(pTerm);
	if (!pColumn)
		return 0;
	const char * szName = pColumn->table();
	if (szName)
	if (stricmp (szName, szTable))
		return 0;
	return pColumn->column();
}
//---------------------------------------------------------------------------
vector<ULONG>
filter1 (const char * szTable, CTable * pTable, CTerm * pTerm, const CDomain & aDomain)
{
	assert (pTable);
	vector<ULONG> aIndex;
	if (pTerm == 0)
		return idx::append (aIndex, 0, pTable->rows());
	CFunction * pFunc = dynamic_cast<CFunction*>(pTerm);
	if (pFunc)
	{
		int nHead = pFunc->head();
		if (pFunc->isRelation())
//		if (nHead == '=')
		{
			CDomain  d0(aDomain);
			d0.erase (szTable);
			CTerm * t0 = pFunc->arg(0);
			CTerm * t1 = pFunc->arg(1);
			assert (t0);
			assert (t1);
			ULONG nRows = pTable->rows();
			ULONG nCols = pTable->cols();
//			CColumn * c0 = dynamic_cast<CColumn*>(t0);
//			CColumn * c1 = dynamic_cast<CColumn*>(t1);
			const char * c0 = column1 (szTable, t0);
			const char * c1 = column1 (szTable, t1);
			const cond_t e0 = Condition(t0, d0);
			const cond_t e1 = Condition(t1, d0);
			const char * cf = 0;
			CTerm * tf = 0;
			if (c0 && e0 == CondUNDEF && e1 != CondUNDEF)
			{
				cf = c0;
				tf = t1;
			}
			if (c1 && e1 == CondUNDEF && e0 != CondUNDEF)
			{
				cf = c1;
				tf = t0;
			}
			if (pTable->get (aIndex, nHead, cf, tf))
			{
				return aIndex;
			}
			if (nHead == lNEQ || nHead == lLEQ || nHead == lGEQ)
			if (pTable->get (aIndex, '=', cf, tf))
			{			
				vector<ULONG> aSub;
				if (nHead == lNEQ)
					return idx::invert (aIndex, nRows);
				if (nHead == lLEQ)
				if (pTable->get (aSub, '<', cf, tf))
					return aIndex |= aSub;
				if (nHead == lGEQ)
				if (pTable->get (aSub, '>', cf, tf))
					return aIndex |= aSub;
			}
			idx::assigne (aIndex, 0, nRows);
			return aIndex;
		}
		if (nHead == lIN)
		{
			CDomain  d0(aDomain);
			d0.erase (szTable);
			CTerm * t0 = pFunc->arg(0);
			assert (t0);
			const char * c0 = column1 (szTable, t0);
			cond_t e0 = Condition(t0, d0);
			int i = 0;
			int n = pFunc->args().size();
			if (c0 && e0 == CondUNDEF)
			for (i=1; i<n; i++)
			{
				CTerm * t1 = pFunc->arg(i);
				assert (t1);
				const char * c1 = column1 (szTable, t1);
				cond_t e1 = Condition(t1, d0);
				vector<ULONG> aItem;
				if (!pTable->get (aItem, '=', c0, t1))
					break;
				aIndex |= aItem;
			}
			else
			for (i=1; i<n; i++)
			{
				CTerm * t1 = pFunc->arg(i);
				assert (t1);
				const char * c1 = column1 (szTable, t1);
				vector<ULONG> aItem;
				if (!pTable->get (aItem, '=', c1, t0))
					break;
				aIndex |= aItem;
			}
			if (i >= n)
				return aIndex;
			ULONG nRows = pTable->rows();
			aIndex.clear();
			idx::append (aIndex, 0, nRows);
			return aIndex;
		}
		if (nHead == lAND)
		{
			int i, n = pFunc->args().size();
			for (i=0; i<n; i++)
			{
				CTerm * pTerm = pFunc->arg(i);
				vector<ULONG> aItem = filter1 (szTable, pTable, pTerm, aDomain);
				if (i == 0)
					aIndex = aItem;
				else
					aIndex &= aItem;
			}
			return aIndex;
		}
		if (nHead == lOR)
		{
			int i, n = pFunc->args().size();
			for (i=0; i<n; i++)
			{
				CTerm * pTerm = pFunc->arg(i);
				vector<ULONG> aItem = filter1 (szTable, pTable, pTerm, aDomain);
				if (i == 0)
					aIndex = aItem;
				else
					aIndex |= aItem;
			}
			return aIndex;
		}
	}
	return idx::assigne (aIndex, 0, pTable->rows());
}
//---------------------------------------------------------------------------
// table-reference - item
//---------------------------------------------------------------------------
struct CTableSet::item
{
	CTableSet*     parent;
	const char*    name;
	BYTE *         outer;
	CTable *       table;
	vector<CValue> record;
	CDomain        domain;
	idx::iterator  iter;

	vector<ULONG> s0; // static subset
	vector<ULONG> ss; // dynamic subset
	ULONG         i;  // current record
	ULONG         n;  // number of records (=count(ss))

};
//---------------------------------------------------------------------------
CTableSet::CTableSet (CStatement & aStatement)
: m_aStatement (aStatement)
{
	m_nRecord = 0;
	m_iRecord = 0;
	m_pFilter = 0;
	m_pTrace = 0;
	m_aJoins = aStatement.m_aJoins;
	m_iOTab = 0;
	m_iORow = 0;

	CTableMap & aTableDict = aStatement.m_aTables;
	CTableMap::const_iterator iTables = aTableDict.begin();
	for (; iTables != aTableDict.end(); iTables++)
	{
		const char * szAlias = iTables->first;
		sqlp::CTable * pTable = iTables->second;
		add (szAlias, pTable);
	}
	const int nItems = m_aItems.size();
	int i;

	//// build header
	vector<CObject*> & aObjects = aStatement.m_aMemory;
	const int nObjects = aObjects.size();
	for (i=0; i<nObjects; i++)
	{
		CColumn * pColumn = dynamic_cast<CColumn*>(aObjects[i]);
		if (!pColumn || !pColumn->column())
			continue;
		m_aTerms.push_back (pColumn);
	}

	aStatement.m_pResult = this;
//	m_pTrace = fopen ("c:\\temp\\CTableSet.log", "wt");
}
//---------------------------------------------------------------------------
CTableSet::~CTableSet()
{
	if (m_pTrace)
		fclose (m_pTrace);
	int i;
	const int nItems = m_aItems.size();
	for (i=0; i<nItems; i++)
	{
		item * pItem = m_aItems[i];
		delete [] pItem->outer;
		delete pItem;
	}
}
 //---------------------------------------------------------------------------
ULONG
CTableSet::rows() const 
{	
	const ULONG n = m_aItems.size();
	const ULONG m = m_aIndex.size();
	if (n == 0)
		return 0;
	if (n == 1)
		return m_aItems[0]->n;
	if (m == 0)
		return m_nRecord;
	return m / n;
}
//---------------------------------------------------------------------------
CTableSet::item*
CTableSet::table (const char * szName)
{
	int i = 0;
	const int n = m_aItems.size();
	assert (n);
	if (szName && *szName)
	for (i=0; i<n; i++)
	{
		item * pItem = m_aItems[i];
		assert (pItem);
		const char * szItem = pItem->name;
		if (stricmp (szName, szItem) == 0)
			return pItem;
	}
	assert (n == 1);
	return m_aItems[0];
}
//---------------------------------------------------------------------------
int
CTableSet::column (CTable * pTable, const char * szColumn)
{
	assert (pTable);
	return pTable->colidx(szColumn);
}
//---------------------------------------------------------------------------
// Tabellen - Referenz hinzu
//---------------------------------------------------------------------------
void
CTableSet::add (const char * szAlias, sqlp::CTable * pTable)
{
	assert (pTable);
	assert (szAlias && *szAlias);
	item * pItem = new item;
	assert (pItem);
	CRecord & raRecord = pItem->record;
	pItem->parent = this;
	pItem->name = szAlias;
	pItem->table = pTable;
	pItem->outer = 0;
	pTable->clear (raRecord);
	m_aItems.push_back (pItem);
}
//---------------------------------------------------------------------------
bool
CTableSet::fetch (item & raItem, ULONG i)
{
	if (!raItem.iter.seek(i))
		return false;
	raItem.i = i;
	CTable * pTable = raItem.table;
	assert (pTable);
	CRecord & raRecord = raItem.record;
	return pTable->read (i, raRecord);
}
//---------------------------------------------------------------------------
bool
CTableSet::move (ULONG iRec)
{
	int nItem = m_aItems.size();
	if (!nItem)
		return false;
	m_iRecord = iRec;
	ULONG nRec = m_aIndex.size() / nItem;
	int i;
	//// index cache
	if (iRec < nRec)
	{
		vector<ULONG>::const_iterator aIndex = m_aIndex.begin() + iRec * nItem;
		for (i=0; i<nItem; i++)
		{
			item * pItem = m_aItems[i];
			ULONG j = aIndex[i];
			pItem->iter.seek(j);
			fetch (*pItem);
//			fetch (*pItem, j);
		}
		return true;
	}
	//// full table scan
	else
	{
		if (iRec == 0)
		if (zero(nItem))
			return true;
		while (next())
		if (iRec + 1 == m_iRecord)
			return true;
//		if (iRec+1 == m_aIndex.size() / nItem)
//			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
// init records (null)
//---------------------------------------------------------------------------
void
CTableSet::setnull()
{
	int i,j;
	const int nItems = m_aItems.size();
	for (i=0; i<nItems; i++)
	{
		item & aItem = *m_aItems[i];
		const int nCols = aItem.record.size();
		for (j=0; j<nCols; j++)
		{
			aItem.record[j].setNull();
		}
	}
}
//---------------------------------------------------------------------------
// init. outer join marker
//---------------------------------------------------------------------------
void
CTableSet::outer (const char * szTable)
{
	assert(szTable);
	item * pItem = table (szTable);
	assert (pItem);
	CTable * pTable = pItem->table;
	assert (pTable);
	const ULONG nRow = pTable->rows();
	assert (!pItem->outer);
	pItem->outer = new BYTE[nRow];
	memset (pItem->outer, false, nRow);
}
//---------------------------------------------------------------------------
bool less_then_item (CTableSet::item * i1, CTableSet::item * i2)
{
#if 0
	if (!i1->outer && i2->outer)
		return true;
	if (i1->outer && !i2->outer)
		return false;
#endif
	return i1->n > i2->n;
}
//---------------------------------------------------------------------------
// create recordset
//---------------------------------------------------------------------------
void
CTableSet::open (CTerm * pFilter)
{
	m_pFilter = pFilter;
	ULONG nOuter = 0;
	const int nItems = m_aItems.size();
	const int nJoins = m_aJoins.size();
	m_aIndex.clear();
	int i;
	/*-----------------------------------------
	todo:
	1) Statisches Filtern der einzelnen Tabellen
	   - quick filter
	   - seq. filter
	   ==> Index[Tab]
	2) Hilstabelle (Carthesian) der Joins erstellen:
	   Index[0] x Index[1] x ... x Index[n]
	   ==> Super Set
	3) Dynamisches Filtern des Cathesian:
	   - fetch[0]  ==> filter[0]-fix
	   - lookup[1] ==> filter[1]-fix
	   - lookup[2] ==> filter[2]-fix
	   ...
	   ==> Index des Cartesian

	-----------------------------------------*/

	////-----------------------------------------
	//// init i,n for each item
	////-----------------------------------------
	for (i=0; i<nItems; i++)
	{
		item * pItem = m_aItems[i];
		assert (pItem);
		CTable * pTable = pItem->table;
		assert (pTable);
		pItem->n = pTable->rows();
		const char * szName = pItem->name;
		if (szName)
		{
			CDomain aDomain;
			aDomain.insert (pItem->name);
			pItem->s0 = filter1 (pItem->name, pItem->table, m_pFilter, aDomain);
			pItem->n = idx::rows (pItem->s0);
		}
//		filter (pItem);
//		pItem->i = 0;
//		pItem->n = pItem->index.size();
		pItem->iter.assign (pItem->s0);
	}
	////-----------------------------------------
	//// joins
	////-----------------------------------------
	const int nJoin = m_aJoins.size();
	for (i=0; i<nJoin; i++)
	{
		const CJoin * pJoin = m_aJoins[i];
		assert (pJoin);
		const char * szTable = 0;
		switch (pJoin->m_nHead)
		{
		case lINNER:
			break;
		case lLEFT:
			outer (pJoin->m_szLeft);
			break;
		case lRIGHT:
			outer (pJoin->m_szRight);
			break;
		case lFULL:
			outer (pJoin->m_szLeft);
			outer (pJoin->m_szRight);
			break;
		default:
			assert (false);
		}
	}
	std::sort (m_aItems.begin(), m_aItems.end(), less_then_item);
	////-----------------------------------------
	//// bind columns
	////-----------------------------------------
	const int nTerm = m_aTerms.size();
	for (i=0; i<nTerm; i++)
	{
		CColumn * pColumn = dynamic_cast<CColumn*>(m_aTerms[i]);
		if (!pColumn || !pColumn->column())
			continue;
		const char * szTable = pColumn->table();
		const char * szColumn = pColumn->column();
		item * pItem = table (szTable);

		CTable * pTable = pItem->table;
		const int nCols = pTable->cols();
		const int j = column (pTable, szColumn);
		if (j >= nCols)
			throw EXC("42S22", 900, "Column not found: [%s].", szColumn);
		assert (j < nCols);
		CValue & raValue = pItem->record[j];
		pColumn->bind (raValue);
	}
	////-----------------------------------------
	//// build domains
	////-----------------------------------------
	CDomain aDomain;
	i = m_aItems.size();
	while (i-- > 0)
	{
		item * pItem = m_aItems[i];
		aDomain.insert (pItem->name);
		pItem->domain = aDomain;
	}
	for (i=0; i<nItems; i++)
		trace (m_aItems[i]);
	////-----------------------------------------
	//// T1 x T2 ... x Tn Index
	////-----------------------------------------
	m_nRecord = ~0;
}
//---------------------------------------------------------------------------
CTableSet::item *
CTableSet::nextouter()
{
	const int nItems = m_aItems.size();
//	int i;
	item* pItem = m_aItems[m_iOTab];
	if (m_iOTab == 0 ||  ++m_iORow >= pItem->n)
	for (; m_iOTab < nItems; m_iOTab++)
	{
		pItem = m_aItems[m_iOTab];
		if (pItem->outer)
		{
			CRecord & raRecord = pItem->record;
			m_iORow = 0;
			setnull();
			//pItem->read (i, raRecord);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
bool CTableSet::next()
{
	long i = 0;
	while (next(i))
	{
		if (zero(i))
		{
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------
bool CTableSet::fetch (item & raItem, CDomain & aDomain)
{
	const int nJoin = m_aJoins.size();
	int i;
	while (fetch (raItem))
	{
		for (i=0; i<nJoin; i++)
		{
			CJoin * pJoin = m_aJoins[i];
			assert (pJoin);
			CTerm * pCond = pJoin->m_pCond;
			if (Condition (pCond, aDomain) == CondFALSE)
				break;
		}
		if (i < nJoin)
			continue;
		if (Condition (m_pFilter, aDomain) != CondFALSE)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool CTableSet::fetch (item & raItem)
{
	if (!raItem.iter.valid())
		return false;
	ULONG & n = raItem.n;
	ULONG i = raItem.iter.get();
	CTable * pTable = raItem.table;
	CRecord & raRecord = raItem.record;
	if (!pTable->read (i, raRecord))
		return false;
	raItem.i = i;
	trace (&raItem);
	raItem.iter++;
	return true;
}
//---------------------------------------------------------------------------
// inc. position <i> or upper
//---------------------------------------------------------------------------
bool CTableSet::next (long & i)
{
	const vector<item*> & aItems = m_aItems;
	const int nItems = aItems.size();
	item * pItem;

	for (; i<nItems; i++)
	{
		pItem = m_aItems[i];
		assert (pItem);
		CDomain & aDomain = pItem->domain;
		if (fetch (*pItem, aDomain))
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool CTableSet::zero (long i)
{
	const vector<item*> & aItems = m_aItems;
	const int nItems = aItems.size();
	item * pItem;
	while (i-- > 0)
	{
		pItem = m_aItems[i];
		pItem->i = 0;
		pItem->ss = pItem->s0;
		if (m_pFilter)
		if (pItem != m_aItems.back())
			pItem->ss &= filter1 (pItem->name, pItem->table, m_pFilter, pItem->domain);
		pItem->iter.assign (pItem->ss);
		if (!fetch (*pItem, pItem->domain))
			return false;
	}

	//// just one table: no index
	if (nItems > 1)
	for (i=0; i<nItems; i++)
	{
		pItem = m_aItems[i];
		const ULONG j = pItem->i;
		if (pItem->outer)
			pItem->outer[j] = true;
		m_aIndex.push_back (j);
	}
	if (m_pTrace)
	{
		fprintf (m_pTrace, "REC ok ----------------------\n");
		fflush (m_pTrace);
	}
	m_iRecord++;
	return true;
}
//---------------------------------------------------------------------------
void CTableSet::trace (item * pItem)
{
	if (!m_pTrace)
		return;
	if (!pItem)
		return;
	if (pItem->iter.valid())
		fprintf (m_pTrace, "%-15s  %9d [%9d,%9d]\n", pItem->name, pItem->i, pItem->iter[0], pItem->iter[1]);
	else
		fprintf (m_pTrace, "%-15s  %9d [%9s,%9s]\n", pItem->name, pItem->i, "?!", "?!");
	fflush (m_pTrace);
}
//---------------------------------------------------------------------------
} // namespace