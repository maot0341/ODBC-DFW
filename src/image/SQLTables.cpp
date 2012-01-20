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
#pragma warning (disable:4786)

#include <windows.h>
#include <sqlext.h>
#include <sql.h>

#include <stdio.h>
#include <assert.h>
#include <crtdbg.h>
#include <time.h>

#include <string>
#include <map>
#include "driver.h"
#include "database.h"
#include "CTableIMG.h"
#include "table.h"
#include "export.h"
#include <sqlplib/index.h>


using namespace std;
//---------------------------------------------------------------------------
class CSQLTables : public CTableData
{
public:
	struct record_t
	{
		string catalog;
		string schema;
		string tablename;
		string tabletype;
		string remarks;
	};

	static CSQLTables * Instance();
	vector<size_t> filter (const string & cat, const string & sch, const string & tab, const string & typ) const;

	CSQLTables();
	size_t rows() const  { return m_aData.size(); }
	virtual void fetch (size_t i, CRecord&) const;

	void record (const char * type, const char * cat, const char * sch, const char * tab, const char *remarks)
	{
		record_t aRecord;
		aRecord.catalog = cat;
		aRecord.schema = sch;
		aRecord.tablename = tab;
		aRecord.tabletype = type;
		aRecord.remarks = remarks;
		m_aData.push_back (aRecord);
	}
	void record (const char * type, const char * tab, const char *remarks)
	{
		record_t aRecord;
		aRecord.catalog.erase();
		aRecord.schema.erase();
		aRecord.tablename = tab;
		aRecord.tabletype = type;
		aRecord.remarks = remarks;
		m_aData.push_back (aRecord);
	}

protected:
	vector<record_t> m_aData;
	static auto_ptr<CSQLTables> ms_aInstance;
};
//---------------------------------------------------------------------------
auto_ptr<CSQLTables> CSQLTables::ms_aInstance;
//---------------------------------------------------------------------------
CSQLTables * 
CSQLTables::Instance()
{
	CSQLTables * pTable = ms_aInstance.get();
	if (pTable)
		return pTable;
	pTable = new CSQLTables();
	ASSUME (pTable);
	ms_aInstance = auto_ptr<CSQLTables>(pTable);
	return pTable;
}
//---------------------------------------------------------------------------
CSQLTables::CSQLTables()
{
	m_aFields.resize(5);
	m_aFields[0].pszSQL_DESC_NAME = strdup ("TABLE_CAT");
	m_aFields[0].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[1].pszSQL_DESC_NAME = strdup ("TABLE_SCHEM");
	m_aFields[1].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[2].pszSQL_DESC_NAME = strdup ("TABLE_NAME");
	m_aFields[2].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[3].pszSQL_DESC_NAME = strdup ("TABLE_TYPE");
	m_aFields[3].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[4].pszSQL_DESC_NAME = strdup ("REMARKS");
	m_aFields[4].nSQL_DESC_TYPE = SQL_CHAR;

	record_t aRecord;
	m_aData.reserve (100);
	record ("TABLE", "reg",  "Modelle der Regression");
	record ("TABLE", "erg",  "Prognose Archiv (Ergebnisse/Rel.Fehler)");
	record ("TABLE", "a",    "Test Tabelle");
	record ("TABLE", "b",    "Test Tabelle");
	record ("TABLE", "test", "Test Tabelle");
};
//---------------------------------------------------------------------------
void 
CSQLTables::fetch (size_t i, CRecord & raRecord) const
{
	const size_t m = m_aData.size();
	const size_t n = m_aFields.size();
	ASSUME (i < m);
	const record_t & aRecord = m_aData[i];
	raRecord[0] = aRecord.catalog.c_str();
	raRecord[1] = aRecord.schema.c_str();
	raRecord[2] = aRecord.tablename.c_str();
	raRecord[3] = aRecord.tabletype.c_str();
	raRecord[4] = aRecord.remarks.c_str();

}
//---------------------------------------------------------------------------
vector<size_t>
CSQLTables::filter (const string & cat, const string & sch, const string & tab, const string & typ) const
{
	size_t nRows = rows();
	vector<size_t> aIndex;
	aIndex.reserve (nRows);
	size_t i;
	char cEsc = '\\';
	char cDel = '_';
	const char * szCat = cat.c_str();
	const char * szSch = sch.c_str();
	const char * szTab = tab.c_str();
	const char * szTyp = typ.c_str();
	vector<string> aCat = match (szCat, cDel, cEsc); 
	vector<string> aSch = match (szSch, cDel, cEsc); 
	vector<string> aTab = match (szTab, cDel, cEsc); 
	vector<string> aTyp = match (szTyp, cDel, cEsc); 
	for (i=0; i<nRows; i++)
	{
		const record_t & aRecord = m_aData[i];
		szCat = aRecord.catalog.c_str();
		szSch = aRecord.schema.c_str();
		szTab = aRecord.tablename.c_str();
		szTyp = aRecord.tabletype.c_str();
		if (match (szCat, aCat))
		if (match (szSch, aSch))
		if (match (szTab, aTab))
		if (match (szTyp, aTyp))
			continue;
		idx::push (aIndex, i);
	}
	return aIndex;
}
//---------------------------------------------------------------------------
