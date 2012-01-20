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

#include <assert.h>
#include <vector>
#include <string>
#include "sqlp.h"
#include "tableset.h"

using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
vector<string> 
match (const char * szPat, char cDel, char cEsc)
{
	vector<string> aToken;
	assert (szPat && *szPat);

	string strToken;
	const char * a = szPat;
	const char * b = a;
	while (true)
	{
		b = strchr (b, cDel);
		if (b == 0)
			break;
		if (b > szPat)
		if (b[-1] == cEsc)
		{
			strToken += string (a, b-a-1);
			strToken += cDel;
		}
		else
		{
			strToken += string (a, b-a);
			aToken.push_back (strToken);
			strToken.erase();
		}
		a = b + 1;
		b = a;
	}
	aToken.push_back (a);
	return aToken;
}
//---------------------------------------------------------------------------
vector<string> 
match (const char * szPat, const char * szEsc)
{
	vector<string> aToken;
	assert (szEsc && *szEsc);
	assert (szPat && *szPat);

	string strToken;
	int nEsc = strlen(szEsc);
	const char * a = szPat;
	const char * b = a;
	while (true)
	{
		b = strstr (b, szEsc);
		if (b == 0)
			break;
		if (strncmp (b+nEsc, szEsc, nEsc) == 0)
		{
			b += nEsc;
			strToken += string (a, b-a);
		}
		else
		{
			strToken += string (a, b-a);
			aToken.push_back (strToken);
			strToken.erase();
		}
		a = b + nEsc;
		b = a;
	}
	aToken.push_back (a);
	return aToken;
}
//---------------------------------------------------------------------------
bool match (const char * str, const vector<string> aToken)
{
	const char * t;
	const char * a = str;
	const char * b = 0;
	const int nToken = aToken.size();
	const int e = nToken-1;
	int i,n;

	if (nToken < 1)
		return true;
	t = aToken.front().c_str();
	n = strlen(t);
	//// first token
	if (*t && strncmp(a,t,n))
		return false;
	if (nToken == 1)
		return true;
	//// middle token
	for (i=1; i<e; i++)
	{
		t = aToken[i].c_str();
		if (*t == 0)
			continue;
		b = strstr(a,t);
		if (b == 0)
			return false;
		if (i == 0)
		if (a != b)
			return false;
		a = b + strlen(t);
	}
	//// last token
	t = aToken.back().c_str();
	n = strlen(t);
	if (*t == 0)
		return true;
	b = a + strlen(a) - n;
	if (*t == 0)
		return true;
	if (a > b)
		return false;
	return strcmp(b,t) == 0;
}
//---------------------------------------------------------------------------
bool match (const char * szStr, const char * szPat, const char * szEsc)
{
	vector<string> aToken = match (szPat, szEsc);
	return match (szStr, aToken);
}
//---------------------------------------------------------------------------
bool match (const char * szStr, const char * szPat, char cDel, char cEsc)
{
	vector<string> aToken = match (szPat, cDel, cEsc);
	return match (szStr, aToken);
}
//---------------------------------------------------------------------------
} // namespace