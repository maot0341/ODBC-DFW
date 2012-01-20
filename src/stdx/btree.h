/***************************************************************************************

    Projekt:    Hasy                                                     (C) 2000 PSI AG
    Komponente: odbc
    Modul:      btree.h

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

#ifndef __STDXBTREE_H__
#define __STDXBTREE_H__

//---------------------------------------------------------------------------
#ifndef BTREE_TRACE
#define BTREE_TRACE trace
#endif
//---------------------------------------------------------------------------
#ifndef BTREE_DUMP
#define BTREE_DUMP(v) (v).dump (__FILE__, __LINE__)
#endif
//---------------------------------------------------------------------------
#define BTREE_CHECK(v) ;
#ifndef BTREE_CHECK
#define BTREE_CHECK(v) (v).check()
#endif
namespace stdx
{
//---------------------------------------------------------------------------
template<class T, short K>
class btree
{
public:
	typedef unsigned short uint;
	typedef unsigned long ulong;

	btree (bool distinct=false);
	virtual ~btree();

 	void insert (const T&);
	virtual short compare (const T&, const T&) const;
	ulong count () const                                                    { return count(m_pRoot); }



protected:
	////-----------------------------------------
	//// structure node
	////-----------------------------------------
	struct node
	{
		uint count;
		T data[2*K];
		node* child[2*K+1];
		node* parent;
	};
	////-----------------------------------------
	typedef node* P;
	node* create (const T & v, node * pl=0, node * pr = 0) const;
	node* create (const T * p=0, uint n=0) const;
	node* split (node * p, uint i);
	void erase (node *);
	static bool empty (const node * p)                                      { return p == 0 || p->count == 0; }

	node* seek (const T & v, uint & i) const;
	bool seek (const node * p, const T & v, uint &  i) const;
 	void insert (node * parent, const T&, node * left, node * right);
 	void insert (node * parent, uint i, const T&, node * left, node * right);
	ulong count (const node*) const;
	////-----------------------------------------
	uint m_nSize;
	node* m_pRoot;
	bool m_bDistinct;

	mutable vector<ulong> m_aIndex;
};
//---------------------------------------------------------------------------
template<class T, short K>
short btree<T,K>::compare (const T& v1, const T& v2) const
{
	if (v1 < v2)
		return -1;
	if (v2 < v1)
		return +1;
	return 0;
}
//---------------------------------------------------------------------------
template<class T, short K>
void 
btree<T,K>::erase (node * p)
{
	if (!p) return;
	uint i;
	const uint n = p->count;
	if (n > 0)
	for (i=0; i<=n; i++)
		erase (p->child[i]);
	delete p;
}
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::node* 
btree<T,K>::create (const T & v, node * pl, node * pr) const
{
	node * p = create();
	p->data[0] = v;
	p->count++;
	p->child[0] = pl;
	p->child[1] = pr;
	if (pl)
		pl->parent = p;
	if (pr)
		pr->parent = p;
	return p;
}
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::node* 
btree<T,K>::create (const T * v, uint n) const
{
	const uint n0 = m_nSize;
	const uint n1 = n0 + 1;
	node * p = new node;
	assert(p);
	p->parent = 0;
	p->count = 0;
	assert (p->data);
	assert (p->child);
	memset (p->data , 0, n0 * sizeof(T));
	memset (p->child, 0, n1 * sizeof(P));
	if (!n)
		return p;
	assert(v);
	memcpy (p->data, v, n * sizeof(T));
	p->count = n;
	return p;
}
//---------------------------------------------------------------------------
template<class T, short K>
void btree<T,K>::insert (const T & v)
{
	node* p = 0;
	uint i;
	if (!m_pRoot)
	{
		m_pRoot = create(v);
		return;
	}
	p = seek (v, i);
	//insert (p, i, v, 0, 0);
	insert (p, v, 0, 0);
}
//---------------------------------------------------------------------------
// inner node insert
//---------------------------------------------------------------------------
template<class T, short K>
void btree<T,K>::insert (node * pNode, uint i, const T & v, node * pLeft, node * pRight)
{
	assert (pNode);
	assert (pNode->count < m_nSize);
	assert (i <= pNode->count);
	T * pd = pNode->data + i;
	P * pc = pNode->child + i;
	assert (pNode->count == i || pc[0] == pLeft);
	short n = pNode->count - i; 
	if (n > 0)
	{
		memmove (pd+1, pd, n * sizeof(T));
		memmove (pc+1, pc, (n+1) * sizeof(P));
	}
	*pd = v;
	pc[0] = pLeft;
	pc[1] = pRight;
	if (pLeft)
		pLeft->parent = pNode;
	if (pRight)
		pRight->parent = pNode;
	pNode->count++;
}
//---------------------------------------------------------------------------
// split node
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::node*
btree<T,K>::split (node * pNode, uint i)
{
	const uint n = m_nSize - i;
	node * pNew = create();
	memcpy (pNew->data, pNode->data + i, sizeof(T) * n);
	memcpy (pNew->child, pNode->child + i, sizeof(node*)*(n+1));
	pNew->count = n;
	pNew->parent = pNode->parent;
	pNode->count = i;
	return pNew;
}
//---------------------------------------------------------------------------
// leaf insert
//---------------------------------------------------------------------------
template<class T, short K>
void btree<T,K>::insert (node * pNode, const T & v, node* pChildL, node* pChildR)
{
	////-----------------------------------------
	//// insert root
	////-----------------------------------------
	if (!pNode)
	{
		m_pRoot = create (v, pChildL, pChildR);
		if (pChildL)
			pChildL->parent = m_pRoot;
		if (pChildR)
			pChildR->parent = m_pRoot;
		return;
	}
	////-----------------------------------------
	//// insert data
	////-----------------------------------------
	uint i;
	if (seek (pNode, v, i))
	if (m_bDistinct)
		return;
	if (pNode->count < m_nSize)
	{
		insert (pNode, i, v, pChildL, pChildR);
		return;
	}
	////-----------------------------------------
	//// split node
	////-----------------------------------------
	node * pNewL = pNode;
	node * pNewR = 0;
	node * pRoot = pNode->parent;
	if (i > K)
	{
		const uint K1 = K + 1;
		pNewR = split (pNode, K1);
		pNewL->count = K;
		insert (pRoot, pNode->data[K], pNewL, pNewR);
		i -= K1;
		insert (pNewR, i, v, pChildL, pChildR);
		return;
	}
	if (i < K)
	{
		const uint K1 = K - 1;
		pNewR = split (pNode, K);
		pNewL->count = K1;
		insert (pRoot, pNode->data[K1], pNewL, pNewR);
		insert (pNewL, i, v, pChildL, pChildR);
		return;
	}
	if (i == K)
	{
		pNewR = split (pNode, K);
		pNewR->child[0] = pChildR;
		pNewL->count = K;
		insert (pRoot, v, pNewL, pNewR);
		return;
	}
	assert (false);
}
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::node * 
btree<T,K>::seek (const T & v, uint & i) const
{
	node *s, *p = m_pRoot;
	while(true)
	{
		if (empty(p))
			return p;
		if (seek (p, v, i))
			return p;
		s = p->child[i];
		if (!s)
			return p;
		p = s;
	}
}
//---------------------------------------------------------------------------
template<class T, short K>
bool
btree<T,K>::seek (const node * p, const T & v, uint &  i) const
{
	uint a = i = 0;
	uint b = p->count;
	short c;
	const T * d = p->data;
	while (a < b)
	{
		i = (a + b) >> 1;
		c = compare (v, d[i]);
		if (c < 0)
			b = i;
		else
		if (c > 0)
			a = i + 1;
		else
			return true;
	}
	i = a;
	return false;
}
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::btree(bool distinct)
{
	m_nSize = (K << 1);
	m_pRoot = create();
	m_bDistinct = distinct;
}
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::~btree()
{
	erase (m_pRoot);
	m_pRoot = 0;
}
//---------------------------------------------------------------------------
template<class T, short K>
btree<T,K>::ulong
btree<T,K>::count (const node * pNode) const
{
	ulong nCount = 0;
	if (!pNode)
		return nCount;
	uint i,n = pNode->count;
	if (n > 0)
	for (i=0; i<=n; i++)
		nCount += count (pNode->child[i]);
	return nCount + n;
}
//---------------------------------------------------------------------------
} // namespace stdx
#endif
