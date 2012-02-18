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

#include <OB/CORBA.h>
#include <idl/IRemote_idl.h>
#include "CHandle.h"
#include "CParam.h"
#include <vector>

using namespace std;
//---------------------------------------------------------------------------
class CStmtHandle : public CHandle
{
public:
	CStmtHandle (CDatabase*, idl::IStmt_var &);
	virtual ~CStmtHandle();

	CDatabase * Database() const { return dynamic_cast<CDatabase*>(m_paParent); }
//	idl::IStmt_ptr operator->();
	void prepare (const char * sql=0);
	const char * SQL() const { return m_szSQL; }
	short cols();
	bool isValid() const;
	bool connect();
	ULONG row() const { return (m_nCache) ? (m_iRecord - 1) % m_nCache : 0; }


	SQLRETURN SQLFetch();
	SQLRETURN SQLGetData (int col, short typ, void* val, long len, long *ind);
	SQLRETURN SQLGetData (const CTarget & t);
	SQLRETURN SQLBindCol (int col, short typ, void* val, long len, long *ind);
	SQLRETURN SQLExecute ();
	SQLRETURN SQLFreeStmt (SQLUSMALLINT option);

	SQLRETURN SQLNumResultCols (SQLSMALLINT *pCols);
	SQLRETURN SQLSpecialColumns (const CORBA::String_var &,const CORBA::String_var &,const CORBA::String_var &, SQLUSMALLINT,SQLUSMALLINT);
	SQLRETURN SQLColAttribute (SQLUSMALLINT, SQLUSMALLINT, idl::typValue_var&);
	SQLRETURN SQLPrepare (const char *);
	SQLRETURN SQLStatistics (const CORBA::String_var &,const CORBA::String_var &,const CORBA::String_var &, SQLUSMALLINT,SQLUSMALLINT);
	SQLRETURN SQLGetTypeInfo (SQLSMALLINT);
	SQLRETURN SQLTables (const CORBA::String_var &,const CORBA::String_var &,const CORBA::String_var &,const CORBA::String_var &);
	SQLRETURN SQLColumns (const CORBA::String_var &,const CORBA::String_var &,const CORBA::String_var &,const CORBA::String_var &);
		

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

	SQLRETURN SQLDescribeParam 
	( SQLUSMALLINT    nParameter
	, SQLSMALLINT *   pDataTypePtr
	, SQLUINTEGER *   pParameterSizePtr
	, SQLSMALLINT *   pDecimalDigitsPtr
	, SQLSMALLINT *   pNullablePtr);

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

protected:
	idl::typHeader m_vHeader;
	idl::typRecord m_vRecord;

	vector<CLink*> m_aLink;
	vector<CTarget> m_aTarget;
//	vector<CParam> m_aParam;
	vector<CParam> m_aAPD;
	idl::typParamset m_aIPD;

	idl::IStmt_var m_aIStub;
	ULONG m_iRecord;
	const char * m_szSQL;

	SQLUINTEGER m_nSQL_ATTR_ROW_ARRAY_SIZE;

	ULONG m_iCache;
	ULONG m_nCache;
};
//---------------------------------------------------------------------------
#endif
