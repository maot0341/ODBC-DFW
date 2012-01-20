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

#ifndef PARAM_H
#define PARAM_H

//---------------------------------------------------------------------------
// CParam
//---------------------------------------------------------------------------
class CParam
{
public:
	CParam();
	CParam (const CParam &);
	const char * asString() const;


	short m_nParam;
	short m_nInputOutputType;
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
