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

#include <math.h>
#include <stdx/debug.h>
#include <stdx/utils.h>
#include "function.h"


using namespace stdx;
using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
double FuncPLUS (const vector<CTerm*> & aArgs)
{
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
double FuncMULT (const vector<CTerm*> & aArgs)
{
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
double FuncMOD (const vector<CTerm*> & aArgs)
{
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
double FuncDIV (const vector<CTerm*> & aArgs)
{
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
double FuncEXP (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm * t1 = aArgs[0];
	const CTerm * t2 = aArgs[1];
	ASSUME (t1 && !t1->isNull());
	ASSUME (t2 && !t2->isNull());
	const double d1 = t1->asDouble();
	const double d2 = t2->asDouble();
	return exp(log(d1) * d2);
}
//---------------------------------------------------------------------------
double FuncLOG (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm * t1 = aArgs[0];
	const CTerm * t2 = aArgs[1];
	ASSUME (t1 && !t1->isNull());
	ASSUME (t2 && !t2->isNull());
	const double d1 = t1->asDouble();
	const double d2 = t2->asDouble();
	return log(d2) / log(d1);
}
//---------------------------------------------------------------------------
double FuncEQU (const vector<CTerm*> & aArgs)
{
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
double FuncLT (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (compare (*pTerm0, *pTerm) < +1)
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
double FuncGT (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (compare (*pTerm0, *pTerm) > -1)
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
double FuncNEQ (const vector<CTerm*> & aArgs)
{
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
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
double FuncLEQ (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
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
double FuncGEQ (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	const CTerm * pTerm0 = aArgs[0];;
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
double FuncAND (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (!pTerm->asBool())
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
double FuncOR (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	const CTerm * pTerm;
	ULONG i;

	ASSUME (nArgs);
	for (i=1; i<nArgs; i++)
	{
		pTerm = aArgs[i];
		ASSUME (pTerm);
		if (pTerm->asBool())
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
CFunctLIKE::CFunctLIKE (int nHead, const vector<CTerm*> & aArgs)
: CFunction (nHead, aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm* pTerm1 = aArgs.front();
	const CTerm* pTerm2 = aArgs.back();
	ASSUME (pTerm1);
	ASSUME (pTerm2);
	const char * s1 = pTerm1->asString();
	const char * s2 = pTerm2->asString();
	m_aToken = match (s2, "%");
}
//---------------------------------------------------------------------------
CFunctLIKE::CFunctLIKE (int head, va_list & args)
: CFunction (head, args)
{
}
//---------------------------------------------------------------------------
double FuncLIKE (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm* pTerm1 = aArgs.front();
	const CTerm* pTerm2 = aArgs.back();
	ASSUME (pTerm1);
	ASSUME (pTerm2);
	const char * s1 = pTerm1->asString();
	const char * s2 = pTerm2->asString();
	if (!s1 || !s2)
		return false;
	if (*s1==0)
		return *s2 == 0;
	if (*s2==0)
		return *s1==0;
	return match (s1, s2, "%");

	const bool bCheckS = s2[0] != '%';
	const bool bCheckE = s2[strlen(s2)-1] != '%';

	//// Tokenliste erstellen
	list<const char *> aToken;
	char szBuff[256];
	strncpy (szBuff, s2, sizeof(szBuff));
	s2 = strtok (szBuff, "%");
	while (s2)
	{
		if (s2 && strlen(s2))
			aToken.push_back (s2);
		s2 = strtok (0, "%");
	}
	//// Anfang testen
	if (bCheckS && aToken.size())
	{
		s2 = aToken.front();
		if (strncmp (s1, s2, strlen(s2)))
			return false;
	}
	//// Ende testen
	if (bCheckE && aToken.size())
	{
		s2 = aToken.back();
		const long i = strlen(s1) - strlen(s2);
		if (i < 0)
			return false;
		if (strcmp (s1+i, s2))
			return false;
	}
	//// Tokenliste abarbeiten
	list<const char *>::const_iterator iToken = aToken.begin();
	list<const char *>::const_iterator eToken = aToken.end();
	for (; iToken != eToken; iToken++)
	{
		s2 = *iToken;
		s1 = strstr(s1,s2);
		if (!s1)
			break;
		s1 += strlen(s2);
	}
	return iToken == aToken.end();
}
//---------------------------------------------------------------------------
double FuncBETWEEN (const vector<CTerm*> & aArgs)
{
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
double FuncIN (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs > 1);
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
double FuncMIN (const vector<CTerm*> & aArgs)
{
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
double FuncMAX (const vector<CTerm*> & aArgs)
{
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
// Sting Functions
//---------------------------------------------------------------------------
string FuncFMT (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs > 1);
	string  strValue;
	CValue* pFormat = dynamic_cast<CValue*>(aArgs[0]);
	CTerm * pTerm = aArgs[1];
	ASSUME (pFormat && pTerm);
	const char * szFormat = pFormat->asString();
	strValue = stringf (szFormat, pTerm);
	return strValue;
}
//---------------------------------------------------------------------------
string FuncCAT (const vector<CTerm*> & aArgs)
{
	string  strValue;
	vector<CTerm*>::const_iterator iTerm = aArgs.begin();
	vector<CTerm*>::const_iterator iEnd = aArgs.end();
	for (; iTerm != iEnd; iTerm++)
	{
		CTerm* pTerm = *iTerm;
		ASSUME (pTerm);
		strValue += pTerm->asString();
	}
	return strValue;
}
//---------------------------------------------------------------------------
} // namespace