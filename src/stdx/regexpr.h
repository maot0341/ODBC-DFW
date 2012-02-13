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
class CRegExp
{
public:
	CRegExp (const char * expr=0, const char * any="*", const char * one="?", bool cs=true);
	CRegExp (const CRegExp&);

	virtual ~CRegExp();
	void init (const char * expr, const char * any="*", const char * one="?", bool cs=true);
	CRegExp & operator= (const CRegExp &);
	void setPattern (const char *);
	void setWildcard(const char *, bool any=true);
	void setCaseSensitive (bool b)                                         { m_bCaseSensitive = b; }
	bool empty() const                                                     { return m_aToken.empty(); }
	size_t size() const                                                    { return m_aToken.size(); }
	
	const char * pattern() const                                           { return m_szExpr; }
	const char * wildcard(bool any=true) const                             { return any ? m_aSpc[0] : m_aSpc[1]; }
	bool casesensitive() const                                             { return m_bCaseSensitive; }

	bool match(const char *) const;
	bool match(const std::string &s) const                                 { return match (s.c_str()); }

protected:
	virtual int compare (const char * s1, const char * s2, size_t n) const;
	const char * strstr (const char *, const char *) const;
	void init();

	bool m_bCaseSensitive;
	const char * m_szExpr;
	const char * m_aSpc[2];

	std::vector<std::string> m_aToken;
};
//---------------------------------------------------------------------------
} // namespace stdx
#endif
