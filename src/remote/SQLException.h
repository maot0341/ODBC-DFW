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

#ifndef __SQLEXCEPTION_H__
#define __SQLEXCEPTION_H__


#include <string>
#include <stdx/debug.h>

using namespace std;
//---------------------------------------------------------------------------
// SQLTables QUERY
//---------------------------------------------------------------------------
class CSQLException
{
public:
	CSQLException (const char * file, size_t line);
	virtual ~CSQLException();
	const CSQLException & set (const char * szState, long nError, const char * szMessage, ...);
	const char * text() const { return m_strText.c_str(); }
	const char * state() const { return m_szState; }
	long code() const { return m_nCode; }

protected:
	char m_szState[6];
	long m_nCode;
	string m_strText;
	const char * m_szFile;
	size_t m_nLine;
};
#define EXC CSQLException(__FILE__,__LINE__).set
//---------------------------------------------------------------------------
#endif
