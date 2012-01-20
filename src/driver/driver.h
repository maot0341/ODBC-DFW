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

#include <windows.h>
#include <sqlext.h>
#include <sql.h>
#include <string>

#include "CHandle.h"


//---------------------------------------------------------------------------


using namespace std;
using namespace sqlp;

#define TRY(s) try { if (s) TRACELN(s)
#define EXCEPTION(h) \
	} \
	catch (const CSQLException & aExc) \
	{ \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->error (aExc); \
	} \
	catch (const std::string & strError) \
	{ \
		OutputDebugString (strError.c_str()); \
	} \
	catch (...) \
	{ \
	}
//---------------------------------------------------------------------------
#define SQLAssign(p,v) if (p) (*(p)) = (v)
bool SQLWrite (const char * szValue, SQLPOINTER pBuffer, SQLSMALLINT nSize, SQLSMALLINT * pLength);
string SQLString (const void* val, long  len);
string SQLString (const void* val, const long* len);

#define SQLWriteNum(t,s,d,i) \
{ \
	if (d) *((t*)d) = ((t)s); \
	if (i) *(i) = sizeof(t); \
}

//---------------------------------------------------------------------------
#endif
