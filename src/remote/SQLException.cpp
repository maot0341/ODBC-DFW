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
const idl::typDiagItem & 
CDebugInfo::create (const char * szState, long nCode, const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	m_aDiag.strText = (const char*)szBuff;
	strncpy (m_aDiag.szState, szState, 5);
	m_aDiag.szState[5] = 0;
	m_aDiag.nCode = nCode;
	return m_aDiag;
}
//---------------------------------------------------------------------------
