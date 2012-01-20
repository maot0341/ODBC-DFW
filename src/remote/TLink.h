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

#ifndef TLINK_H
#define TLINK_H

#include <time.h>


//---------------------------------------------------------------------------
// CLink
//---------------------------------------------------------------------------
class CLink
{
public:
	virtual void write (const idl::typVariant &) const = 0;
	static CLink* create (SQLSMALLINT type, SQLPOINTER val, SQLINTEGER len=0, SQLINTEGER* ind=0);
};
//---------------------------------------------------------------------------
#define SQL_SETPTR(p,v) if (p) (*p) = (v)
template<class T>
class TLink: public CLink
{
public:
	TLink (SQLPOINTER val, SQLINTEGER len=0, SQLINTEGER* ind=0)
	: m_pVal((T*)val), m_nLen(len), m_pInd(ind)
	{}

	virtual void write (const idl::typVariant & vVariant) const;

protected:
	T*           m_pVal;
	SQLINTEGER   m_nLen;
	SQLINTEGER*  m_pInd;
};
//---------------------------------------------------------------------------
template<class T>
void TLink<T>::write (const idl::typVariant & vVariant) const
{
	////-----------------------------------------
	//// Indicator
	////-----------------------------------------
	if (vVariant.isNull)
	{
		SQL_SETPTR(m_pInd, SQL_NULL_DATA);
		return;
	}
	SQL_SETPTR (m_pInd, sizeof(T));
	////-----------------------------------------
	//// Value
	////-----------------------------------------
	if (!m_pVal)
		return;
	const idl::typValue & aValue = vVariant.aValue;
	const idl::typTerm eType = aValue._d();
	switch (eType)
	{
	case idl::TermSTRING:
		strncpy ((char*)m_pVal, aValue.aString(), m_nLen);
		break;
	case idl::TermBOOLEAN:
		*m_pVal = aValue.nInteger();
		break;
	case idl::TermINTEGER:
		*m_pVal = aValue.nInteger();
		break;
	case idl::TermNUMBER:
		*m_pVal = aValue.dNumber();
		break;
	case idl::TermTIME:
		*m_pVal = aValue.dTime();
		break;
	default:
		throw "term-type!?";
	}
}
//---------------------------------------------------------------------------
void TLink<TIMESTAMP_STRUCT>::write (const idl::typVariant & vVariant) const
{
	////-----------------------------------------
	//// Indicator
	////-----------------------------------------
	if (vVariant.isNull)
	{
		SQL_SETPTR(m_pInd, SQL_NULL_DATA);
		return;
	}
	SQL_SETPTR (m_pInd, sizeof(T));
	////-----------------------------------------
	//// Value
	////-----------------------------------------
	if (!m_pVal)
		return;
	const idl::typValue & aValue = vVariant.aValue;
	const idl::typTerm eType = aValue._d();
	const time_t nTime
	= (eType == idl::TermTIME) ? aValue.dTime()
	: (eType == idl::TermNUMBER) ? aValue.dNumber()
	: (eType == idl::TermINTEGER) ? aValue.nInteger()
	: 0;
	struct tm * pTM = localtime (&nTime);
	if (!pTM)
		SQL_SETPTR(m_pInd, SQL_NULL_DATA);
	else
	{
		m_pVal->year = pTM->tm_year + 1900;
		m_pVal->month = pTM->tm_mon + 1;
		m_pVal->day = pTM->tm_mday;
		m_pVal->hour = pTM->tm_hour;
		m_pVal->minute = pTM->tm_min;
		m_pVal->second = pTM->tm_sec;
		m_pVal->fraction = 0;
	}
}
//---------------------------------------------------------------------------
inline
CLink* CLink::create (SQLSMALLINT type, SQLPOINTER val, SQLINTEGER len, SQLINTEGER* ind)
{
	if (type == SQL_C_CHAR)
		return new TLink<char>(val,len,ind);
	if (type == SQL_C_LONG)
		return new TLink<long>(val,len,ind);
	if (type == SQL_C_SHORT)
		return new TLink<short>(val,len,ind);
	if (type == SQL_C_FLOAT)
		return new TLink<float>(val,len,ind);
	if (type == SQL_C_TYPE_TIMESTAMP)
		return new TLink<TIMESTAMP_STRUCT>(val,len,ind);
	return 0;
}
//---------------------------------------------------------------------------
#endif
