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


#include <OB/CORBA.h>
#include <idl/IRemote_idl.h>
#include <stdx/debug.h>
#include <string>

using namespace std;
//---------------------------------------------------------------------------
// Debug Info
//---------------------------------------------------------------------------
class CDebugInfo
{
public:
	CDebugInfo (const char * file, size_t line) : m_szFile(file), m_nLine(line) {}
	const idl::typDiagItem  & create (const char * szState, long nError, const char * szMessage, ...);

protected:
	const char * m_szFile;
	size_t m_nLine;
	idl::typDiagItem  m_aDiag;
};

#define EXC CDebugInfo(__FILE__,__LINE__).create
//---------------------------------------------------------------------------
#endif
