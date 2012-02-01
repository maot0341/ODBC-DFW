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
#include "idlcpy.h"
#include "TLink.h"
#include "driver.h"
#include <stdx/debug.h>

using namespace stdx;
//---------------------------------------------------------------------------
// STATEMENT
//---------------------------------------------------------------------------
CStmtHandle::CStmtHandle (CDatabase * paParent, idl::IStmt_var & vStmt)
: CHandle (paParent)
{
	m_aStmtStub = vStmt;
	assert (paParent);
	m_nSQL_ATTR_ROW_ARRAY_SIZE = 1;

	m_iCache = 0;
	m_nCache = 1000;
}
//---------------------------------------------------------------------------
CStmtHandle::~CStmtHandle()
{
	ASSUME (m_paParent);
	m_aStmtStub->destroy();
	int nLink = m_aLink.size();
	int i;
	for (i=0; i<nLink;i++)
		delete m_aLink[i];
//	m_aStmtStub = idl::IStmt::_nil();
}
 //---------------------------------------------------------------------------
bool CStmtHandle::isValid() const
{ 
	return !CORBA::is_nil (m_aStmtStub) && !m_aStmtStub->_non_existent(); 
}
//---------------------------------------------------------------------------
bool CStmtHandle::connect()
{ 
	CDatabase * paDatabase = dynamic_cast<CDatabase*>(m_paParent);
	if (!paDatabase)
		return false;
	m_aStmtStub = (*paDatabase)->SQLAllocStmt();
	if(CORBA::is_nil (m_aStmtStub))
		return false;
	return true;
}
//---------------------------------------------------------------------------
void
CStmtHandle::prepare(const char * szSQL)
{
	m_iRecord = 0;
//	m_aError.clear();
	m_szSQL = szSQL;
	m_vHeader.length(0);
	m_vRetn = 0;
	m_iError = 0;
}
//---------------------------------------------------------------------------
#if 0
idl::IStmt_ptr
CStmtHandle::operator->()
{
	ASSUME(!CORBA::is_nil (m_aStmtStub));
	if (m_aStmtStub->_non_existent())
		connect();
//		throw EXC("08S01", 2500, "driver is not connected!");
	return m_aStmtStub;
//	assert(!CORBA::is_nil (m_aStmtStub));
//	return m_aStmtStub;
}
#endif
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLBindCol (int col, short typ, void* val, long len, long *ind)
{
	int i = col - 1;
	if (col <= 0)
		return SQL_ERROR;
	if (col > m_aLink.size())
		m_aLink.resize (col);
	m_aLink[i] = CLink::create (typ, val, len, ind);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
short
CStmtHandle::cols()
{
	short nCols = m_vHeader.length();
	if (nCols)
		return nCols;
	m_aStmtStub->desc (m_vHeader);
	return m_vHeader.length();
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLFreeStmt (SQLUSMALLINT nOption)
{
	if (nOption == SQL_CLOSE)
		m_iRecord = 0;
	if (nOption == SQL_UNBIND)
		m_aTarget.clear();
	if (CORBA::is_nil (m_aStmtStub))
		return SQL_ERROR;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLFetch()
{
	if (CORBA::is_nil (m_aStmtStub))
		return SQL_ERROR;
//	idl::RETN nRetn = SQL_SUCCESS;
	idl::RETN_var vRetn;
	//ASSUME (m_nRow > 0);
	const ULONG nCols = cols();

	const ULONG iRec = m_iRecord;
	const ULONG nRow = m_nCache;
	const ULONG iRow = (nRow) ? iRec % nRow : 0;
	if (iRow == 0)
	try
	{
		m_iCache = iRec;
#if 0
		m_vRetn = m_aStmtStub->SQLFetchRef (iRec, nRow, m_vRecord);
		m_iError = 0;
#else
		idl::typRecord_var vRecord;
		m_vRetn = m_aStmtStub->SQLFetch (iRec, nRow, vRecord);
		m_vRecord = vRecord;
		m_iError = 0;
#endif
		if (m_vRetn != SQL_SUCCESS)
		if (m_vRetn != SQL_SUCCESS_WITH_INFO)
			return m_vRetn->nRetn;
	}
	catch (const idl::typException & aExc)
	{
		m_vRetn = IDL (aExc);
	}
	catch (const CORBA::UserException & aExc)
	{
		int i = 2;
		i++;
	}
	const ULONG nRows = m_vRecord.length() / nCols;
	if (m_nCache == 0)
		m_nCache = nRows;
	if (iRow >= nRows)
		return SQL_NO_DATA;
	m_iRecord++;

	const int nTarget = m_aTarget.size();
	const int nLink = m_aLink.size();
	ULONG i,j;
	for (i=0; i<nTarget; i++)
	{
		if (i >= nCols)
			break;
		j = iRow * nCols + i;
		const CTarget & aDesc = m_aTarget[i];
		if (aDesc.pVal || aDesc.pInd)
		idlcpy (m_vRecord[j], aDesc.nTyp, aDesc.pVal, aDesc.nLen, aDesc.pInd);
	}
//	TLink<int> aLink((void*)0);
//	TLink<char> aLink1((void*)0);
//	TLink<TIMESTAMP_STRUCT> aLink2((void*)0);
	for (i=0; i<nLink; i++)
	{
		if (i >= nCols)
			break;
		const CLink * pLink = m_aLink[i];
		if (!pLink)
			continue;
		j = iRow * nCols + i;
		pLink->write (m_vRecord[j]);
	}
	return m_vRetn->nRetn;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtHandle::SQLGetData (const CTarget & aTarget)
{
	return SQLGetData (aTarget.nCol, aTarget.nTyp, aTarget.pVal, aTarget.nLen, aTarget.pInd); 
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
	ULONG iRow = row();
	ULONG iCol = nCol - 1;
	const int nCols = cols();
	const int nRows = m_vRecord.length() / nCols;
	if (0 > iCol || iCol >= nCols)
		return SQL_ERROR;
	if (0 > iRow|| iRow >= nRows)
		return SQL_NO_DATA;
	ULONG i = iRow * nCols + iCol;
//	return SQL_SUCCESS;
	idlcpy (m_vRecord[i], nTargetTyp, pTargetVal, nTargetLen, pTargetInd);
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
	short nCols = m_vHeader.length();
	if (nCols == 0)
		m_aStmtStub->desc (m_vHeader);
	nCols = m_vHeader.length();
	short i = nColumnNumber - 1;
	if (0 > i || i >= nCols)
		return SQL_INVALID_HANDLE;
	const idl::typDesc & vDesc = m_vHeader[i];
	const char * szName = vDesc.m_SQL_DESC_NAME;
	if (szColumnName && nBufferLength > 0)
	{
		if (pnNameLength)
			*pnNameLength = strlen(szName);
		strncpy ((char*)szColumnName, szName, nBufferLength);
		szColumnName[nBufferLength - 1] = 0;
	}
	if (pnDataType)
		*pnDataType = vDesc.m_SQL_DESC_CONCISE_TYPE;
	if (pnColumnSize)
		*pnColumnSize = vDesc.size;
	if (pnDecimalDigits)
		*pnDecimalDigits = vDesc.digits;
	if (pnNullable)
		*pnNullable = vDesc.m_SQL_DESC_NULLABLE;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
#define ATTRN(a,p,v) \
	case (a): \
		if (p) (*(p) = (v.m_##a)); \
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
	short nCols = m_vHeader.length();
	if (nCols == 0)
		m_aStmtStub->desc (m_vHeader);
	nCols = m_vHeader.length();
	short i = iCol - 1;
	if (0 > i || i >= nCols)
		return SQL_INVALID_HANDLE;
	const idl::typDesc & vDesc = m_vHeader[i];
	const char * szName = vDesc.m_SQL_DESC_NAME;
	switch (nDescType)
	{
	ATTRN (SQL_DESC_AUTO_UNIQUE_VALUE, pfDesc, vDesc);
	ATTRN (SQL_DESC_TYPE, pfDesc, vDesc);
	ATTRN (SQL_DESC_LENGTH, pfDesc, vDesc);
	ATTRN (SQL_DESC_PRECISION, pfDesc, vDesc);
	ATTRN (SQL_DESC_NULLABLE, pfDesc, vDesc);
	case SQL_COLUMN_DISPLAY_SIZE:
		SQLAssign (pfDesc, vDesc.size);
	case SQL_DESC_NAME:
		SQLWrite (vDesc.m_SQL_DESC_NAME, pDescVal, cbDescMax, pcbDesc);
		break;
	default:
		;
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtHandle::SQLBindParameter
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
	CParam & raParam = m_aAPD[i];
	raParam.m_nParam = nParam;
	raParam.m_nInputOutputType = nInputOutputType;
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
	short i,nParam = m_aAPD.size();
	m_vRetn = m_aStmtStub->SQLDescribeParams (m_aIPD);
	m_iError = 0;
	idl::typParamset aAPD (m_aIPD);
	for (i=0; i<nParam; i++)
	{
		if (m_aAPD[i].m_nValueType == SQL_C_DEFAULT)
			m_aAPD[i].m_nValueType = m_aIPD[i].m_nValueType;
		idlcpy (aAPD[i], m_aAPD[i]);
	}
	m_vRetn = m_aStmtStub->SQLParams (aAPD);
	m_iError = 0;
	m_vRetn = m_aStmtStub->SQLExecute ();
	m_iError = 0;
	return m_vRetn->nRetn;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
