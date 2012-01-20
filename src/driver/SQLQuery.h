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

#ifndef __SQLQUERY_H__
#define __SQLQUERY_H__

#include "CTableImpl.h"
#include "CParamImpl.h"
#include <stdx/debug.h>
#include <sqlplib/resultset.h>
#include <memory>

using namespace sqlp;
namespace sqlp
{
class CStatement;
}
class CDatabase;
//---------------------------------------------------------------------------
// SQLTables QUERY
//---------------------------------------------------------------------------
class CSQLQuery: public CTableImpl
{
public:
	CSQLQuery (CDatabase *, const char * sql=0);

	void open();
	ULONG rows() const;
	CValue attr (short col, short id) const;

	void assign (const vector<CParamImpl> &);
	virtual bool read (ULONG row, CRecord &) const;

	sqlp::CStatement m_aStmt;

};
//---------------------------------------------------------------------------
#endif
