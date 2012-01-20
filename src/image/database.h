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

#ifndef DATABASE_H
#define DATABASE_H


#include <io.h>
#include <map>
#include <string>
#include <vector>
//#include "CTableIMG.h"
#include "sqlplib/sqlp.h"
#include "driver.h"


using namespace std;
using namespace sqlp;
//---------------------------------------------------------------------------
// column descriptor
//---------------------------------------------------------------------------
struct  column_t
{
	const char * name;
	term_t type;
};
//---------------------------------------------------------------------------
class CTableBase : public CTable
{
public:

	CTableBase (const column_t desc[], size_t columns);
	virtual ~CTableBase();

	//// interface implementation
	const char* col_name (size_t i) const { return m_aDesc[i].name; }
	term_t      col_type (size_t i) const { return m_aDesc[i].type; }
	size_t rows() const { return m_nRows; }
	size_t cols() const { return m_nCols; }
	void clear (CRecord & rec) const;
	CValue desc (int attr, size_t i) const;

protected:
	const column_t * m_aDesc;
	size_t m_nRows;
	size_t m_nCols;
};
//---------------------------------------------------------------------------
class CTableTST : public CTableBase
{
public:
	CTableTST (const char * t=0);
	virtual bool fetch (size_t row, CRecord &) const;

protected:
	static const column_t ms_aDesc[];
};
//---------------------------------------------------------------------------
#endif
