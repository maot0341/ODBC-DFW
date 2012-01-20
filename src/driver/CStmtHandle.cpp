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

#include "CStmtHandle.h"
#include "SQLQuery.h"
#include "SQLColumns.h"
#include "SQLGetTypeInfo.h"
#include "driver.h"
#include "sqlcpy.h"
#include <stdx/debug.h>
#include <assert.h>

using namespace stdx;
//---------------------------------------------------------------------------
// STATEMENT
//---------------------------------------------------------------------------
CStmtHandle::CStmtHandle (CDatabase * paParent)
: CHandle (paParent)
{
	assert (paParent);
	m_nSQL_ATTR_ROW_ARRAY_SIZE = 1;
}
//---------------------------------------------------------------------------
CStmtHandle::~CStmtHandle()
{
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLPrepare(const char * szSQL)
{
	m_iRecord = 0;
	m_aError.clear();
	if (szSQL)
	{
		CDatabase * pDatabase = dynamic_cast<CDatabase*> (parent());
		assert (pDatabase);
		CSQLQuery * pTable = new CSQLQuery (pDatabase, szSQL);
		assert (pTable);
		m_aTable = auto_ptr<CTableImpl> (pTable);
		pTable->clear (m_aRecord);
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLBindCol (int col, short typ, void* val, long len, long *ind)
{
	int i = col - 1;
	if (col <= 0)
		return SQL_ERROR;
	if (col > m_aLink.size())
		m_aLink.resize (col);
	m_aLink[i] = CLink (typ, val, len, ind);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
short
CStmtHandle::cols()
{
	CTableImpl * pTable = m_aTable.get();
	if (!pTable)
		return 0;
	return pTable->cols();
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLFreeStmt (SQLUSMALLINT nOption)
{
	if (nOption == SQL_CLOSE)
		m_iRecord = 0;
	if (nOption == SQL_UNBIND)
		m_aLink.clear();
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLFetch()
{
	CTableImpl * pTable = m_aTable.get();
	if (!pTable)
		return SQL_ERROR;
	short nCols = cols();
	if (!pTable->read (m_iRecord, m_aRecord))
		return SQL_NO_DATA;
	m_iRecord++;

	const int nLink = m_aLink.size();
	short i;
	for (i=0; i<nLink; i++)
	{
		if (i >= nCols)
			break;
		const CLink & aLink = m_aLink[i];
		if (aLink.pVal || aLink.pInd)
		sqlcpy (m_aRecord[i], aLink.nTyp, aLink.pVal, aLink.nLen, aLink.pInd);
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLGetData (int nCol, const CLink & aLink)
{
	return SQLGetData (nCol, aLink.nTyp, aLink.pVal, aLink.nLen, aLink.pInd); 
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLGetData (int nCol
, short nTargetTyp
, void* pTargetVal
, long nTargetLen
, long* pTargetInd)
{
	if (!m_iRecord)
		return SQL_ERROR;
	ULONG i = nCol - 1;
	const int nCols = cols();
	if (0 > i || i >= nCols)
		return SQL_ERROR;
	sqlcpy (m_aRecord[i], nTargetTyp, pTargetVal, nTargetLen, pTargetInd);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLDescribeCol (SQLUSMALLINT nColumnNumber
, SQLCHAR * szColumnName, SQLSMALLINT nBufferLength, SQLSMALLINT *pnNameLength
, SQLSMALLINT *pnDataType, SQLUINTEGER *pnColumnSize
, SQLSMALLINT *pnDecimalDigits, SQLSMALLINT *pnNullable
)
{
	short i = nColumnNumber - 1;
	CTableImpl * pTable = m_aTable.get();
	if (!pTable)
		return SQL_INVALID_HANDLE;
	const CDesc * pDesc = pTable->desc(i);
	ASSUME (pTable);
	const char * szName = pDesc->name();
	if (szColumnName && nBufferLength > 0)
	{
		if (pnNameLength)
			*pnNameLength = strlen(szName);
		strncpy ((char*)szColumnName, szName, nBufferLength);
		szColumnName[nBufferLength - 1] = 0;
	}
	if (pnDataType)
		*pnDataType = pDesc->type();
	if (pnColumnSize)
		*pnColumnSize = pDesc->size();
	if (pnDecimalDigits)
		*pnDecimalDigits = pDesc->digits();
	if (pnNullable)
		*pnNullable = pDesc->nullable();
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
#define ATTRN(a,p,v) \
	case (a): \
		if (p) (*(p) = (v->m_##a)); \
		break;

SQLRETURN 
CStmtHandle::SQLColAttribute
( SQLUSMALLINT iCol
, SQLUSMALLINT nDescType
, SQLPOINTER   pDescVal
, SQLSMALLINT  cbDescMax
, SQLSMALLINT *pcbDesc
, SQLINTEGER  *pfDesc
)
{
	const short i = iCol - 1;
	CTableImpl * pTable = m_aTable.get();
	if (!pTable)
		return SQL_INVALID_HANDLE;
	const CDesc * pDesc = pTable->desc(i);
	ASSUME (pDesc);
	const char * szName = pDesc->name();
	switch (nDescType)
	{
	ATTRN (SQL_DESC_AUTO_UNIQUE_VALUE, pfDesc, pDesc);
	ATTRN (SQL_DESC_TYPE, pfDesc, pDesc);
	ATTRN (SQL_DESC_LENGTH, pfDesc, pDesc);
	ATTRN (SQL_DESC_PRECISION, pfDesc, pDesc);
	ATTRN (SQL_DESC_NULLABLE, pfDesc, pDesc);
	case SQL_COLUMN_DISPLAY_SIZE:
		SQLAssign (pfDesc, pDesc->size());
		break;
	case SQL_DESC_NAME:
		SQLWrite (pDesc->name(), pDescVal, cbDescMax, pcbDesc);
		break;
	default:
		;
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtHandle:: SQLBindParameter
( SQLUSMALLINT nParam
, SQLSMALLINT nInputOutputType
, SQLSMALLINT nValueType
, SQLSMALLINT nParameterType
, SQLUINTEGER nColumnSize
, SQLSMALLINT nDecimalDigits
, SQLPOINTER pParameterValuePtr
, SQLINTEGER nBufferLength
, SQLINTEGER *pStrLen_or_IndPtr)
{
	short n = m_aAPD.size();
	short i = nParam - 1;
	assert (nParam > 0);
	if (i >= n)
		m_aAPD.resize (nParam);
	CParamImpl & raParam = m_aAPD[i];
	raParam.m_nValueType = nValueType;
	raParam.m_nParameterType = nParameterType;
	raParam.m_nColumnSize = nColumnSize;
	raParam.m_nDecimalDigits = nDecimalDigits;
	raParam.m_pParameterValuePtr = pParameterValuePtr;
	raParam.m_nBufferLength = nBufferLength;
	raParam.m_pStrLen_or_IndPtr = pStrLen_or_IndPtr;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtHandle::SQLExecute()
{
	CSQLQuery * pTable = dynamic_cast<CSQLQuery*> (m_aTable.get());
	ASSUME (pTable);
	pTable->assign (m_aAPD);
	pTable->open();
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLTables (const string & strCatalog
, const string & strSchema
, const string & strTable
, const string & strType)
{
	m_iRecord = 0;
	m_aError.clear();
	CDatabase * pDatabase = dynamic_cast<CDatabase*> (parent());
	assert (pDatabase);
	CSQLTables * pTable = pDatabase->SQLTables (strCatalog.c_str(), strSchema.c_str(), strTable.c_str(), strType.c_str());
	assert (pTable);
	m_aTable = auto_ptr<CTableImpl> (pTable);
	pTable->clear (m_aRecord);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLColumns (const string & strCatalog
, const string & strSchema
, const string & strTable
, const string & strColumn)
{
	m_iRecord = 0;
	m_aError.clear();
	CDatabase * pDatabase = dynamic_cast<CDatabase*> (parent());
	assert (pDatabase);
	CSQLColumns * pTable
	= new CSQLColumns (pDatabase
	, strCatalog.c_str()
	, strSchema.c_str()
	, strTable.c_str()
	, strColumn.c_str());
	assert (pTable);
	m_aTable = auto_ptr<CTableImpl> (pTable);
	pTable->clear (m_aRecord);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLGetTypeInfo (short nDataType)
{
	m_iRecord = 0;
	m_aError.clear();
	CDatabase * pDatabase = dynamic_cast<CDatabase*> (parent());
	assert (pDatabase);
	CSQLGetTypeInfo * pTable = new CSQLGetTypeInfo (nDataType);
	assert (pTable);
	m_aTable = auto_ptr<CTableImpl> (pTable);
	pTable->clear (m_aRecord);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
