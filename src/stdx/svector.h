/***************************************************************************************

    Projekt:    Hasy                                                     (C) 2000 PSI AG
    Komponente: odbc
    Modul:      svec.h

    sorted vector

    Systeme: Alpha, Linux, NT 4

    $Author: hasyjv $
    $Date: 2005/08/19 10:43:18 $
    $Revision: 1.4 $
    $State: Exp $

    Änderung  Datum     Autor  Bemerkung
    ------------------------------------------------------------------------------------
    @1        ??.??.00  ??
***************************************************************************************/

#ifndef __SVECTOR_H__
#define __SVECTOR_H__

//---------------------------------------------------------------------------
#define SVEC_TRACE
#ifndef SVEC_TRACE
#define SVEC_TRACE trace
#endif
//---------------------------------------------------------------------------
#ifndef SVEC_DUMP
#define SVEC_DUMP(v) (v).dump (__FILE__, __LINE__)
#endif
//---------------------------------------------------------------------------
#define SVEC_CHECK(v) ;
#ifndef SVEC_CHECK
#define SVEC_CHECK(v) (v).check()
#endif

namespace stdx
{
//---------------------------------------------------------------------------
template<class T>
class svec
{
public:
	typedef unsigned long uint;
	typedef unsigned long ulong;

	svec (bool distinct=false, uint p=1000);
	virtual ~svec();

	ulong count() const;
 	void insert (const T&);
	void index (bool rebuild=false) const;
	const T & operator[] (ulong) const;
	T & operator[] (ulong);
	void clear();
	void reorg();
	bool empty() const { return m_nPage == 0; }

	const T * find (const T&, short mode=0, ULONG * i=0, ULONG * j=0) const;
	const T * read (ULONG & i, ULONG & j) const;
	const T * prev (ULONG & i, ULONG & j) const;
	const T * next (ULONG & i, ULONG & j) const { return read (i,++j); }
	void end (ULONG & i, ULONG & j) const { i = m_nPage; j = 0; }

	virtual short compare (const T&, const T&) const;

	uint pages() const     { return m_nPage; }
	uint size() const      { return m_nSize; }
	uint cluster() const   { return m_nPageSize; }
	bool distinct() const  { return m_bDistinct; }

	void check() const;
	virtual void dump (const char * file, int line) const;


protected:
	////-----------------------------------------
	//// structure page
	////-----------------------------------------
	struct page
	{
		////-----------------------------------------
#if 1
		void append (const T & v)
		{
//			assert (count < size);
			data[count++] = v;
		}
#endif
		////-----------------------------------------
		uint count;
		T*   data;
	};
	////-----------------------------------------
	const T & first (const page & p) const { return p.data[0];}
	const T & last  (const page & p) const { return p.data[p.count-1]; }
	const page * seeki (ulong & i) const;
	const page * seek0 (ulong & i) const;
	page * split (page*, uint i);
	void resize (uint n);

	page* m_pPage;
	uint m_nPage;
	uint m_nSize;
	uint m_nPageSize;
	uint m_nPageInc;
	bool m_bDistinct;

