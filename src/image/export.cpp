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

#include <windows.h>
#include <sqlext.h>
#include <sql.h>

#include <stdio.h>
#include <assert.h>
#include "export.h"


using namespace sqlp;
//---------------------------------------------------------------------------
// Basic Types
//---------------------------------------------------------------------------
inline
void ExpCHAR (void * pValue, long nSize, long * pInd, const char * szValue)
{
	int nLen = strlen(szValue);
	if (pInd)
		*pInd = nLen;
	if (!pValue)
		return;
	if (nLen < nSize)
		memcpy (pValue, szValue, nLen+1);
	else
		memcpy (pValue, szValue, nSize);
}
//---------------------------------------------------------------------------
void ExpCHAR (void * pValue, long nSize, long * pInd, double dValue)
{
	char szValue[256];
	sprintf (szValue, "%g", dValue);
	ExpCHAR (pValue, nSize, pInd, szValue);
}
//---------------------------------------------------------------------------
void ExpCHAR (void * pValue, long nSize, long * pInd, long nValue)
{
	char szValue[256];
	sprintf (szValue, "%d", nValue);
	ExpCHAR (pValue, nSize, pInd, szValue);
}
//---------------------------------------------------------------------------
void ExpTIMESTAMP (void * pValue, long nSize, long * pInd, time_t nTime)
{
	if (pInd)
		*pInd = SQL_NULL_DATA;
	if (nTime == 0)
		return;
	const size_t nLen = sizeof(TIMESTAMP_STRUCT);
	ASSUME (nLen <= nSize);
	if (pInd)
		*pInd = nLen;
	if (!pValue)
		return;
	tm * pTime = localtime (&nTime);
	ASSUME (pTime);
	TIMESTAMP_STRUCT * pTarget = (TIMESTAMP_STRUCT*)pValue;
	pTarget->year = pTime->tm_year + 1900;
	pTarget->month = pTime->tm_mon + 1;
	pTarget->day = pTime->tm_mday;
	pTarget->hour = pTime->tm_hour;
	pTarget->minute = pTime->tm_min;
	pTarget->second = pTime->tm_sec;
}
//---------------------------------------------------------------------------
// Term -- Exports
//---------------------------------------------------------------------------
bool ExpNULL (long * pInd, const CTerm * pTerm)
{
	assert (pTerm);
	bool bNull = pTerm->isNull();
	if (!bNull)
		return false;
	if (pInd)
		*pInd = SQL_NULL_DATA;
	return true;
}
//---------------------------------------------------------------------------
void ExpCHAR_TermFLOAT (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	assert (pTerm);
	if (ExpNULL (pInd, pTerm))
		return;
	char szValue[256];
	double dValue = pTerm->asDouble();
	sprintf (szValue, "%g", dValue);
	ExpCHAR (pValue, nSize, pInd, szValue);
}
//---------------------------------------------------------------------------
void ExpCHAR_TermINT (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	assert (pTerm);
	if (ExpNULL (pInd, pTerm))
		return;
	char szValue[256];
	long nValue = pTerm->asInteger();
	sprintf (szValue, "%d", nValue);
	ExpCHAR (pValue, nSize, pInd, szValue);
}
//---------------------------------------------------------------------------
void ExpCHAR_TermTIME (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	assert (pTerm);
	if (ExpNULL (pInd, pTerm))
		return;
	_daylight = 0;
	char szTime[256] = "";
	time_t nTime = pTerm->asDouble();

	struct tm * pTime = localtime(&nTime);
	ASSUME (pTime);

	//strftime (szTime, sizeof(szTime), "%d.%m.%Y %H:%M:%S", pTime);
	strftime (szTime, sizeof(szTime), "%Y-%m-%d %H:%M:%S", pTime);
	ExpCHAR (pValue, nSize, pInd, szTime);
}
//---------------------------------------------------------------------------
void ExpTermLONG (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	if (ExpNULL (pInd, pTerm))
		return;
	long nValue = pTerm->asDouble();
	long nLen = sizeof(nValue);
	ASSUME (nLen <= nSize);
	if (pInd)
		*pInd = nLen;
	memcpy (pValue, &nValue, nLen);
}
//---------------------------------------------------------------------------
void ExpTermSHORT (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	if (ExpNULL (pInd, pTerm))
		return;
	short nValue = pTerm->asDouble();
	long nLen = sizeof(nValue);
	ASSUME (nLen <= nSize);
	if (pInd)
		*pInd = nLen;
	memcpy (pValue, &nValue, nLen);
}
//---------------------------------------------------------------------------
void ExpTermDOUBLE (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	if (ExpNULL (pInd, pTerm))
		return;
	double dValue = pTerm->asDouble();
	long nLen = sizeof(dValue);
	ASSUME (nLen <= nSize);
	if (pInd)
		*pInd = nLen;
	memcpy (pValue, &dValue, nLen);
}
//---------------------------------------------------------------------------
void ExpTermFLOAT (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	if (ExpNULL (pInd, pTerm))
		return;
	float dValue = pTerm->asDouble();
	long nLen = sizeof(dValue);
	ASSUME (nLen <= nSize);
	if (pInd)
		*pInd = nLen;
	memcpy (pValue, &dValue, nLen);
}
//---------------------------------------------------------------------------
void ExpTermTIMESTAMP (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	if (ExpNULL (pInd, pTerm))
		return;
	time_t nTime = pTerm->asDouble();
	ExpTIMESTAMP (pValue, nSize, pInd, nTime);
}
//---------------------------------------------------------------------------
void ExpTermCHAR (void * pValue, long nSize, long * pInd, const CTerm * pTerm)
{
	if (ExpNULL (pInd, pTerm))
		return;
	const char * szValue = pTerm->asString();
	ExpCHAR (pValue, nSize, pInd, szValue);
}
//---------------------------------------------------------------------------
/*
void *
ExpCHAR (term_t eTerm)
{
	void (*export)(void *, long, long*, const CTerm *);
	if (eTerm == TermINT)
		export = ExpCHAR_TermINT;
	if (eTerm == TermFLOAT)
		export = ExpCHAR_TermFLOAT;
	if (eTerm == TermVCHAR)
		export = ExpCHAR_TermVCHAR;
	if (eTerm == TermTIME)
		export = ExpCHAR_TermTIME;
	return export;
}
*/







