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


#include "SQLError.h"
#include <stdx/utils.h>
#include <stdarg.h>
#include <sql.h>

using namespace stdx;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
CSQLException::CSQLException (const char * file, size_t line)
: m_szFile(file)
, m_nLine(line)
, m_nCode(0)
{
	*m_szState = 0;
}
//---------------------------------------------------------------------------
CSQLException::~CSQLException()
{
}
//---------------------------------------------------------------------------
const CSQLException &
CSQLException::set (const char * szState, long nError, const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	m_strText = szBuff;
	strncpy (m_szState, szState, 5);
	m_szState[5] = 0;
	m_nCode = nError;
	return *this;
}
//---------------------------------------------------------------------------
//CSQLError CSQLError::ms_aInstance;
//---------------------------------------------------------------------------
CSQLError::CSQLError()
{
	m_nSQL_DIAG_ROW_COUNT = 0;
	memset (&m_aData, 0, sizeof(m_aData));
	m_iRecord = 0;
}
//---------------------------------------------------------------------------
CSQLError::~CSQLError()
{
	const int n = DIM(m_aData);
	int i;
	for (i=0; i<n; i++)
	{
		record_t & raRecord = m_aData[i];
		free (raRecord.szSQL_DIAG_MESSAGE_TEXT);
		raRecord.szSQL_DIAG_MESSAGE_TEXT = 0;
	}
}
//---------------------------------------------------------------------------
void CSQLError::set (const CSQLException & aExc)
{
	m_nSQL_DIAG_ROW_COUNT = 1;
	m_iRecord = 0;
	record_t & raRecord = m_aData[0];

	free (raRecord.szSQL_DIAG_MESSAGE_TEXT);
	memset (&raRecord, 0, sizeof(raRecord));

	raRecord.nSQL_DIAG_NATIVE = aExc.code();
	raRecord.szSQL_DIAG_MESSAGE_TEXT = strdup (aExc.text());
	strncpy (raRecord.szSQL_DIAG_SQLSTATE, aExc.state(), 5);
	raRecord.szSQL_DIAG_SQLSTATE[5] = 0;
	m_nSQL_DIAG_RETURNCODE = SQL_ERROR;
}
//---------------------------------------------------------------------------
long CSQLError::set (long nRet, const char * szState, long nError, const char * szFormat, ...)
{
	m_nSQL_DIAG_ROW_COUNT = 0;
	m_iRecord = 0;
	va_list aArgs;
	va_start (aArgs, szFormat);
	add (szState, nError, szFormat, aArgs);
	va_end(aArgs);
	return m_nSQL_DIAG_RETURNCODE = nRet;
}
//---------------------------------------------------------------------------
void CSQLError::set (const char * szState, long nError, const char * szFormat, ...)
{

	m_nSQL_DIAG_ROW_COUNT = 0;
	m_iRecord = 0;
	va_list aArgs;
	va_start (aArgs, szFormat);
	add (szState, nError, szFormat, aArgs);
	va_end(aArgs);
}
//---------------------------------------------------------------------------
void CSQLError::add (const char * szState, long nError, const char * szFormat, ...)
{
	int i = m_nSQL_DIAG_ROW_COUNT;
	if (i >= DIM(m_aData))
		return;
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);

	record_t & raRecord = m_aData[i];
	free (raRecord.szSQL_DIAG_MESSAGE_TEXT);
	memset (&raRecord, 0, sizeof(raRecord));

	raRecord.nSQL_DIAG_NATIVE = nError;
	raRecord.szSQL_DIAG_MESSAGE_TEXT = strdup (szBuff);
	strncpy (raRecord.szSQL_DIAG_SQLSTATE, szState, 5);
	raRecord.szSQL_DIAG_SQLSTATE[5] = 0;
	m_nSQL_DIAG_ROW_COUNT++;
}
//---------------------------------------------------------------------------
void CSQLError::clear(bool bFree)
{
	const int n = DIM(m_aData);
	int i;
	if (bFree)
	for (i=0; i<n; i++)
	{
		record_t & raRecord = m_aData[i];
		free (raRecord.szSQL_DIAG_MESSAGE_TEXT);
		raRecord.szSQL_DIAG_MESSAGE_TEXT = 0;
	}
	m_nSQL_DIAG_ROW_COUNT = 0;
	m_iRecord = 0;
}
//---------------------------------------------------------------------------
const CSQLError::record_t *
CSQLError::fetch()
{
	if (m_iRecord >= count())
		return 0;
	return m_aData + (m_iRecord++);
}
//---------------------------------------------------------------------------