	mutable vector<ulong> m_aIndex;
};
//---------------------------------------------------------------------------
template<class T>
svec<T>::svec(bool distinct, uint p)
{
	uint n = 1024;
	m_nPage = 0;
	m_nSize = n;
	m_nPageSize = p ? p : 1000;
	m_nPageInc = n;
	m_pPage = new page[m_nSize];
	memset (m_pPage, 0, m_nSize * sizeof(page));
	m_bDistinct = distinct;
}
//---------------------------------------------------------------------------
template<class T>
svec<T>::~svec()
{
	uint i;
	for (i=0; i<m_nPage; i++)
	{
		delete [] m_pPage[i].data;
	}
	delete [] m_pPage;
	m_pPage = 0;
}
//---------------------------------------------------------------------------
template<class T>
short svec<T>::compare (const T& v1, const T& v2) const
{
	if (v1 < v2)
		return -1;
	if (v2 < v1)
		return +1;
	return 0;
}
//---------------------------------------------------------------------------
template<class T>
void svec<T>::clear()
{
	uint i;
	for (i=0; i<m_nPage; i++)
	{
		delete [] m_pPage[i].data;
	}
	memset (m_pPage, 0, m_nPage * sizeof(page));
	m_nPage = 0;
}
//---------------------------------------------------------------------------
// create index
//---------------------------------------------------------------------------
template<class T>
void svec<T>::index (bool bRebuild) const
{
	if (bRebuild)
		m_aIndex.clear();
	if (!m_aIndex.empty())
		return;
	ulong n=0;
	uint j;
	for (j=0; j<m_nPage; j++)
	{
		const page & aPage = m_pPage[j];
		n += aPage.count;
		m_aIndex.push_back(n);
	}
}
//---------------------------------------------------------------------------
// seek simple (without index)
//---------------------------------------------------------------------------
template<class T>
const svec<T>::page * 
svec<T>::seek0 (ulong  & i) const
{
	uint j;
	for (j=0; j<m_nPage; j++)
	{
		const page* pPage = &m_pPage[j];
		register const ulong n = pPage->count;
		if (i < n)
			return pPage;
		i -= n;
	}
	return 0;
}
//---------------------------------------------------------------------------
// seek with index
//---------------------------------------------------------------------------
template<class T>
const svec<T>::page * 
svec<T>::seeki (ulong  & i) const
{
	if (m_aIndex.empty())
		return seek0 (i);
	//// seek in index
	uint a = 0;
	uint b = m_aIndex.size();
	ulong m,v;
	while (a < b)
	{
		m = (a + b) >> 1;
		v = m_aIndex[m] - 1; // last item v in page [m]
		if (i < v)
			b = m;
		else
		if (i > v)
			a = m + 1;
		else
			a = b = m;
	}
	assert (a < m_nPage);
	const page * p = &m_pPage[a];
	v = m_aIndex[a];
	assert (i <= v);
	i -= (v - p->count);
	assert (i < p->count);
	return p;
}
//---------------------------------------------------------------------------
template<class T>
inline
const T & svec<T>::operator[] (ulong i) const
{
	const page * pPage = seeki(i);
	assert (pPage);
	return pPage->data[i];
}
//---------------------------------------------------------------------------
template<class T>
inline
T & svec<T>::operator[] (ulong i)
{
	const page * pPage = seeki(i);
	assert (pPage);
	return pPage->data[i];
}
//---------------------------------------------------------------------------
template<class T>
void svec<T>::resize (uint n)
{
	assert(n>0);
	uint nCopy = m_nSize;
	if (n < nCopy)
		nCopy = n;
	if (n < m_nPage)
		m_nPage = n;
	page * pNew = new page[n];
	assert (pNew);
	memcpy (pNew, m_pPage, nCopy * sizeof(page));
	delete [] m_pPage;
	m_pPage = pNew;
	m_nSize = n;
}
//---------------------------------------------------------------------------
template<class T>
svec<T>::page * svec<T>::split (page* pSrc, uint i)
{
	page * pNew = new page (m_nPageSize);
	assert (pNew);
	return pNew;
}
//---------------------------------------------------------------------------
template<class T>
void svec<T>::insert (const T& aValue)
{
	////-----------------------------------------
	//// invalid index
	////-----------------------------------------
	m_aIndex.clear();
	////-----------------------------------------
	//// resize page stack
	////-----------------------------------------
	if (m_nPage >= m_nSize)
	{
		assert (m_nPage == m_nSize);
		resize (m_nSize + m_nPageInc);
	}
	////-----------------------------------------
	//// first page
	////-----------------------------------------
	if (m_nPage == 0)
	{
		m_pPage[0].data = new T[m_nPageSize];
		m_pPage[0].count = 1;
		m_pPage[0].data[0] = aValue;
		m_nPage++;
		return;
	}
#if 1
	ULONG pi,pj;
	const T * pValue = find (aValue, 0, &pi, &pj);
	page * pPage = m_pPage + (pi++);
#else
	////-----------------------------------------
	//// seek page <pi>
	////-----------------------------------------
	uint a = 0;
	uint b = m_nPage;
	uint i;
	short c;
	page * pPage = 0;
	const T * pItem = 0;
	while (a < b)
	{
		i = (a + b) >> 1;
		pPage = m_aPage[i];
		const T & aFirst = *(pPage->data);
		c = compare (aValue, aFirst);
		if (c < 0)
			b = i;
		else
		if (c > 0)
			a = i + 1;
		else
		{
			pItem = &aFirst;
			a = b = i + 1;
		}
	}
	if (pItem && m_bDistinct)
		return;
	uint pi = a; // page index > value!
	uint pj = 0; // data index > value!
	pPage = 0;   // page to insert data...
	if (a > 0)
		a--;
	assert (a < m_nPage);
	pPage = m_aPage[a];
	uint iPage = a; // index of page
	assert (pPage);
	////-----------------------------------------
	//// seek item <pj>
	////-----------------------------------------
	a = 0;
	b = pPage->count;
	while (a < b)
	{
		i = (a + b) >> 1;
		T & aItem = pPage->data[i];
		c = compare (aValue, aItem);
		if (c < 0)
			b = i;
		else
		if (c > 0)
			a = i + 1;
		else
		{
			pItem = &aItem;
			a = b = i + 1;
			break;
		}
	}
	pj = a;		
#endif
	////-----------------------------------------
	//// resize page
	////-----------------------------------------
	if (pPage->count >= m_nPageSize)
	{
		uint n = m_nPage - pi;
		page * p = m_pPage + pi;
		if (n > 0)
			memmove (p+1, p, n * sizeof(page));

		uint h1 = m_nPageSize>>1;
		uint h2 = pPage->count - h1;
		assert (pi <= m_nPage);
		page * pNew = m_pPage + pi;
		pNew->data = new T[m_nPageSize];
		pNew->count = h2;
		memcpy (pNew->data, pPage->data + h1, h2 * sizeof(T));
		pPage->count = h1;
		if (pj > pPage->count)
		{
			pj -= pPage->count;
			pPage = pNew;
		}
		m_nPage++;
	}
	////-----------------------------------------
	//// update page
	////-----------------------------------------
	assert (pPage->count < m_nPageSize);
#if 0
	uint i;
	T * p = pPage->data;
	T * p1 = p - 1;
	for (i=pPage->count; i>pj; i--)
		p[i] = p1[i];
	p[i] = aValue;
	pPage->count++;
#else
	uint n = pPage->count - pj;
	T * p = pPage->data + pj;
	if (n > 0)
		memmove (p+1,p, n * sizeof(T));
	*p = aValue;
	pPage->count++;
#endif
	SVEC_CHECK(*this);
}
//---------------------------------------------------------------------------
template<class T>
svec<T>::ulong 
svec<T>::count() const
{
	index();
	if (m_aIndex.empty())
		return 0;
	return m_aIndex.back();
}
//---------------------------------------------------------------------------
template<class T>
void svec<T>::dump (const const char * szFile, int nLine) const
{
	uint i,j;
	ulong nCount = 0;
	SVEC_TRACE ("\nVECTOR DUMP *****\n");
	if (szFile)
		SVEC_TRACE ("file: %s:%d\n", szFile, nLine);
	for (i=0; i<m_nPage; i++)
	{
		SVEC_TRACE ("page[%lu]\n", (long unsigned)i);
		const page * pPage = &m_pPage[i];
		const T * pData = pPage->data;
		const uint n = pPage->count;
		for (j=0; j<n; j++)
		{
			SVEC_TRACE ("%6g  ",  (double)pData[j]);
		}
		nCount += n;
		SVEC_TRACE ("\n");
	}
	SVEC_TRACE ("page=%u size=%u count=%lu\n", m_nPage, m_nSize, nCount);
}
//---------------------------------------------------------------------------
template<class T>
void svec<T>::reorg()
{
	if (m_nPage < 2)
		return;

	page * pDst = m_pPage;
	page * pSrc = m_pPage;
	page * pEnd = m_pPage + m_nPage;

	while (pDst < pEnd && pDst->count == m_nPageSize)
		pDst++;
	if (pDst == pEnd)
		return;
	pSrc = pDst + 1;
	while (pSrc < pEnd)
	{
		uint nd = m_nPageSize - pDst->count;
		if (nd > pSrc->count)
			nd = pSrc->count;
		uint ns = pSrc->count - nd;
		T * pd = pDst->data + pDst->count;
		T * ps = pSrc->data;
		memcpy (pd, ps, nd * sizeof(T));
		pDst->count += nd;
		memmove (ps, ps + nd, ns * sizeof(T)); 
		pSrc->count -= nd;
		if (pDst->count == m_nPageSize)
			pDst++;
		if (pDst == pSrc)
			pSrc++;
		if (pSrc->count == 0)
			pSrc++;
	}
	pSrc = pDst;
	if (pSrc->count)
		pSrc++;
	m_nPage = pSrc - m_pPage;
	for (; pSrc < pEnd; pSrc++)
	{
		assert (pSrc->count == 0);
		delete [] pSrc->data;
		pSrc->data = 0;
	}
}
//---------------------------------------------------------------------------
template<class T>
const T * svec<T>::prev (ULONG & i, ULONG & j) const
{
	if (j == 0 && 0 < i && i <= m_nPage)
	{
		const page * pPage = pPage = &m_pPage[--i];
		assert (pPage->count > 0);
		j = pPage->count;
		return pPage->data + (--j);
	}
	if (i < m_nPage)
	{
		const page * pPage = &m_pPage[i];
		if (0 < j && j <= pPage->count)
			return pPage->data + (--j);
	}
	return 0;
}
//---------------------------------------------------------------------------
template<class T>
const T * svec<T>::read (ULONG & i, ULONG & j) const
{
	for (; i<m_nPage; i++)
	{
		page * pPage = &m_pPage[i];
		if (j < pPage->count)
			return pPage->data + j;
		j -= pPage->count;
	}
	return 0;
}
//---------------------------------------------------------------------------
template<class T>
const T * svec<T>::find (const T& v, short nMode, ULONG * px, ULONG * py) const
{
	if (m_nPage == 0)
		return 0;
	const page * pPage;
	uint a,b,i;
	short c;
	////-----------------------------------------
	//// seek page <pi>
	////-----------------------------------------
	a = 0;
	b = m_nPage;
	while (a < b)
	{
		i = (a + b) >> 1;
		pPage = &m_pPage[i];
		c = compare (v, pPage->data[0]);
		if (c == 0)
			c = nMode;
		if (c < 0)
			b = i;
		else
		if (c > 0)
			a = i + 1;
		else
			a = b = i + 1;
	}
	i = (a > 0) ? a - 1 : a;
	pPage = &m_pPage[i];
	if (px)
		*px = i;
	if (py)
		*py = 0;
	if (c == 0)
		return pPage->data;
	////-----------------------------------------
	//// seek item <pj>
	////-----------------------------------------
	a = 0;
	b = pPage->count;
	const T * pData = pPage->data;
	const T * pItem = 0;
	while (a < b)
	{
		i = (a + b) >> 1;
		const T & w = pData[i];
		c = compare (v, w);
		if (c == 0)
			c = nMode;
		if (c < 0)
			b = i;
		else
		if (c > 0)
			a = i + 1;
		else
		{
			pItem = pData + i;
			a = b = i + 1;
		}
	}
	i = (c == 0) ? a - 1 : a;
	assert (c != 0 || a > 0);
	if (py)
		*py = i;
	return pItem; 
}
//---------------------------------------------------------------------------
template<class T>
void svec<T>::check() const
{
	const uint m = m_nPage;
	uint i;
	for (i=1; i<m; i++)
	{
		const page * p1 = &m_pPage[i-1];
		const page * p2 = &m_pPage[i];
		const T & v1 = last(*p1);
		const T & v2 = first(*p2);
//		const T & v1 = p1->data[0];
//		const T & v2 = p2->data[0];
		assert (v1 <= v2);
	}
	for (i=0; i<m; i++)
	{
		const page * p = &m_pPage[i];
		const uint n = p->count;
		uint j;
		for (j=1; j<n; j++)
		{
			const T & v1 = p->data[j-1];
			const T & v2 = p->data[j];
			assert (v1 <= v2);
		}
	}
}
//---------------------------------------------------------------------------
} // namespace stdx
#endif
