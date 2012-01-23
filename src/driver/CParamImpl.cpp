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
#include <sql.h>
#include <sqlext.h>
#include <assert.h>
#include "CParamImpl.h"


//---------------------------------------------------------------------------
CParamImpl::CParamImpl()
{
	m_nValueType = 0;
	m_nParameterType = 0;
	m_nColumnSize = 0;
	m_nDecimalDigits = 0;
	m_pParameterValuePtr = 0;
	m_nBufferLength = 0;
	m_pStrLen_or_IndPtr = 0;
	*m_szValue = 0;
}
//---------------------------------------------------------------------------
CParamImpl::CParamImpl (const CParamImpl & aParam)
{
	m_nValueType = aParam.m_nValueType;
	m_nParameterType = aParam.m_nParameterType;
	m_nColumnSize = aParam.m_nColumnSize;
	m_nDecimalDigits = aParam.m_nDecimalDigits;
	m_pParameterValuePtr = aParam.m_pParameterValuePtr;
	m_nBufferLength = aParam.m_nBufferLength;
	m_pStrLen_or_IndPtr = aParam.m_pStrLen_or_IndPtr;
	*m_szValue = 0;
}
//---------------------------------------------------------------------------
#if 0
const char * 
CParamImpl::asString() const
{
	if (!m_pParameterValuePtr)
		return 0;
	switch (m_nValueType)
	{
	case SQL_C_CHAR:
		if (!m_pStrLen_or_IndPtr || *m_pStrLen_or_IndPtr == SQL_NTS)
			strcpy (m_szValue, (char*)m_pParameterValuePtr);
		else
			strncpy (m_szValue, (char*)m_pParameterValuePtr, *m_pStrLen_or_IndPtr);
		break;
	case SQL_C_LONG:
		wsprintf (m_szValue, "%ld", *(long*)m_pParameterValuePtr);
		break;
	case SQL_C_SHORT:
		wsprintf (m_szValue, "%ld", *(short*)m_pParameterValuePtr);
		break;
	case SQL_C_FLOAT:
		wsprintf (m_szValue, "%ld", *(float*)m_pParameterValuePtr);
		break;
	case SQL_C_DOUBLE:
		wsprintf (m_szValue, "%ld", *(double*)m_pParameterValuePtr);
		break;
	default:
		assert (false);
	}
	return m_szValue;
}
#endif
//---------------------------------------------------------------------------
