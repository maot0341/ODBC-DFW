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

#ifndef TABLE_H
#define TABLE_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>
#include "driver.h"

using namespace std;
typedef vector<CField> CTableDef;
//typedef vector<CValue> CRecord;
//---------------------------------------------------------------------------
// Basic Class
//---------------------------------------------------------------------------
class ITable
{
public:
	virtual size_t rows() const = 0;
	virtual size_t cols() const = 0;
	virtual void desc (CTableDef & f)const = 0;
	virtual const CTableDef & defs() const = 0;
	virtual void fetch (size_t i, CRecord&) const = 0;
};
//---------------------------------------------------------------------------
// Basic Class
//---------------------------------------------------------------------------
class CTableData : public ITable
{
public:
	virtual size_t rows() const = 0;
	virtual size_t cols() const                                             { return m_aFields.size(); }
	virtual void desc (CTableDef & f)const                                  { f = m_aFields; }
	virtual const CTableDef & defs() const                                  { return m_aFields; }

	virtual void fetch (size_t i, CRecord&) const = 0;
protected:
	CTableDef m_aFields;
};
//---------------------------------------------------------------------------
// Table Subset
//---------------------------------------------------------------------------
class CTableSubset : public ITable
{
public:
	CTableSubset (ITable *);
	virtual size_t rows() const;
	virtual size_t cols() const                                              { return m_pData->cols(); }
	virtual void desc (CTableDef & f) const                                  { m_pData->desc (f); }
	virtual const CTableDef & defs() const                                   { return m_pData->defs(); }

	virtual void fetch (size_t i, CRecord&) const;

protected:
	ITable * m_pData;
	vector<size_t> m_aIndex;
	size_t m_nRows;
};
//---------------------------------------------------------------------------
class CBind
{
public:
	CBind (short typ=SQL_C_DEFAULT, char*val=0, long len=0, long* ind=0)
	: m_nTargetTyp(typ), m_pTargetVal(val), m_nTargetLen(len), m_pTargetInd(ind) {}
	short			m_nTargetTyp;			/* BIND: C DATA TYPE ie SQL_C_CHAR			*/
	char* 			m_pTargetVal;			/* BIND: POINTER FROM APPLICATION TO COPY TO*/
	long			m_nTargetLen;			/* BIND: MAX SPACE IN pTargetValue			*/
	long*			m_pTargetInd;			/* BIND: TO RETURN LENGTH OR NULL INDICATOR	*/
};
//---------------------------------------------------------------------------
// Cursor Class
//---------------------------------------------------------------------------
class CCursor
{
public:

	virtual 
	SQLRETURN fetch();

	virtual 
	SQLRETURN fetch (size_t i) = 0;

protected:
	size_t m_iRecord;
	size_t m_nRecord;
};
//---------------------------------------------------------------------------
// Cursor Class
//---------------------------------------------------------------------------
class CTableCursor : public CCursor
{
public:
	CTableCursor (ITable * pData)
	{
		ASSUME (pData);
		m_pData = pData;
		m_iRecord = 0;
		m_nRecord = pData->rows();
	}
#if 0
	virtual
	SQLRETURN bind (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd);

	virtual 
	SQLRETURN get (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd);
#endif
	virtual 
	SQLRETURN fetch (size_t i);

	ITable * m_pData;

protected:
	CRecord m_aRecord;
	size_t m_iRecord;
	size_t m_nRecord;
};
//---------------------------------------------------------------------------
// Table Join
//---------------------------------------------------------------------------
class CTableJoin : public CCursor
{
public:
	CTableJoin();
	virtual size_t rows() const;
	virtual size_t cols() const;
	virtual void desc (CTableDef & f) const;
	virtual const CTableDef & defs() const;
	void add (ITable *);

	virtual
	SQLRETURN fetch (size_t i);

protected:
	vector<CTableCursor> m_aTable;
	vector<size_t> m_aIndex;
	mutable CTableDef m_aFields;
};
//---------------------------------------------------------------------------
#endif
