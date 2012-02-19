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

#ifndef __STDX_UTILS_H__
#define __STDX_UTILS_H__

#include <stdarg.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

using namespace std;

#define BIT(i) (1 << (i))
#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))
#define STRLEN(s) ((s) && *(s))
#define STRNUL(s) (!(s) || (*(s)) == 0)
#define NVL(v,n) (((v)==0) ? (n) : (v))
#define LIMIT(v,g1,g2)  (((v) < (g1)) ? (g1) : ((v) > (g2)) ? g2 : (v))
#define FIND(v,x) std::find ((v).begin(), (v).end(), x)
#define CONTAINS(v,x) (std::find ((v).begin(), (v).end(), x) != (v).end())
#define SKIPSPC(p) while(*(p) && isspace(*(p))) (p)++
//#define STRNVL(s) (s) ? (s) : string()

namespace stdx
{
//---------------------------------------------------------------------------
#define VSORT(v) if ((v).size() > 1) sort((v).begin(), (v).end())
//---------------------------------------------------------------------------
template<class K, class T>
vector<K> keys (const std::map<K,T> & aMap)
{
	vector<K> aList;
	split (aMap, &aList, (vector<T>*)0);
	return aList;
}
//---------------------------------------------------------------------------
template<class K, class T>
vector<T> data (const std::map<K,T> & aMap)
{
	vector<T> aList;
	split (aMap, 0, &aList);
	return aList;
}
//---------------------------------------------------------------------------
template<class K, class T>
void split (const std::map<K,T> & aMap, vector<K> * pKeys, vector<T> * pData)
{
	if (pKeys == 0 && pData == 0)
		return;
	const size_t n = aMap.size();
	std::map<K,T>::const_iterator i = aMap.begin();
	std::map<K,T>::const_iterator e = aMap.end();
	size_t k=0;
	size_t l=0;
	if (pKeys)
		pKeys->reserve(n);
	if (pData)
		pData->reserve(n);
	for (; i!=e ; i++)
	{
		if (pKeys)
			pKeys->push_back ((*i).first);
		if (pData)
			pData->push_back ((*i).second);
	}
}
//-----------------------------------------------------------------------------
inline
std::string strprintf (const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	return std::string(szBuff);
}
//---------------------------------------------------------------------------
inline std::string
STRNVL (const char * s)
{
	if (s)
		return std::string(s);
	return string();
}
//---------------------------------------------------------------------------
inline
vector<string> split (const char * szText, char cDel, char cStr=0, bool bAllowEmpty=true)
{
	vector<string> aList;
	if (STRNUL(szText))
		return aList;
	string strName;
	const char * a = szText;
	const char * k;
	const char * e;
	while (true)
	{
		while (*a && isspace(*a))
			a++;
		if (*a == 0)
			break;
		k = strchr (a, cDel);
		if (cStr && *a == cStr)
		{
			a++;
			e = strchr (a, cStr);
			assert (e);
			assert (k == 0 || e < k);
			strName.assign (a, e-a);
		}
		else
		{
			e = (k == 0) ? a + strlen(a) : k;
			while (--e >= a && isspace(*e));
			strName.assign (a, e-a+1);
		}
		if (!strName.empty() || bAllowEmpty)
			aList.push_back (strName);
		if (k == 0)
			break;
		a = k + 1;
	}
	return aList;
}
//---------------------------------------------------------------------------
inline
string join (const string & s1, const string & s2, const string & delem)
{
	if (s1.empty())
		return s2;
	if (s2.empty())
		return s1;
	return  s1 + delem + s2;
}
//---------------------------------------------------------------------------
inline
string join (const char* s1, const char* s2, const char* delem)
{
	return join (STRNVL(s1), STRNVL(s2), STRNVL(delem));
}
//---------------------------------------------------------------------------
inline
const char *
stristr (const char * s1, const char * s2)
{
	if (!STRLEN(s1))
		return 0;
	if (!STRLEN(s2))
		return s1;
	const char * p1;
	const size_t n1 = strlen(s1);
	const size_t n2 = strlen(s2);
	const char * e1 = s1 + (n1 - n2);
	for (p1=s1; p1<=e1; p1++)
	{
		if (strnicmp (p1,s2,n2) == 0)
			return p1;
	}
	return 0;
}
//---------------------------------------------------------------------------
} // namespace stdx
#endif
