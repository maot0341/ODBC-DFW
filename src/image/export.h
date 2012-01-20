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

#ifndef EXPORT_H
#define EXPORT_H

#include "sqlplib/sqlp.h"
using namespace sqlp;
//---------------------------------------------------------------------------
// Export -- Functions
//---------------------------------------------------------------------------
void ExpCHAR (void * pValue, long nSize, long * pInd, const char * szValue);
void ExpCHAR (void * pValue, long nSize, long * pInd, double dValue);
void ExpCHAR (void * pValue, long nSize, long * pInd, long nValue);

bool ExpNULL (long * pInd, const CTerm * pTerm);
//void ExpCHAR_TermSTR (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
#define ExpCHAR_TermVCHAR ExpTermCHAR
void ExpCHAR_TermFLOAT (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
void ExpCHAR_TermINT (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
void ExpCHAR_TermTIME (void * pValue, long nSize, long * pInd, const CTerm * pTerm);

// Defaulte Exports
void ExpTermLONG (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
void ExpTermSHORT (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
void ExpTermTIMESTAMP (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
void ExpTermDOUBLE (void * pValue, long nSize, long * pInd, const CTerm * pTerm);
void ExpTermCHAR (void * pValue, long nSize, long * pInd, const CTerm * pTerm);

//void * ExpCHAR (term_t eTerm);
//---------------------------------------------------------------------------
// Export Class Template
//---------------------------------------------------------------------------
class CExport
{
public:
	CExport (short nType=SQL_DEFAULT, void* val=0, long size=0, long* ind=0);

	virtual void operator=(const char *) const;
	virtual void operator=(double) const;
	virtual void operator=(long) const ;
	virtual void operator=(short) const;

	mutable void (*m_char)(void* val, long len, long* ind, const char *);
	mutable void (*m_double)(void* val, long len, long* ind, double);
	mutable void (*m_long)(void* val, long len, long* ind, long);
	mutable void (*m_short)(void* val, long len, long* ind, long);

	short m_nType;
	void * m_pValue;
	long * m_pInd;
	long m_nSize;
};
//---------------------------------------------------------------------------
template <class T>
class TExport
{
public:
	TExport (void* val=0, long size=0, long* ind=0)
	: m_pValue(val), m_nSize(size), m_pInd(ind), value(*((T*)m_pValue))
	{}

	T& value;

	void operator=(const char *);
	void operator=(double);

	
	void * m_pValue;
	long * m_pInd;
	long m_nSize;
};
//---------------------------------------------------------------------------
void ExportLong (short nType, void * pValue, long nSize, long * pIndicator, long nValue);
void ExportTime (short nType, void * pValue, long nSize, long * pIndicator, time_t nValue);
void ExportDouble (short nType, void * pValue, long nSize, long * pIndicator, double dValue);
void ExportChar (short nType, void * pValue, long nSize, long * pIndicator, const char * szValue);
void ExportTerm (short nType, void * pValue, long nSize, long * pIndicator, const CTerm & aTerm);
//---------------------------------------------------------------------------
#endif
