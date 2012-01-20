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

#ifndef TABLE_IMG_H
#define TABLE_IMG_H


#include <io.h>
#include <map>
#include <string>
#include <vector>
#include "database.h"
#include "sqlplib/sqlp.h"


using namespace std;
using namespace sqlp;

//---------------------------------------------------------------------------
// CTableIMG0
//---------------------------------------------------------------------------
class CTableIMG0 : public CTableBase
{
public:
	CTableIMG0 (const char * t=0);
	virtual bool fetch (size_t row, CRecord &) const;

protected:
	static const column_t ms_aDesc[];
	vector<_finddata_t> m_aFile;
};
//---------------------------------------------------------------------------
// CTableIMG1
//---------------------------------------------------------------------------
class CTableIMG1 : public CTableBase
{
public:
	CTableIMG1 (const char * t=0);
	~CTableIMG1();
	virtual bool fetch (size_t row, CRecord &) const;
	void filter (const char * dir, const char * expr);
	void recurse (const char * dir, const char * expr);

protected:
	typedef map<string, _finddata_t> CFileMap;
	static const column_t ms_aDesc[];
	CFileMap m_aFile;
	mutable const char ** m_pKeys;
};
//---------------------------------------------------------------------------
// CTableIMG1
//---------------------------------------------------------------------------
class CTableIMG2 : public CTableBase
{
public:
	CTableIMG2 (const char * t=0);
	~CTableIMG2();
	virtual bool fetch (size_t row, CRecord &) const;
	void filter (const char * dir, const char * expr);
	void recurse (const char * dir, const char * expr);

protected:
	typedef map<string, _finddata_t> CFileMap;
	static const column_t ms_aDesc[];
	CFileMap m_aFile;
	mutable const char ** m_pKeys;
};
//---------------------------------------------------------------------------
typedef CTableIMG1 CTableIMG;
//---------------------------------------------------------------------------
#endif
