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

#include "yac.h"
#include "sqlp.h"
#include "desc.h"
#include "tableset.h"
#include "resultset.h"
#include "function.h"
#include "unary.h"
#include <stdx/debug.h>
#include <stdx/utils.h>

#include <assert.h>
#include <typeinfo.h>
#include <windows.h>
#include <sqlext.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include <string>
#include <set>
#include <algorithm>


using namespace std;
using namespace stdx;
//---------------------------------------------------------------------------
void yyfprintf (FILE * f, const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	OutputDebugString (szBuff);
}

using namespace std;
using namespace stdx;
namespace sqlp {

//---------------------------------------------------------------------------
double SQLTime (time_t nTime)
{
	static const double dSecPerDay = 3600*24;
	return ((double)nTime) / dSecPerDay;
}
//---------------------------------------------------------------------------
vector<string> SQLNameList (const char * szList)
{
	vector<string> aList;
	if (!STRLEN(szList))
		return aList;
	string strName;
	const char * a = szList;
	const char * k;
	const char * e;
	while (true)
	{
		while (*a && isspace(*a))
			a++;
		if (*a == 0)
			break;
		k = strchr (a, ',');
		if (*a == '\'')
		{
			a++;
			e = strchr (a, '\'');
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
		if (!strName.empty())
			aList.push_back (strName);
		if (k == 0)
			break;
		a = k + 1;
	}
	return aList;
}
//---------------------------------------------------------------------------
short SQLTypeOrder (short nType)
{
	switch (nType)
	{
	case SQL_BIT:
		return 1;
	case SQL_INTEGER:
	case SQL_SMALLINT:
	case SQL_BIGINT:
	case SQL_TINYINT:
	case SQL_GUID:
		return 2;
	case SQL_NUMERIC:
	case SQL_DECIMAL:
	case SQL_FLOAT:
	case SQL_REAL:
	case SQL_DOUBLE:
		return 3;
	case SQL_DATETIME:
	case SQL_TYPE_DATE:
	case SQL_TYPE_TIME:
	case SQL_TYPE_TIMESTAMP:
	case SQL_TIME:
	case SQL_TIMESTAMP:
		return 4;
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
		return 5;
	case SQL_BINARY:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
		return 6;
	}
	return 0;
}
//---------------------------------------------------------------------------
string stringf (const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	return string(szBuff);
}
//---------------------------------------------------------------------------
string stringf (CTerm* pTerm)
{
	char szValue[256] = "<nothing>";
	if (!pTerm)
		return szValue;
	if (pTerm->isNull())
		return "nul";
	short nType = pTerm->type();
	if (nType == SQL_UNKNOWN_TYPE)
		return szValue;

	if (CTerm::isString (nType))
	{
		sprintf (szValue, "%-10s", pTerm->asString());
		return szValue;
	}
	double dValue = pTerm->asDouble();
	if (CTerm::isFloat (nType))
		sprintf (szValue, "%f", dValue);
	if (CTerm::isInteger (nType))
		sprintf (szValue, "%6d", (long)dValue);
	if (CTerm::isDateTime (nType))
	{
		time_t nTime = pTerm->asInteger();
		struct tm * pTime = localtime (&nTime);
		if (!pTime)
			return "null";
		sprintf (szValue, "%02d.%02d.%4d  %02d:%02d"
		, pTime->tm_mday
		, pTime->tm_mon + 1
		, pTime->tm_year + 1900
		, pTime->tm_hour
		, pTime->tm_min);
		return szValue;
	}
	if (CTerm::isBool (nType))
		sprintf (szValue, "%c", pTerm->asBool() ? 'T' : 'F');
	return szValue;
}
//---------------------------------------------------------------------------
string stringf (const char * szFormat, CTerm* pTerm)
{
	if (!szFormat || !pTerm || pTerm->isNull())
		return stringf (pTerm);

	const short nType = pTerm->type();
	char szResult[8000] = "<value>";
	const char * sz1;
	const char * sz2;

	sz1 = strchr (szFormat, '%');
	while (sz1 && *(sz1+1) == '%')
		sz1 = strchr (sz1+2, '%');
	if (sz1 == 0)
		return szFormat;

	for (sz2=sz1; *sz2 != 0 && !isalpha(*sz2); sz2++);
	string strFormat (sz1, sz2 - sz1);
	if (CTerm::isFloat (nType))
	{
		double dValue = pTerm->asDouble();
		if (*sz2 == 'x' || *sz2 == 'X')
		{
			strFormat += *sz2;
			sprintf (szResult, strFormat.c_str(), (long)dValue);
		}
		else if (strspn(sz2, "eEfgG"))
		{
			strFormat += *sz2;
			sprintf (szResult, strFormat.c_str(), dValue);
		}
		else
		{
			strFormat += "f";
			sprintf (szResult, strFormat.c_str(), dValue);
		}
	}
	if (CTerm::isInteger (nType))
	{
		long nValue = pTerm->asInteger();
		strFormat += "ld";
		sprintf (szResult, strFormat.c_str(), nValue);
	}
	if (CTerm::isString (nType))
	{
		const char * szValue = pTerm->asString();
		strFormat += 's';
		sprintf (szResult, strFormat.c_str(), szValue);
	}
	if (CTerm::isDateTime (nType))
	{
		time_t nTime = pTerm->asInteger();
		struct tm * pTime = localtime (&nTime);
		if (!pTime)
			return "null";
		strftime (szResult, sizeof(szResult), szFormat, pTime);
		return szResult;
	}
	string strValue;
	strValue.append (szFormat, sz1 - szFormat);
	strValue.append (szResult);
	strValue.append (sz1 + (++sz2 - sz1));
	return strValue;
}
//---------------------------------------------------------------------------
void dump (const CValue & aValue)
{
	short nType = aValue.type();
	if (aValue.isNull())
		printf ("<null>\t");
	else if (CTerm::isString (nType))
		printf ("%s\t", aValue.asString());
	else if (CTerm::isFloat (nType))
		printf ("%lf\t", aValue.asDouble());
	else if (CTerm::isInteger (nType))
		printf ("%d\t", aValue.asInteger());
	else if (CTerm::isBool (nType))
		printf ("%s\t", aValue.asBool() ? "true" : "false");
	else
		printf ("<unknown>\t");
}
//---------------------------------------------------------------------------
void dump (const CRecord & aRecord)
{
	ULONG i;
	const ULONG n = aRecord.size();
	for (i=0; i<n; i++)
	{
		const CValue & aValue = aRecord[i];
		dump (aValue);
	}
	printf ("\n");
}
//---------------------------------------------------------------------------
void dump (const vector<CRecord> & aRecordset)
{
	ULONG i;
	const ULONG n = aRecordset.size();
	for (i=0; i<n; i++)
	{
		const CRecord & aRecord = aRecordset[i];
		dump (aRecord);
	}
}
//---------------------------------------------------------------------------
// Args - Id
//---------------------------------------------------------------------------
string id(const CObject* pObject);
string id(const vector<CTerm*> & aArgs, const char * szSep=",")
{
	string str;
	vector<CTerm*>::const_iterator iTerm = aArgs.begin();
	for (; iTerm != aArgs.end(); iTerm++)
	{
		if (iTerm != aArgs.begin())
			str.append (szSep);
		const CTerm * pTerm = *iTerm;
		string strTerm = id(pTerm);
		str.append (strTerm);
	}
	return str;
}
//---------------------------------------------------------------------------
// Args - Id
//---------------------------------------------------------------------------
string id (vector<CTerm*>::const_iterator ia, vector<CTerm*>::const_iterator ie)
{
	string str = "(";
	vector<CTerm*>::const_iterator i = ia;
	for (; i != ie; i++)
	{
		if (i != ia)
			str += ",";
		str += id(*i);
	}
	str += ")";
	return str;
}
//---------------------------------------------------------------------------
// Term - Id
//---------------------------------------------------------------------------
string id(const CObject* pObject)
{
	const CTable * pTable = dynamic_cast<const CTable*>(pObject);
	if (pTable)
	{
		return STRNVL(pTable->name());
	}
	const CColumn * pColumn = dynamic_cast<const CColumn*>(pObject);
	if (pColumn)
	{
		const char * szTable = pColumn->table();
		const char * szColumn = pColumn->column();
//		return strColumn;
		string str;
		if (szTable && *szTable)
		{
			str.append (szTable);
			str.append (".");
		}
		if (szColumn && *szColumn)
			str.append (szColumn);
		else
			str.append ("*");
		return str;
	}
	const CParam * pParam = dynamic_cast<const CParam*>(pObject);
	if (pParam)
	{
		return "?";
	}
	const CValue * pValue = dynamic_cast<const CValue*>(pObject);
	if (pValue)
	{
		const short nType = pValue->type();
		char sz[8000];
		if (CTerm::isString (nType))
			sprintf (sz, "'%s'", pValue->asString());
		else if (CTerm::isNumber (nType))
			sprintf (sz, "%lg", pValue->asDouble());
		else
			strcpy (sz, "val");
		return string(sz);
	}
	const CUnary * pUnary = dynamic_cast<const CUnary*>(pObject);
	if (pUnary)
	{
		string strTerm = "*";
		if (pUnary->m_pTerm)
			strTerm = id(pUnary->m_pTerm);
		const int nHead = pUnary->m_nHead;
		const char * szTerm = strTerm.c_str();
		const char * szFunc
		= nHead == lCOUNT ? "COUNT"
		: nHead == lSUM ? "SUM"
		: nHead == lAVG ? "AVG"
		: nHead == lMAX ? "MAX"
		: nHead == lMIN ? "MIN"
		: nHead == lFIRST ? "FIRST"
		: nHead == lLAST ? "LAST"
		: nHead == lNAME ? "NAME"
		: nHead == lABS ? "ABS"
		: nHead == lNULL ? "IS NULL"
		: nHead == lNOT ? "NOT"
		: nHead == lLEN ? "LEN"
		: nHead == lLOG ? "LOG"
		: nHead == lEXP ? "EXP"
		: nHead == lSQRT ? "SQRT"
		: nHead == '/' ? "1/"
		: 0;
		if (szFunc)
			return stringf ("%s(%s)", szFunc, szTerm);

		szFunc
		= nHead == lASC ? "ASC"
		: nHead == lDESC ? "DESC"
		: 0;
		if (szFunc)
			stringf ("%s %s", szTerm, szFunc);
		if (ispunct(nHead))
			return stringf ("%c%s", (char)nHead, szTerm);
		return stringf ("#%d(%s)", nHead, szTerm);
	}
	const CFunction * pFunc = dynamic_cast<const CFunction*>(pObject);
	if (pFunc)
	{
		const int nHead = pFunc->head();
		const vector<CTerm*> & aArgs = pFunc->args();
		const char * szFunc = 0;

		szFunc
		= nHead == lAND ? "AND"
		: nHead == lOR ? "OR"
		: nHead == lNEQ ? "<>"
		: nHead == lLEQ ? "<="
		: nHead == lGEQ ? ">="
		: nHead == lNULL ? ","
		: nHead == '>' ? ">"
		: nHead == '<' ? "<"
		: nHead == '=' ? "="
		: nHead == '+' ? "+"
		: nHead == '*' ? "*"
		: 0;
		if (szFunc)
			return id (aArgs, szFunc);

		szFunc
		= nHead == lMIN ? "MIN"
		: nHead == lMAX ? "MAX"
		: nHead == lEXP ? "EXP"
		: nHead == lLOG ? "LOG"
		: 0;
		if (szFunc)
			return string(szFunc) + id(aArgs.begin(),aArgs.end());

		if (nHead == lBETWEEN)
		{
			vector<CTerm*>::const_iterator iTerm = aArgs.begin();
			const CTerm * pTerm = *iTerm;
			const CTerm * pLim1 = *(iTerm++);
			const CTerm * pLim2 = *(iTerm++);
			ASSUME (pTerm && pLim1 && pLim2);
			string str = id(pTerm);
			str += " BETWEEN ";
			str +=  id (pLim1);
			str += " AND ";
			str += id (pLim2);
			return str;
		}
		if (nHead == lIN)
		{
			const int nArgs = aArgs.size();
			assert (nArgs > 0);
			const CTerm * pTerm = aArgs[0];
			string str = id (pTerm);
			str += " IN ";
			str += id (aArgs.begin()+1, aArgs.end());
			return str;
		}
		const char * szFormat
		= nHead == lFMT ? "FMT(%s)"
		: 0;
		char sz[8000];
		int nMax = 7000;
		string strArgs = id (aArgs);
		if (strArgs.length() >= nMax)
		{
			strArgs.erase (nMax-1);
			strArgs += " ...";
		}
		if (szFormat)
			sprintf (sz, szFormat, strArgs.c_str());
		else
			sprintf (sz, "#%d(%s)", nHead, strArgs.c_str());
		return sz;
	}
	const CJoin* pJoin = dynamic_cast<const CJoin*>(pObject);
	if (pJoin)
	{
		char sz[8000];
		const int nHead = pJoin->m_nHead;
		const char * szHead
		= nHead == lINNER ? "INNER"
		: nHead == lLEFT ? "LEFT"
		: nHead == lRIGHT ? "RIGHT"
		: nHead == lFULL ? "FULL"
		: "???";
		string strCond = id (pJoin->m_pCond);
		sprintf (sz, "%s %s JOIN %s ON %s", pJoin->m_szLeft, szHead, pJoin->m_szRight, strCond.c_str());
		return sz;
	}
	return "<object>";
}
//---------------------------------------------------------------------------
// Term - Gruppenfunktion
//---------------------------------------------------------------------------
const CUnary * GroupFn (const CObject* pTerm)
{
	const CUnary * pUnary = dynamic_cast<const CUnary*>(pTerm);
	if (!pUnary)
		return 0;
	const int nHead = pUnary->head();
	if (nHead == lCOUNT)
		return pUnary;
	if (nHead == lAVG)
		return pUnary;
	if (nHead == lSUM)
		return pUnary;
	if (nHead == lMIN)
		return pUnary;
	if (nHead == lMAX)
		return pUnary;
	if (nHead == lMIN)
		return pUnary;
	if (nHead == lFIRST)
		return pUnary;
	if (nHead == lLAST)
		return pUnary;
	return 0;
}
//---------------------------------------------------------------------------
bool contains (const vector<CTerm*> & aGroup, const CTerm * pTerm)
{
	const ULONG nGroup = aGroup.size();
	ULONG i;
	if (!pTerm)
		return true;
	if (GroupFn(pTerm))
		return true;
	for (i=0; i<nGroup; i++)
	if (aGroup[i] == pTerm)
		return true;
	const CValue * pValue = dynamic_cast<const CValue*>(pTerm);
	if (pValue)
		return true;
	const CUnary * pUnary = dynamic_cast<const CUnary*>(pTerm);
	if (pUnary)
		return contains (aGroup, pUnary->m_pTerm);
	const CColumn * pColumn = dynamic_cast<const CColumn*>(pTerm);
	if (pColumn)
		return false;
	const CFunction * pFunc = dynamic_cast<const CFunction*>(pTerm);
	if (pFunc)
	{
		const vector<CTerm*> & aArgs = pFunc->args();
		vector<CTerm*>::const_iterator iArgs = aArgs.begin();
		for (; iArgs != aArgs.end(); iArgs++)
		if (!contains (aGroup, *iArgs))
			return false;
		return true;
	}
	ASSUME (false);
	return false;
}
//---------------------------------------------------------------------------
bool contains (const vector<CTerm*> & aGroup, const vector<CTerm*> & aList)
{
	const ULONG n = aList.size();
	ULONG i;
	for (i=0; i<n; i++)
		if (!contains(aGroup, aList[i]))
			return false;
	return true;
}
//---------------------------------------------------------------------------
// Record - Vergleich
//---------------------------------------------------------------------------
int cmp (const vector<CValue> & r1, const vector<CValue> & r2, ULONG n=0)
{
	if (n == 0)
	{
		n = r1.size();
		ASSUME (r1.size() == r2.size());
	}
	ASSUME (n <= r1.size());
	ASSUME (n <= r2.size());
	int i,nCmp=0;
	for (i=0; !nCmp, i<n; i++)
		nCmp = compare (r1[i], r2[i]);
	return nCmp;
}
//---------------------------------------------------------------------------
int cmp (const vector<long> * pOrder
, const vector<CValue> & aRecord1
, const vector<CValue> & aRecord2)
{
	if (!pOrder)
		return cmp (aRecord1, aRecord2);
	const int n = pOrder->size();
	int i,nCmp=0;
	for (i=0; !nCmp, i<n; i++)
	{
		const int iCol = (*pOrder)[i];
		const int j = abs(iCol) - 1;
		ASSUME (0 <= j && j < aRecord1.size());
		ASSUME (0 <= j && j < aRecord2.size());
		const CValue & v1 = aRecord1[j];
		const CValue & v2 = aRecord2[j];
		if (iCol > 0)
			nCmp = compare (v1, v2);
		else
			nCmp = compare (v2, v1);
		//if (nCmp) return (iCol<0) ? -nCmp : nCmp;
	}
	return nCmp;
}
//---------------------------------------------------------------------------
int compare (const CTerm & aTerm1, const CTerm & aTerm2)
{
	if (aTerm1.isNull())
		return aTerm2.isNull() ? 0 : -1;
	if (aTerm2.isNull())
		return aTerm1.isNull() ? 0 : +1;

	const short nType = aTerm1.type();
	switch (nType)
	{
	CASE_SQL_NUMBER:
		{
		assert (CTerm::isNumber (aTerm2.type()));
		const double d1 = aTerm1.asDouble();
		const double d2 = aTerm2.asDouble();
		if (d1 < d2)
			return -1;
		if (d2 < d1)
			return +1;
		return 0;
		}
	CASE_SQL_STRING:
		{
		assert (CTerm::isString(aTerm2.type()));
		const char * s1 = aTerm1.asString();
		const char * s2 = aTerm2.asString();
		return strcmp (s1,s2);
		}
	default:
		assert (false);
	}
	return 0;
}
//---------------------------------------------------------------------------
void fetch (vector<CValue> & aRecord, const vector<CTerm*> & aTerms)
{
	assert (aRecord.size() == aTerms.size());
	const ULONG n = aTerms.size();
	ULONG i;
	for (i=0; i<n; i++)
	{
		aRecord[i] = *aTerms[i];
	}
}
//---------------------------------------------------------------------------
void format (vector<CValue> & aRecord, const vector<CTerm*> & aTerms)
{
	const ULONG n = aTerms.size();
	aRecord.resize(n);
	ULONG i;
	for (i=0; i<n; i++)
	{
		const short nType = aTerms[i]->type();
		aRecord[i].clear (nType);
	}
}
//---------------------------------------------------------------------------
// CTerm
//---------------------------------------------------------------------------
CTerm::CTerm()
{
	m_paValue = 0;
}
//---------------------------------------------------------------------------
CTerm::~CTerm()
{
}
//---------------------------------------------------------------------------
bool
CTerm::isString (short nType)
{
	switch (nType)
	{
	CASE_SQL_STRING:
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool
CTerm::isNumber (short nType)
{
	switch (nType)
	{
	CASE_SQL_NUMBER:
		return true;
	}
	return false;
	return !isString (nType);
}
//---------------------------------------------------------------------------
bool
CTerm::isBool (short nType)
{
	return nType == SQL_BIT;
}
//---------------------------------------------------------------------------
bool
CTerm::isFloat (short nType)
{
	switch (nType)
	{
	CASE_SQL_FLOAT:
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool
CTerm::isBinary (short nType)
{
	switch (nType)
	{
	CASE_SQL_BINARY:
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool
CTerm::isInteger (short nType)
{
	switch (nType)
	{
	CASE_SQL_INTEGER:
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool
CTerm::isDateTime (short nType)
{
	switch (nType)
	{
	CASE_SQL_DATETIME:
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
bool 
CTerm::isRelation (short nHead)
{
	if (nHead != '=')
	if (nHead != '<')
	if (nHead != '>')
	if (nHead != lNEQ)
	if (nHead != lLEQ)
	if (nHead != lGEQ)
	if (nHead != lLIKE)
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool
CRecordset::EOT (ULONG i)
{
	if (i < rows())
		return false;
	const ULONG n = m_aTerms.size();
	ULONG j;
	for (j=0; j<n; j++)
	{
//		m_aTerms[j]->bind (CValue::null);
	}
	return true;
}
//---------------------------------------------------------------------------
vector<ULONG>
CRecordset::filter (const CTerm * pFilter)
{
	ASSUME (contains (m_aTerms, pFilter));
	const ULONG nRows = rows();
	vector<ULONG> aIndex;
	ULONG i;
	aIndex.reserve (nRows);
	for (i=0; i<nRows; i++)
	{
		move (i);
		if (pFilter && !pFilter->asBool())
			continue;
		aIndex.push_back (i);
	}
	return aIndex;
}
//---------------------------------------------------------------------------
vector<ULONG>
CRecordset::sort (const vector<CTerm*> & aOrder, const CTerm * pFilter)
{
	const int nOrder = aOrder.size();
	if (!nOrder)
		return filter (pFilter);
	const ULONG nRows = rows();
	//// Index erstellen
	vector<ULONG> aIndex;
	return aIndex;
}
//---------------------------------------------------------------------------
// CValue
//---------------------------------------------------------------------------
CValue::CValue (short t)
{
	m_bValid = false;
	m_nType = t;
	m_dNumber = 0;
	m_pString = 0;
	m_nString = 0;
	m_strDebug = id(this);
}
//---------------------------------------------------------------------------
CValue::CValue (short t, double val)
{
	m_bValid = true;
	m_nType = t;
	m_dNumber = val;
	m_pString = 0;
	m_nString = 0;
	m_strDebug = id(this);
}
//---------------------------------------------------------------------------
CValue::CValue (const char * val)
{
	m_dNumber = 0;
	m_pString = 0;
	m_nString = 0;
	set (SQL_CHAR, val);
	m_strDebug = id(this);
}
//---------------------------------------------------------------------------
CValue::CValue (const CTerm & val)
{
	m_dNumber = 0;
	m_pString = 0;
	m_nString = 0;
	operator= (val);
	m_strDebug = id(this);
}
//---------------------------------------------------------------------------
CValue::~CValue()
{
	free (m_pString);
	m_pString = 0;
}
//---------------------------------------------------------------------------
void
CValue::clear(short nType)
{
	m_bValid = false;
	m_nType = nType;
}
//---------------------------------------------------------------------------
char*
CValue::alloc (size_t nSize)
{
	nSize++;
	if (nSize < 10)
		nSize = 10;
	if (m_nString >= nSize && m_pString)
		return m_pString;
	free (m_pString);
	m_pString = (char*) malloc (nSize);
	m_nString = nSize;
	return m_pString;
}
//---------------------------------------------------------------------------
const char*
CValue::set (const char * szValue)
{
	if (m_pString)
		*m_pString = 0;
	if (!szValue)
		return m_pString;
	alloc (strlen(szValue));
	strcpy (m_pString, szValue);
//	CStringpool & aMist = gs_aStringpool;
//	m_szString = gs_aStringpool[szValue];
	return m_pString;
}
//---------------------------------------------------------------------------
const CValue &
CValue::operator= (double dValue)
{
//	assert (isNumber(type()));
	m_bValid = true;
	m_dNumber = dValue;
	return *this;
}
//---------------------------------------------------------------------------
const CValue &
CValue::operator= (const char * szValue)
{
	assert(isString(type()));
	m_bValid = (szValue != 0);
	set (szValue);
//	CStringpool & aMist = gs_aStringpool;
//	m_szString = gs_aStringpool[szValue];
	return *this;
}
//---------------------------------------------------------------------------
const CValue &
CValue::operator= (const std::string & strValue)
{
	operator= (strValue.c_str());
	return *this;
}
//---------------------------------------------------------------------------
const CValue &
CValue::operator= (const CValue & aValue)
{
	if (this == &aValue)
		return *this;
	if (aValue.m_nType != SQL_UNKNOWN_TYPE)
		m_nType = aValue.m_nType;
	m_bValid = aValue.m_bValid;
	m_dNumber = aValue.m_dNumber;
	set (aValue.m_pString);
	return *this;
}
//---------------------------------------------------------------------------
const CValue &
CValue::operator= (const CTerm & aTerm)
{
	if (this == &aTerm)
		return *this;
	m_bValid = !aTerm.isNull();
	m_nType = aTerm.type();
	switch (m_nType)
	{
	CASE_SQL_STRING:
		set (aTerm.asString());
		break;
	CASE_SQL_NUMBER:
		m_dNumber = aTerm.asDouble();
		break;
	}
	return *this;
}
//---------------------------------------------------------------------------
CValue &
CValue::set (short nType, double dValue)
{
//	assert (isNumber(eType));
	m_bValid = true;
	m_nType = nType;
	m_dNumber = dValue;
	return *this;
}
//---------------------------------------------------------------------------
CValue &
CValue::set (short nType, const char * szValue)
{
//	assert (isString(eType));
	m_bValid = true;
	m_nType = nType;
//	m_aString = STRNVL(szValue);
	set (szValue);
	return *this;
}
//---------------------------------------------------------------------------
double
CValue::asDouble() const
{
//	ASSUME (m_eType == TermNUMBER);
	return m_dNumber;
}
//---------------------------------------------------------------------------
time_t
CValue::asTime() const
{
//	ASSUME (m_eType == TermNUMBER);
	return m_dNumber;
}
//---------------------------------------------------------------------------
long
CValue::asInteger() const
{
//	ASSUME (m_eType == TermNUMBER);
	return m_dNumber;
}
//---------------------------------------------------------------------------
const char *
CValue::asString() const
{
	if (isNull())
		return "";
	if(isString(m_nType))
		return m_pString;
	CValue * pThis = const_cast<CValue*>(this);
//	if (false)
	if (isBool(m_nType))
	{
		const char * szValue = asBool() ? "yes" : "no";
		return pThis->set (szValue);
	}
	if (isDateTime(m_nType))
	{
		_daylight = 0;
		time_t nTime = m_dNumber;
		struct tm * pTime = localtime(&nTime);
		if (!pTime)
			return 0;
		char szTime[256];
		//strftime (szTime, sizeof(szTime), "%d.%m.%Y %H:%M:%S", pTime);
		strftime (szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", pTime);
		return pThis->set (szTime);
	}
	if (isInteger(m_nType))
	{
		char  szValue[256];
		sprintf (szValue, "%ld", (long)m_dNumber);
		return pThis->set (szValue);
	}
	if (isFloat(m_nType))
	{
		char  szValue[256];
		sprintf (szValue, "%g", m_dNumber);
		return pThis->set (szValue);
	}
	return "<unknown>";
}
//---------------------------------------------------------------------------
short
CUnary::type() const
{
	if (m_nHead == lCOUNT)
		return SQL_INTEGER;
	if (m_nHead == lAVG)
		return SQL_DOUBLE;
	if (m_nHead == lNULL)
		return SQL_BIT;
	if (m_nHead == lNOT)
		return SQL_BIT;
	if (m_nHead == lDATE)
		return SQL_DATETIME;
	if (m_nHead == lTIME)
		return SQL_DATETIME;
	if (m_nHead == lTIMESTAMP)
		return SQL_DATETIME;
	if (m_nHead == lLEN)
		return SQL_INTEGER;
/*
	if (m_nHead == AVG)
		return TermFLOAT;
	if (m_nHead == SUM)
		return TermFLOAT;
*/
	if (m_pTerm)
		return m_pTerm->type();
	return SQL_UNKNOWN_TYPE;
}
//---------------------------------------------------------------------------
bool
CUnary::isNull() const
{
	if (m_paValue)
		return m_paValue->isNull();
	if (m_pTerm)
		return m_pTerm->isNull();
	return false;
}
//---------------------------------------------------------------------------
double
CUnary::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	if (m_nHead == lNULL)
		return !m_pTerm || m_pTerm->isNull();
	double dValue = 0;
	if(!m_pTerm)
		return dValue;
	if (m_nHead == lLEN)
	{
		const char * szValue = m_pTerm->asString();
		if (!szValue)
			return 0;
		return strlen(szValue);
	}
	dValue = m_pTerm->asDouble();
	if (m_nHead == lABS)
		return fabs(dValue);
	if (m_nHead == lSIGN)
	{
		if (dValue < 0)
			return -1;
		if (dValue > 0)
			return +1;
		return 0;
	}
	if (m_nHead == lSQRT)
		return sqrt (dValue);
	if (m_nHead == lEXP)
		return exp (dValue);
	if (m_nHead == lLOG)
		return log (dValue);
	if (m_nHead == lNOT)
		return ((int)dValue) == 0;
	if (m_nHead == '-')
		return -dValue;
	if (m_nHead == '/')
	{
		if (!dValue)
			throw EXC("22012", 22012, "Division by zero: %s", m_strDebug.c_str());
		return 1/dValue;
	}
/*
	if (m_nHead == DATE)
	{
		const char * szTime = m_pTerm->asString();
		struct tm aTime;
		memset (&aTime, 0, sizeof(aTime));
		const int nRead = sscanf (szTime, "%d-%d-%d", &aTime.tm_year, &aTime.tm_mon, &aTime.tm_mday);
		ASSUME (nRead == 3);
		aTime.tm_year -= 1900;
		aTime.tm_mon  -= 1;
//		aTime.tm_hour = 12;
		return mktime (&aTime);
	}
	if (m_nHead == TIME)
	{
		const char * szTime = m_pTerm->asString();
		struct tm aTime;
		memset (&aTime, 0, sizeof(aTime));
		const int nTime = sscanf (szTime, "%d:%d:%d", &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
		ASSUME (nTime == 3);
		return mktime (&aTime);
	}
	if (m_nHead == TIMESTAMP)
	{
		const double dTime = m_pTerm->asDouble();
		if (dTime != CValue.null.asDouble())
			return (time_t) dTime;
		const char * szTime = m_pTerm->asString();
		struct tm aTime;
		memset (&aTime, 0, sizeof(aTime));
		const int nRead = sscanf (szTime, "%d-%d-%d %d:%d:%d"
		, &aTime.tm_year, &aTime.tm_mon, &aTime.tm_mday
		, &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
		ASSUME (nRead == 6);
		aTime.tm_year -= 1900;
		aTime.tm_mon  -= 1;
		const time_t nTime = mktime (&aTime);
		CValue * paValue = dynamic_cast<CValue*>(m_pTerm);
		if (paValue)
			paValue->m_dNumber = nTime;
		return nTime;
	}
*/
	return dValue;
}
//---------------------------------------------------------------------------
const char *
CUnary::asString() const
{
	if (m_paValue)
		return m_paValue->asString();
	ASSUME(m_pTerm);
	return m_pTerm->asString();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
short
CFunction::type (const vector<CTerm*> & aArgs)
{
	const ULONG nArgs = aArgs.size();
	ULONG i;
	if (!nArgs)
		return SQL_UNKNOWN_TYPE;
	short  nType = aArgs[0]->type();
	for (i=1; i<nArgs; i++)
	{
		const CTerm * pTerm = aArgs[i];
		assert (pTerm);
		short nTerm = pTerm->type();
		if (nTerm == SQL_UNKNOWN_TYPE)
			continue;
		if (SQLTypeOrder (nTerm) <= SQLTypeOrder(nType))
			continue;
		nType = nTerm;
	}
	return nType;
}
//---------------------------------------------------------------------------
CFunction::CFunction (int nHead, const vector<CTerm*> & aArgs)
: m_nHead(nHead)
, m_aArgs(aArgs)
{
	m_strDebug = id(this); 
}
//---------------------------------------------------------------------------
CFunction::CFunction (int nHead, va_list & aArgs)
{
	m_nHead = nHead;
	while (true)
	{
		CTerm * pTerm = va_arg (aArgs, CTerm*);
		if (!pTerm)
			break;
		m_aArgs.push_back(pTerm);
		//arg (pTerm);
	}
	m_strDebug = id(this); 
}
//---------------------------------------------------------------------------
CFunction::~CFunction ()
{
}
//---------------------------------------------------------------------------
bool
CFunction::isAssociative (CFunction * pFunc)
{
	if (!pFunc)
		return false;
	if (pFunc->m_nHead == m_nHead)
		return true;
	if (pFunc->m_nHead == NULL)
		return true;
	return false;
}
//---------------------------------------------------------------------------
void
CFunction::arg (CTerm *pTerm)
{
	ASSUME (pTerm);
	CFunction * pFunc = dynamic_cast<CFunction*>(pTerm);
	if (!isAssociative (pFunc))
	{
		m_aArgs.push_back(pTerm);
		return;
	}
	const vector<CTerm*> & aArgs = pFunc->m_aArgs;
	vector<CTerm*>::const_iterator iArgs = aArgs.begin();
	for (; iArgs != aArgs.end(); iArgs++)
	{
		arg (*iArgs);
	}
}
//---------------------------------------------------------------------------
CTerm*
CFunction::arg(int i)
{
	if (i >= m_aArgs.size())
		return 0;
	return m_aArgs[i];
}
//---------------------------------------------------------------------------
short
CFunction::type() const
{
	static const int aBool[] =
	{ '=', '<', '>', lNEQ, lGEQ, lLEQ, lLIKE, lIN, lBETWEEN };
	const int * pBool1 = aBool;
	const int * pBool2 = aBool + DIM(aBool);
	if (find (pBool1, pBool2, m_nHead) < pBool2)
		return SQL_BIT;
	return type (m_aArgs);
}
//---------------------------------------------------------------------------
bool
CFunction::isNull() const
{
	const ULONG n = m_aArgs.size();
	ULONG i;
	for (i=0; i<n; i++)
	{
		CTerm * pTerm = m_aArgs[i];
		if (pTerm && !pTerm->isNull())
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
double
CFunction::asDouble() const
{
	throw EXC("42S22", 900, "double?!: [%s] (not implemented).", m_strDebug.c_str());
	const CTerm * pTerm;
	const ULONG nArgs = m_aArgs.size();
	assert (nArgs);
	ULONG i;

	if (m_paValue)
		return m_paValue->asDouble();
	if (m_nHead == '+')
	{
		double dValue = 0;
		for (i=0; i<nArgs; i++)
		{
			pTerm = m_aArgs[i];
			assert (pTerm);
			dValue += pTerm->asDouble();
		}
		return dValue;
	}
	if (m_nHead == '*')
	{
		double dValue = 1;
		for (i=0; i<nArgs; i++)
		{
			pTerm = m_aArgs[i];
			assert (pTerm);
			dValue *= pTerm->asDouble();
		}
		return dValue;
	}
	if (m_nHead == lMOD)
	{
		CTerm* pTerm1 = m_aArgs[0];
		assert (pTerm1);
		CTerm* pTerm2 = m_aArgs[1];
		assert (pTerm2);
		const long i1 = pTerm1->asDouble();
		const long i2 = pTerm2->asDouble();
		ASSUME (i2 != 0);
		return i1 % i2;
	}
	if (m_nHead == lDIV)
	{
		CTerm* pTerm1 = m_aArgs[0];
		assert (pTerm1);
		CTerm* pTerm2 = m_aArgs[1];
		assert (pTerm2);
		const double d1 = pTerm1->asDouble();
		const double d2 = pTerm2->asDouble();
		ASSUME (d2 != 0);
		return (long) d1 / d2;
	}
	if (m_nHead == lEXP)
	{
		CTerm* pTerm1 = m_aArgs[0];
		assert (pTerm1);
		CTerm* pTerm2 = m_aArgs[1];
		assert (pTerm2);
		const double d1 = pTerm1->asDouble();
		const double d2 = pTerm2->asDouble();
		return exp(log(d1) * d2);
	}
	if (m_nHead == lLOG)
	{
		CTerm* pTerm1 = m_aArgs[0];
		assert (pTerm1);
		CTerm* pTerm2 = m_aArgs[1];
		assert (pTerm2);
		const double d1 = pTerm1->asDouble();
		const double d2 = pTerm2->asDouble();
		return log(d2) / log(d1);
	}
	if (m_nHead == '=')
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			pTerm = m_aArgs[i];
			assert (pTerm);
			if (*pTerm0 != *pTerm )
				return false;
		}
		return true;
	}
	if (m_nHead == '>')
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (compare (*pTerm0, *pTerm) < +1)
				return false;
		}
		return true;
	}
	if (m_nHead == '<')
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (compare (*pTerm0, *pTerm) > -1 )
				return false;
		}
		return true;
	}
	if (m_nHead == lNEQ)
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (compare (*pTerm0, *pTerm) == 0)
				return false;
		}
		return true;
	}
	if (m_nHead == lLEQ)
	{
		CTerm* pTerm0 = m_aArgs[0];
		ASSUME (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (compare (*pTerm0, *pTerm) > 0 )
				return false;
		}
		return true;
	}
	if (m_nHead == lGEQ)
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (compare (*pTerm0, *pTerm) < 0 )
				return false;
		}
		return true;
	}
	if (m_nHead == lAND)
	{
		for (i=0; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (!pTerm->asBool())
				return false;
		}
		return true;
	}
	if (m_nHead == lOR)
	{
		for (i=0; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert(pTerm);
			if (pTerm->asBool())
				return true;
		}
		return false;
	}
	if (m_nHead == lLIKE)
	{
		ASSUME (nArgs == 2);
		const CTerm* pTerm1 = m_aArgs.front();
		const CTerm* pTerm2 = m_aArgs.back();
		ASSUME (pTerm1);
		ASSUME (pTerm2);
		const char * s1 = pTerm1->asString();
		const char * s2 = pTerm2->asString();
		if (!s1 || !s2)
			return false;
		if (!strlen(s1) || !strlen(s2))
			return strlen(s1) == strlen(s2);
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
	if (m_nHead == lBETWEEN)
	{
		assert (nArgs == 3);
		CTerm * pTerm = m_aArgs[0];
		CTerm * pBoundL = m_aArgs[1];
		CTerm * pBoundU = m_aArgs[2];
		if (*pBoundL > *pBoundU)
			swap (pBoundL, pBoundU);
		if (*pBoundL > *pTerm)
			return false;
		if (*pBoundU < *pTerm)
			return false;
		return true;
	}
	if (m_nHead == lIN)
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			if (*pTerm0 == *pTerm )
				return true;
		}
		return false;
	}
	if (m_nHead == lMIN)
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		double dResult = pTerm0->asDouble();
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			double dValue = pTerm->asDouble();
			if (dValue < dResult)
				dResult = dValue;
		}
		return dResult;
	}
	if (m_nHead == lMAX)
	{
		CTerm* pTerm0 = m_aArgs[0];
		assert (pTerm0);
		double dResult = pTerm0->asDouble();
		for (i=1; i<nArgs; i++)
		{
			CTerm* pTerm = m_aArgs[i];
			assert (pTerm);
			double dValue = pTerm->asDouble();
			if (dValue > dResult)
				dResult = dValue;
		}
		return dResult;
	}
	ASSUME (false);
	return 0;
}
//---------------------------------------------------------------------------
const char *
CFunction::asString() const
{
	throw EXC("42S22", 900, "string?!: [%s] (not implemented).", m_strDebug.c_str());
	string & strValue = const_cast<CFunction*>(this)->m_strValue;
	strValue.erase();
	if (m_nHead == lFMT)
	{
		string & strValue = const_cast<CFunction*>(this)->m_strValue;
		ASSUME (m_aArgs.size() >= 2);
		CValue* pFormat = dynamic_cast<CValue*>(m_aArgs[0]);
		CTerm * pTerm = m_aArgs[1];
		ASSUME (pFormat && pTerm);
		const char * szFormat = pFormat->asString();
		strValue = stringf (szFormat, pTerm);
		return strValue.c_str();

	}
	if (m_paValue)
		return m_paValue->asString();
	short nType = type();
	if (isNumber(nType))
	{
		CValue aValue = *this;
		strValue = aValue.asString();
		return strValue.c_str();
	}
	if (isString(nType))
	if (m_nHead == '+')
	{
		vector<CTerm*>::const_iterator iTerm = m_aArgs.begin();
		for (; iTerm != m_aArgs.end(); iTerm++)
		{
			CTerm* pTerm = *iTerm;
			ASSUME (pTerm);
			strValue += pTerm->asString();
		}
		return strValue.c_str();
	}
	return "";
}
//---------------------------------------------------------------------------
// CColumn
//---------------------------------------------------------------------------
CColumn::CColumn (const char * t, const char * c)
{ 
	m_szTable=t;
	m_szColumn=c;
	m_pTable = 0;
	m_strDebug = id(this); 
}
//---------------------------------------------------------------------------
string
CColumn::key (const char * szTable, const char * szColumn)
{
	ASSUME (szColumn && *szColumn);
	if (!szTable || *szTable==0)
		return string(szColumn);
	char szKey[256];
	sprintf (szKey, "%s.%s", szTable, szColumn);
	return string(szKey);
}
//---------------------------------------------------------------------------
short
CColumn::type() const
{
	if (m_paValue)
		return m_paValue->type();
	if (m_pTable)
	{
		ULONG i = m_pTable->colidx (column());
		const CDesc * pDesc = m_pTable->desc(i);
		if (!pDesc)
			return SQL_UNKNOWN_TYPE;
		short nType = pDesc->type();
		return nType;

	}
	return SQL_UNKNOWN_TYPE;
}
//---------------------------------------------------------------------------
const CDesc *
CColumn::desc() const
{
	if (!m_pTable)
		return 0;
	ULONG i = m_pTable->colidx(column());
	return m_pTable->desc (i);
}
//---------------------------------------------------------------------------
bool
CColumn::isNull() const
{
	return (!m_paValue || m_paValue->isNull());
}
//---------------------------------------------------------------------------
double
CColumn::asDouble() const
{
	if (m_paValue)
		return m_paValue->asDouble();
	return 0;
}
//---------------------------------------------------------------------------
const char *
CColumn::asString() const
{
	if (m_paValue)
		return m_paValue->asString();
	return "";
}
//---------------------------------------------------------------------------
CParam::CParam()
{
	m_nType = SQL_UNKNOWN_TYPE;
}
//---------------------------------------------------------------------------
#if 0
time_t
CParam::getTime (const char * szParam)
{
	struct tm aTime;
	double dParam = 0;
	int nScan = 0;

	memset (&aTime, 0, sizeof(aTime));
	if (nScan < 3)
	nScan = sscanf (szParam, "%d-%d-%d %d:%d:%d"
	, &aTime.tm_year, &aTime.tm_mon, &aTime.tm_mday
	, &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
	if (nScan < 3)
	nScan = sscanf (szParam, "%d.%d.%d %d:%d:%d"
	, &aTime.tm_mday, &aTime.tm_mon, &aTime.tm_year
	, &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
	if (nScan < 3)
	if (sscanf (szParam, "%lf", &dParam))
	{
		dParam -= 25569;
		dParam *= 24 * 3600;
		dParam -= 3600;
		dParam = floor(dParam + 0.5);
		return dParam;
	}
	aTime.tm_year -= 1900;
	aTime.tm_mon -= 1;
	if (nScan < 3)
		return 0;
	dParam = mktime (&aTime);
	return dParam;
}
//---------------------------------------------------------------------------
double
CParam::getReal (const char * szParam)
{
	double dParam = 0;
	if (szParam)
	sscanf (szParam, "%lf", &dParam);
	return dParam;
}
#endif
//---------------------------------------------------------------------------
#if 0
void
CParam::setTyp (short nTyp)
{
	m_nType = nTyp;
	if (m_nTargetTyp == SQL_C_CHAR)
	if (isDateTime(m_nType))
	{
		char szParam[256];
		memset (szParam, 0, sizeof(szParam));
		if (m_pTargetInd && *m_pTargetInd != SQL_NTS)
			memcpy (szParam, m_pTargetVal, *m_pTargetInd);
		else
			strncpy (szParam, (char*)m_pTargetVal, sizeof(szParam));
		char * pc;
		for (pc=szParam; *pc; pc++)
		if (*pc == ',')
			*pc='.';

		struct tm aTime;
		double dTime;
		memset (&aTime, 0, sizeof(aTime));
		int nScan = 0;
		if (nScan < 3)
		nScan = sscanf (szParam, "%d-%d-%d %d:%d:%d"
		, &aTime.tm_year, &aTime.tm_mon, &aTime.tm_mday
		, &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
		if (nScan < 3)
		nScan = sscanf (szParam, "%d.%d.%d %d:%d:%d"
		, &aTime.tm_mday, &aTime.tm_mon, &aTime.tm_year
		, &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
		aTime.tm_year -= 1900;
		aTime.tm_mon -= 1;
		if (sscanf (szParam, "%lf", &dTime))
		{
			dTime -= 25569;
			dTime *= 24 * 3600;
			dTime -= 3600;
			m_dNumber = floor (dTime + 0.5);
			return;
		}
		if (nScan < 3)
			m_bValid = false;
		else
			m_dNumber = mktime (&aTime);
		return;
	}
	if (m_nTargetTyp == SQL_C_CHAR)
	if (isFloat(m_nType))
	{
		const char * szParam = (const char *) m_pTargetVal;
		if (!szParam)
			return;
		if (!sscanf (szParam, "%lf", &m_dNumber))
			m_bValid = false;
		return;
	}
	if (m_nTargetTyp == SQL_C_CHAR
	|| (isString(m_nType) && m_nTargetTyp == SQL_C_DEFAULT))
	{
		if (m_pTargetInd && *m_pTargetInd != SQL_NTS)
			const_cast<string&>(m_aString).assign ((char*)m_pTargetVal, *m_pTargetInd);
		else
			const_cast<string&>(m_aString).assign ((char*)m_pTargetVal);
		return;
	}
	if (m_nTargetTyp == SQL_C_TIMESTAMP)
	{
		TIMESTAMP_STRUCT & aTarget = *((TIMESTAMP_STRUCT*)m_pTargetVal);
		struct tm aTime;
		memset (&aTime, 0, sizeof(aTime));
		aTime.tm_year = aTarget.year - 1900;
		aTime.tm_mon = aTarget.month - 1;
		aTime.tm_mday = aTarget.day;
		aTime.tm_hour = aTarget.hour;
		aTime.tm_min = aTarget.minute;
		aTime.tm_sec = aTarget.second;
		m_dNumber = mktime (&aTime);
		return;
	}
	char * szEnd = 0;
	if (m_nTargetTyp == SQL_C_DOUBLE)
		m_dNumber = *(double*) m_pTargetVal;
	if (m_nTargetTyp == SQL_C_FLOAT)
		m_dNumber = *(double*) m_pTargetVal;
	if (m_nTargetTyp == SQL_C_CHAR)
		m_dNumber = strtod ((const char*) m_pTargetVal, &szEnd);
	if (m_nTargetTyp == SQL_C_SLONG)
		m_dNumber = *(signed long*) m_pTargetVal;
	if (m_nTargetTyp == SQL_C_ULONG)
		m_dNumber = *(unsigned long*) m_pTargetVal;
}
#endif
//---------------------------------------------------------------------------
IDatabase::~IDatabase()
{
}
//---------------------------------------------------------------------------
CTable::~CTable()
{
}
//---------------------------------------------------------------------------
bool
CTable::get (vector<ULONG> & raIndex, int nHeader, const char * szColumn, CTerm* pValue) const
{
	if (szColumn == 0 || pValue == 0)
		return false;
	const ULONG i = colidx (szColumn);
	if (i < cols())
		return get(raIndex, nHeader, i, pValue);
	return false;
}
//---------------------------------------------------------------------------
bool
CTable::get (vector<ULONG> & raIndex, int nHeader, int nCol, CTerm* pValue) const
{
	return false;
}
//---------------------------------------------------------------------------
ULONG
CTable::colidx (const char * szName) const
{
	const ULONG nCols = cols();
	ULONG i;
	for (i=0; i<nCols; i++)
	{
		const CDesc * pDesc = desc(i);
		assert (pDesc);
		const char * szColumn = pDesc->name();
		if (!stricmp (szColumn, szName))
			break;
	}
	return i;
}
//---------------------------------------------------------------------------
ULONG
CTable::colidx (const string & strName) const
{
	return colidx (strName.c_str());
}
//---------------------------------------------------------------------------
#if 0
ULONG
CTable::cols() const
{
	return desc (SQL_DESC_COUNT).asInteger();
}
#endif
//---------------------------------------------------------------------------
CStatement::CStatement (IDatabase * db)
: m_aSelection(100)
{
	assert (db);
	m_pResult = 0;
	m_pTableSet = 0;
	m_pDatabase = db;
	setSQL ("");
	clear();
}
//---------------------------------------------------------------------------
CStatement::~CStatement()
{
	clear();
}
//---------------------------------------------------------------------------
void
CStatement::clear()
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject != m_aMemory.end(); iObject++)
	{
		CObject * pObject = *iObject;
		delete pObject;
	}
	m_aMemory.clear();
	vector<const char *>::iterator iAtom = m_aAtoms.begin();
	for (; iAtom != m_aAtoms.end(); iAtom++)
	{
		const char * szAtom = *iAtom;
		free ((void*)szAtom);
	}
	m_aAtoms.clear();
	m_aTables.clear();
	m_bDistinct = false;
	m_aSelection.clear();
	m_pWhere = 0;
	m_aGroup.clear();
	m_aOrder.clear();
	m_pHaving = 0;
	m_aNames.clear();
	delete m_pTableSet;
	m_pTableSet = 0;
	m_pResult = 0;
}
//---------------------------------------------------------------------------
const char *
CStatement::atom (const char * szValue, long n)
{
	const long nSize = (n < 0) ? strlen(szValue) + n : n; 
	assert (nSize >= 0);
	char * szAtom = strdup (szValue);
	szAtom[nSize] = 0;

	vector<const char*> & raAtoms = m_aAtoms;
	vector<const char*>::iterator iAtoms = raAtoms.begin();
	for (; iAtoms != raAtoms.end(); iAtoms++)
	{
		const char * szItem = *iAtoms;
		if (strcmp (szItem , szAtom) == 0)
		{
			free (szAtom);
			return szItem;
		}
	}
	raAtoms.push_back (szAtom);
	return szAtom;
}
//---------------------------------------------------------------------------
const char *
CStatement::atom (const char * szValue)
{
	if (STRNUL (szValue))
		return 0;
	vector<const char*> & raAtoms = m_aAtoms;
	vector<const char*>::iterator iAtoms = raAtoms.begin();
	for (; iAtoms != raAtoms.end(); iAtoms++)
	{
		const char * szAtom = *iAtoms;
		if (strcmp (szAtom, szValue) == 0)
			return szAtom;
	}
	char * szAtom = strdup (szValue);
	raAtoms.push_back (szAtom);
	return szAtom;
}
//---------------------------------------------------------------------------
const char *
CStatement::sql (int nLength, bool bQuoted)
{
	const char * szSQL = m_strSQL.c_str() + (m_iSQL - nLength);
	if (bQuoted)
	{
		szSQL += 1;
		nLength -= 2;
	}
	char szText[256];
	memset (szText, 0, sizeof(szText));
	memcpy (szText, szSQL, nLength);
	//for (int i=0; i<nLength; i++)
	//	szText[i] = toupper(szSQL[i]);
	return atom (szText);
}
//---------------------------------------------------------------------------
CValue *
CStatement::value (short nType, double dValue)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CValue * pTerm = dynamic_cast<CValue*>(*iObject);
		if (!pTerm)
			continue;
		short nType = pTerm->type();
		if (CTerm::isNumber(nType))
		if (pTerm->asDouble() == dValue)
			return pTerm;
	}
	CValue * pTerm = new CValue (nType, dValue);
	m_aMemory.push_back (pTerm);
	return pTerm;
}
//---------------------------------------------------------------------------
CValue *
CStatement::value (const char * szValue)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CValue * pTerm = dynamic_cast<CValue*>(*iObject);
		if (!pTerm)
			continue;
		short nType = pTerm->type();
		if (CTerm::isString (nType))
		if (strcmp (pTerm->asString(),szValue) == 0)
			return pTerm;
	}
	CValue * pTerm = new CValue (szValue);
	m_aMemory.push_back (pTerm);
	return pTerm;
}
//---------------------------------------------------------------------------
#if 0
CValue *
CStatement::value (long int nValue)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CValue * pTerm = dynamic_cast<CValue*>(*iObject);
		if (!pTerm)
			continue;
		short nType = pTerm->type();
		if (CTerm::isInteger (nType))
		if (pTerm->asInteger() == nValue)
			return pTerm;
	}
	CValue * pTerm = new CValue(SQL_INTEGER, nValue);
	m_aMemory.push_back (pTerm);
	return pTerm;
}
//---------------------------------------------------------------------------
CValue *
CStatement::value (double dValue)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CValue * pTerm = dynamic_cast<CValue*>(*iObject);
		if (!pTerm)
			continue;
		short nType = pTerm->type();
		if (CTerm::isNumber(nType))
		if (pTerm->asDouble() == dValue)
			return pTerm;
	}
	CValue * pTerm = new CValue (SQL_DOUBLE, dValue);
	m_aMemory.push_back (pTerm);
	return pTerm;
}
//---------------------------------------------------------------------------
CValue *
CStatement::value (const char * szValue)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CValue * pTerm = dynamic_cast<CValue*>(*iObject);
		if (!pTerm)
			continue;
		short nType = pTerm->type();
		if (CTerm::isString (nType))
		if (strcmp (pTerm->asString(),szValue) == 0)
			return pTerm;
	}
	CValue * pTerm = new CValue (szValue);
	m_aMemory.push_back (pTerm);
	return pTerm;
}
#endif
//---------------------------------------------------------------------------
CValue *
CStatement::time (double dTime)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CValue * pTerm = dynamic_cast<CValue*>(*iObject);
		if (!pTerm)
			continue;
		short nType = pTerm->type();
		if (CTerm::isDateTime (nType))
		if ((pTerm->asDouble()) == dTime)
			return pTerm;
	}
	CValue * pTerm = new CValue (SQL_DATETIME, dTime);
	m_aMemory.push_back (pTerm);
	return pTerm;
}
//---------------------------------------------------------------------------
CValue *
CStatement::time (const char * szValue)
{
	static const double dSecPerDay = 3600 * 24;
	struct tm aTime;
	memset (&aTime, 0, sizeof(aTime));
	const int nRead = sscanf (szValue, "%d-%d-%d %d:%d:%d"
	, &aTime.tm_year, &aTime.tm_mon, &aTime.tm_mday
	, &aTime.tm_hour, &aTime.tm_min, &aTime.tm_sec);
	ASSUME (nRead >= 3);
	aTime.tm_year -= 1900;
	aTime.tm_mon  -= 1;
	const time_t nTime = mktime (&aTime);
	return time (nTime / dSecPerDay);
}
//---------------------------------------------------------------------------
CTerm *
CStatement::column (const char * szSchema, const char * szTable, const char * szColumn)
{
	string strTable = stdx::join (szSchema, szTable, ".");
	szTable = atom (strTable.c_str());
	map<string,CTerm*>::const_iterator iNames = m_aNames.end();
	if (szColumn && *szColumn)
	if (STRNUL(szTable))
	if ((iNames = m_aNames.find(szColumn)) != m_aNames.end())
	{
		CTerm * pTerm = (*iNames).second;
		return unary(lNAME, pTerm);
	}
	const int nTables = m_aTables.size();
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CColumn * pColumn = dynamic_cast<CColumn*>(*iObject);
		if (!pColumn)
			continue;
		if (nTables != 1)
		if (!szTable && !pColumn->table())
			continue;
		if (nTables != 1)
		if (szTable && szTable != pColumn->table())
			continue;
		if (!szColumn && !pColumn->column())
			continue;
		const char * szItem = pColumn->column();
		if (szItem && stricmp (szItem, szColumn) == 0)
			return pColumn;
	}
	CColumn * pColumn = new CColumn (szTable, szColumn);
	ASSUME (pColumn);
	m_aMemory.push_back (pColumn);
	return pColumn;
}
//---------------------------------------------------------------------------
CUnary *
CStatement::unary (int nHead, CTerm * pTerm)
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CUnary * pUnary = dynamic_cast<CUnary*>(*iObject);
		if (!pUnary)
			continue;
		if (pUnary->head() != nHead)
			continue;
		if (pUnary->arg() != pTerm)
			continue;
		return pUnary;
	}
	CUnary * pUnary = 0;
	switch (nHead)
	{
	case lLEN:
		pUnary = new TUnary<lLEN> (pTerm);
		break;
	case lABS:
		pUnary = new TUnary<lABS> (pTerm);
		break;
	case lSIGN:
		pUnary = new TUnary<lSIGN> (pTerm);
		break;
	case lSQRT:
		pUnary = new TUnary<lSQRT> (pTerm);
		break;
	case lEXP:
		pUnary = new TUnary<lEXP> (pTerm);
		break;
	case lLOG:
		pUnary = new TUnary<lLOG> (pTerm);
		break;
	case lNOT:
		pUnary = new TUnary<lNOT> (pTerm);
		break;
	case '-':
		pUnary = new TUnary<'-'> (pTerm);
		break;
	case '/':
		pUnary = new TUnary<'/'> (pTerm);
		break;
	default:
		pUnary = new CUnary (nHead, pTerm);
	}
	
	ASSUME (pUnary);
	m_aMemory.push_back (pUnary);
	return pUnary;
}
//---------------------------------------------------------------------------
CFunction *
CStatement::func (int nHead, va_list args)
{
	vector<CTerm*> aArgs;
	aArgs.clear();
	aArgs.reserve(2);
	////-----------------------------------------
	//// build args: a,(b,c) ==> (a,b,c)
	////-----------------------------------------
	while(true)
	{
		CTerm * pTerm = va_arg (args, CTerm*);
		if (!pTerm)
			break;
		CFunction * pFunc = dynamic_cast<CFunction*>(pTerm);
		if (pFunc == 0 || pFunc->head() != lNULL)
			aArgs.push_back (pTerm);
		else
		{
			const vector<CTerm*> & aList = pFunc->args();
			aArgs.insert (aArgs.end(), aList.begin(), aList.end());
		}
	}
	////-----------------------------------------
	//// lookup (fuction allready defined?)
	////-----------------------------------------
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject!=m_aMemory.end(); iObject++)
	{
		CFunction * pFunc = dynamic_cast<CFunction*>(*iObject);
		if (!pFunc)
			continue;
		if (pFunc->head() != nHead)
			continue;
		const vector<CTerm*> & aFunc = pFunc->args();
		if (aFunc.size() != aArgs.size())
			continue;
		vector<CTerm*>::const_iterator iArgs = aArgs.begin();
		for (; iArgs != aArgs.end(); iArgs++)
		{
			CTerm * pTerm = *iArgs;
			vector<CTerm*>::const_iterator iFunc = find (aFunc.begin(), aFunc.end(), pTerm);
			if (iFunc == aFunc.end())
				break;
		}
		if (iArgs != aArgs.end())
			continue;
		return pFunc;
	}
	////-----------------------------------------
	//// new function: register
	////-----------------------------------------
	CFunction * pFunc = 0;
	switch (nHead)
	{
	case '+':
		pFunc = new CFunc<'+'>(aArgs);
		break;
	case '*':
		pFunc = new CFunc<'*'>(aArgs);
		break;
	case lMOD:
		pFunc = new CFunc<lMOD>(aArgs);
		break;
	case lDIV:
		pFunc = new CFunc<lDIV>(aArgs);
		break;
	case lEXP:
		pFunc = new CFunc<lEXP>(aArgs);
		break;
	case lLOG:
		pFunc = new CFunc<lLOG>(aArgs);
		break;
	case '=':
		pFunc = new CFunc<'='>(aArgs);
		break;
	case '<':
		pFunc = new CFunc<'<'>(aArgs);
		break;
	case '>':
		pFunc = new CFunc<'>'>(aArgs);
		break;
	case lNEQ:
		pFunc = new CFunc<lNEQ>(aArgs);
		break;
	case lLEQ:
		pFunc = new CFunc<lLEQ>(aArgs);
		break;
	case lGEQ:
		pFunc = new CFunc<lGEQ>(aArgs);
		break;
	case lAND:
		pFunc = new CFunc<lAND>(aArgs);
		break;
	case lOR:
		pFunc = new CFunc<lOR>(aArgs);
		break;
	case lBETWEEN:
		pFunc = new CFunc<lBETWEEN>(aArgs);
		break;
	case lIN:
		pFunc = new CFunc<lIN>(aArgs);
		break;
	case lMIN:
		pFunc = new CFunc<lMIN>(aArgs);
		break;
	case lMAX:
		pFunc = new CFunc<lMAX>(aArgs);
		break;
	case lFMT:
		pFunc = new CFunc<lFMT>(aArgs);
		break;
	case lNVL:
		pFunc = new CFunc<lNVL>(aArgs);
		break;
	default:
		pFunc = new CFunction (nHead, aArgs);
	}
	ASSUME (pFunc);
	m_aMemory.push_back (pFunc);
	return pFunc;
}
//---------------------------------------------------------------------------
CParam *
CStatement::param()
{
	CParam * pParam = new CParam;
	ASSUME (pParam);
	m_aMemory.push_back (pParam);
	m_aParam.push_back (pParam);
	return pParam;
}
//---------------------------------------------------------------------------
const char * 
CStatement::table (const char * szCatatlog, const char * szSchema, const char * szName, const char * szAlias)
{
	ASSUME (szName && *szName);
	//ASSUME (szAlias && *szAlias);
	string strTable = stdx::join (szSchema, szName, ".");
	if (!szAlias)
		szAlias = atom (strTable.c_str());
	map<const char*,CTable*>::iterator iTable = m_aTables.find (szAlias);
	if (iTable != m_aTables.end())
		return iTable->first;
	CTable * pTable =  m_pDatabase->table (szCatatlog, szSchema, szName);
	ASSUME (pTable);
	m_aMemory.push_back (pTable);
	m_aTables[szAlias] = pTable;
	return szAlias;
}
//---------------------------------------------------------------------------
const char *
CStatement::join (int nHead, const char * pLeft, const char * pRight, CTerm * pCondition)
{
	CJoin * pJoin = new CJoin (nHead, pLeft, pRight, pCondition);
	m_aMemory.push_back (pJoin);
	m_aJoins.push_back (pJoin);
//	join_t aJoin = { nHead, pLeft, pRight, pCondition};
	return pLeft;
}
//---------------------------------------------------------------------------
void
CStatement::select (const char * szTable, int nIndex)
{
	vector<CTerm*>::iterator iResult = m_aSelection.begin() + nIndex;
	ASSUME (0<=nIndex && nIndex < m_aSelection.size());
	m_aSelection.erase (iResult);
	////-----------------------------------------
	//// table.*
	////-----------------------------------------
	if (szTable)
	{
		CTableMap::iterator iTable = m_aTables.find (szTable);
		ASSUME (iTable != m_aTables.end());
		vector<CTerm*> aSelection = select (iTable);
		m_aSelection.insert (iResult, aSelection.begin(), aSelection.end());
	}
	////-----------------------------------------
	//// *
	////-----------------------------------------
	else
	{
		CTableMap::iterator iTable = m_aTables.begin();
		for (; iTable != m_aTables.end(); iTable++)
		{
			vector<CTerm*> aSelection = select (iTable);
			m_aSelection.insert (iResult, aSelection.begin(), aSelection.end());
			iResult += aSelection.size();
		}
	}
}
//---------------------------------------------------------------------------
vector<CTerm*>
CStatement::select (const CTableMap::iterator & iTable)
{
	const char * szTable = iTable->first;
	const CTable & aTable = *iTable->second;
	const int nColumns = aTable.cols();
	vector<CTerm*> aResult;
	int i;

	aResult.reserve (nColumns);
	for (i=0; i<nColumns; i++)
	{
		const CDesc * pDesc = aTable.desc(i);
		assert (pDesc);
		const char * szColumn = pDesc->name();
		CTerm * pResult = column (0, szTable, szColumn);
		aResult.push_back (pResult);
	}
	return aResult;
}
//---------------------------------------------------------------------------
void
CStatement::select()
{
	EXC_BEGIN;
	return;
	int i,n;

	//// Spalte * expandieren
	n = m_aSelection.size();
	for (i=0; i<n; i++)
	{
		const CTerm * pResult = dynamic_cast<const CTerm*>(m_aSelection[i]);
		if (!pResult) continue;
		const CColumn * pColumn = dynamic_cast<const CColumn*>(pResult);
		if (!pColumn || pColumn->column())
			continue;
		select (pColumn->table(), i);
		n = m_aSelection.size();
	}
	EXC_END;
}
//---------------------------------------------------------------------------
CTerm*
CStatement::result (CTerm* pTerm)
{
	EXC_BEGIN;
	CValue * pValue = dynamic_cast<CValue*>(pTerm);
	if (!pValue)
		return pTerm;
	short nType = pValue->type();
	if (!CTerm::isInteger (nType))
		throw EXC("42S22", 12, "Column Index?!: [%s]", id(pTerm).c_str());
	const int i = pValue->asDouble() - 1;
	const int n = m_aSelection.size();
	if (0 > i || i >= n)
		throw EXC("42S22", 12, "Column Index [%d] out of range [1..%d]", i+1, n);
	return m_aSelection[i];
	EXC_END;
	return pTerm;
}
//---------------------------------------------------------------------------
void
CStatement::setFormat (CTerm* pTerm, const char * szFormat)
{
	EXC_BEGIN;
	ULONG i;
	const CUnary * pUnary = dynamic_cast<const CUnary*>(pTerm);
	if (pUnary && pUnary->m_nHead == lNAME)
		pTerm = pUnary->m_pTerm;
	const CValue * pValue = dynamic_cast<const CValue*>(pTerm);
	if (pValue)
		i = pValue->asInteger() - 1;
	else
	{
		const ULONG nSelection = m_aSelection.size();
		for (i=0; i<nSelection; i++)
		if (m_aSelection[i] == pTerm)
			break;
		if (i >= nSelection)
			throw EXC("42S22", 12, "Column not found: [%s]", id(pTerm).c_str());
//		ASSUME (i < nSelection);
	}
	m_aFormat[i] = szFormat;
	EXC_END;
}
//---------------------------------------------------------------------------
const char *
CStatement::getFormat (CTerm* pTerm) const
{
	const ULONG nSelection = m_aSelection.size();
	ULONG i;
	for (i=0; i<nSelection; i++)
	if (m_aSelection[i] == pTerm)
		break;
	ASSUME (i < nSelection);
	map<ULONG,string>::const_iterator iFormat = m_aFormat.find (i);
	if (iFormat == m_aFormat.end())
		return 0;
	const char * szFormat = (*iFormat).second.c_str();
	return szFormat;
}
//---------------------------------------------------------------------------
void
CStatement::prepare (CFunction & aFunc)
{
	vector<CTerm*> & aArgs = aFunc.m_aArgs;
	const ULONG nArgs = aArgs.size();
	const CDesc * pDesc = 0;
	CValue aValue;
	short  nTyp = SQL_UNKNOWN_TYPE;
	ULONG i;
	for (i=0; i<nArgs; i++)
	{
		CTerm * pTerm = aArgs[i];
		assert (pTerm);
		CParam * pParam = dynamic_cast<CParam*>(pTerm);
		if (pParam)
			continue;
		CColumn * pColumn = dynamic_cast<CColumn*>(pTerm);
		if (pColumn)
		if (pDesc == 0)
			pDesc = pColumn->desc();

		short nTerm = pTerm->type();
		if (nTyp != SQL_UNKNOWN_TYPE)
		if (SQLTypeOrder (nTerm) <= SQLTypeOrder(nTyp))
			continue;
		if (nTerm != SQL_UNKNOWN_TYPE)
			nTyp = nTerm;
	}

	if (pDesc)
	{
		for (i=0; i<nArgs; i++)
		{
			CParam * pParam = dynamic_cast<CParam*>(aArgs[i]);
			if (!pParam)
				continue;
			pParam->m_IOType         = SQL_PARAM_INPUT;
			pParam->m_nType       = pDesc->type();
			pParam->m_nNullable      = pDesc->aSQL_DESC_NULLABLE();
			pParam->m_nDecimalDigits = pDesc->digits();
			pParam->m_nColumnSize    = pDesc->size();
		}
	}
	else
	for (i=0; i<nArgs; i++)
	{
		CParam * pParam = dynamic_cast<CParam*>(aArgs[i]);
		if (!pParam)
			continue;
// todo:
//		pParam->setTyp (nTyp);
	}
}
//---------------------------------------------------------------------------
void
CStatement::prepare()
{
	EXC_BEGIN;
	int i,n;
	//// Spalte * expandieren
	n = m_aSelection.size();
	for (i=0; i<n; i++)
	{
		const CTerm * pResult = dynamic_cast<const CTerm*>(m_aSelection[i]);
		if (!pResult) continue;
		const CColumn * pColumn = dynamic_cast<const CColumn*>(pResult);
		if (!pColumn || pColumn->column())
			continue;
		select (pColumn->table(), i);
		n = m_aSelection.size();
	}
	//// Spalten binden
	n = m_aMemory.size();
	for (i=0; i<n; i++)
	{
		CTerm * pResult = dynamic_cast<CTerm*>(m_aMemory[i]);
		if (!pResult) continue;
		pResult->bind(0);
		CColumn * pColumn = dynamic_cast<CColumn*>(pResult);
		if (!pColumn || !pColumn->column())
			continue;
		const char * szTable = pColumn->table();
		const char * szColumn = pColumn->column();
		if (STRNUL (szTable))
		if (stricmp (szColumn, "rownum") == 0)
			continue;
		CTableMap::const_iterator iTable = m_aTables.begin();
		if (!STRNUL(szTable))
			iTable = m_aTables.find (szTable);
		else if (m_aTables.size() > 1)
			throw EXC("42S22", 12, "Ambiguous Column: [%s]", szColumn);
		if (iTable == m_aTables.end())
			throw EXC("42S22", 12, "Column not found: [%s.%s]", szTable, szColumn);
		CTable *  pTable = iTable->second;
		assert (pTable);
		const ULONG nCols = pTable->cols();
		const ULONG j = pTable->colidx (szColumn);
		if (j == nCols)
			throw EXC("42S22", 12, "Column not found: [%s.%s]", szTable, szColumn);
		pColumn->assign (pTable);
	}
	//// Parameter typisieren
	n = m_aMemory.size();
	for (i=0; i<n; i++)
	{
		CFunction * pFunc = dynamic_cast<CFunction*>(m_aMemory[i]);
		if (!pFunc)
			continue;
		prepare (*pFunc);
	}

	delete m_pTableSet;
	m_pTableSet = new CTableSet (*this); 
	m_pResult = 0;
	EXC_END;
}
//---------------------------------------------------------------------------
void
CStatement::execute()
{
	EXC_BEGIN;

	if (!m_pTableSet)
		return;

	ULONG i;
	const vector<CTerm*> & aSelection = m_aSelection;
	char * szParam = "12";
	double dParam = 12;

	vector<const char*> aMist = keys(m_aTables);
	VSORT (aMist);

	//// Group
	const vector<CTerm*> & aGroup = m_aGroup;
	const ULONG nGroup = aGroup.size();

	//// Having
	CTerm * pHaving = m_pHaving;

	const int nOrder = m_aOrder.size();
	const vector<CTerm*> & aOrder = m_aOrder;

	//// Aggregation
	vector<CUnary*> & aAggr = m_aAggr;
	const int nAggr = aAggr.size();

	m_pTableSet->open (m_pWhere);
	m_pResult = m_pTableSet;
	if (!nAggr && !nGroup && !nOrder && !m_bDistinct)
		return;

	CResultset * pRecordset = new CResultset();
	assert (pRecordset);
	m_aTmpTbl = CResult (pRecordset);
	const int nSelection = aSelection.size();
	if (nAggr || nGroup)
	{
		m_pResult = pRecordset;
		for (i=0; i<nGroup; i++)
			pRecordset->add (aGroup[i], true);
		for (i=0; i<nAggr; i++)
			pRecordset->aggr (aAggr[i]);
	}
	else if (m_bDistinct)
	{
		m_pResult = pRecordset;
		for (i=0; i<nSelection; i++)
			pRecordset->add (aSelection[i], true);
		for(i=0; i<nOrder; i++)
			pRecordset->add (aOrder[i], false);
	}
	else
	{
		for(i=0; i<nGroup; i++)
			pRecordset->add (aGroup[i], false);
		for(i=0; i<nOrder; i++)
			pRecordset->add (aOrder[i], false);
	}
	pRecordset->create();
	time_t t0 = ::time(0);
	while (m_pTableSet->fetch())
		pRecordset->update();
	time_t te = ::time(0);
	time_t td = te - t0;

	const ULONG nRow = pRecordset->rows();
	m_aIndex.clear();
	m_aIndex.reserve (nRow);
	if (pHaving)
	for (i=0; i<nRow; i++)
	{
		pRecordset->move (i);
		if (!pHaving->asBool())
			continue;
		m_aIndex.push_back(i);
	}
	else
	for (i=0; i<nRow; i++)
	{
		m_aIndex.push_back(i);
	}

	if (nAggr && nOrder)
	{
		CResultset * pResultset = new CResultset();
		assert (pResultset);
		CResult aResult (pResultset);
		for (i=0; i<nOrder; i++)
			pResultset->add (aOrder[i], false);
		pResultset->create();
		ULONG nRows = rows();
		for (i=0; i<nRows; i++)
		{
			move(i);
			pResultset->update();
		}
		pResultset->sort (aOrder, m_aIndex);
	}
	else
	{
		pRecordset->sort (aOrder, m_aIndex);
	}
	const ULONG nRows = m_pResult->rows();
	printf ("%lu rows selected\n", nRows);
	return;
	EXC_END;
}
//---------------------------------------------------------------------------
void
CStatement::setSQL (const char * szSQL, int nLength)
{
	if (nLength == SQL_NTS)
		m_strSQL = szSQL;
	else
		m_strSQL.assign (szSQL, nLength);
	m_iSQL = 0;
}
//---------------------------------------------------------------------------
ULONG
CStatement::rows() const
{
	if (!m_pResult)
		return 0;
	if (m_aIndex.size())
		return m_aIndex.size();
	return m_pResult->rows();
}
//---------------------------------------------------------------------------
bool
CStatement::move (ULONG i)
{
	ASSUME (m_pResult);
	if (m_aIndex.size())
	{
		if (i >= m_aIndex.size())
			return false;
		i = m_aIndex[i];
	}
	return m_pResult->move(i);
}
//---------------------------------------------------------------------------
void
CStatement::dump()
{
	vector<CObject*>::iterator iObject = m_aMemory.begin();
	for (; iObject != m_aMemory.end(); iObject++)
	{
		CObject * pObject = *iObject;
		const char * szType = typeid (*pObject).name();
		string strObject = id(pObject);
		const char * szObject = strObject.c_str();
		const CTerm * pTerm = dynamic_cast<const CTerm*>(pObject);
		const char * szTerm = "";
		if (pTerm)
		{
			const short nType = pTerm->type();
			szTerm
			= CTerm::isBool (nType)    ? "bool"
			: CTerm::isInteger (nType) ? "int"
			: CTerm::isString (nType)  ? "char"
			: CTerm::isDateTime(nType) ? "time"
			: CTerm::isFloat (nType)   ? "real"
			: "term";
		}
		printf ("0x%x\t %s\t %-4s  %s\n", pObject, szType, szTerm, szObject);
	}
}
//---------------------------------------------------------------------------
string
CStatement::exec (const char * szCmd)
{
	if (stricmp("dump", szCmd) == 0)
	{
		dump();
		return "ok";
	}
	if (stricmp("clear", szCmd) == 0)
	{
		clear();
		return "ok";
	}
	if (stricmp("prep", szCmd) == 0)
	{
		prepare();
		return "ok";
	}
	if (stricmp("exec", szCmd) == 0)
	{
		execute();
		return "ok";
	}
	if (stricmp("names", szCmd) == 0)
	{
		printf("---- Tables: %d\n", (int)m_aTables.size());
		CTableMap::const_iterator iTables = m_aTables.begin();
		for (; iTables != m_aTables.end(); iTables++)
		{
			const string& strKey = iTables->first;
			const CTable* pTable = iTables->second;
			assert (pTable);
			const char * szType = typeid (*pTable).name();
			const char * szTerm = pTable->name();
			printf ("0x%x\t %s\t %s->%s\n", pTable, szType, strKey.c_str(), szTerm);
		}
		printf("---- Names: %d\n", (int)m_aNames.size());
		map<string,CTerm*>::const_iterator iNames = m_aNames.begin();
		for (; iNames != m_aNames.end(); iNames++)
		{
			const string& strKey = iNames->first;
			const CTerm* pTerm = iNames->second;
			assert (pTerm);
			const char * szType = typeid (*pTerm).name();
			const string strTerm = id(pTerm);
			const char * szTerm = strTerm.c_str();
			printf ("0x%x\t %s\t %s->%s\n", pTerm, szType, strKey.c_str(), szTerm);
		}
		return "ok";
	}
	if (stricmp("print", szCmd) == 0)
	{
		if (!m_pResult)
			return "no result. exec!";
		vector<ULONG> aIndex = m_pResult->sort (m_aOrder, m_pHaving);
		const ULONG nRows = aIndex.size();
//		const ULONG nRows = m_aResult->rows();
		const ULONG nCols = m_aSelection.size();
		CRecord aRecord (nCols);
		format (aRecord, m_aSelection);
		ULONG i,j;
		for (i=0; i<nRows; i++)
		{
			m_pResult->move (aIndex[i]);
			fetch (aRecord, m_aSelection);
			for (j=0; j<nCols; j++)
			{
				CTerm * pTerm = m_aSelection[j];
				const char * szFormat = getFormat (pTerm);
				string strTerm = stringf (szFormat, pTerm);
				printf ("%s  ", strTerm.c_str());
			}
			printf ("\n");
		}
		printf ("%lu rows selected\n", nRows);
		return "ok";
	}
	if (stricmp("clear formats", szCmd) == 0)
	{
		m_aFormat.clear();
		return "ok";
	}
	if (stricmp("formats", szCmd) == 0)
	{
		map<ULONG,string>::const_iterator iFormat = m_aFormat.begin();
		const ULONG nSelection = m_aSelection.size();
		for (; iFormat != m_aFormat.end(); iFormat++)
		{
			const ULONG i = (*iFormat).first;
			const string & strFormat = (*iFormat).second;
			const CTerm * pTerm = (i < nSelection) ? m_aSelection[i] : 0;
			printf ("[%lu] %s: %s\n", i+1, id(pTerm).c_str(), strFormat.c_str());
		}
		return "ok";
	}
	if (stricmp("quit", szCmd) == 0)
	{
		exit(0);
		return "ok";
	}
	if (stricmp("exit", szCmd) == 0)
	{
		exit(0);
		return "ok";
	}
	return "?!";
}
//---------------------------------------------------------------------------
} // namespace