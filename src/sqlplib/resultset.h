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

#ifndef RESULTSET_H
#define RESULTSET_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include <set>
#include "sqlp.h"

using namespace std;
namespace sqlp {

//---------------------------------------------------------------------------
class CResultset;
typedef unsigned char BYTE;
typedef BYTE* HREC;
//---------------------------------------------------------------------------
// Interpreter fuer void* (char*)
//---------------------------------------------------------------------------
class CResultsetField
{
public:
	friend CResultset;

	CResultsetField (unsigned s): m_nSize(s), m_nOffset(0) {}
	virtual ~CResultsetField() {}

	virtual SQLSMALLINT SQLType() const = 0;

	virtual void  set (BYTE* val, const sqlp::CTerm &) const = 0;
	virtual void  get (const BYTE* val, sqlp::CValue &) const = 0;
	virtual short cmp (const BYTE* rec1, const BYTE* rec2) const = 0;

	unsigned size() const { return m_nSize; }
	unsigned offset() const { return m_nOffset; }
	virtual void setNull (BYTE* ptr) const = 0;
	virtual bool isNull (const BYTE* ptr) const = 0;

protected:
	const char * m_szColumn;
	const char * m_szTable;
	const unsigned m_nSize;
	unsigned m_nOffset;
};
//---------------------------------------------------------------------------
#define VALPTR(t,p) (t)((p)+m_nOffset)
#define VALUE(t,p) (*(t*)((p)+m_nOffset))
//#define VALUE(t,p) *static_cast<t>((p) + m_nOffset)
//---------------------------------------------------------------------------
template<class T>
class TNumber: public CResultsetField
{
public:
	TNumber(): CResultsetField (sizeof(T)) {}
	SQLSMALLINT SQLType() const;
	void set (BYTE* rec, const sqlp::CTerm &t) const;
	void get (const BYTE* rec, sqlp::CValue &val) const                      { val = VALUE(T,rec); }
	short cmp (const BYTE* r1, const BYTE* r2) const;

	bool isNull (const BYTE* ptr) const                                      { return VALUE(T,ptr) == ms_null; }
	void setNull (BYTE* ptr) const                                           { VALUE(T,ptr) = ms_null; }
	static const T ms_null;
};
typedef TNumber<double>  CDouble;
typedef TNumber<long>    CInteger;
typedef TNumber<BYTE>    CBool;
//---------------------------------------------------------------------------
class CVarchar: public CResultsetField
{
public:
	typedef const char* type;
	CVarchar(bool pool=false): CResultsetField(sizeof(type)), m_bPool(pool)  { m_aStringVec.reserve(500); }
	virtual ~CVarchar();
	SQLSMALLINT SQLType() const                                              { return SQL_C_CHAR; };
	void set (BYTE* val, const CTerm &t) const;
	void get (const BYTE* val, CValue &t) const                              { t = VALUE(type,val); }
	short cmp (const BYTE* rec1, const BYTE* rec2) const;
	void setNull (BYTE* rec) const                                           { VALUE(type,rec) = 0; }
	bool isNull (const BYTE* rec) const                                      { return VALUE(const type,rec) == 0; }

	bool m_bPool;
	mutable vector<type>     m_aStringVec;
	mutable std::set<string> m_aStringSet;
};
//---------------------------------------------------------------------------
class CFixString: public CResultsetField
{
public:
	CFixString (ULONG size): CResultsetField(size) {}
	SQLSMALLINT SQLType() const                                              { return SQL_C_CHAR; };
	short cmp (const BYTE* rec1, const BYTE* rec2) const;
	void set (BYTE* val, const CTerm &t) const;
	void get (const BYTE* val, CValue &t) const                              { t = ((char*)val); }
};
//---------------------------------------------------------------------------
class CAverage: public CResultsetField
{
public:
	struct avg {double summe; ULONG count;	};

	CAverage(): CResultsetField(sizeof(avg)) {}
	SQLSMALLINT SQLType() const                                              { return SQL_C_DOUBLE; };
	ULONG size() const                                                      { return sizeof(avg); }
	void set (BYTE* rec, const sqlp::CTerm &t) const;
	void get (const BYTE* rec, sqlp::CValue &v) const;
	short cmp (const BYTE* rec1, const BYTE* rec2) const;
	void add (BYTE* sum, const BYTE* rec) const;
	static double value (const avg & aAvg)                                   { return aAvg.summe / aAvg.count; }
	void setNull (BYTE* rec) const                                           { VALUE(avg,rec).count = 0; }
	bool isNull (const BYTE* rec) const                                      { return VALUE(const avg, rec).count == 0; }

