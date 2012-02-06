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


#include "idlcpy.h"
#include "driver.h"
#include <assert.h>
#include <windows.h>
#include <sqlext.h>
#include <time.h>

//---------------------------------------------------------------------------
void idlcpy (const idl::typVariant & vValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (vValue.isNull)
	{
		if (pInd)
			*pInd = SQL_NULL_DATA;
		return;
	}
	switch (vValue.aValue._d())
	{
	case idl::TermSTRING:
		idlcpy (vValue.aValue.aString(), nType, pValue, nLen, pInd);
		break;
	case idl::TermTIME:
		idltime (vValue.aValue.dTime(), nType, pValue, nLen, pInd);
		break;
	case idl::TermNUMBER:
		if (nType == SQL_C_DEFAULT)
			nType = (nLen == 8) ? SQL_C_DOUBLE : (nLen == 4) ? SQL_C_FLOAT : SQL_C_DEFAULT;
		idlcpy (vValue.aValue.dNumber(), nType, pValue, nLen, pInd);
		break;
	case idl::TermINTEGER:
		if (nType == SQL_C_DEFAULT)
			nType = (nLen >= 4) ? SQL_C_LONG : (nLen >= 2) ? SQL_C_SHORT : SQL_C_DEFAULT;
		if (nType == SQL_C_BINARY)
			nType = SQL_C_CHAR;
		idlcpy (vValue.aValue.nInteger(), nType, pValue, nLen, pInd);
		break;
	case idl::TermBOOLEAN:
		idlcpy ((long)vValue.aValue.bBoolean(), nType, pValue, nLen, pInd);
//		idlcpy (vValue.aValue.nInteger(), nType, pValue, nLen, pInd);
		break;
	default:
		break;
	}
};
//---------------------------------------------------------------------------
void idlcpy (double dValue, short nType, void* pValue, long nLen, long * pInd)
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
void idlcpy (const char * szValue, short nType, void* pValue, long nLen, long * pInd)
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
void idlcpy (long nValue, short nType, void* pValue, long nLen, long * pInd)
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
time_t SQL2UTC (double dTime)
{
	static const double dSecPerDay = 3600 * 24;
	return dTime * dSecPerDay;
}
//---------------------------------------------------------------------------
void idltime (double dTime, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		SQLWriteNum (long, dTime, pValue, pInd);
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		SQLWriteNum (short, dTime, pValue, pInd);
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		SQLWriteNum (float, dTime, pValue, pInd);
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		SQLWriteNum (double, dTime, pValue, pInd);
		return;
	}
	if (nType == SQL_TIMESTAMP)
	{
		TIMESTAMP_STRUCT * pTime = (TIMESTAMP_STRUCT*)pValue;
		if (pTime)
		{
			const time_t nTime = SQL2UTC(dTime);
			struct tm * pTM = localtime (&nTime);
			if (pTM == 0)
			{
				assert(pInd);
				*pInd = SQL_NULL_DATA;
				return;
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
		const time_t nTime = SQL2UTC(dTime);
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
void idlcpy (const idl::typVariant &vValue, long * pnValue)
{
	assert (pnValue);
	switch (vValue.aValue._d())
	{
	case idl::TermBOOLEAN:
		*pnValue = (long) vValue.aValue.bBoolean();
		break;
	case idl::TermINTEGER:
		*pnValue = (long) vValue.aValue.nInteger();
		break;
	case idl::TermNUMBER:
		*pnValue = (long) vValue.aValue.dNumber();
		break;
	case idl::TermTIME:
		*pnValue = (long) vValue.aValue.dTime();
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
void idlcpy (const idl::typVariant & vValue, void* pStrVal, SQLSMALLINT nStrLen, SQLSMALLINT * pStrLen)
{
	assert (pStrVal);
	assert (vValue.aValue._d() == idl::TermSTRING);
	char * szStrVal = (char*) pStrVal;
	const char * szValue = vValue.aValue.aString();
	if (pStrLen)
		*pStrLen = strlen(szValue);
	strncpy (szStrVal, szValue, nStrLen);
}
//---------------------------------------------------------------------------
char *
IDLString (void * pValue, long* pInd)
{
	assert (pValue);
	if (!pInd || *pInd == SQL_NTS)
		return CORBA::string_dup ((char*)pValue);
	const ULONG n = (*pInd);
	char * szValue = CORBA::string_alloc (n+1);
	strncpy (szValue, (char*)pValue, n);
	szValue[n] = 0;
	return szValue;
}
//---------------------------------------------------------------------------
void idlcpy (idl::typParam & crbParam, const CParam & aParam)
{
	crbParam.m_nParam = aParam.m_nParam;
	crbParam.m_nInputOutputType = aParam.m_nInputOutputType;
	crbParam.m_nParameterType = aParam.m_nParameterType;
	crbParam.m_nColumnSize = aParam.m_nColumnSize;
	crbParam.m_nDecimalDigits = aParam.m_nDecimalDigits;

	if (aParam.m_nValueType != SQL_C_DEFAULT)
		crbParam.m_nValueType = aParam.m_nValueType;

	idl::typValue & raValue = crbParam.m_aValue.aValue;
	short nType = crbParam.m_nValueType;
	void* pValue = aParam.m_pParameterValuePtr;
	long* pInd = aParam.m_pStrLen_or_IndPtr;
	crbParam.m_aValue.isNull = false;
	if (pInd && *pInd == SQL_NULL_DATA)
		crbParam.m_aValue.isNull = true;
	else
	switch (nType)
	{
	case SQL_C_CHAR:
		raValue.aString (IDLString (pValue, pInd));
		break;
	case SQL_C_LONG:
		raValue.nInteger(*(long*)pValue);
		break;
	case SQL_C_SHORT:
		raValue.nInteger(*(short*)pValue);
		break;
	case SQL_C_FLOAT:
		raValue.dNumber(*(float*)pValue);
		break;
	case SQL_C_DOUBLE:
		raValue.dNumber(*(double*)pValue);
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
idl::RETN * IDL (const idl::typException & aExc)
{
	idl::RETN_var vRetn = new idl::RETN;
	vRetn->nRetn = SQL_ERROR;
	ULONG i,n = aExc.aDiag.length();
	vRetn->aDiag.length(n);
	for (i=0; i<n; i++)
	{
		idl::typDiagItem & raDst = vRetn->aDiag[i];
		const idl::typDiagItem & aSrc = aExc.aDiag[i];
		strncpy (raDst.szState, aSrc.szState, 6);
		raDst.nCode = aSrc.nCode;
		raDst.strText = CORBA::string_dup (aSrc.strText);
		raDst.strFile = CORBA::string_dup (aSrc.strFile);
		raDst.nLine = aSrc.nLine;
	}
	return vRetn._retn();
}
//---------------------------------------------------------------------------
idl::RETN * IDL (const ::CORBA::Exception & aExc)
{
	idl::RETN_var vRetn = new idl::RETN;
	vRetn->nRetn = SQL_ERROR;
	vRetn->aDiag.length(1);
	vRetn->aDiag[0] = EXC("08S01", 1999, aExc._to_string());
	return vRetn._retn();
}
//---------------------------------------------------------------------------
