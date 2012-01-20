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

#ifndef __SQLCPY_H__
#define __SQLCPY_H__


#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <sqlplib/sqlp.h>
#include "CParamImpl.h"


using namespace std;
using namespace sqlp;
//---------------------------------------------------------------------------
void sqlcpy (const CTerm & aValue, short nType, void* pValue, long nLen, long * pInd);
void sqlcpy (const char * szValue, short nType, void* pValue, long nLen, long * pInd);
void sqlcpy (double dValue, short nType, void* pValue, long nLen, long * pInd);
void sqlcpy (long nValue, short nType, void* pValue, long nLen, long * pInd);
void sqltime (double dValue, short nType, void* pValue, long nLen, long * pInd);

void sqlcpy (const CTerm & aValue, long* pValue);
void sqlcpy (const CTerm & aValue, void* pValue, SQLSMALLINT nStrLen, SQLSMALLINT * pStrLen);

void sqlcpy (CParam *, const CParamImpl &);
//---------------------------------------------------------------------------
#endif
