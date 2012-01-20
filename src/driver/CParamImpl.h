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

#ifndef __CPARAMIMPL_H__
#define __CPARAMIMPL_H__

#include <sqlplib/sqlp.h>
using namespace sqlp;
//---------------------------------------------------------------------------
// CParamImpl
//---------------------------------------------------------------------------
class CParamImpl : public CParam
{
public:
	CParamImpl();
	CParamImpl (const CParamImpl &);
//	const char * asString() const;


	short m_nValueType;
	short m_nParameterType;
	unsigned m_nColumnSize;
	short m_nDecimalDigits;

	void* m_pParameterValuePtr;
	long m_nBufferLength;
	long* m_pStrLen_or_IndPtr;

	mutable char m_szValue[256];
};
//---------------------------------------------------------------------------
#endif
