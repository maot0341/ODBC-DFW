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

#ifndef __STDXREGEXPR_H__
#define __STDXREGEXPR_H__

#pragma warning (disable: 4786)

#include <string>
#include <vector>

namespace stdx
{
//---------------------------------------------------------------------------
class CRegExpr
{
public:
	CRegExpr (const char * expr, const char * any="*", const char * one="?");
	virtual ~CRegExpr();
	bool match(const char *) const;
	bool match(const std::string &s) const { return match (s.c_str()); }

protected:
	void init();

	const char * m_szExpr;
	const char * m_aSpc[2];

	std::vector<std::string> m_aToken;
};
//---------------------------------------------------------------------------
} // namespace stdx
#endif
