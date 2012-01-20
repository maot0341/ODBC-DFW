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

#ifndef CSTMTHANDLE_H
#define CSTMTHANDLE_H

#include "CHandle.h"
#include "database.h"
#include "CParamImpl.h"

#include <vector>

using namespace std;
//---------------------------------------------------------------------------
class CStmtHandle : public CHandle
{
public:
	CStmtHandle (CDatabase*);
	virtual ~CStmtHandle();

	CDatabase * Database() const { return dynamic_cast<CDatabase*>(m_paParent); }
	short cols();

	SQLRETURN SQLPrepare (const char * sql=0);
	SQLRETURN SQLFetch();
	SQLRETURN SQLGetData (int col, short typ, void* val, long len, long *ind);
	SQLRETURN SQLGetData (int col, const CLink & t);
	SQLRETURN SQLBindCol (int col, short typ, void* val, long len, long *ind);
	SQLRETURN SQLExecute ();
	SQLRETURN SQLFreeStmt (SQLUSMALLINT option);

	SQLRETURN SQLTables 
	( const string & catalog
	, const string & schema
	, const string & table
	, const string & type
	);

	SQLRETURN SQLColumns 
	( const string & catalog
	, const string & schema
	, const string & table
	, const string & column
	);

	SQLRETURN SQLGetTypeInfo (short datatype); 

	SQLRETURN SQLDescribeCol 
	( SQLUSMALLINT nColumnNumber
	, SQLCHAR * szColumnName, SQLSMALLINT nBufferLength, SQLSMALLINT *pnNameLength
	, SQLSMALLINT *pnDataType, SQLUINTEGER *pnColumnSize
	, SQLSMALLINT *pnDecimalDigits, SQLSMALLINT *pnNullable
	);

	SQLRETURN SQLColAttribute
	( SQLUSMALLINT iCol
	, SQLUSMALLINT nDescType
	, SQLPOINTER rgbDesc
	, SQLSMALLINT cbDescMax
	, SQLSMALLINT *pcbDesc
	, SQLINTEGER *pfDesc
	);

	SQLRETURN SQLBindParameter
	( SQLUSMALLINT nParam
	, SQLSMALLINT nInputOutputType
	, SQLSMALLINT nValueType
	, SQLSMALLINT nParameterType
	, SQLUINTEGER nColumnSize
	, SQLSMALLINT nDecimalDigits
	, SQLPOINTER pParameterValuePtr
	, SQLINTEGER nBufferLength
	, SQLINTEGER *pStrLen_or_IndPtr
	);


	CRecord       m_aRecord;

	auto_ptr<CTableImpl> m_aTable;

	vector<CLink> m_aLink;
	vector<CParamImpl> m_aAPD;

	CSQLError m_aError;
	ULONG m_iRecord;

	SQLUINTEGER m_nSQL_ATTR_ROW_ARRAY_SIZE;

};
//---------------------------------------------------------------------------
#endif
