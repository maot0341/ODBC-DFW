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
#include <IRemote_idl.h>
#include <sqlplib/sqlp.h>
#include <sqlplib/desc.h>
//#include <string>


//using namespace std;
using namespace sqlp;
//---------------------------------------------------------------------------
void idlcpy (idl::typValue &, bool bValue);
void idlcpy (idl::typValue &, const char *);
void idlcpy (idl::typValue &, const std::string &);
void idlcpy (idl::typValue &, long);
void idlcpy (idl::typValue &, short);
void idlcpy (idl::typValue &, double);
void idlcpy (idl::typValue &, const CTerm *);
void idlcpy (idl::typValue &, const CTerm &);
void idlnull (idl::typValue &, short type);

void idlcpy (idl::typDesc &, const CDesc &);

void idlcpy (CValue &, const idl::typVariant &);
void idlcpy (CValue &, const idl::typValue &);

void idlcpy (CParam &, const idl::typParam &);

//void idlcpy (idl::typDiagSeq &, const CDiagInfo &);

idl::typException IDL (const CException &);
idl::RETN * IDL (short retn, const CDiagInfo *);
//---------------------------------------------------------------------------
#if 0
void idlcpy (const idl::typVariant & vValue, short nType, void* pValue, long nLen, long * pInd);
void idlcpy (long * pnValue, const idl::typVariant &vValue);
void idlcpy (SQLPOINTER pStrVal, SQLSMALLINT nStrLen, SQLSMALLINT * pStrLen, const idl::typVariant & vValue);
void idlcpy (const char * szValue, short nType, void* pValue, long nLen, long * pInd);
void idlcpy (double dValue, short nType, void* pValue, long nLen, long * pInd);
void idlcpy (long nValue, short nType, void* pValue, long nLen, long * pInd);
void idltime (double dValue, short nType, void* pValue, long nLen, long * pInd);
#endif
//---------------------------------------------------------------------------
#endif
