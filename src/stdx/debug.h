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

#ifndef __STDX_DEBUG_H__
#define __STDX_DEBUG_H__


#include <windows.h>
#include <stdarg.h>
#include <string>

//-----------------------------------------------------------------------------
#ifndef TRACE
#	define TRACE OutputDebugString
#endif
//-----------------------------------------------------------------------------
#ifdef  NDEBUG
#	define TRACELN(m) ((void)0)
#else
#	define TRACELN(m) if (m) stdx::trace ("%-30s %s:%d\n", (const char *)(m), __FILE__, __LINE__)
#endif
//-----------------------------------------------------------------------------
#define ASSUME(expr) \
	if (!(expr)) \
		throw _stdxexc ("%s[%ld]: %s  assertion failed!", __FILE__, __LINE__,  #expr)
//-----------------------------------------------------------------------------
inline
std::string _stdxexc (const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	return std::string(szBuff);
}
//-----------------------------------------------------------------------------
//void trace (const char * szFormat, ...);
//---------------------------------------------------------------------------
namespace stdx
{
//-----------------------------------------------------------------------------
inline
void trace (const char * szFormat, ...)
{
	char szBuff[8096];
	va_list aList;
	va_start (aList, szFormat);
	vsprintf (szBuff, szFormat, aList);
	va_end (aList);
	//strcat (szBuff, "\n");
//	fprintf (stderr, szBuff);
	TRACE(szBuff);
}
//-----------------------------------------------------------------------------
} // namespace
#endif
