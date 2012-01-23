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


#include "SQLException.h"
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
