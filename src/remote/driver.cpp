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
#include <OB/CORBA.h>

#include <stdx/debug.h>
#include <crtdbg.h>
#include "driver.h"
#include "CHandle.h"


using namespace std;
using namespace stdx;

//---------------------------------------------------------------------------
// Main -- DLL-Initialisierung
//---------------------------------------------------------------------------
BOOL WINAPI DllMain (HINSTANCE hInstanceDLL, DWORD dwReason, LPVOID pReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH)
	{
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);
//		_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
		CDriver::ms_hInstance = hInstanceDLL;
	}
//	if (dwReason == DLL_PROCESS_DETACH || dwReason == DLL_THREAD_DETACH)
	if (dwReason == DLL_PROCESS_DETACH)
	{
		//_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
//		_CrtDumpMemoryLeaks();
//		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
//		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
//		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);
//		_CrtCheckMemory();
//		_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
//		CDriver::destroy();
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// Server Shutdown (rundll32)
//---------------------------------------------------------------------------
void CALLBACK ServerShutDown (
  HWND hwnd,        // handle to owner window
  HINSTANCE hinst,  // instance handle for the DLL
  LPTSTR lpCmdLine, // string the DLL will parse
  int nCmdShow      // show state
)
{
	CORBA::ORB_ptr orb = CDriver::ORB();
	CORBA::Object_var obj;
	idl::IHost_var aHostStub;
	try 
	{
		if (!lpCmdLine || *lpCmdLine == 0)
			obj = orb->string_to_object ("corbaloc:iiop:localhost:5000/dbhost");
		else
		{
			char szBuffer[256];
			sprintf (szBuffer, "corbaloc:iiop:%s/dbhost", (const char*) lpCmdLine);
			obj = orb->string_to_object (szBuffer);
		}
		//obj = orb->string_to_object ("corbaloc:iiop:localhost:8000/dblnk");
//		obj = orb->string_to_object ("corbaloc:iiop:gowsy21:5000/dblnk");
		aHostStub = idl::IHost::_narrow(obj);
		if(CORBA::is_nil (aHostStub))
			return;
		aHostStub->shutdown (false);
		//aHostStub->exec ("shutdown");
	}
	catch(const CORBA::Exception& aExc)
	{
		trace ("CORBA: %s\n", aExc._to_string());
	}
	catch (...)
	{
	}
}
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
CORBA::String_var
SQLString (SQLCHAR* pText, SQLSMALLINT nText)
{
	CORBA::String_var vText;
	const char * szText = (const char *)pText;
	if (!szText)
		szText = "";
	if (nText == SQL_NTS)
		nText = strlen (szText);
	vText = CORBA::string_alloc (nText);
	memcpy (vText, szText, nText);
	vText[nText] = 0;
	return vText;
//	return vText._retn();
}
//---------------------------------------------------------------------------
