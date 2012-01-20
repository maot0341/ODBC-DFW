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

#include  <string.h>
#include  <stdlib.h>
#include  <string>
#include "regexpr.h"

using namespace std;

namespace stdx
{
//---------------------------------------------------------------------------
CRegExpr::CRegExpr (const char * szExpr, const char * szAny, const char * szOne)
{
	m_szExpr = szExpr ? strdup(szExpr) : 0;
	m_aSpc[0] = szAny ? strdup(szAny): 0;
	m_aSpc[1] = szOne ? strdup(szOne): 0;
	init();
}
//---------------------------------------------------------------------------
CRegExpr::~CRegExpr()
{
	free ((void*)m_szExpr);
	free ((void*)m_aSpc[0]);
	free ((void*)m_aSpc[1]);
}
#define TOKEN(s,b) (strncmp (s, b, strlen(b)) == 0)
//---------------------------------------------------------------------------
void CRegExpr::init()
{
	const char * szSpc = m_aSpc[0];
	const char * szExp = m_szExpr;

	if (!szExp || *szExp == 0)
		return;
	if (!szSpc || *szSpc == 0)
		return;
	const int nSpc = strlen(szSpc);
	const char * a = szExp;
	const char * e = 0;
	while (*a)
	{
		string strToken;
		e = strstr (a, szSpc);
		if (e == 0)
			strToken = a;
		else
			strToken.assign (a, e-a);
		m_aToken.push_back (strToken);
		if (e == 0)
			break;
		a = e + nSpc;
	}
	if (e && strcmp (e,szSpc) == 0)
		m_aToken.push_back (string());
}
//---------------------------------------------------------------------------
bool CRegExpr::match(const char *szString) const
{
	if (!m_aToken.size())
		return true;
	const int n = m_aToken.size() - 1;
	const char * s = szString;
	const char * szToken = m_aToken[0].c_str();
	if (*szToken)
	{
		const int n = strlen(szToken);
		if (strncmp(s, szToken, n))
			return false;
		s += n;
	}
	int i;
	for (i=1; i<n; i++)
	{
		szToken = m_aToken[i].c_str();
		if (!szToken)
			continue;
		s = strstr (s, szToken);
		if (!s)
			return false;
		s += strlen(szToken);
	}
	if (i > n)
		return true;
	szToken = m_aToken[i].c_str();
	if (*szToken && strcmp(s, szToken))
		return false;
	return true;



	const char * p = m_szExpr;
	const char * e = m_szExpr;
	while (true)
	{
		if (TOKEN(e, m_aSpc[0]))
		{
			e++;
			const char * a = e;
			while (*e && !TOKEN(e,m_aSpc[0]) && !TOKEN(e,m_aSpc[1]))
				e++;
			string strToken(a, e-a);
			const char * szToken = strToken.c_str();

		}
		if (TOKEN(e, m_aSpc[1]))
		{
			s++;
			e++;
		}
		if (*e == *s)
		{
			e++;
			s++;
		}

	}
}
//---------------------------------------------------------------------------
} // namespace stdx