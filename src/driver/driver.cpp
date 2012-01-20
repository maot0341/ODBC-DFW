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


#include <vld.h>
#include <windows.h>
#include <sqlext.h>
#include <sql.h>

#include <stdio.h>
#include <assert.h>
#include <crtdbg.h>
#include <time.h>

#include <algorithm>
#include <string>
#include <map>

#include <sqlplib/sqlp.h>
#include <sqlplib/desc.h>
#include <stdx/debug.h>
#include "driver.h"
#include "database.h"
#include "CStorage.h"



using namespace std;
//---------------------------------------------------------------------------
// Main -- DLL-Initialisierung
//---------------------------------------------------------------------------
#if 1
#ifdef __cplusplus
extern "C" { 			/* Assume C declarations for C++   */
#endif  /* __cplusplus */

BOOL WINAPI DllMain (HINSTANCE hInstanceDLL, DWORD dwReason, LPVOID pReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH)
	{
//		_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
	}
//	if (dwReason == DLL_PROCESS_DETACH || dwReason == DLL_THREAD_DETACH)
	if (dwReason == DLL_PROCESS_DETACH)
	{
		//_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
//		_CrtDumpMemoryLeaks();
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);
//		_CrtCheckMemory();
		_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
	}
	return TRUE;
}
#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif
//---------------------------------------------------------------------------
bool 
SQLWrite (const char * szValue, SQLPOINTER pBuffer, SQLSMALLINT nSize, SQLSMALLINT * pLength)
{
	if (!pBuffer) return false;
	if (!pLength) return false;
	if (!szValue) szValue = "";
	char * szBuffer = (char*)pBuffer;
	const int nLength = strlen (szValue);
	strncpy (szBuffer, szValue, nSize);
	szBuffer [nSize - 1] = 0;
	*pLength = strlen (szBuffer);
	return (nLength < nSize);
};
//---------------------------------------------------------------------------
// Test Umgebung:
// D:\jvater\odbc test\OdbcTE32.Exe
//---------------------------------------------------------------------------
string SQLString (const void * pStr, long nLen)
{
	const char * szStr = (const char *) pStr;
	if (szStr && nLen == SQL_NTS)
		return string(szStr);
	if (szStr && nLen > 0)
		return string (szStr, nLen);
	return string();
}
//---------------------------------------------------------------------------
string SQLString (const void * pStr, const long * pLen)
{
	const char * szStr = (const char *) pStr;
	if (szStr && (!pLen || *pLen == SQL_NTS))
		return string(szStr);
	if (szStr && pLen && *pLen > 0)
		return string (szStr, *pLen);
	return string();
}
//---------------------------------------------------------------------------
