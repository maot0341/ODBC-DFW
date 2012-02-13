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

#ifndef FUNCTION_H
#define FUNCTION_H

#include "sqlp.h"
#include "yac.h"
#include <stdx/debug.h>
#include <stdx/regexpr.h>
#include <math.h>
#include <assert.h>
#include <vector>

using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
class CFuncLIKE : public CFunction
{
public:
	CFuncLIKE (int head, const vector<CTerm*> & args);
	CFuncLIKE (int head, va_list &);

protected:
	stdx::CRegExp m_aRegExp;
//	vector<string> m_aToken;
};
//---------------------------------------------------------------------------
template <short T>
class CFunc: public CFunction
{
public:
	CFunc (const vector<CTerm*> & args);
	CFunc (va_list &);
	virtual double asDouble() const;
	virtual const char * asString() const;
};
//---------------------------------------------------------------------------
template <short T> inline
CFunc<T>::CFunc (const vector<CTerm*> & args)
: CFunction (T, args)
{
}
//---------------------------------------------------------------------------
template <short T> inline
CFunc<T>::CFunc(va_list & args)
: CFunction (T, args)
{
}
//---------------------------------------------------------------------------
template <short T> inline
double CFunc<T>::asDouble() const
{ 	
	throw EXC("42S22", 900, "double?!\n[%s] not implemented.", m_strDebug.c_str());
	return 0; 
}
//---------------------------------------------------------------------------
template <short T> inline
const char * CFunc<T>::asString() const
{ 	
	throw EXC("42S22", 900, "string?!\n[%s] not implemented.", m_strDebug.c_str());
	return 0;
}
//---------------------------------------------------------------------------
// Functions (Implementation)
//---------------------------------------------------------------------------
inline double
CFunc<'+'>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	double dValue = 0;
	ULONG nValue = 0;
	for (i=0; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (pTerm->isNull())
			continue;
		dValue += pTerm->asDouble();
		nValue++;
	}
	return dValue;
}
//---------------------------------------------------------------------------
inline const char *
CFunc<'+'>::asString() const
{
	const vector<CTerm*> & aArgs = args();
	vector<CTerm*>::const_iterator iTerm = aArgs.begin();
	vector<CTerm*>::const_iterator iEnd = aArgs.end();
	for (m_strValue.erase(); iTerm != iEnd; iTerm++)
	{
		CTerm* pTerm = *iTerm;
		ASSUME (pTerm);
		m_strValue += pTerm->asString();
	}
	return m_strValue.c_str();
}
//---------------------------------------------------------------------------
inline double
CFunc<'*'>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	double dValue = 1;
	ULONG nValue = 0;
	for (i=0; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (pTerm->isNull())
			continue;
		dValue *= pTerm->asDouble();
		nValue++;
	}
	return dValue;
}
//---------------------------------------------------------------------------
inline double
CFunc<lMOD>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();

	ASSUME (nArgs == 2);
	const CTerm * t1 = aArgs[0];
	const CTerm * t2 = aArgs[1];
	ASSUME (t1 && !t1->isNull());
	ASSUME (t2 && !t2->isNull());
	long n1 = t1->asInteger();
	long n2 = t2->asInteger();
	return n1 % n2;
}
//---------------------------------------------------------------------------
inline double
CFunc<lDIV>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();

	ASSUME (nArgs == 2);
	const CTerm * t1 = aArgs[0];
	const CTerm * t2 = aArgs[1];
	ASSUME (t1 && !t1->isNull());
	ASSUME (t2 && !t2->isNull());
	long n1 = t1->asInteger();
	long n2 = t2->asInteger();
	return n1 / n2;
}
//---------------------------------------------------------------------------
inline double
CFunc<lEXP>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm * t1 = aArgs[0];
	const CTerm * t2 = aArgs[1];
	ASSUME (t1 && !t1->isNull());
	ASSUME (t2 && !t2->isNull());
	const double d1 = t1->asDouble();
	const double d2 = t2->asDouble();
	return exp(log(d2) * d1);
}
//---------------------------------------------------------------------------
inline double
CFunc<lLOG>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm * t1 = aArgs[0];
	const CTerm * t2 = aArgs[1];
	ASSUME (t1 && !t1->isNull());
	ASSUME (t2 && !t2->isNull());
	const double d1 = t1->asDouble();
	const double d2 = t2->asDouble();
	return log(d1) / log(d2);
}
//---------------------------------------------------------------------------
inline double
CFunc<'='>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (*pTerm0 != *pTerm)
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<'<'>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (compare (*pTerm0, *pTerm) >= 0)
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<'>'>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (compare (*pTerm0, *pTerm) <= 0)
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<lNEQ>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (*pTerm0 == *pTerm)
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<lLEQ>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (compare (*pTerm0, *pTerm) > 0 )
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<lGEQ>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (compare (*pTerm0, *pTerm) < 0 )
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<lAND>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	for (i=0; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (!pTerm->asBool())
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<lOR>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	for (i=0; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (pTerm->asBool())
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
inline double
CFunc<lBETWEEN>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 3);
	CTerm * pTerm = aArgs[0];
	CTerm * pBoundL = aArgs[1];
	CTerm * pBoundU = aArgs[2];
	if (*pBoundL > *pBoundU)
		swap (pBoundL, pBoundU);
	if (*pBoundL > *pTerm)
		return false;
	if (*pBoundU < *pTerm)
		return false;
	return true;
}
//---------------------------------------------------------------------------
inline double
CFunc<lIN>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	//ASSUME (nArgs > 1);
	CTerm* pTerm0 = aArgs[0];
	ASSUME (pTerm0);
	ULONG i;
	for (i=1; i<nArgs; i++)
	{
		CTerm* pTerm = aArgs[i];
		ASSUME (pTerm);
		if (*pTerm0 == *pTerm )
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
inline double
CFunc<lMIN>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs);
	CTerm* pTerm0 = aArgs[0];
	ASSUME (pTerm0);
	double dResult = pTerm0->asDouble();
	ULONG i;
	for (i=1; i<nArgs; i++)
	{
		CTerm* pTerm = aArgs[i];
		ASSUME (pTerm);
		double dValue = pTerm->asDouble();
		if (dValue < dResult)
			dResult = dValue;
	}
	return dResult;
}
//---------------------------------------------------------------------------
inline double
CFunc<lNVL>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	CTerm* pTerm0 = aArgs[0];
	ASSUME (pTerm0);
	CTerm* pTerm1 = aArgs[1];
	ASSUME (pTerm1);
	if (pTerm0->isNull())
	{
		ASSUME (!pTerm1->isNull());
		return pTerm1->asDouble();
	}
	return pTerm0->asDouble();
}
//---------------------------------------------------------------------------
inline const char *
CFunc<lNVL>::asString() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	CTerm* pTerm0 = aArgs[0];
	ASSUME (pTerm0);
	CTerm* pTerm1 = aArgs[1];
	ASSUME (pTerm1);
	if (pTerm0->isNull())
	{
		ASSUME (!pTerm1->isNull());
		return pTerm1->asString();
	}
	return pTerm0->asString();
}
//---------------------------------------------------------------------------
inline double
CFunc<lMAX>::asDouble() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs);
	CTerm* pTerm0 = aArgs[0];
	ASSUME (pTerm0);
	double dResult = pTerm0->asDouble();
	ULONG i;
	for (i=1; i<nArgs; i++)
	{
		CTerm* pTerm = aArgs[i];
		ASSUME (pTerm);
		double dValue = pTerm->asDouble();
		if (dValue > dResult)
			dResult = dValue;
	}
	return dResult;
}
//---------------------------------------------------------------------------
// String Functions
//---------------------------------------------------------------------------
inline const char *
CFunc<lFMT>::asString() const
{
	const vector<CTerm*> & aArgs = args();
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs > 1);
	CValue* pFormat = dynamic_cast<CValue*>(aArgs[0]);
	CTerm * pTerm = aArgs[1];
	ASSUME (pFormat && pTerm);
	const char * szFormat = pFormat->asString();
	m_strValue = stringf (szFormat, pTerm);
	return m_strValue.c_str();
}
//---------------------------------------------------------------------------
} // namespace
#endif
