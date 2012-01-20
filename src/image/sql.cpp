/***************************************************************************************

    Projekt:    Hasy                                                     (C) 2000 PSI AG
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
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <commdlg.h>

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include "driver.h"
#include "sqlplib/sqlp.h"
#include "sqlplib/yac.h"


//---------------------------------------------------------------------------
// exception handling
//---------------------------------------------------------------------------
#define TRY(s) try { if (s) TRACE(s)
#define EXCEPTION(h) \
	} \
	catch (const CMessage & aError) \
	{ \
		if (h) ((CHandle*)h)->push (aError); \
	} \
	catch (const std::string & strError) \
	{ \
		if (h) ((CHandle*)h)->push(MSG("HY000", 0001, strError.c_str())); \
	} \
	catch (...) \
	{ \
		if (h) ((CHandle*)h)->push(MSG("HY000", 0001, "unkonw exception")); \
	} \
	return SQL_ERROR
//---------------------------------------------------------------------------
void trace (const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	OutputDebugString (szBuff);
//	TRACE (szBuff);
}
//---------------------------------------------------------------------------
// ODBC -- Funktionen
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLAllocHandle(SQLSMALLINT nType,
           SQLHANDLE hInput, SQLHANDLE * phOutput)
{
	TRACE ("SQLAllocHandle");
	if (!phOutput)
		return SQL_INVALID_HANDLE;
	if (nType == SQL_HANDLE_ENV)
		return SQLAllocEnv (phOutput);
	if (nType == SQL_HANDLE_DBC)
		return SQLAllocConnect (hInput, phOutput);
	if (nType == SQL_HANDLE_STMT)
		return SQLAllocStmt (hInput, phOutput);
	//ASSUME (false);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLAllocEnv (SQLHENV * phenv)
{
	TRACE ("SQLAllocEnv");
	CDriver * paDriver = CDriver::Instance();
	*phenv = (SQLHENV) new CEnvironment (paDriver);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLAllocConnect (SQLHENV henv, SQLHDBC* phdbc)
{
	TRACE ("SQLAllocConnect");
	CEnvironment * paEnv = (CEnvironment*)henv;
	if (!phdbc || !paEnv)
		return SQL_INVALID_HANDLE;
	CDatabase * paDatabase = new CDatabase (paEnv);
	assert (paDatabase);
	*phdbc = (SQLHDBC) paDatabase;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLAllocStmt (SQLHDBC hdbc, SQLHSTMT * phstmt)
{
	TRACE ("SQLAllocStmt");
	CDatabase * paDatabase = (CDatabase*)hdbc;
	if (!paDatabase)
		return SQL_INVALID_HANDLE;
	CStmtHandle * pStmt = new CStmtHandle (paDatabase);
	*phstmt = (SQLHSTMT) pStmt;
	return SQL_SUCCESS;
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
//	TRACE ("SQLBindCol");
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	return paStmt->bind (nCol, nTargetTyp, pTargetVal, nTargetLen, pnTargetInd);
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
	TRACE ("SQLBindParameter");
	CStmtQuery * paStmt = dynamic_cast<CStmtQuery*>(CStmtHandle::ptr (hstmt));
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	vector<CParam*> & aParams = paStmt->m_aStatement.m_aParam;
	const size_t i = nParameter - 1;
	assert (i < aParams.size());
	assert (nInputOutputType == SQL_PARAM_INPUT);
	aParams[i]->bind (nValueType, pParameterValuePtr, nBufferLength, pStrLen_or_IndPtr);

	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLNumParams (SQLHSTMT hstmt
, SQLSMALLINT * pNumber)
{
	TRACE ("SQLNumParams");
	CStmtQuery * paStmt = dynamic_cast<CStmtQuery*>(CStmtHandle::ptr (hstmt));
	if (!hstmt)
		return SQL_INVALID_HANDLE;
	if (!pNumber)
		return SQL_INVALID_HANDLE;
	if (paStmt)
		*pNumber = paStmt->m_aStatement.m_aParam.size();
	else
		*pNumber = 0;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLBindParam(SQLHSTMT hstmt
, SQLUSMALLINT nIndex, SQLSMALLINT nValueType
, SQLSMALLINT nParameterType
, SQLUINTEGER nLengthPrecision
, SQLSMALLINT nParameterScale
, SQLPOINTER pParameterValue
, SQLINTEGER *pStrLen_or_Ind)
{
	TRACE ("SQLBindParam");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLColumns(SQLHSTMT hstmt
, SQLCHAR *szCatalogName, SQLSMALLINT nNameLength1
, SQLCHAR *szSchemaName, SQLSMALLINT nNameLength2
, SQLCHAR *szTableName, SQLSMALLINT nNameLength3
, SQLCHAR *szColumnName, SQLSMALLINT nNameLength4)
{
	TRY ("SQLColumns");
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle & aStmt = CStmtHandle::get (hstmt);
//	aStmt = new CStmtColumns (&aStmt, strTable.c_str());
	CDatabase * paDatabase = dynamic_cast<CDatabase*> (aStmt.Database());
	assert (paDatabase);
	if (nNameLength3 == SQL_NTS)
		paDatabase->desc (&aStmt, (const char*)szTableName);
	else
	{
		std::string strTable ((const char*)szTableName, nNameLength3);
		paDatabase->desc (&aStmt, strTable.c_str());
	}
	return SQL_SUCCESS;
	EXCEPTION (hstmt);
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLConnect (SQLHDBC hdbc
, SQLCHAR * szServer, SQLSMALLINT nLen1
, SQLCHAR * szUser, SQLSMALLINT nLen2
, SQLCHAR * szPasswd, SQLSMALLINT nLen3
)
{
	TRACE ("SQLConnect");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLDescribeCol(SQLHSTMT hstmt,SQLUSMALLINT nColumnNumber
, SQLCHAR * szColumnName, SQLSMALLINT nBufferLength, SQLSMALLINT *pnNameLength
, SQLSMALLINT *pnDataType, SQLUINTEGER *pnColumnSize
, SQLSMALLINT *pnDecimalDigits, SQLSMALLINT *pnNullable)
{
	TRACE ("SQLDescribeCol");
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	const int nField = paStmt->fields();
	if ((--nColumnNumber) >= nField)
		return SQL_ERROR;
	const CField & aField = paStmt->fields (nColumnNumber);
	SetString (aField.pszSQL_DESC_NAME, szColumnName, nBufferLength, pnNameLength);
	if (pnDataType)
		*pnDataType = aField.nSQL_DESC_TYPE;
	if (pnColumnSize)
		*pnColumnSize = aField.nSQL_DESC_DISPLAY_SIZE;
	if (pnDecimalDigits)
		*pnDecimalDigits = 0;
	if (pnNullable)
		*pnNullable = aField.nSQL_DESC_NULLABLE;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLDisconnect (SQLHDBC hdbc)
{
	TRACE ("SQLDisconnect");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLError (SQLHENV henv, SQLHDBC hdbc, SQLHSTMT hstmt
, SQLCHAR *szSQL_State, SQLINTEGER *pnNativeError
, SQLCHAR *szMessageText, SQLSMALLINT nBufferLength, SQLSMALLINT *pnTextLength)
{
	TRACE ("SQLError");
//	return SQL_NO_DATA_FOUND;
	CMessage * paMessage = 0;
	if (!paMessage && hstmt)
		paMessage = ((CStmtHandle*)hstmt)->pop();
	if (!paMessage && hdbc)
		paMessage = ((CDatabase*)hdbc)->pop();
	if (!paMessage)
		return SQL_NO_DATA_FOUND;
	const CMessage & aMessage = *paMessage;
	if (pnNativeError)
		*pnNativeError = aMessage.nId;
	if (pnTextLength)
		*pnTextLength = 0;
	if (szMessageText && nBufferLength>1)
	{
		memset (szMessageText, 0, nBufferLength);
		char * szText = (char*)szMessageText;
		if (aMessage.szFile)
		sprintf (szText, "ODBC-Error %05ld: %s\n\n[%5s]: %s[%ld]"
		, (long)aMessage.nId
		, aMessage.strText.c_str()
		, aMessage.szState
		, aMessage.szFile
		, (long)aMessage.nLine);
		else
		sprintf (szText, "ODBC-Error %05ld: %s"
		, (long)aMessage.nId
		, aMessage.strText.c_str());
		//strncpy ((char*)szMessageText, aMessage.strText.c_str(), nBufferLength-1);
		if (pnTextLength)
			*pnTextLength = strlen ((char*)szMessageText);
	}
	if (szSQL_State)
		strncpy ((char*)szSQL_State, paMessage->szState, 6);
	delete paMessage;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLExecDirect (SQLHSTMT hstmt, SQLCHAR *szSQL, SQLINTEGER nLength)
{
	TRACE ("SQLExecDirect");
	SQLRETURN nResult = SQLPrepare (hstmt, szSQL, nLength);
	if (nResult != SQL_SUCCESS)
		return nResult;
	return SQLExecute (hstmt);
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLExecute (SQLHSTMT hstmt)
{
	TRY ("SQLExecute");
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle & aStmt = *(CStmtHandle*) hstmt;
	aStmt->exec();
//	aStmt = new CStmtTest (&aStmt);
	return SQL_SUCCESS;
	EXCEPTION (hstmt);
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFetch (SQLHSTMT hstmt)
{
//	TRACE ("SQLFetch");
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	return paStmt->fetch();
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFreeConnect (SQLHENV hdbc)
{
	TRACE ("SQLFreeConnect");
	CDatabase * paDatabase = (CDatabase*)hdbc;
	if (!paDatabase)
		return SQL_INVALID_HANDLE;
	delete paDatabase;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFreeEnv (SQLHENV henv)
{
	TRACE ("SQLFreeEnv");
	return SQL_SUCCESS;

	CDriver * paDriver = (CDriver*)henv;
	if (!paDriver)
		return SQL_INVALID_HANDLE;
	delete paDriver;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFreeStmt (SQLHSTMT hstmt, SQLUSMALLINT nOption)
{
	const char * szOption
	= nOption == SQL_DROP ? "DROP"
	: nOption == SQL_CLOSE ? "CLOSE"
	: nOption == SQL_UNBIND ? "UNBIND"
	: nOption == SQL_RESET_PARAMS ? "RESET"
	: "unknown";
	trace ("0x%0x SQLFreeStmt %s\n", hstmt, szOption);
	//TRACE ("SQLFreeStmt");
	if (nOption == SQL_DROP)
	{
		CStmtHandle * paStmt = (CStmtHandle*) hstmt;
		delete paStmt;
		return SQL_SUCCESS;
	}
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_SUCCESS;
	if (nOption == SQL_CLOSE)
		return paStmt->close();
	if (nOption == SQL_UNBIND)
		return paStmt->unbind();
	if (nOption == SQL_RESET_PARAMS)
		return SQL_SUCCESS;
	assert (false);
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLGetConnectAttr (SQLHDBC ConnectionHandle,
           SQLINTEGER Attribute, SQLPOINTER pValue,
           SQLINTEGER BufferLength, SQLINTEGER *pnStringLength)
{
	TRACE ("SQLGetConnectAttr");
	assert (pValue);
	assert (pnStringLength);
	*(char*)pValue = 0;
	*pnStringLength = 0;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLGetConnectOption (SQLHDBC hdbc, SQLUSMALLINT nOption, SQLPOINTER pValue)
{
	TRACE ("SQLGetConnectOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLGetStmtAttr (SQLHSTMT hstmt
, SQLINTEGER nAttribute, SQLPOINTER pValue
, SQLINTEGER nBufferLength, SQLINTEGER *pnStringLength)
{
	TRACE ("SQLGetStmtAttr");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLGetTypeInfo (SQLHSTMT hstmt, SQLSMALLINT nDataType)
{
	TRACE ("SQLGetTypeInfo");
//	return SQL_NO_DATA_FOUND;
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle * paStmt = (CStmtHandle*) hstmt;
	(*paStmt) = new CStmtTypeInfo (paStmt, nDataType);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLNumResultCols (SQLHSTMT hstmt, SQLSMALLINT *pCols)
{
	TRACE ("SQLNumResultCols");
	assert (pCols);
	if (!hstmt)
		return SQL_INVALID_HANDLE;
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	*pCols = paStmt->fields();
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLSetEnvAttr (SQLHENV EnvironmentHandle
, SQLINTEGER nAttribute
, SQLPOINTER pValue
, SQLINTEGER nStringLength)
{
	TRACE ("SQLSetEnvAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLGetEnvAttr(SQLHENV EnvironmentHandle
, SQLINTEGER Attribute, SQLPOINTER Value
, SQLINTEGER BufferLength, SQLINTEGER *StringLength)
{
	TRACE ("SQLGetEnvAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLCancel (SQLHSTMT hstmt)
{
	TRACE ("SQLCancel");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API
SQLCloseCursor(SQLHSTMT StatementHandle)
{
	TRACE ("SQLCloseCursor");
	return SQL_SUCCESS;
}
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
	TRACE ("SQLColAttribute");
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	iCol--;
	const int nField = paStmt->fields();
	assert (0 <= iCol && iCol < nField);
	const CField & aField = paStmt->fields(iCol);
	if (nDescType == SQL_COLUMN_DISPLAY_SIZE)
	{
		if (pNumVal)
			*((SQLINTEGER*)pNumVal) = aField.nSQL_DESC_DISPLAY_SIZE;
		return SQL_SUCCESS;
	}
	if (nDescType == SQL_COLUMN_UNSIGNED)
	{
		if (pNumVal)
			*((SQLINTEGER*)pNumVal) = aField.bSQL_DESC_UNSIGNED;
		return SQL_SUCCESS;
	}
	return SQL_ERROR;
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
	TRACE ("SQLColAttributes");
	CStmtData * paStmt = CStmtHandle::ptr (hstmt);
	if (!paStmt)
		return SQL_INVALID_HANDLE;
	iCol--;
	const int nField = paStmt->fields();
	assert (0 <= iCol && iCol < nField);
	const CField & aField = paStmt->fields(iCol);
	if (nDescType == SQL_COLUMN_DISPLAY_SIZE)
	{
		if (pcbDesc)
			*pcbDesc = sizeof(aField.nSQL_DESC_DISPLAY_SIZE);
		if (pfDesc)
			*pfDesc = aField.nSQL_DESC_DISPLAY_SIZE;
		return SQL_SUCCESS;
	}
	if (nDescType == SQL_COLUMN_UNSIGNED)
	{
		if (pcbDesc)
			*pcbDesc = sizeof(aField.bSQL_DESC_UNSIGNED);
		if (pfDesc)
			*pfDesc = aField.bSQL_DESC_UNSIGNED;
		return SQL_SUCCESS;
	}
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLCopyDesc (SQLHDESC SourceDescHandle, SQLHDESC TargetDescHandle)
{
	TRACE ("SQLCopyDesc");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLDataSources (SQLHENV EnvironmentHandle, SQLUSMALLINT Direction
, SQLCHAR *ServerName, SQLSMALLINT BufferLength1, SQLSMALLINT *NameLength1
, SQLCHAR *Description, SQLSMALLINT BufferLength2, SQLSMALLINT *NameLength2)
{
	TRACE ("SQLDataSources");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLEndTran(SQLSMALLINT HandleType, SQLHANDLE Handle, SQLSMALLINT CompletionType)
{
	TRACE ("SQLEndTran");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLFetchScroll(SQLHSTMT StatementHandle,
           SQLSMALLINT FetchOrientation, SQLINTEGER FetchOffset)
{
	TRACE ("SQLFetchScroll");
	assert (false);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetCursorName(SQLHSTMT StatementHandle,
           SQLCHAR *CursorName, SQLSMALLINT BufferLength,
           SQLSMALLINT *NameLength)
{
	TRACE ("SQLGetCursorName");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetData(SQLHSTMT hstmt,
           SQLUSMALLINT nCol, SQLSMALLINT nTyp,
           SQLPOINTER pVal, SQLINTEGER nLen,
           SQLINTEGER *pInd)
{
//	TRACE ("SQLGetData");
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle & aStmt = CStmtHandle::get(hstmt);
	return aStmt->getData (nCol, nTyp, pVal, nLen, pInd);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLGetDescField(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength,
           SQLINTEGER *StringLength)
{
	TRACE ("SQLGetDescField");
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
	TRACE ("SQLGetDescRec");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLGetDiagField (SQLSMALLINT HandleType, SQLHANDLE Handle,
           SQLSMALLINT RecNumber, SQLSMALLINT DiagIdentifier,
           SQLPOINTER DiagInfo, SQLSMALLINT BufferLength,
           SQLSMALLINT *StringLength)
{
	TRACE ("SQLGetDiagField");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLGetDiagRec (SQLSMALLINT nHandleType, SQLHANDLE nHandle
, SQLSMALLINT nRecNumber, SQLCHAR * szSqlstate, SQLINTEGER *pnNativeError
, SQLCHAR * szMessageText, SQLSMALLINT nBufferLength, SQLSMALLINT * pnTextLength)
{
	TRACE ("SQLGetDiagRec");
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
	TRACE ("SQLGetStmtOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLParamData(SQLHSTMT StatementHandle,
           SQLPOINTER *Value)
{
	TRACE ("SQLParamData");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLPrepare (SQLHSTMT hstmt, SQLCHAR *szSQL, SQLINTEGER nLength)
{
	TRY ("SQLPrepare");

	CStmtHandle & aStmt = *(CStmtHandle*)hstmt;
	CStmtQuery * pQuery = new CStmtQuery(&aStmt);
	aStmt = pQuery;

	CStatement * pStmt = &pQuery->m_aStatement;
	pStmt->m_pDatabase = aStmt.Database();
	pStmt->setSQL ((const char*)szSQL, nLength);

	yyinit (pStmt,0);
//	sqlp::yydebug=1;
	int n = yyparse();
	if (n != 0)
		throw MSG("42000", 900, "Syntax error or access violation");
	n = pStmt->m_aSelection.size();
	pStmt->prepare();
	pQuery->m_aFields.resize(n);
	int i;
	for (i=0; i<n; i++)
	{
		CTerm * pTerm = pStmt->m_aSelection[i];
		assert (pTerm);
		string strTerm = id(pTerm);
		term_t eType = pTerm->type();
		int nType = CDriver::sql_type (eType);
		int nSize = CField::displaysize (nType);
		assert (nType != SQL_UNKNOWN_TYPE);
		CField & raField = pQuery->m_aFields[i];
		raField.pszSQL_DESC_NAME = strdup (strTerm.c_str());
		raField.nSQL_DESC_TYPE = nType;

		const CStmtTypeInfo::record_t * pTypeInfo = CStmtTypeInfo::at (nType);
		if (pTypeInfo)
			raField.nSQL_DESC_DISPLAY_SIZE = pTypeInfo->columnsize;
		else
			raField.nSQL_DESC_DISPLAY_SIZE = nSize;
	}
	return SQL_SUCCESS;
	EXCEPTION (hstmt);
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLPutData(SQLHSTMT StatementHandle,
           SQLPOINTER Data, SQLINTEGER StrLen_or_Ind)
{
	TRACE ("SQLPutData");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLRowCount(SQLHSTMT StatementHandle, 
	   SQLINTEGER *RowCount)
{
	TRACE ("SQLRowCount");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLSetConnectAttr(SQLHDBC hdbc, SQLINTEGER nAttribute, SQLPOINTER pValue, SQLINTEGER nLength)
{
	TRACE ("SQLSetConnectAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLSetConnectOption (SQLHDBC hdbc, SQLUSMALLINT nOption, SQLUINTEGER pValue)
{
	TRACE ("SQLSetConnectOption");
	if (nOption == SQL_ATTR_CURRENT_CATALOG)
		return SQL_SUCCESS;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetCursorName(SQLHSTMT StatementHandle,
           SQLCHAR *CursorName, SQLSMALLINT NameLength)
{
	TRACE ("SQLSetCursorName");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetDescField(SQLHDESC DescriptorHandle,
           SQLSMALLINT RecNumber, SQLSMALLINT FieldIdentifier,
           SQLPOINTER Value, SQLINTEGER BufferLength)
{
	TRACE ("SQLSetDescField");
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
	TRACE ("SQLSetDescRec");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetParam(SQLHSTMT StatementHandle,
           SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
           SQLSMALLINT ParameterType, SQLUINTEGER LengthPrecision,
           SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue,
           SQLINTEGER *StrLen_or_Ind)
{
	TRACE ("SQLSetParam");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetStmtAttr(SQLHSTMT StatementHandle,
           SQLINTEGER Attribute, SQLPOINTER Value,
           SQLINTEGER StringLength)
{
	TRACE ("SQLSetStmtAttr");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLSetStmtOption(SQLHSTMT StatementHandle,
           SQLUSMALLINT Option, SQLUINTEGER Value)
{
	TRACE ("SQLSetStmtOption");
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLSpecialColumns(SQLHSTMT hstmt
, SQLUSMALLINT IdentifierType
, SQLCHAR *CatalogName, SQLSMALLINT NameLength1
, SQLCHAR *SchemaName, SQLSMALLINT NameLength2
, SQLCHAR *szTableName, SQLSMALLINT nNameLength3
, SQLUSMALLINT Scope, SQLUSMALLINT Nullable)
{
	TRACE ("SQLSpecialColumns");
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle & aStmt = *(CStmtHandle*)hstmt;
	if (nNameLength3 == SQL_NTS)
		aStmt = new CStmtSpecialColumns (&aStmt, (const char*)szTableName);
	else
	{
		std::string strTable ((const char*)szTableName, nNameLength3);
		aStmt = new CStmtSpecialColumns (&aStmt, strTable.c_str());
	}
	
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API 
SQLStatistics (SQLHSTMT hstmt
, SQLCHAR *CatalogName, SQLSMALLINT NameLength1
, SQLCHAR *SchemaName, SQLSMALLINT NameLength2
, SQLCHAR *TableName, SQLSMALLINT NameLength3
, SQLUSMALLINT Unique, SQLUSMALLINT Reserved)
{
	TRACE ("SQLStatistics");
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle & aStmt = *(CStmtHandle*)hstmt;
	aStmt = new CStmtStatistics (&aStmt);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLTables (SQLHSTMT hstmt
, SQLCHAR *CatalogName, SQLSMALLINT NameLength1
, SQLCHAR *SchemaName, SQLSMALLINT NameLength2
, SQLCHAR *TableName, SQLSMALLINT NameLength3
, SQLCHAR *TableType, SQLSMALLINT NameLength4)
{
	TRACE ("SQLTables");
	if (hstmt == SQL_NULL_HANDLE)
		return SQL_INVALID_HANDLE;
	CStmtHandle & aStmt = *(CStmtHandle*)hstmt;
	aStmt = new CStmtTables (&aStmt);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN  SQL_API SQLTransact(SQLHENV EnvironmentHandle,
           SQLHDBC ConnectionHandle, SQLUSMALLINT CompletionType)
{
	TRACE ("SQLTransact");
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
	TRACE ("SQLDriverConnect");
	CDatabase * pHandle = (CDatabase*) (hdbc);
	CDatabase * pDatabase = dynamic_cast<CDatabase*>(pHandle);
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
	const char * szAttr = strstr (szConnect, "PATH=");
	if (szAttr)
	{
		for (szAttr+=5; isspace(*szAttr); szAttr++);
		char * psz = strchr(szAttr, ';');
		if (psz)
			*psz = 0;
	}
	if (false)
	if (fDriverCompletion != SQL_DRIVER_NOPROMPT)
	{
		char szFile[256];
		OPENFILENAME aDlg;
		memset (&aDlg, 0, sizeof(aDlg));
		memset (szFile,0,sizeof(szFile));
		aDlg.lStructSize = sizeof(aDlg);
		aDlg.hwndOwner = hwnd;
		aDlg.hInstance = 0;
		aDlg.lpstrFilter = "Prognose (*.dat)\0*.dat\0";
//		aDlg.lpstrCustomFilter = "Prognose\0*.dat\0\0";
		aDlg.lpstrCustomFilter = 0;
		aDlg.nMaxCustFilter = 256;
		aDlg.nFilterIndex = 0;
		aDlg.lpstrFile = szFile;
		aDlg.nMaxFile = sizeof(szFile);
		aDlg.lpstrFileTitle = 0;
		aDlg.nMaxFileTitle = 0;
		aDlg.lpstrInitialDir = "";
		aDlg.lpstrTitle = "Prognose-Dateien";
		aDlg.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
		if (!GetOpenFileName (&aDlg))
			return SQL_NO_DATA;
		if (aDlg.nFileOffset < 1)
			return SQL_NO_DATA;
//		pDatabase->m_strPath.assign (szFile, aDlg.nFileOffset-1);
	}
	char * szDSN = strchr (szConnect, ';');
//	sprintf (szDSN, ";%PATH=%s", pDatabase->m_strPath.c_str());
	SetString (szConnect, (char*)szConnStrOut, cbConnStrOutMax, pcbConnStrOut);
	return SQL_SUCCESS;

	if (szConnStrOut)
		strncpy ((char*)szConnStrOut, (char*)szConnStrIn, cbConnStrOutMax);
	if (szConnStrOut && pcbConnStrOut)
		*pcbConnStrOut = strlen ((char*)szConnStrOut);
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
	TRACE ("SQLBrowseConnect");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API 
SQLFetchScroll (SQLHSTMT hstmt
, SQLUSMALLINT nFetchOrientation
, SQLINTEGER nFetchOffset)
{
	TRACE ("SQLExtendedFetch");
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
	TRACE ("SQLExtendedFetch");
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
	TRACE ("SQLForeignKeys");
	return SQL_ERROR;
}
//---------------------------------------------------------------------------
BOOL SQL_API
ConfigDSN (HWND hWnd, WORD dRequest, LPCSTR  szDriver, LPCSTR szAttr)
{
	TRACE ("ConfigDSN");
	MessageBox (hWnd, "ODBC Hallo World.", "ODBC - ConfigDSN", MB_OK);
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL SQL_API
ConfigDriver (HWND     hwnd
, WORD     fRequest
, LPCSTR     lpszDriver
, LPCSTR     lpszArgs
, LPSTR     lpszMsg
, WORD     cbMsgMax
, WORD *     pcbMsgOut)
{
	TRACE ("ConfigDriver");
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL SQL_API 
ConfigTranslator (HWND hwnd, DWORD * pvOption)
{
	TRACE ("ConfigTranslator");
	return TRUE;
}
//---------------------------------------------------------------------------
/*
BOOL SQL_API 
SQLConfigDataSource(
     HWND      hwndParent,
     WORD      fRequest,
     LPCSTR      lpszDriver,
     LPCSTR      lpszAttributes)
{
	TRACE ("SQLConfigDataSource");
	return TRUE;
}
#endif
//---------------------------------------------------------------------------
BOOL SQL_API 
SQLConfigDriver(
     HWND      hwndParent,
     WORD      fRequest,
     LPCSTR      lpszDriver,
     LPCSTR      lpszArgs,
     LPSTR      lpszMsg,
     WORD      cbMsgMax,
     WORD *      pcbMsgOut)
{
	TRACE ("SQLConfigDriver");
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL SQL_API 
SQLCreateDataSource(
     HWND     hwnd,
     LPSTR     lpszDS)
{
	TRACE ("SQLCreateDataSource");
	return TRUE;
}
*/
//---------------------------------------------------------------------------
