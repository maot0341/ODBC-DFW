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
#include <algorithm>
#include <set>
#include "yac.h"
#include "resultset.h"
#include <stdx/svector.h>
#include <stdx/debug.h>
#include <stdx/utils.h>

using namespace stdx;
using namespace std;

namespace sqlp {
//---------------------------------------------------------------------------
template<class T> 
short cmp (const T * p1, const T * p2)
{
	if (*p1 < *p2)
		return -1;
	if (*p2 < *p1)
		return +1;
	return 0;
}
//---------------------------------------------------------------------------
void read (const CTerm& aTerm, double & raValue)
{
	raValue = aTerm.asDouble();
}
//---------------------------------------------------------------------------
void read (const CTerm& aTerm, long & raValue)
{
	raValue = aTerm.asDouble();
}
//---------------------------------------------------------------------------
void read (const CTerm& aTerm, const char *& raValue)
{
	raValue = strdup(aTerm.asString());
}
//---------------------------------------------------------------------------
void read (const CTerm& aTerm, char raValue[])
{
	strncpy (raValue, aTerm.asString(), 255);
}
//---------------------------------------------------------------------------
// Number
//---------------------------------------------------------------------------
template<class T>
short TNumber<T>::cmp (const BYTE* r1, const BYTE* r2) const //{ return mist::cmp ((T*)rec1, (T*)rec2); }
{
	const T & v1 = VALUE(T,r1);
	const T & v2 = VALUE(T,r2);

	if (v1 < v2)
		return -1;
	if (v2 < v1)
		return +1;
	return 0;
}
//---------------------------------------------------------------------------
template<class T>
void  TNumber<T>::set (BYTE* rec, const sqlp::CTerm &t) const
{
	if (t.isNull())
		setNull (rec);
	else
		VALUE(T,rec) = t.asDouble();
}
//---------------------------------------------------------------------------
SQLSMALLINT TNumber<double>::SQLType() const { return SQL_C_DOUBLE; }
SQLSMALLINT TNumber<long>::SQLType() const   { return SQL_C_LONG;   }
SQLSMALLINT TNumber<ULONG>::SQLType() const { return SQL_C_ULONG;  }
const double TNumber<double>::ms_null  = DBL_MAX;
const long TNumber<long>::ms_null = LONG_MAX;
const ULONG TNumber<ULONG>::ms_null = ULONG_MAX;
//---------------------------------------------------------------------------
typedef TNumber<double> CDouble;
typedef TNumber<long> CInteger;
typedef TNumber<BYTE> CBool;
//---------------------------------------------------------------------------
// Varchar
//---------------------------------------------------------------------------
CVarchar::~CVarchar()
{
	ULONG i;
	ULONG nStrings = m_aStringVec.size();
	for (i=0; i<nStrings; i++)
	{
		type & s = m_aStringVec[i];
		free ((void*)s);
		s = 0;
	}
}
//---------------------------------------------------------------------------
void CVarchar::set (BYTE* rec, const CTerm &t) const 
{
//	BYTE * val = rec + m_nOffset;
	////-----------------------------------------
	//// string pool
	////-----------------------------------------
	if (m_bPool)
	{
		string s = t.asString();
		std::set<string>::_Pairib aRes = m_aStringSet.insert(s);
		std::set<string>::iterator iSet = aRes.first;
		const string & str = *iSet;
		VALUE(type,rec) = str.c_str();
	}
	////-----------------------------------------
	//// string list
	////-----------------------------------------
	else
	{
		type s = strdup(t.asString());
		m_aStringVec.push_back (s);
		VALUE (type,rec) = s;
	}
}
//---------------------------------------------------------------------------
short CVarchar::cmp (const BYTE* pRec1, const BYTE* pRec2) const
{
	type & v1 = VALUE(type,pRec1);
	type & v2 = VALUE(type,pRec2);
	return strcmp (v1,v2);
}
//---------------------------------------------------------------------------
// String (fix length)
//---------------------------------------------------------------------------
void CFixString::set (BYTE* rec, const CTerm &t) const
{ 
	strncpy(VALPTR(char*,rec), t.asString(), size());
}
//---------------------------------------------------------------------------
short CFixString::cmp (const BYTE* pRec1, const BYTE* pRec2) const
{
	const char * v1 = VALPTR(const char*, pRec1);
	const char * v2 = VALPTR(const char*, pRec2);
	return strncmp (v1, v2, m_nSize);
}
//---------------------------------------------------------------------------
// Average
//---------------------------------------------------------------------------
void CAverage::set (BYTE* pRec, const CTerm & aTerm) const
{
	if (aTerm.isNull())
		return;
	avg & raValue = VALUE(avg, pRec);
	if (raValue.count == 0)
	{
		raValue.summe = aTerm.asDouble();
		raValue.count = 1;
	}
	else
	{
		raValue.summe += aTerm.asDouble();
		raValue.count++;
	}
}
//---------------------------------------------------------------------------
void CAverage::get (const BYTE* pRec, CValue & raValue) const
{
	const avg & aAvg = VALUE(const avg, pRec);
	if (aAvg.count)
		raValue = aAvg.summe / aAvg.count;
	else 
		raValue.setNull();
//	raValue = value(pField);
}
//---------------------------------------------------------------------------
short CAverage::cmp (const BYTE* r1, const BYTE* r2) const
{
	const avg & a1 = VALUE(const avg, r1);
	const avg & a2 = VALUE(const avg, r2);
	double v1 = value(a1);
	double v2 = value(a2);
	if (v1 < v2)
		return -1;
	if (v2 < v1)
		return +1;
	return 0;
}
//---------------------------------------------------------------------------
// Counter
//---------------------------------------------------------------------------
void 
CCounter::set (BYTE* pRec, const CTerm & aTerm) const
{
	const CUnary* pAggr = dynamic_cast<const CUnary*>(&aTerm);
	assert (pAggr);
	assert (pAggr->head() == lCOUNT);
	if (m_pTerm && m_pTerm->isNull())
		return;
	ULONG & raValue = VALUE(ULONG,pRec);
	if (raValue == ms_null) 
		raValue = 0;
	else
		raValue++;
}
//---------------------------------------------------------------------------
// Summe
//---------------------------------------------------------------------------
void CSumme::set (BYTE* rec, const CTerm &t) const
{
	if (t.isNull())
		return;
	double & val = VALUE(double,rec);
	if (val == ms_null)
	{
		val = t.asDouble();
	}
	else
	{
		val += t.asDouble();
	}
}
//---------------------------------------------------------------------------
template<class T>
void TMin<T>::set (BYTE* rec, const CTerm & aTerm) const
{
	if (aTerm.isNull()) return;
	CValue aValue (aTerm.type());
	T::get (rec, aValue);
	if (isNull(rec) || aTerm < aValue)
		T::set (rec, aTerm);
}
//---------------------------------------------------------------------------
template<class T>
void TMax<T>::set (BYTE* rec, const CTerm & aTerm) const
{
	if (aTerm.isNull()) return;
	CValue aValue (aTerm.type());
	T::get (rec, aValue);
	if (isNull(rec) || aValue < aTerm)
		T::set (rec, aTerm);
}
//---------------------------------------------------------------------------
// Recordset -- Implemtierung
//---------------------------------------------------------------------------
CResultset::CResultset()
{
	m_nRowCount = 0;
	m_nRowSize = 0;
	m_nPageSize = 8 * 1024;
	m_aHeader.reserve(20);
	m_aPages.reserve(20);
	m_nPrimaryKey = 0;
	m_bAggregate = false;
}
//---------------------------------------------------------------------------
CResultset::~CResultset()
{
	erase();
}
//---------------------------------------------------------------------------
bool 
CResultset::move (ULONG iRow)
{
	if (iRow >= rows())
		return false;
	const BYTE* pRecord = record(iRow);
	ULONG nItems = m_aTerms.size();
	assert (m_aHeader.size() == nItems);
	assert (pRecord);
	ULONG i;
	sqlp::format (m_aCursor, m_aTerms);
	for (i=0; i<nItems; i++)
	{
		CResultsetField * pField = m_aHeader[i];
		assert (pField);
		CValue & raValue = m_aCursor[i];
		if (pField->isNull(pRecord))
			raValue.setNull();
		else
			pField->get (pRecord, raValue);
		m_aTerms[i]->bind (raValue);
	}
	return true;
}
//---------------------------------------------------------------------------
bool 
CResultset::get (HREC hrec, ULONG i, sqlp::CValue & raValue)
{
	BYTE* pRecord = (BYTE*)hrec;
	if (!pRecord)
		return false;
	ULONG nItems = m_aTerms.size();
	assert (m_aHeader.size() == nItems);
	if (i >= nItems)
		return false;
	CResultsetField * pField = m_aHeader[i];
	assert (pField);
	if (pField->isNull(pRecord))
		raValue.setNull();
	else
		pField->get (pRecord, raValue);
	return true;
}
//---------------------------------------------------------------------------
bool 
CResultset::get (HREC hrec, sqlp::CRecord & raData)
{
	BYTE* pRecord = (BYTE*)hrec;
	if (!pRecord)
		return false;
	ULONG nItems = m_aTerms.size();
	assert (m_aHeader.size() == nItems);
	assert (pRecord);
	ULONG i;
	sqlp::format (raData, m_aTerms);
	for (i=0; i<nItems; i++)
	{
		CResultsetField * pField = m_aHeader[i];
		assert (pField);
		CValue & raValue = raData[i];
		if (pField->isNull(pRecord))
			raValue.setNull();
		else
			pField->get (pRecord, raValue);
		m_aTerms[i]->bind (raValue);
	}
	return true;
}
//---------------------------------------------------------------------------
// ASC/DESC Funct.:
// Store Args instead the term itself
//---------------------------------------------------------------------------
CTerm* ResultsetTerm (CTerm* pTerm)
{
	CUnary * pUnary = dynamic_cast<CUnary*>(pTerm);
	if (!pUnary)
		return pTerm;
	int nHead = pUnary->head();
	if (nHead == lASC)
		return pUnary->m_pTerm;
	if (nHead == lDESC)
		return pUnary->m_pTerm;
	return pTerm;
}
//---------------------------------------------------------------------------
void
CResultset::aggr (sqlp::CUnary * pAggr)
{
	assert (pAggr);
	if (CONTAINS (m_aTerms, pAggr))
		return;
	CResultsetField * pField = 0;
	short nType = pAggr->type();
	int nHead = pAggr->head();
	switch (nHead)
	{
	case lMIN:
		switch (nType)
		{
		CASE_SQL_INTEGER:
			pField = new TMin<CInteger>;
			break;
		CASE_SQL_FLOAT:
			pField = new TMin<CDouble>;
			break;
		CASE_SQL_DATETIME:
			pField = new TMin<CInteger>;
			break;
		CASE_SQL_STRING:
			pField = new TMin<CVarchar>;
			break;
		default:
			assert (false);
		}
		break;
	case lMAX:
		switch (nType)
		{
		CASE_SQL_INTEGER:
			pField = new TMax<CInteger>;
			break;
		CASE_SQL_FLOAT:
			pField = new TMax<CDouble>;
			break;
		CASE_SQL_DATETIME:
			pField = new TMax<CInteger>;
			break;
		CASE_SQL_STRING:
			pField = new TMax<CVarchar>;
			break;
		default:
			assert (false);
		}
		break;
	case lSUM:
		pField = new CSumme();
		break;
	case lCOUNT:
		pField = new CCounter(pAggr->m_pTerm);
		break;
	case lAVG:
		pField = new CAverage();
		break;
	default:
		pField = 0;
	}
	assert(pField);
	m_aHeader.push_back (pField);
	m_aTerms.push_back (pAggr);
	m_bAggregate = true;
}
//---------------------------------------------------------------------------
void
CResultset::add (sqlp::CTerm * pTerm, bool bPrimaryKey)
{
	pTerm = ResultsetTerm(pTerm);
	assert (pTerm);
	if (CONTAINS(m_aTerms, pTerm))
		return;
	short nType = pTerm->type();
	CResultsetField * pField = 0;
	switch (nType)
	{
	CASE_SQL_BOOL:
	CASE_SQL_INTEGER:
	CASE_SQL_DATETIME:
		pField = new CInteger;
		break;
	CASE_SQL_FLOAT:
		pField = new CDouble();
		break;
	CASE_SQL_STRING:
		pField = new CVarchar();
		break;
	default:
		assert (false);
	}
	assert(pField);
	if (bPrimaryKey)
	{
		assert (m_nPrimaryKey == m_aHeader.size());
		m_nPrimaryKey++;
	}
	m_aHeader.push_back (pField);
	m_aTerms.push_back (pTerm);
}
//---------------------------------------------------------------------------
void
CResultset::add (short nType)
{
	CResultsetField * pField = 0;
	switch (nType)
	{
	CASE_SQL_BOOL:
	CASE_SQL_INTEGER:
	CASE_SQL_DATETIME:
		pField = new CInteger();
		break;
	CASE_SQL_FLOAT:
		pField = new CDouble();
		break;
	CASE_SQL_STRING:
		pField = new CVarchar();
		break;
	default:
		assert (false);
	}
	add (pField);
}
//---------------------------------------------------------------------------
#if 0
void
CResultset::add (SQLSMALLINT nType)
{
	CResultsetField * pField = 0;
	if (nType == SQL_C_DOUBLE)
		pField = new CDouble();
	else if (nType == SQL_C_LONG)
		pField = new CInteger();
	else if (nType == SQL_C_CHAR)
		pField = new CVarchar();
	add (pField);
}
#endif
//---------------------------------------------------------------------------
void
CResultset::add (CResultsetField * pField)
{
	assert (pField);
	m_aHeader.push_back (pField);
}
//---------------------------------------------------------------------------
ULONG
CResultset::index (BYTE* pRecord) const
{
	ULONG i;
	ULONG iRecord = 0;
	assert (m_nRowSize);
	const ULONG nPage = m_aPages.size();
	for (i=0; i<nPage; i++)
	{
		BYTE* pPage = m_aPages[i];
		if (pRecord < pPage)
			continue;
		iRecord = (pRecord - pPage) / m_nRowSize;
		if (iRecord < m_nPageSize)
			break;
	}
	assert (i<nPage);
	const ULONG nRows = rows_per_page();
	return i * nRows + iRecord;
}
//---------------------------------------------------------------------------
HREC
CResultset::update()
{
	ULONG i;

	//// pimary key lesen
	BYTE * pRecord = append();
	for (i=0; i<m_nPrimaryKey; i++)
	{
		const CResultsetField * pField = m_aHeader[i];
		assert (pField);
		const CTerm * pTerm = m_aTerms[i];
		assert (pTerm);
		pField->set (pRecord, *pTerm);
	}
	//// Record lokalisieren
	if (m_nPrimaryKey || m_bAggregate)
	{
		CKeyPrivate aKey(this, pRecord);
		index_t::iterator iKey = m_aKey.find(aKey);
		if (iKey == m_aKey.end())
			m_aKey[aKey] = pRecord;
		else
		{
			m_nRowCount--;
			pRecord = (*iKey).second;
		}
	}
	assert (pRecord);

	//// Record updaten / aggregieren
	const ULONG nHeader = m_aHeader.size();
	for (i=m_nPrimaryKey; i<nHeader; i++)
	{
		const CResultsetField * pField = m_aHeader[i];
		assert (pField);
		const CTerm * pTerm = m_aTerms[i];
		assert (pTerm);
		pField->set(pRecord, *pTerm);
	}
	return pRecord;
}
//---------------------------------------------------------------------------
BYTE*
CResultset::append()
{
	assert (m_nRowSize);
	assert (m_nPageSize);
	const ULONG nRowsPerPage = rows_per_page();
	const ULONG i = m_nRowCount / nRowsPerPage;
	const ULONG j = m_nRowCount % nRowsPerPage;
	BYTE* pRow = 0;
	if (i < m_aPages.size())
		pRow =  m_aPages[i] + j * m_nRowSize;
	else
	{
		pRow = new BYTE[m_nPageSize];
		assert(pRow);
		m_aPages.push_back(pRow);
		assert(j==0);
	}
	m_nRowCount++;
	//memset (pRow, 0xff, m_nRowSize);
	ULONG h;
	const ULONG nHeader = m_aHeader.size();
	for (h=0; h<nHeader; h++)
	{
		const CResultsetField * pField = m_aHeader[h];
		assert (pField);
		pField->setNull(pRow);
	}
	return pRow;
}
//---------------------------------------------------------------------------
BYTE*
CResultset::record (ULONG iRow) const
{
	assert (m_nRowSize);
	assert (m_nPageSize);
	const ULONG nRowsPerPage = rows_per_page();
	const ULONG i = iRow / nRowsPerPage;
	const ULONG j = iRow % nRowsPerPage;
	assert (i<m_aPages.size());
	return m_aPages[i] + j * m_nRowSize;
}
//---------------------------------------------------------------------------
// Datenbereich loeschen und neu formatieren
//---------------------------------------------------------------------------
void
CResultset::create()
{
	m_nRowCount = 0;
	m_nRowSize = 0;

	ULONG i;
	const ULONG nFields = m_aHeader.size();
	for (i=0; i<nFields; i++)
	{
		CResultsetField & aField = *m_aHeader[i];
		aField.m_nOffset = m_nRowSize;
		m_nRowSize += aField.size();
	}
	const ULONG nPages = m_aPages.size();
	for (i=0; i<nPages; i++)
		delete [] m_aPages[i];
	m_aPages.clear();
}
//---------------------------------------------------------------------------
// Datenbereich loeschen
//---------------------------------------------------------------------------
void
CResultset::empty()
{
	const ULONG nPages = m_aPages.size();
	ULONG i;
	for (i=0; i<nPages; i++)
		delete [] m_aPages[i];
	m_aPages.clear();
}
//---------------------------------------------------------------------------
// Alles (Header + Datenbereich) loeschen
//---------------------------------------------------------------------------
void
CResultset::erase()
{
	m_nPrimaryKey = 0;
	const ULONG nHeader = m_aHeader.size();
	ULONG i;
//	for (i=0; i<m_nRowCount; i++
	for (i=0; i<nHeader; i++)
		delete m_aHeader[i];
	m_aHeader.clear();
	const ULONG nPages = m_aPages.size();
	for (i=0; i<nPages; i++)
		delete [] m_aPages[i];
	m_aPages.clear();
}
//---------------------------------------------------------------------------
// Record -- Vergleich
//---------------------------------------------------------------------------
short
CResultset::cmp (short nFields, const BYTE* pRec1, const BYTE* pRec2) const
{
	if (!nFields)
		return 0;
	ULONG i;
	short nCmp;
	const ULONG nHeader = m_aHeader.size();
	assert (nFields <= nHeader);
	for (i=0; i<nFields; i++)
	{
		CResultsetField * pField = m_aHeader[i];
		assert (pField);
		nCmp = pField->cmp (pRec1, pRec2);
		if (nCmp) return nCmp;
	}
	return 0;
}
//---------------------------------------------------------------------------
short
CResultset::cmp (const vector<short> & aFields, const BYTE* pRec1, const BYTE* pRec2) const
{
	ULONG i,j;
	short nCmp;
	const ULONG nHeader = m_aHeader.size();
	const ULONG nFields = aFields.size();
	for (i=0; i<nFields; i++)
	{
		short iField = aFields[i];
//		if (iField > nHeader) continue;
		j = abs(iField) - 1;
		assert (j < nHeader);
		const CResultsetField * pField = m_aHeader[j];
		assert (pField);
		if (iField > 0)
			nCmp = pField->cmp (pRec1, pRec2);
		else
			nCmp = pField->cmp (pRec2, pRec1);
		if (nCmp)
			return nCmp;
	}
	return 0;
}
//---------------------------------------------------------------------------
static const CResultset * gs_pRecordset = 0;
static const vector<short> * gs_pOrder = 0;
bool less_then_idx (ULONG i1, ULONG i2)
{
	BYTE* pRec1 = gs_pRecordset->record(i1);
	BYTE* pRec2 = gs_pRecordset->record(i2);
	return gs_pRecordset->cmp (*gs_pOrder, pRec1, pRec2) < 0;
}
//---------------------------------------------------------------------------
void
CResultset::sort(const vector<CTerm*> & aOrder, vector<ULONG> & raIndex) const
{
	int i,j;
	const int nOrder = aOrder.size();
	const int nTerm = m_aTerms.size();
	vector<short> aIndicator (nOrder);
	for (j=0; j<nOrder; j++)
	{
		const CTerm * pTerm = ResultsetTerm (aOrder[j]);
		for (i=0; i<nTerm; i++)
		if (pTerm == m_aTerms[i])
			break;
		ASSUME (i < nTerm);
//		const long iCol = j+1;
		const long iCol = i+1;
		aIndicator[j] = iCol;
		const CUnary * pUnary = dynamic_cast<const CUnary*>(aOrder[j]);
		if (!pUnary)
			continue;
		const int nHead = pUnary->m_nHead;
		if (nHead == lASC)
			aIndicator[j] = +iCol;
		if (nHead == lDESC)
			aIndicator[j] = -iCol;
	}
	sort (aIndicator, raIndex);
}
//---------------------------------------------------------------------------
void
CResultset::sort(const vector<short> & aOrder, vector<ULONG> & raIndex) const
{
	if (!aOrder.size())
		return;
	gs_pRecordset = this;
	gs_pOrder = &aOrder;
	std::sort (raIndex.begin(), raIndex.end(), less_then_idx);
	gs_pRecordset = 0;
	gs_pOrder = 0;
}
//---------------------------------------------------------------------------
bool less_then_rec (BYTE* pRec1, BYTE* pRec2)
{
	unsigned short nKey = gs_pRecordset->m_nPrimaryKey;
	return gs_pRecordset->cmp (nKey, pRec1, pRec2) < 0;
}
//---------------------------------------------------------------------------
void
CResultset::sort(const vector<short> & aOrder, vector<BYTE*> & aIndex) const
{
	ULONG i;
	aIndex.resize (m_nRowCount);
	for (i=0; i<m_nRowCount; i++)
		aIndex[i] = record(i);

	gs_pRecordset = this;
	std::sort (aIndex.begin(), aIndex.end(), less_then_rec);
	gs_pRecordset = 0;
}
//---------------------------------------------------------------------------
bool CKeyPrivate::operator< (const CKeyPrivate& aKey) const
{
	assert (m_pParent);
	assert (m_pData);
	assert (m_pParent == aKey.m_pParent);
	return m_pParent->cmp (m_pData, aKey.m_pData) < 0;
}
//---------------------------------------------------------------------------
} // namespace