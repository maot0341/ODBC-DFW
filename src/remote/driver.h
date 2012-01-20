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

#ifndef DRIVER_H
#define DRIVER_H

#include <OB/CORBA.h>
#include <idl/IRemote_idl.h>
#include <windows.h>
#include <sql.h>
#include <assert.h>
#include <crtdbg.h>

//#include <string>
//#include <list>
//#include <vector>
//#include <map>



// Define _WINSOCKAPI_ to avoid including winsock.h.
//#define _WINSOCKAPI_

//---------------------------------------------------------------------------
// exception handling
//---------------------------------------------------------------------------
#define TRY(s) try { if (s) TRACELN(s)
//#define TRY(s) try {
#define EXCEPTION(h) \
	} \
	catch (const CSQLException & aExc) \
	{ \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->error (aExc); \
	} \
	catch (const std::string & strError) \
	{ \
		TRACELN (strError.c_str()); \
	} \
	catch (const ::CORBA::Exception& aExc) \
	{ \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->error (EXC("08S01", 1999, aExc._to_string())); \
		const char * szText = aExc._to_string(); \
		TRACELN (szText); \
	} \
	catch (...) \
	{ \
	}

using namespace std;
//---------------------------------------------------------------------------
#define SQLAssign(p,v) if (p) (*(p)) = (v)
//#define SQLWrite(t,p,v) if(p) (*(t*)p) = (v)
//---------------------------------------------------------------------------
#if 0
template<class T>
bool SQLWrite (T val, SQLPOINTER pBuffer, SQLSMALLINT nSize, SQLSMALLINT * pLength)
{
	if (!pBuffer) return false;
	if (nSize < sizeof(T)) return false;
	*(T*)pBuffer = val;
	if (pLength)
		*pLength = sizeof(T);
	return true;
}
#endif
//---------------------------------------------------------------------------
bool SQLWrite (const char * szValue, SQLPOINTER pBuffer, SQLSMALLINT nSize, SQLSMALLINT * pLength);

//bool SQLWrite (const char * value, SQLPOINTER buffer, SQLSMALLINT size, SQLSMALLINT * length);
CORBA::String_var SQLString (SQLCHAR * pString, SQLSMALLINT nLength);
//---------------------------------------------------------------------------
#define SQLWriteNum(t,s,d,i) \
{ \
	if (d) *((t*)d) = ((t)s); \
	if (i) *(i) = sizeof(t); \
}
//---------------------------------------------------------------------------
#endif
