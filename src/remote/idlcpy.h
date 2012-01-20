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

#ifndef __IDLCPY_H__
#define __IDLCPY_H__

#include <OB/CORBA.h>
#include <idl/IRemote_idl.h>

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "CParam.h"


using namespace std;
//---------------------------------------------------------------------------
void idlcpy (const idl::typVariant & vValue, short nType, void* pValue, long nLen, long * pInd);
void idlcpy (const char * szValue, short nType, void* pValue, long nLen, long * pInd);
void idlcpy (double dValue, short nType, void* pValue, long nLen, long * pInd);
void idlcpy (long nValue, short nType, void* pValue, long nLen, long * pInd);
void idltime (double dValue, short nType, void* pValue, long nLen, long * pInd);

void idlcpy (const idl::typVariant& vValue, long* pValue);
void idlcpy (const idl::typVariant& vValue, void* pValue, SQLSMALLINT nStrLen, SQLSMALLINT * pStrLen);

void idlcpy (idl::typParam &, const CParam & );
//---------------------------------------------------------------------------
#endif
