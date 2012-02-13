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

#include "function.h"
#include <stdx/debug.h>


using namespace stdx;
using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
CFuncLIKE::CFuncLIKE (int nHead, const vector<CTerm*> & aArgs)
: CFunction (nHead, aArgs)
{
	const ULONG nArgs = aArgs.size();
	ASSUME (nArgs == 2);
	const CTerm* pTerm1 = aArgs.front();
	const CTerm* pTerm2 = aArgs.back();
	ASSUME (pTerm1);
	ASSUME (pTerm2);
	const char * s1 = pTerm1->asString();
	const char * s2 = pTerm2->asString();
	m_aRegExp.init (s2, "%", "?", false);
//	m_aToken = match (s2, "%");
}
//---------------------------------------------------------------------------
CFuncLIKE::CFuncLIKE (int head, va_list & args)
: CFunction (head, args)
{
}
//---------------------------------------------------------------------------
} // namespace