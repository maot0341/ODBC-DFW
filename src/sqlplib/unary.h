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

#ifndef UNARY_H
#define UNARY_H

#include "sqlp.h"
#include "yac.h"
#include <stdx/debug.h>
#include <math.h>
#include <assert.h>

using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
template <short T>
class TUnary: public CUnary
{
public:
	TUnary (CTerm *term) : CUnary(T,term) {}
	virtual double asDouble() const;
	virtual const char * asString() const;
	short type() const { return CUnary::type(); }
	virtual const char * fn() const { return ""; }
};
//---------------------------------------------------------------------------
template <short T> inline
double TUnary<T>::asDouble() const
{ 	
	throw EXC("42S22", 900, "double?!\n[%s] not implemented.", m_strDebug.c_str());
	return 0; 
}
//---------------------------------------------------------------------------
template <short T> inline
const char * TUnary<T>::asString() const
{ 	
	throw EXC("42S22", 900, "string?!\n[%s] not implemented.", m_strDebug.c_str());
	return 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
inline double
TUnary<lNULL>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	return !m_pTerm || m_pTerm->isNull();
}
const char * TUnary<lNULL>::fn() const  { return "ISNULL"; }
//---------------------------------------------------------------------------
inline double
TUnary<lLEN>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	const char * szValue = m_pTerm ? m_pTerm->asString() : 0;
	if (!szValue)
		return 0;
	return strlen(szValue);
}
short TUnary<lLEN>::type() const { return SQL_INTEGER; }
const char * TUnary<lLEN>::fn() const  { return "LEN"; }
//---------------------------------------------------------------------------
inline double
TUnary<lABS>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	return fabs (dValue);
}
const char * TUnary<lABS>::fn() const  { return "ABS"; }
//---------------------------------------------------------------------------
inline double
TUnary<lSIGN>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	if (dValue < 0)
		return -1;
	if (dValue > 0)
		return +1;
	return 0;
}
short TUnary<lSIGN>::type() const { return SQL_SMALLINT; }
const char * TUnary<lSIGN>::fn() const  { return "SIGN"; }
//---------------------------------------------------------------------------
inline double
TUnary<lSQRT>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	return sqrt (dValue);
}
short TUnary<lSQRT>::type() const { return SQL_DOUBLE; }
const char * TUnary<lSQRT>::fn() const  { return "SQRT"; }
//---------------------------------------------------------------------------
inline double
TUnary<lEXP>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	return exp (dValue);
}
short TUnary<lEXP>::type() const { return SQL_DOUBLE; }
const char * TUnary<lEXP>::fn() const  { return "SQRT"; }
//---------------------------------------------------------------------------
inline double
TUnary<lLOG>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	return log (dValue);
}
short TUnary<lLOG>::type() const { return SQL_DOUBLE; }
const char * TUnary<lLOG>::fn() const  { return "LOG"; }
//---------------------------------------------------------------------------
inline double
TUnary<lNOT>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	return ((int)dValue) == 0;
}
short TUnary<lNOT>::type() const { return SQL_SMALLINT; }
const char * TUnary<lNOT>::fn() const  { return "NOT"; }
//---------------------------------------------------------------------------
inline double
TUnary<'-'>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	return -dValue;
}
const char * TUnary<'-'>::fn() const  { return "-"; }
//---------------------------------------------------------------------------
inline double
TUnary<'/'>::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if(!m_pTerm)
		return 0;
	const double dValue = m_pTerm->asDouble();
	if (!dValue)
		throw EXC("22012", 22012, "Division by zero: %s", m_strDebug.c_str());
	return 1/dValue;
}
//---------------------------------------------------------------------------
short TUnary<'/'>::type() const { return SQL_DOUBLE; }
const char * TUnary<'/'>::fn() const  { return "1/"; }
//---------------------------------------------------------------------------
} // namespace
#endif
