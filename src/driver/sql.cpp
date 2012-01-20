/***************************************************************************************

    Projekt:    remote                                                     (C) 2000 PSI AG
    Komponente: 
    Modul:      

    <Bexhreibung>

    Systeme: Alpha, Linux, NT 4

    $Author: jv $
    $Date: 2006/09/29 10:43:18 $
    $Revision: 1.4 $
    $State: Exp $

    Änderung  Datum     Autor  Bemerkung
    ------------------------------------------------------------------------------------
    @1        ??.??.00  ??
***************************************************************************************/
#pragma warning (disable:4786)

//#include <vld.h>
#include "driver.h"
#include "CStmtHandle.h"

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <commdlg.h>
#include <stdx/debug.h>
#include <assert.h>

#include <stdio.h>
#include <tchar.h>
#include <string.h>


using namespace stdx;
//---------------------------------------------------------------------------
// ODBC -- Funktionen
//---------------------------------------------------------------------------
BOOL SQL_API
ConfigDSN (HWND hWnd, WORD dRequest, LPCSTR  szDriver, LPCSTR szAttr)
{
	TRACELN ("ConfigDSN");
	MessageBox (hWnd, "ODBC Hallo World.", "ODBC - ConfigDSN", MB_OK);
	return TRUE;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLAllocEnv (SQLHENV * phenv)
{
	TRACELN ("SQLAllocEnv");
	CDriver * paDriver = CDriver::Instance();
	CEnvironment * pEnv = new CEnvironment (paDriver);
	*phenv = (SQLHENV) pEnv;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLAllocConnect (SQLHENV henv, SQLHDBC* phdbc)
{
	TRY ("SQLAllocConnect");
	CEnvironment * paEnv = (CEnvironment*)henv;
	if (!phdbc || !paEnv)
		return SQL_INVALID_HANDLE;
	CDatabase * pDatabase = new CDatabase (paEnv);
	assert (pDatabase);
	*phdbc = (SQLHDBC) pDatabase;
	return SQL_SUCCESS;
	EXCEPTION(henv);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLAllocStmt (SQLHDBC hdbc, SQLHSTMT * phstmt)
{
	TRY ("SQLAllocStmt");
	if (!hdbc)
		return SQL_INVALID_HANDLE;
	CDatabase * paDatabase = static_cast<CDatabase*>(hdbc);
	CStmtHandle * pStmt = new CStmtHandle (paDatabase);
	*phstmt = (SQLHSTMT) pStmt;
	return SQL_SUCCESS;
	EXCEPTION(hdbc);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLBindCol (SQLHSTMT hstmt
, SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	//TRACELN ("SQLBindCol");
	try {
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	return pStmt->SQLBindCol (nCol, nTargetTyp, pTargetVal, nTargetLen, pnTargetInd);
	EXCEPTION(hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLBindParameter (SQLHSTMT hstmt
, SQLUSMALLINT nParameter
, SQLSMALLINT nInputOutputType
, SQLSMALLINT nValueType
, SQLSMALLINT nParameterType
, SQLUINTEGER nColumnSize
, SQLSMALLINT nDecimalDigits
, SQLPOINTER pParameterValuePtr
, SQLINTEGER nBufferLength
, SQLINTEGER *pStrLen_or_IndPtr)
{
	TRY ("SQLBindParameter");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	return pStmt->SQLBindParameter (nParameter
	, nInputOutputType
	, nValueType
	, nParameterType
	, nColumnSize
	, nDecimalDigits
	, pParameterValuePtr
	, nBufferLength
	, pStrLen_or_IndPtr);
	EXCEPTION(hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLNumParams (SQLHSTMT hstmt
, SQLSMALLINT * pNumber)
{
	TRACELN ("SQLNumParams");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLBindParam(SQLHSTMT hstmt
, SQLUSMALLINT nIndex
, SQLSMALLINT nValueType
, SQLSMALLINT nParameterType
, SQLUINTEGER nLengthPrecision
, SQLSMALLINT nParameterScale
, SQLPOINTER pParameterValue
, SQLINTEGER *pStrLen_or_Ind)
{
	TRACELN ("SQLBindParam");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLConnect (SQLHDBC hdbc
, SQLCHAR * szServer, SQLSMALLINT nLen1
, SQLCHAR * szUser, SQLSMALLINT nLen2
, SQLCHAR * szPasswd, SQLSMALLINT nLen3
)
{
	TRACELN ("SQLConnect");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLDescribeCol(SQLHSTMT hstmt,SQLUSMALLINT nColumnNumber
, SQLCHAR * szColumnName, SQLSMALLINT nBufferLength, SQLSMALLINT *pnNameLength
, SQLSMALLINT *pnDataType, SQLUINTEGER *pnColumnSize
, SQLSMALLINT *pnDecimalDigits, SQLSMALLINT *pnNullable)
{
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	TRY ("SQLDescribeCol");
	return pStmt->SQLDescribeCol 
	(	nColumnNumber
	,	szColumnName
	,	nBufferLength
	,	pnNameLength
	,	pnDataType
	,	pnColumnSize
	,	pnDecimalDigits
	,	pnNullable
	);
	EXCEPTION (pStmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLDisconnect (SQLHDBC hdbc)
{
	TRY ("SQLDisconnect");
	CDatabase * pDatabase = static_cast<CDatabase*>(hdbc);
	if (!pDatabase)
		return SQL_INVALID_HANDLE;
	pDatabase->close();
	return SQL_SUCCESS;
	EXCEPTION(hdbc);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLError (SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt
, SQLCHAR *szSQL_State, SQLINTEGER *pnNativeError
, SQLCHAR *szMessageText, SQLSMALLINT nBufferLength, SQLSMALLINT *pnTextLength)
{
	TRACELN ("SQLError");

	SQLRETURN nRetn = SQL_ERROR;
	CHandle * pHandle = 0;
	const char * szState = 0;
	const char * szText;
	long nCode;

	if (hstmt) 
	try 
	{
		CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
		pHandle = pStmt;
	}
	catch (const CSQLException & aExc)
	{
		szText = aExc.text();
		szState = aExc.state();
		nCode = aExc.code();
	}
	catch (const std::string & strError)
	{
		szText = strError.c_str();
		szState = "S2222";
		nCode = 0xff;
		OutputDebugString (szText);
	}
	catch (...)
	{
		szText = "unknown exception";
		szState = "S2222";
		nCode = 0xff;
	}
//	EXCEPTION (hstmt)

	else 
	if (hdbc) try {
	CDatabase * pDBC = static_cast<CDatabase*>(hdbc);
	pHandle = pDBC;
	EXCEPTION (hdbc);

	const CSQLError::record_t * pError = (pHandle) ? pHandle->m_aError.fetch() : 0;
	if (pError)
	{
		szText = pError->szSQL_DIAG_MESSAGE_TEXT;
		szState = pError->szSQL_DIAG_SQLSTATE;
		nCode = pError->nSQL_DIAG_NATIVE;
	}
	if (!szState)
		return SQL_NO_DATA_FOUND;

	if (szMessageText)
	{
		strncpy ((char*)szMessageText, szText, nBufferLength);
		szMessageText[nBufferLength-1] = 0;
	}
	if (pnTextLength)
		*pnTextLength = strlen(szText);
	if (szSQL_State)
	{
		strncpy ((char*)szSQL_State, szState, 5);
		szSQL_State[6] = 0;
	}
	if (pnNativeError)
		*pnNativeError = nCode;
	return nRetn;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLExecDirect (SQLHSTMT hstmt, SQLCHAR *szSQL, SQLINTEGER nLength)
{
	TRY ("SQLExecDirect");
	SQLRETURN nResult = SQLPrepare (hstmt, szSQL, nLength);
	if (nResult != SQL_SUCCESS)
		return nResult;
	return SQLExecute (hstmt);
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLPrepare (SQLHSTMT hstmt, SQLCHAR *szInSQL, SQLINTEGER nLength)
{
	TRY ("SQLPrepare");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	string strSQL;
	if (nLength == SQL_NTS)
		strSQL = (const char *)szInSQL;
	else
		strSQL.assign ((const char *)szInSQL, nLength);
	return pStmt->SQLPrepare (strSQL.c_str());
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLExecute (SQLHSTMT hstmt)
{
	TRY ("SQLExecute");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	return pStmt->SQLExecute();
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFetch (SQLHSTMT hstmt)
{
//	TRACELN ("SQLFetch");
	try {
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
#if 1
	SQLRETURN nRetn = pStmt->SQLFetch();
#else	
	idl::typRecord_var vRecord;
	idl::RETN nRetn = (*pStmt)->SQLFetch (vRecord);
	vector<CTarget> & raTarget = pStmt->m_aTarget;
	const int nTarget = raTarget.size();
	int i;
	if (nRetn == SQL_SUCCESS)
	for (i=0; i<nTarget; i++)
	{
		CTarget & raField = raTarget[i];
		idlcpy (vRecord[i], raField.nTyp, raField.pVal, raField.nLen, raField.pInd);
	}
#endif
	return nRetn;
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFreeConnect (SQLHENV hdbc)
{
	TRY ("SQLFreeConnect");
	CDatabase * paDatabase = static_cast<CDatabase*>(hdbc);
	if (!paDatabase)
		return SQL_INVALID_HANDLE;
	delete paDatabase;
	return SQL_SUCCESS;
	EXCEPTION (hdbc);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFreeEnv (SQLHENV henv)
{
	TRY ("SQLFreeEnv");
	return SQL_SUCCESS;
	CDriver * paDriver = (CDriver*)henv;
	if (!paDriver)
		return SQL_INVALID_HANDLE;
	delete paDriver;
	return SQL_SUCCESS;
	EXCEPTION (henv);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFreeStmt (SQLHSTMT hstmt, SQLUSMALLINT nOption)
{
	TRY ("SQLFreeStmt");
	const char * szOption
	= nOption == SQL_DROP ? "DROP"
	: nOption == SQL_CLOSE ? "CLOSE"
	: nOption == SQL_UNBIND ? "UNBIND"
	: nOption == SQL_RESET_PARAMS ? "RESET"
	: "unknown";
	trace ("0x%0x SQLFreeStmt %s\n", hstmt, szOption);
	//TRACELN ("SQLFreeStmt");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	SQLRETURN  nRetn = SQL_SUCCESS; 
	if (nOption == SQL_DROP)
		delete pStmt;
	else
		nRetn = pStmt->SQLFreeStmt (nOption);
	return nRetn;
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLGetConnectAttr (SQLHDBC hdbc,
           SQLINTEGER Attribute, SQLPOINTER pValue,
           SQLINTEGER BufferLength, SQLINTEGER *pnStringLength)
{
	TRY ("SQLGetConnectAttr");
	assert (pValue);
	assert (pnStringLength);
	*(char*)pValue = 0;
	*pnStringLength = 0;
	return SQL_SUCCESS;
	EXCEPTION (hdbc);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLGetConnectOption (SQLHDBC hdbc, SQLUSMALLINT nOption, SQLPOINTER pValue)
{
	TRACELN ("SQLGetConnectOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLGetStmtAttr (SQLHSTMT hstmt
, SQLINTEGER nAttribute, SQLPOINTER pValue
, SQLINTEGER nBufferLength, SQLINTEGER *pnStringLength)
{
	TRACELN ("SQLGetStmtAttr");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLNumResultCols (SQLHSTMT hstmt, SQLSMALLINT *pCols)
{
	TRY ("SQLNumResultCols");
	assert (pCols);
	if (!hstmt)
		return SQL_INVALID_HANDLE;
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	*pCols = pStmt->cols();
	return SQL_SUCCESS;
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLSetEnvAttr (SQLHENV EnvironmentHandle
, SQLINTEGER nAttribute
, SQLPOINTER pValue
, SQLINTEGER nStringLength)
{
	TRACELN ("SQLSetEnvAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLGetEnvAttr(SQLHENV EnvironmentHandle
, SQLINTEGER Attribute, SQLPOINTER Value
, SQLINTEGER BufferLength, SQLINTEGER *StringLength)
{
	TRACELN ("SQLGetEnvAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLCancel (SQLHSTMT hstmt)
{
	TRACELN ("SQLCancel");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLCloseCursor(SQLHSTMT StatementHandle)
{
	TRACELN ("SQLCloseCursor");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLColAttributes (SQLHSTMT hstmt
, SQLUSMALLINT iCol
, SQLUSMALLINT nDescType
, SQLPOINTER rgbDesc
, SQLSMALLINT cbDescMax
, SQLSMALLINT *pcbDesc
, SQLINTEGER *pfDesc)
{
	TRY ("SQLColAttributes");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
#if 1
	return pStmt->SQLColAttribute (iCol, nDescType, rgbDesc, cbDescMax, pcbDesc, pfDesc);
	EXCEPTION (hstmt);
	return SQL_ERROR;
#else
	idl::typVariant_var crbValue;
	idl::RETN nRetn = (*pStmt)->SQLColAttribute (iCol - 1, nDescType, crbValue);
	if (crbValue->aValue._d() == idl::TermINTEGER)
	{
		*pfDesc = crbValue->aValue.nInteger();
		return nRetn;
	}
	if (crbValue->aValue._d() == idl::TermSTRING)
	{
		const char * szValue = crbValue->aValue.aString();
		strncpy ((char*)rgbDesc, szValue, cbDescMax);
		((char*)rgbDesc)[cbDescMax-1] = 0;
		return nRetn;
	}
	return nRetn;
	EXCEPTION (hstmt);
	return SQL_ERROR;
#endif
}
//---------------------------------------------------------------------------
#define ATTRN(i) if (nDescType == i) \
	sqlcpy (crbValue, pnNumVal, nStrLen, pStrLen)
#define ATTRS(i) if (nDescType == i) \
	sqlcpy (crbValue, pStrVal, nStrLen, pStrLen);
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLColAttribute (SQLHSTMT hstmt
, SQLUSMALLINT iCol
, SQLUSMALLINT nDescType
, SQLPOINTER pStrVal
, SQLSMALLINT nStrLen
, SQLSMALLINT *pStrLen
, SQLPOINTER pNumVal)
{
	TRY ("SQLColAttribute");
//todo:
#if 0
	long * pnNumVal = (long*) pNumVal;
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	SQLRETURN nRetn = SQL_ERROR;
	ATTRN (SQL_DESC_AUTO_UNIQUE_VALUE);
	switch (nDescType)
	{
	case SQL_DESC_AUTO_UNIQUE_VALUE:
		break;
	case SQL_DESC_BASE_COLUMN_NAME:
		break;
	default:
		;
	}
	return nRetn;
#else
	return SQL_ERROR;
#endif
	EXCEPTION (hstmt);
	return SQL_ERROR;

	TRACELN ("SQLColAttribute");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLCopyDesc (SQLHDESC SourceDescHandle, SQLHDESC TargetDescHandle)
{
	TRACELN ("SQLCopyDesc");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLDataSources (SQLHENV EnvironmentHandle, SQLUSMALLINT Direction
, SQLCHAR *ServerName, SQLSMALLINT BufferLength1, SQLSMALLINT *NameLength1
, SQLCHAR *Description, SQLSMALLINT BufferLength2, SQLSMALLINT *NameLength2)
{
	TRACELN ("SQLDataSources");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType)
{
	TRACELN ("SQLEndTran");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFetchScroll(SQLHSTMT StatementHandle,
           SQLSMALLINT FetchOrientation, SQLINTEGER FetchOffset)
{
	TRACELN ("SQLFetchScroll");
	assert (false);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetCursorName(SQLHSTMT StatementHandle,
           SQLCHAR *CursorName, SQLSMALLINT BufferLength,
           SQLSMALLINT *NameLength)
{
	TRACELN ("SQLGetCursorName");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetData(SQLHSTMT hstmt,
           SQLUSMALLINT nCol, SQLSMALLINT nTyp,
           SQLPOINTER pVal, SQLINTEGER nLen,
           SQLINTEGER *pInd)
{
//	TRACELN ("SQLGetData");
	try {
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	return pStmt->SQLGetData (nCol, nTyp, pVal, nLen, pInd);
	EXCEPTION (hstmt);
	return SQL_ERROR;
//	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetDescField(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength,
           SQLINTEGER *StringLength)
{
	TRACELN ("SQLGetDescField");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetDescRec(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLCHAR *Name,
           SQLSMALLINT BufferLength, SQLSMALLINT *StringLength,
           SQLSMALLINT *Type, SQLSMALLINT *SubType, 
           SQLINTEGER *Length, SQLSMALLINT *Precision, 
           SQLSMALLINT *Scale, SQLSMALLINT *Nullable)
{
	TRACELN ("SQLGetDescRec");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLGetDiagField (SQLSMALLINT HandleType, SQLHANDLE Handle,
           SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
           SQLPOINTER DiagInfo, SQLSMALLINT BufferLength,
           SQLSMALLINT *StringLength)
{
	TRACELN ("SQLGetDiagField");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLGetDiagRec (SQLSMALLINT nHandleType, SQLHANDLE nHandle
, SQLSMALLINT nRecNumber, SQLCHAR * szSqlstate, SQLINTEGER *pnNativeError
, SQLCHAR * szMessageText, SQLSMALLINT nBufferLength, SQLSMALLINT * pnTextLength)
{
	TRACELN ("SQLGetDiagRec");
	return SQL_NO_DATA_FOUND;
	strcpy ((char*) szSqlstate, "00000");

	if (szMessageText)
		szMessageText[0] = 0;

    if (pnTextLength)
        *pnTextLength = 0;

	if (pnNativeError)
		*pnNativeError = 0;

	return SQL_NO_DATA_FOUND;

	assert (false);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetStmtOption(SQLHSTMT StatementHandle,
           SQLUSMALLINT Option, SQLPOINTER Value)
{
	TRACELN ("SQLGetStmtOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLParamData(SQLHSTMT StatementHandle,
           SQLPOINTER *Value)
{
	TRACELN ("SQLParamData");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLPutData(SQLHSTMT StatementHandle,
           SQLPOINTER Data, SQLINTEGER StrLen_or_Ind)
{
	TRACELN ("SQLPutData");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLRowCount(SQLHSTMT StatementHandle, 
	   SQLINTEGER *RowCount)
{
	TRACELN ("SQLRowCount");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLSetConnectAttr(SQLHDBC hdbc, SQLINTEGER nAttribute, SQLPOINTER pValue, SQLINTEGER nLength)
{
	TRACELN ("SQLSetConnectAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLSetConnectOption (SQLHDBC hdbc, SQLUSMALLINT nOption, SQLUINTEGER pValue)
{
	TRACELN ("SQLSetConnectOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetCursorName(SQLHSTMT StatementHandle,
           SQLCHAR *CursorName, SQLSMALLINT NameLength)
{
	TRACELN ("SQLSetCursorName");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetDescField(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength)
{
	TRACELN ("SQLSetDescField");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetDescRec(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT Type,
           SQLSMALLINT SubType, SQLINTEGER Length,
           SQLSMALLINT Precision, SQLSMALLINT Scale,
           SQLPOINTER Data, SQLINTEGER *StringLength,
           SQLINTEGER *Indicator)
{
	TRACELN ("SQLSetDescRec");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetParam(SQLHSTMT StatementHandle,
           SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
           SQLSMALLINT ParameterType, SQLUINTEGER LengthPrecision,
           SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue,
           SQLINTEGER *StrLen_or_Ind)
{
	TRACELN ("SQLSetParam");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetStmtAttr(SQLHSTMT StatementHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER StringLength)
{
	TRACELN ("SQLSetStmtAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetStmtOption(SQLHSTMT StatementHandle,
           SQLUSMALLINT Option, SQLUINTEGER Value)
{
	TRACELN ("SQLSetStmtOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLSpecialColumns(SQLHSTMT hstmt
, SQLUSMALLINT IdentifierType
, SQLCHAR *pCatalog, SQLSMALLINT nCatalog
, SQLCHAR *pSchema, SQLSMALLINT nSchema
, SQLCHAR *pTable, SQLSMALLINT nTable
, SQLUSMALLINT nScope, SQLUSMALLINT nNullable)
{
	TRY ("SQLSpecialColumns");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	return SQL_ERROR;
	EXCEPTION (hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLStatistics (SQLHSTMT hstmt
, SQLCHAR *pCatalog, SQLSMALLINT nCatalog
, SQLCHAR *pSchema, SQLSMALLINT nSchema
, SQLCHAR *pTable, SQLSMALLINT nTable
, SQLUSMALLINT nUnique, SQLUSMALLINT nReserved)
{
	TRACELN ("SQLStatistics");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	pStmt->clear();
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
// extern...
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLTransact(SQLHENV EnvironmentHandle,
           SQLHDBC ConnectionHandle, SQLUSMALLINT CompletionType)
{
	TRACELN ("SQLTransact");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLDriverConnect
(	SQLHDBC            hdbc
,	SQLHWND            hwnd
,	SQLCHAR 		  *szConnStrIn
,	SQLSMALLINT        cbConnStrIn
,	SQLCHAR           *szConnStrOut
,	SQLSMALLINT        cbConnStrOutMax
,	SQLSMALLINT 	  *pcbConnStrOut
,	SQLUSMALLINT       fDriverCompletion
)
{
	TRACELN ("SQLDriverConnect");
	CDatabase * pDatabase = static_cast<CDatabase*>(hdbc);
	if (!pDatabase)
		return SQL_INVALID_HANDLE;
	char szConnect[256];
	if (cbConnStrIn == SQL_NTS)
		strcpy (szConnect, (const char*)szConnStrIn);
	else
	{
		assert (cbConnStrIn > 0);
		assert (cbConnStrIn < sizeof(szConnect));
		memcpy (szConnect, szConnStrIn, cbConnStrIn);
		szConnect[cbConnStrIn] = 0;
	}
	SQLRETURN nState = SQL_SUCCESS;
	SQLWrite (szConnect, (char*)szConnStrOut, cbConnStrOutMax, pcbConnStrOut);
	return nState;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLBrowseConnect(SQLHDBC ConnectionHandle
, SQLCHAR *InConnectionString
, SQLSMALLINT StringLength1
, SQLCHAR *OutConnectionString
, SQLSMALLINT BufferLength
, SQLSMALLINT *StringLength2Ptr)
{
	TRACELN ("SQLBrowseConnect");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLFetchScroll (SQLHSTMT hstmt
, SQLUSMALLINT nFetchOrientation
, SQLINTEGER nFetchOffset)
{
	TRACELN ("SQLExtendedFetch");
	if (nFetchOrientation == SQL_FETCH_NEXT)
	{
		return SQLFetch (hstmt);
	}
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLExtendedFetch (SQLHSTMT hstmt
, SQLUSMALLINT nFetchOrientation
, SQLINTEGER nFetchOffset
, SQLUINTEGER *pRowCountPtr
, SQLUSMALLINT *pRowStatusArray)
{
	TRACELN ("SQLExtendedFetch");
	if (nFetchOrientation == SQL_FETCH_NEXT)
	{
		if (pRowCountPtr)
			*pRowCountPtr = 1;
		if (pRowStatusArray)
			pRowStatusArray[0] = SQL_ROW_SUCCESS;
		return SQLFetch (hstmt);
	}
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
::SQLTables (SQLHSTMT hstmt
, SQLCHAR *pCatalog, SQLSMALLINT nCatalog
, SQLCHAR *pSchema, SQLSMALLINT nSchema
, SQLCHAR *pTable, SQLSMALLINT nTable
, SQLCHAR *pType, SQLSMALLINT nType)
{
	TRY ("SQLTables");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	pStmt->SQLPrepare();
	string strCatalog = SQLString (pCatalog, nCatalog);
	string strSchema = SQLString (pSchema, nSchema);
	string strTable = SQLString (pTable, nTable);
	string strType = SQLString (pType, nType);
	return pStmt->SQLTables (strCatalog, strSchema, strTable, strType);
	EXCEPTION(hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLColumns(SQLHSTMT hstmt
, SQLCHAR *pCatalog, SQLSMALLINT nCatalog
, SQLCHAR *pSchema, SQLSMALLINT nSchema
, SQLCHAR *pTable, SQLSMALLINT nTable
, SQLCHAR *pColumn, SQLSMALLINT nColumn)
{
	TRY ("SQLColumns");
	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	pStmt->SQLPrepare();
	string strCatalog = SQLString (pCatalog, nCatalog);
	string strSchema = SQLString (pSchema, nSchema);
	string strTable = SQLString (pTable, nTable);
	string strColumn = SQLString (pColumn, nColumn);
	return pStmt->SQLColumns (strCatalog, strSchema, strTable, strColumn);
	EXCEPTION(hstmt);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
::SQLGetTypeInfo (SQLHSTMT hstmt, SQLSMALLINT nDataType)
{
	TRACELN ("SQLGetTypeInfo");
//	return SQL_NO_DATA;

	CStmtHandle * pStmt = static_cast<CStmtHandle*>(hstmt);
	if (!pStmt)
		return SQL_INVALID_HANDLE;
	pStmt->SQLPrepare();
	return pStmt->SQLGetTypeInfo (nDataType);

}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLForeignKeys (SQLHSTMT hstmt
, SQLCHAR *PKCatalogName
, SQLSMALLINT NameLength1
, SQLCHAR *PKSchemaName
, SQLSMALLINT NameLength2
, SQLCHAR *PKTableName
, SQLSMALLINT NameLength3
, SQLCHAR *FKCatalogName
, SQLSMALLINT NameLength4
, SQLCHAR *FKSchemaName
, SQLSMALLINT NameLength5
, SQLCHAR *FKTableName
, SQLSMALLINT NameLength6)
{
	TRACELN ("SQLForeignKeys");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
