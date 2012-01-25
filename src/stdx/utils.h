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

#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <map>

using namespace std;

#define BIT(i) (1 << (i))
#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))
#define NVL(v,n) (((v)==0) ? (n) : (v))
#define LIMIT(v,g1,g2)  (((v) < (g1)) ? (g1) : ((v) > (g2)) ? g2 : (v))
#define FIND(v,x) std::find ((v).begin(), (v).end(), x)
#define CONTAINS(v,x) (std::find ((v).begin(), (v).end(), x) != (v).end())
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
//---------------------------------------------------------------------------
template<class T>
T format (const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	return T(szBuff);
}
//---------------------------------------------------------------------------
inline bool
STRNUL (const char * s)
{
	return s == 0 || *s == 0;
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
} // namespace stdx
#endif
