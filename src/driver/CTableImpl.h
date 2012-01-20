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

#ifndef __CTABLEIMPL_H__
#define __CTABLEIMPL_H__

#include <sqlplib/sqlp.h>
#include <sqlplib/desc.h>

#include <string>
#include <vector>

using namespace std;
using namespace sqlp;

//---------------------------------------------------------------------------
class CTableImpl: public sqlp::CTable
{
public:
	CTableImpl (const char * szName=0);
	void column (short type, const char * name, unsigned size=16, short digits=0, short nullable=SQL_NULLABLE);
	ULONG cols() const;

	const char * catalog() const  { return 0; }
	const char * schema() const   { return m_strSchema.c_str(); }
	const char * name() const     { return m_strName.c_str(); }

	CValue attr (int attr) const;
	const CDesc * desc (ULONG col) const;

	void clear (CRecord &) const;
	virtual ULONG rows() const = 0;

protected:
	vector<CDesc> m_aDesc;
	string m_strName;
	string m_strSchema;
};
//---------------------------------------------------------------------------
#endif