	ULONG m_nSize;
};
//---------------------------------------------------------------------------
class CCounter: public TNumber<ULONG>
{
public:
	CCounter (const CTerm * term=0): m_pTerm(term) {}
	void set (BYTE* val, const CTerm &t) const;
//	void get (const BYTE* val, CValue &t) const                              { t = *((ULONG*)val); }
	void setNull (BYTE* rec) const                                           { VALUE(ULONG,rec) = 0; }

protected:
	const CTerm * m_pTerm;
};
//---------------------------------------------------------------------------
class CSumme: public CDouble
{
public:
	void set (BYTE* rec, const CTerm &t) const;
};
//---------------------------------------------------------------------------
template<class T>
class TMin: public T
{
public:
	void set (BYTE* rec, const CTerm & aTerm) const;
};
//---------------------------------------------------------------------------
template<class T>
class TMax: public T
{
public:
	void set (BYTE* rec, const CTerm & aTerm) const;
};
//---------------------------------------------------------------------------
// abstract pointer interpreter
//---------------------------------------------------------------------------
class CKeyPrivate
{
public:
	CKeyPrivate(const CResultset *p=0, BYTE* r=0): m_pParent(p), m_pData(r) {}
	CKeyPrivate(const CKeyPrivate& ref): m_pParent(ref.m_pParent), m_pData(ref.m_pData) {}
	CKeyPrivate& operator= (const CKeyPrivate& obj) { m_pParent = obj.m_pParent; m_pData = obj.m_pData; }
	bool operator< (const CKeyPrivate&) const;
	void assign (const CResultset* p, BYTE* data) { m_pParent = p; m_pData = data; }

	const CResultset * m_pParent;
	BYTE * m_pData;
};
//---------------------------------------------------------------------------
// Recordset
//---------------------------------------------------------------------------
class CResultset: public sqlp::CRecordset
{
public:

	CResultset();
	virtual ~CResultset();

	//// Implementierung des Intrefaces (sqlp::Recordset)
	virtual bool move (ULONG i);
	virtual ULONG cols() const { return m_aHeader.size();}
	virtual ULONG rows() const { return m_nRowCount; }
	virtual sqlp::CTerm * value(ULONG i)  { return m_aTerms[i]; }


	void aggr (sqlp::CUnary * term);
	void add (sqlp::CTerm * term, bool key=false);
	void add (short type);
	void add (CResultsetField *);
	void erase();
	void create();
	void empty();
	HREC update();

	bool get (HREC, sqlp::CRecord & raData);
	bool get (HREC, ULONG column, sqlp::CValue & raData);

	const CResultsetField & operator[] (ULONG i) const { return *m_aHeader[i]; }

	short cmp (const vector<short> & fields, const BYTE* rec1, const BYTE* rec2) const;
	short cmp (short fields, const BYTE* rec1, const BYTE* rec2) const;
	short cmp (const BYTE* rec1, const BYTE* rec2) const { return cmp(m_nPrimaryKey, rec1, rec2); }

	void sort (const vector<CTerm*> & order, vector<ULONG> & index) const;
	void sort (const vector<short> & order, vector<ULONG> & index) const;
	void sort (const vector<short> & order, vector<BYTE*> & index) const;
	ULONG rows_per_page() const { return m_nPageSize / m_nRowSize; }

	BYTE* append();
	BYTE* record(ULONG recno) const;
	ULONG rowsize() const { return m_nRowSize; }
	ULONG index (BYTE* record) const;

	vector<CResultsetField*> m_aHeader;
	vector<BYTE*> m_aPages;
	ULONG m_nRowCount;
	ULONG m_nRowSize;
	ULONG m_nPageSize;
	unsigned short m_nPrimaryKey;
	bool m_bAggregate;

	typedef map<CKeyPrivate,BYTE*> index_t;
	index_t m_aKey;

	vector<CTerm*> m_aTerms;
	sqlp::CRecord m_aCursor;
};
//---------------------------------------------------------------------------
} // namespace
#endif