//---------------------------------------------------------------------------
void
TExport<char>::operator=(const char * szValue)
{
	ExpCHAR (m_pValue, m_nSize, m_pInd, szValue);
}
//---------------------------------------------------------------------------
void
TExport<char>::operator=(double dValue)
{
	ExpCHAR (m_pValue, m_nSize, m_pInd, dValue);
}
//---------------------------------------------------------------------------
CExport::CExport (short nType, void* val, long size, long* ind)
: m_nType(nType), m_pValue(val), m_nSize(size), m_pInd(ind)
{
	m_char = 0;
	m_double = 0;
	m_long = 0;
	m_short = 0;
}
//---------------------------------------------------------------------------
void CExport::operator=(const char * szValue) const
{
	long nLen = 0;
	if (szValue)
		nLen = strlen(szValue);
	switch (m_nType)
	{
	case SQL_C_CHAR:
		if (m_pInd)
			*m_pInd = nLen;
		if (!m_pValue)
			break;
		if (!m_nSize)
			break;
		if (nLen < m_nSize)
			strcpy ((char*)m_pValue, szValue);
		else
			memcpy (m_pValue, szValue, m_nSize);
		break;
	default:
		ASSUME (false);
	}
}
//---------------------------------------------------------------------------
void CExport::operator=(double dValue) const
{
	if (!m_pValue)
		return;
	switch (m_nType)
	{
	case SQL_C_CHAR:
		ExpCHAR (m_pValue, m_nSize, m_pInd, dValue);
		break;
	case SQL_C_DOUBLE:
//		ExportLong (m_pValue, m_nSize, m_pInd, dValue);
		break;
	case SQL_C_LONG:
//		ExportLong (m_pValue, m_nSize, m_pInd, (long)dValue);
		break;
	default:
		ASSUME (false);
	}

}
//---------------------------------------------------------------------------
void CExport::operator=(long nValue) const
{
	if (!m_long)
	{
		if (m_nType == SQL_CHAR)
			m_long = ExpCHAR;
		ASSUME (m_long);
	}
	m_long (m_pValue, m_nSize, m_pInd, nValue);
}
//---------------------------------------------------------------------------
void CExport::operator=(short nValue) const
{
	if (!m_short)
	{
		if (m_nType == SQL_CHAR)
			m_short = ExpCHAR;
		ASSUME (m_short);
	}
	m_short (m_pValue, m_nSize, m_pInd, nValue);
}
//---------------------------------------------------------------------------























