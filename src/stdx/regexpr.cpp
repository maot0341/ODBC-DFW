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

#include "regexpr.h"
#include "utils.h"
#include  <assert.h>
#include  <string.h>
#include  <stdlib.h>
#include  <string>

using namespace std;

namespace stdx
{
//---------------------------------------------------------------------------
CRegExp::CRegExp (const char * szExpr, const char * szAny, const char * szOne, bool cs)
{
	m_szExpr = STRLEN(szExpr) ? strdup(szExpr) : 0;
	m_aSpc[0] = STRLEN(szAny) ? strdup(szAny): 0;
	m_aSpc[1] = STRLEN(szOne) ? strdup(szOne): 0;
	m_bCaseSensitive = cs;
	init();
}
//---------------------------------------------------------------------------
CRegExp::CRegExp (const CRegExp & aRegExpr)
{
	const char * szExpr = aRegExpr.m_szExpr;
	const  char * szAny = aRegExpr.m_aSpc[0];
	const  char * szOne = aRegExpr.m_aSpc[1];
	m_szExpr = STRLEN(szExpr) ? strdup(szExpr) : 0;
	m_aSpc[0] = STRLEN(szAny) ? strdup(szAny): 0;
	m_aSpc[1] = STRLEN(szOne) ? strdup(szOne): 0;
	m_bCaseSensitive = aRegExpr.m_bCaseSensitive;
	init();
}
//---------------------------------------------------------------------------
CRegExp::~CRegExp()
{
	free ((void*)m_szExpr);
	free ((void*)m_aSpc[0]);
	free ((void*)m_aSpc[1]);
}
//---------------------------------------------------------------------------
CRegExp &
CRegExp::operator= (const CRegExp & aRegExpr)
{
	if (this == &aRegExpr)
		return *this;
	m_bCaseSensitive = aRegExpr.m_bCaseSensitive;
	init (aRegExpr.m_szExpr, aRegExpr.m_aSpc[0], aRegExpr.m_aSpc[1]);
	return *this;
}
//---------------------------------------------------------------------------
void CRegExp::init(const char * szExpr, const char * szAny, const char * szOne, bool cs)
{
	free ((void*)m_szExpr);
	free ((void*)m_aSpc[0]);
	free ((void*)m_aSpc[1]);
	m_szExpr = STRLEN(szExpr) ? strdup(szExpr) : 0;
	m_aSpc[0] = STRLEN(szAny) ? strdup(szAny): 0;
	m_aSpc[1] = STRLEN(szOne) ? strdup(szOne): 0;
	m_bCaseSensitive = cs;
	init();
}
//---------------------------------------------------------------------------
void CRegExp::init()
{
	const char * szSpc = m_aSpc[0];
	const char * szExp = m_szExpr;

	if (!STRLEN(szExp))
		return;
	if (!STRLEN(szSpc))
		return;
	const int nSpc = strlen(szSpc);
	const char * a = szExp;
	const char * e = 0;
	string strToken;
	while (*a)
	{
		e = strstr (a, szSpc);
		if (e == 0)
			break;
		strToken.assign (a, e-a);
		if (!strToken.empty())
			m_aToken.push_back (strToken);
		a = e + nSpc;
		e = 0;
	}
	if (!m_aToken.empty() || *a)
		m_aToken.push_back (a);
}
//---------------------------------------------------------------------------
int CRegExp::compare (const char * szText, const char * szPattern, size_t n) const
{
	////-----------------------------------------
	//// '?' wildcard OFF
	////-----------------------------------------
	const char * szSpc = m_aSpc[1];
	const size_t nSpc = strlen (szSpc);
	if (szSpc == 0 || nSpc == 0)
	{
		if (m_bCaseSensitive)
			return strncmp (szText, szPattern, n);
		else
			return strnicmp (szText, szPattern, n);
	}
	////-----------------------------------------
	//// '?' wildcard ON
	////-----------------------------------------
	const char * p1 = szText;
	const char * p2 = szPattern;
	assert (p1 && p2);
	////-----------------------------------------
	//// case sensitive ON
	////-----------------------------------------
	if (m_bCaseSensitive)
	while (*p1 && *p2 && n>0)
	{
		if (strcmp (p2, szSpc)==0)
		{
			p1 += nSpc;
			p2 += nSpc;
			n -= nSpc;
			continue;
		}
		if (*p1 < *p2)
			return -1;
		if (*p1 > *p2)
			return -1;
		p1++;
		p2++;
		n--;
	}
	////-----------------------------------------
	//// case sensitive OFF
	////-----------------------------------------
	else
	while (*p1 && *p2 && n>0)
	{
		if (strcmp (p2, szSpc))
		{
			p1 += nSpc;
			p2 += nSpc;
			n -= nSpc;
			continue;
		}
		const char c1 = tolower(*p1);
		const char c2 = tolower(*p2);
		if ( c1 < c2)
			return -1;
		if (c1 > c2)
			return -1;
		p1++;
		p2++;
		n--;
	}
	if (!*p1 && *p2)
		return -1;
	if (*p1 && !*p2)
		return +2;
	assert (n == 0);
	return 0;
}
//---------------------------------------------------------------------------
const char * CRegExp::strstr (const char * szText, const char * szPattern) const
{
	if (m_bCaseSensitive)
		return ::strstr (szText, szPattern);
	const char * p = szText;
	while (p)
	{
		if (*p == 0)
			break;
		if (compare (p, szPattern, strlen(szPattern)) == 0)
			return p;
		p++;
	}
	return 0;
}
//---------------------------------------------------------------------------
bool CRegExp::match (const char *szText) const
{
	if (!m_aToken.size())
		return true;
	const int n = m_aToken.size() - 1;
	const char * s = szText;
	const char * szToken;
	int i;
	for (i=0; i<n; i++)
	{
		szToken = m_aToken[i].c_str();
		if (!szToken)
			continue;
		s = strstr (s, szToken);
		if (!s)
			return false;
		if (i==0 && s != szText)
			return false;
		s += strlen(szToken);
	}
	if (i > n)
		return true;
	////-----------------------------------------
	//// last token
	////-----------------------------------------
	szToken = m_aToken[i].c_str();
	if (STRLEN(szToken))
	if (compare (s, szToken, strlen(szToken)) != 0)
		return false;
	return true;
}
//---------------------------------------------------------------------------
} // namespace stdx