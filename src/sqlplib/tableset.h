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

#ifndef TABLESET_H
#define TABLESET_H

#include <vector>
#include <set>
//#include "sqlp.h"

using namespace std;
namespace sqlp {

class CStatement;
class CTerm;
class CTable;
class CJoin;
typedef std::set<const char*> CDomain;
/*---------------------------------------------------------------------------
tables with the same interface get ther data from a shared superset by
an sparsed index. example:

SELECT ... FROM emp t1, emp t2 WHERE filter
1) interfaces of t1 and t2 are the same: <emp>
   build shared superset T = <t1 x t2>
2) build subsets t1 and t2 of T (i.e. sparsed index)

----------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//// table container for
//// - table access obj. (interface)
//// - one or more tabel refs (alias, subset data)
//// - data cache: superset of all ref. tables
//---------------------------------------------------------------------------
class CTableSet: public CRecordset 
{
public:

	CValue attr (int attr) const;
	const CDesc * desc (ULONG col) const;

	ULONG cols() const;


	struct item;
	typedef vector<ULONG> index;
	typedef vector<const char*> domain;

	CTableSet (CStatement &);
	virtual ~CTableSet();
	void open (CTerm * filter);
	bool move (ULONG recno);
	bool fetch() { return next(); }
	void rewind() { move(0); }
	ULONG rows() const;

protected:
	void add (const char * table, CTable *);
	bool fetch (item &, ULONG recno);
	item* table (const char * name);
	item* nextouter();
	int column (CTable*, const char* column);
	void outer (const char * table);
	void setnull();

	bool fetch (item&, CDomain &);
	bool fetch (item&);
	bool next (long &);
	bool zero (long);
	bool next();

	void trace (item *) ;


	CStatement &   m_aStatement;
	vector<CJoin*> m_aJoins;
	vector<item*>  m_aItems;
	CTerm * m_pFilter;
	ULONG m_iRecord;
	ULONG m_nRecord;
	index  m_aIndex;
	ULONG  m_iOTab;
	ULONG  m_iORow;

	FILE * m_pTrace;
};
//---------------------------------------------------------------------------
} // namespace
#endif
