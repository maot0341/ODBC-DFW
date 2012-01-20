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

#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include "sqlp.h"

using namespace std;
namespace sqlp {
//---------------------------------------------------------------------------
double Plus (const vector<CTerm*> & aArgs);
double Mult (const vector<CTerm*> & aArgs);
//---------------------------------------------------------------------------
class CFunctLIKE : public CFunction
{
	CFunctLIKE (int head, const vector<CTerm*> & args);
	CFunctLIKE (int head, va_list &);

	vector<string> m_aToken;
};
//---------------------------------------------------------------------------
} // namespace
#endif