//---------------------------------------------------------------------------
void ExportLong (short nType, void * pValue, long nSize, long * pIndicator, long nValue)
{
	char * szValue = (char*) pValue;
	////-----------------------------------------
	//// default mapping
	////-----------------------------------------
	if (nType == SQL_C_DEFAULT)
	{
		if (nSize >= sizeof(double))
			nType = SQL_C_DOUBLE;
		else
		if (nSize >= sizeof(long))
			nType = SQL_C_LONG;
		else
			nType = SQL_C_SHORT;
	}
	////-----------------------------------------
	//// type case
	////-----------------------------------------
	switch (nType)
	{
	case SQL_C_CHAR:
		if (szValue)
		{
			ASSUME (nSize >= 8);
			if (szValue)
				sprintf (szValue, "%d", nValue);
			if (pIndicator)
				*pIndicator = strlen(szValue);
		}
		break;
	case SQL_C_LONG:
		ASSUME (nSize >= sizeof(long));
		if (pValue)
			*((long*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(long);
		break;
	case SQL_C_SHORT:
		ASSUME (nSize >= sizeof(short));
		if (pValue)
			*((short*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(short);
		break;
	case SQL_C_FLOAT:
		ASSUME (nSize >= sizeof(float));
		if (pValue)
			*((float*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(float);
		break;
	case SQL_C_DOUBLE:
		ASSUME (nSize >= sizeof(double));
		if (pValue)
			*((double*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(double);
		break;
	case SQL_C_TIMESTAMP:
		if (nValue == 0)
		{
			if (pIndicator)
				*pIndicator = SQL_NULL_DATA;
			break;
		}
		else
		{
			const size_t nLen = sizeof(TIMESTAMP_STRUCT);
			ASSUME (nLen <= nSize);
			if (pIndicator)
				*pIndicator = nLen;
			if (!pValue)
				break;
			tm * pTime = localtime (&nValue);
			ASSUME (pTime);
			TIMESTAMP_STRUCT * pTarget = (TIMESTAMP_STRUCT*)pValue;
			pTarget->year = pTime->tm_year + 1900;
			pTarget->month = pTime->tm_mon + 1;
			pTarget->day = pTime->tm_mday;
			pTarget->hour = pTime->tm_hour;
			pTarget->minute = pTime->tm_min;
			pTarget->second = pTime->tm_sec;
		}
		break;
	default:
		ASSUME (false);
	}
}
//---------------------------------------------------------------------------
void ExportTime (short nType, void * pValue, long nSize, long * pIndicator, time_t nValue)
{
	////-----------------------------------------
	//// null data
	////-----------------------------------------
	if (nValue <= 0)
	{
		if (pIndicator)
			*pIndicator = SQL_NULL_DATA;
		return;
	}
	////-----------------------------------------
	//// default mapping
	////-----------------------------------------
	if (nType == SQL_C_DEFAULT)
	{
		if (nSize >= sizeof(TIMESTAMP_STRUCT))
			nType = SQL_C_TIMESTAMP;
		else
		if (nSize >= sizeof(double))
			nType = SQL_C_DOUBLE;
		else
		if (nSize >= sizeof(long))
			nType = SQL_C_LONG;
		else
			nType = SQL_C_SHORT;
	}
	////-----------------------------------------
	//// type case
	////-----------------------------------------
	switch (nType)
	{
	case SQL_C_CHAR:
		if (pValue)
		{
			char * szValue = (char*) pValue;
			struct tm * pTime = localtime(&nValue);
			ASSUME (pTime);
			//strftime (szTime, sizeof(szTime), "%d.%m.%Y %H:%M:%S", pTime);
			strftime (szValue, nSize, "%Y-%m-%d %H:%M:%S", pTime);
		}
		if (pIndicator)
			*pIndicator = 20;
		break;
	case SQL_C_LONG:
		ASSUME (nSize >= sizeof(long));
		if (pValue)
			*((long*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(long);
		break;
	case SQL_C_SHORT:
		ASSUME (nSize >= sizeof(short));
		if (pValue)
			*((short*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(short);
		break;
	case SQL_C_FLOAT:
		ASSUME (nSize >= sizeof(float));
		if (pValue)
			*((float*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(float);
		break;
	case SQL_C_DOUBLE:
		ASSUME (nSize >= sizeof(double));
		if (pValue)
			*((double*)pValue) = nValue;
		if (pIndicator)
			*pIndicator = sizeof(double);
		break;
	case SQL_C_TIMESTAMP:
		if (pValue)
		{
			const size_t nLen = sizeof(TIMESTAMP_STRUCT);
			ASSUME (nLen <= nSize);
			tm * pTime = localtime (&nValue);
			ASSUME (pTime);
			TIMESTAMP_STRUCT * pTarget = (TIMESTAMP_STRUCT*)pValue;
			pTarget->year = pTime->tm_year + 1900;
			pTarget->month = pTime->tm_mon + 1;
			pTarget->day = pTime->tm_mday;
			pTarget->hour = pTime->tm_hour;
			pTarget->minute = pTime->tm_min;
			pTarget->second = pTime->tm_sec;
		}
		if (pIndicator)
			*pIndicator = sizeof(TIMESTAMP_STRUCT);
		break;
	default:
		ASSUME (false);
	}
}
//---------------------------------------------------------------------------
void ExportDouble (short nType, void * pValue, long nSize, long * pIndicator, double dValue)
{
	////-----------------------------------------
	//// default mapping
	////-----------------------------------------
	char * szValue = (char*) pValue;
	if (nType == SQL_C_DEFAULT)
	{
		if (nSize >= sizeof(double))
			nType = SQL_C_DOUBLE;
		else
		if (nSize >= sizeof(float))
			nType = SQL_C_FLOAT;
		else
			nType = SQL_C_SHORT;
	}
	////-----------------------------------------
	//// type case
	////-----------------------------------------
	switch (nType)
	{
	case SQL_C_CHAR:
		if (szValue)
		{
			ASSUME (nSize >= 12);
			if (szValue)
				sprintf (szValue, "%g", dValue);
			if (pIndicator)
				*pIndicator = strlen(szValue);
		}
		break;
	case SQL_C_LONG:
		if (pValue)
			*((long*)pValue) = dValue;
		if (pIndicator)
			*pIndicator = sizeof(long);
		break;
	case SQL_C_SHORT:
		if (pValue)
			*((short*)pValue) = dValue;
		if (pIndicator)
			*pIndicator = sizeof(short);
		break;
	case SQL_C_FLOAT:
		if (pValue)
			*((float*)pValue) = dValue;
		if (pIndicator)
			*pIndicator = sizeof(float);
		break;
	case SQL_C_DOUBLE:
		if (pValue)
			*((double*)pValue) = dValue;
		if (pIndicator)
			*pIndicator = sizeof(double);
		break;
	default:
		ASSUME (false);
	}

}
//---------------------------------------------------------------------------
void ExportChar (short nType, void * pValue, long nSize, long * pIndicator, const char * szValue)
{
	////-----------------------------------------
	//// null data
	////-----------------------------------------
	if (!szValue || *szValue==0)
	{
		if (pIndicator)
			*pIndicator = SQL_NULL_DATA;
		return;
	}
	////-----------------------------------------
	//// default mapping
	////-----------------------------------------
	if (nType == SQL_C_DEFAULT)
	{
		nType = SQL_C_CHAR;
	}
	////-----------------------------------------
	//// type case
	////-----------------------------------------
	const size_t nLen = strlen(szValue);
	switch (nType)
	{
	case SQL_C_CHAR:
		if (pValue)
		{
			if (nLen < nSize)
				strcpy ((char*)pValue, szValue);
			else
				memcpy (pValue, szValue, nSize);
		}
		if (pIndicator)
			*pIndicator = nLen;
		break;
	default:
		ASSUME (false);
	}
}
//---------------------------------------------------------------------------
void ExportTerm (short nType, void * pValue, long nSize, long * pIndicator, const CTerm & aTerm)
{
	if (aTerm.isNull())
	{
		if (pIndicator)
			*pIndicator = SQL_NULL_DATA;
		return;
	}
	term_t eType = aTerm.type();
	switch (eType)
	{
	case TermBOOL:
	case TermINT:
		ExportLong (nType, pValue, nSize, pIndicator, aTerm.asInteger());
		break;
	case TermFLOAT:
		ExportDouble (nType, pValue, nSize, pIndicator, aTerm.asDouble());
		break;
	case TermTIME:
		ExportTime (nType, pValue, nSize, pIndicator, aTerm.asInteger());
		break;
	case TermVCHAR:
		ExportChar (nType, pValue, nSize, pIndicator, aTerm.asString());
		break;
	default:
		ASSUME(false);
	}
}
//---------------------------------------------------------------------------
