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


#include "sqlcpy.h"
#include "driver.h"

#include <assert.h>
#include <windows.h>
#include <sqlext.h>
#include <time.h>

//---------------------------------------------------------------------------
void sqlcpy (const CTerm & aValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (aValue.isNull())
	{
		if (pInd)
			*pInd = SQL_NULL_DATA;
		return;
	}
	short nTerm = aValue.type();
	switch (nTerm)
	{
	CASE_SQL_STRING:
		sqlcpy (aValue.asString(), nType, pValue, nLen, pInd);
		break;
	CASE_SQL_FLOAT:
		if (nType == SQL_C_DEFAULT)
			nType = (nLen == 8) ? SQL_C_DOUBLE : (nLen == 4) ? SQL_C_FLOAT : SQL_C_DEFAULT;
		sqlcpy (aValue.asDouble(), nType, pValue, nLen, pInd);
		break;
	CASE_SQL_DATETIME:
		sqltime (aValue.asTime(), nType, pValue, nLen, pInd);
		break;
	CASE_SQL_INTEGER:
		if (nType == SQL_C_DEFAULT)
			nType = (nLen >= 4) ? SQL_C_LONG : (nLen >= 2) ? SQL_C_SHORT : SQL_C_DEFAULT;
		if (nType == SQL_C_BINARY)
			nType = SQL_C_CHAR;
		sqlcpy (aValue.asInteger(), nType, pValue, nLen, pInd);
		break;
	CASE_SQL_BOOL:
		sqlcpy ((long)aValue.asBool(), nType, pValue, nLen, pInd);
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
void sqlcpy (double dValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		SQLWriteNum (long, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		SQLWriteNum (short, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		SQLWriteNum (float, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		SQLWriteNum (double, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		char * szValue = (char*) pValue;
		sprintf (szValue, "%g", dValue);
		if (pInd)
			*pInd = strlen(szValue);
		return;
	}
//	assert (false);
}
//---------------------------------------------------------------------------
void sqlcpy (const char * szValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_CHAR || nType == SQL_C_DEFAULT)
	{
		strncpy ((char*)pValue, szValue, nLen);
		if (pInd)
			*pInd = strlen (szValue);
		((char*)pValue)[nLen-1] = 0;
		return;
	}
	assert (false);
}
//---------------------------------------------------------------------------
void sqlcpy (long nValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		SQLWriteNum (long, nValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		SQLWriteNum (short, nValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		SQLWriteNum (float, nValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		SQLWriteNum (double, nValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		char * szValue = (char*)pValue;
		sprintf (szValue, "%8d", nValue);
		//szValue[nLen-1] = 0;
		if (pInd)
			*pInd = strlen(szValue);
		return;
	}
	assert (false);
}
//---------------------------------------------------------------------------
void idltime (double dValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		SQLWriteNum (long, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		SQLWriteNum (short, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		SQLWriteNum (float, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		SQLWriteNum (double, dValue, pValue, pInd);
		return;
	}
	if (nType == SQL_TIMESTAMP)
	{
		TIMESTAMP_STRUCT * pTime = (TIMESTAMP_STRUCT*)pValue;
		if (pTime)
		{
			const time_t nTime = dValue;
			struct tm * pTM = localtime (&nTime);
			if (pTM == 0)
			{
				assert(pInd);
				*pInd = SQL_NULL_DATA;
			}
			pTime->year = pTM->tm_year + 1900;
			pTime->month = pTM->tm_mon + 1;
			pTime->day = pTM->tm_mday;
			pTime->hour = pTM->tm_hour;
			pTime->minute = pTM->tm_min;
			pTime->second = pTM->tm_sec;
			pTime->fraction = 0;
		}
		if (pInd)
//			*pInd = SQL_DATA_AT_EXEC; //SQL_NULL_DATA;
			*pInd = sizeof(TIMESTAMP_STRUCT);
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		const time_t nTime = dValue;
		struct tm * pTime = localtime (&nTime);
		char * szValue = (char*) pValue;
		size_t n = strftime (szValue, nLen, "%Y-%m-%d %H:%M:%S", pTime);
		szValue[nLen-1] = 0;
		if (pInd)
			*pInd = strlen(szValue);
		return;
	}
//	assert (false);
}
//---------------------------------------------------------------------------
// SQLAttributes:
//---------------------------------------------------------------------------
void sqlcpy (const CTerm &aValue, long * pnValue)
{
	assert (pnValue);
	short nTerm = aValue.type();
	switch (nTerm)
	{
	case SQL_BIT:
		*pnValue = (long) aValue.asBool();
		break;
	case SQL_INTEGER:
	case SQL_SMALLINT:
	case SQL_BIGINT:
	case SQL_TINYINT:
	case SQL_GUID:
		*pnValue = (long) aValue.asInteger();
		break;
	case SQL_NUMERIC:
	case SQL_DECIMAL:
	case SQL_FLOAT:
	case SQL_REAL:
	case SQL_DOUBLE:
		*pnValue = (long) aValue.asDouble();
		break;
	case SQL_DATETIME:
	case SQL_TYPE_DATE:
	case SQL_TYPE_TIME:
	case SQL_TYPE_TIMESTAMP:
	case SQL_TIME:
	case SQL_TIMESTAMP:
		*pnValue = (long) aValue.asTime();
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
void sqlcpy (const CValue & aValue, void* pStrVal, SQLSMALLINT nStrLen, SQLSMALLINT * pStrLen)
{
	short nType = aValue.type();
	assert (pStrVal);
	assert (CTerm::isString (nType));
	char * szStrVal = (char*) pStrVal;
	const char * szValue = aValue.asString();
	if (pStrLen)
		*pStrLen = strlen(szValue);
	strncpy (szStrVal, szValue, nStrLen);
}
//---------------------------------------------------------------------------
void sqltime (double dValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		*((long*)pValue) = (long) dValue;
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		*((short*)pValue) = (short) dValue;
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		*((float*)pValue) = (float) dValue;
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		*((double*)pValue) = (double) dValue;
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		const time_t nTime = dValue;
		struct tm * pTime = localtime (&nTime);
		char * szValue = (char*) pValue;
		size_t n = strftime (szValue, nLen, "%Y-%m-%d %H:%M:%S", pTime);
		szValue[nLen-1] = 0;
		if (pInd)
			*pInd = n;
		return;
	}
	if (nType == SQL_C_TIMESTAMP)
	{
		const time_t nTime = dValue;
		TIMESTAMP_STRUCT * pTime = (TIMESTAMP_STRUCT*)pValue;
		struct tm * pTM = localtime (&nTime);
		if (pTM == 0)
		{
			assert(pInd);
			*pInd = SQL_NULL_DATA;
			return;
		}
		assert (pValue); 
		pTime->year = pTM->tm_year + 1900;
		pTime->month = pTM->tm_mon + 1;
		pTime->day = pTM->tm_mday;
		pTime->hour = pTM->tm_hour;
		pTime->minute = pTM->tm_min;
		pTime->second = pTM->tm_sec;
		pTime->fraction = 0;
		if (pInd)
			*pInd = sizeof(TIMESTAMP_STRUCT);
		return;
	}
	assert (false);
}
//---------------------------------------------------------------------------
void sqlcpy (CParam * pIPD, const CParamImpl & aAPD)
{
	assert (pIPD);
	void * pVal = aAPD.m_pParameterValuePtr;
	long * pInd = aAPD.m_pStrLen_or_IndPtr;
	short nType = aAPD.m_nValueType;
	CValue & raValue = *pIPD;
	raValue.setNull();
	if (pInd && *pInd == SQL_NULL_DATA)
		return;
	if (nType == SQL_C_DEFAULT)
		nType = pIPD->type();
	if (nType == SQL_C_CHAR)
	{
		string strValue = SQLString (pVal, pInd);
		const char * szValue = strValue.c_str();
		short nType = raValue.type();
		switch (nType)
		{
		case SQL_CHAR:
		case SQL_VARCHAR:
		case SQL_LONGVARCHAR:
			raValue  = strValue;
			break;
		case SQL_INTEGER:
		case SQL_SMALLINT:
		case SQL_BIGINT:
		case SQL_TINYINT:
		case SQL_GUID:
			raValue = atoi (szValue);
			break;
		case SQL_NUMERIC:
		case SQL_DECIMAL:
		case SQL_FLOAT:
		case SQL_REAL:
		case SQL_DOUBLE:
			raValue = atof (szValue);
			break;
		default:
			assert (false);
		}
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		raValue = *(double*)pVal;
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		raValue = *(float*)pVal;
		return;
	}
	if (nType == SQL_C_LONG)
	{
		raValue = *(long*)pVal;
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		raValue = *(short*)pVal;
		return;
	}
	if (nType == SQL_C_TIMESTAMP)
	{
		TIMESTAMP_STRUCT * pTime = (TIMESTAMP_STRUCT*)pVal;
		struct tm aTM;
		memset (&aTM, 0, sizeof(aTM));
		aTM.tm_year = pTime->year - 1900;
		aTM.tm_mon = pTime->month - 1;
		aTM.tm_mday = pTime->day;
		aTM.tm_hour = pTime->hour;
		aTM.tm_min = pTime->minute;
		aTM.tm_sec = pTime->second;
		//pTime->fraction = 0;
		const time_t nTime = mktime (&aTM);;
		raValue = nTime;
		return;
	}
	assert (false);
}
//---------------------------------------------------------------------------
