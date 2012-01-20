#include <assert.h>
#include <tchar.h>
#include "database.h"
#include "utils.h"

//---------------------------------------------------------------------------
// CET to SQL-Timestamp
//---------------------------------------------------------------------------
SQL_TIMESTAMP_STRUCT SQLTime (time_t t)
{
	SQL_TIMESTAMP_STRUCT aTime;
	t += 3600; // CET
	struct tm * pTime = gmtime (&t);
	aTime.year = pTime->tm_year + 1900;
	aTime.month = pTime->tm_mon + 1;
	aTime.day = pTime->tm_mday;
	aTime.hour = pTime->tm_hour;
	aTime.minute = pTime->tm_min;
	aTime.second = pTime->tm_sec;
	aTime.fraction = 0;
	return aTime;
}
//---------------------------------------------------------------------------
time_t SQLTime (const SQL_TIMESTAMP_STRUCT & aTime)
{
	struct tm trec;
	memset (&trec, 0, sizeof(trec));
	trec.tm_sec = aTime.second;
	trec.tm_min = aTime.minute;
	trec.tm_hour = aTime.hour;
	trec.tm_mday = aTime.day;
	trec.tm_mon = aTime.month - 1;
	trec.tm_year = aTime.year - 1900;
	time_t t = mktime (&trec);
	return t;
}
//---------------------------------------------------------------------------
void SQLCheck (SQLRETURN nRet, HSTMT hstmt, bool bExc, const char * szFile, int nLine)
{
	if (nRet == SQL_SUCCESS)
		return;
	char szState[6];
	char szMsg[8000];
	CString strMsg;
	strMsg.clear (8000);
	SQLSMALLINT nMsg = sizeof (szMsg) - 1;
	SQLSMALLINT nLen = 0;
	SQLINTEGER nError = 0;
	memset (szState, 0, sizeof(szState));
	memset (szMsg, 0, sizeof(szMsg));
	szMsg [nLen] = 0;
	SQLRETURN nDbg = SQLError (NULL, NULL, hstmt, (SQLCHAR*)szState, &nError, (SQLCHAR*)szMsg, nMsg, &nLen);
	if (nRet == SQL_ERROR)
	{
		strMsg.Format ("ODBC:Error[%d-%s]  %s\nODBC [%s]:%d\n", nError, szState, szMsg, szFile, nLine);
		OutputDebugString (strMsg);
		if (!bExc)
			return;
		throw strMsg;
	}
	if (nRet == SQL_SUCCESS_WITH_INFO)
		strMsg.Format ("[%d-%s]  %s\nODBC [%s]:%d\n", nError, szState, szMsg, szFile, nLine);
	if (nRet == SQL_NO_DATA)
		strMsg.Format ("ODBC:NO DATA[%d-%s]  %s\nODBC [%s]:%d\n", nError, szState, szMsg, szFile, nLine);
	OutputDebugString (strMsg);
}
//---------------------------------------------------------------------------
// CDatabase
//---------------------------------------------------------------------------
HENV CDatabase::ms_henv = 0;
int  CDatabase::ms_ndbc = 0;
//---------------------------------------------------------------------------
CDatabase::CDatabase()
{
	ms_ndbc++;
	SQLRETURN nRet;
	HENV henv = GetHENV();
	nRet = SQLSetEnvAttr (henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_NTS);
	nRet = SQLAllocConnect (henv, &m_hdbc);
	m_bOpen = false;
}
//---------------------------------------------------------------------------
CDatabase::~CDatabase()
{
	SQLRETURN nRet;
	nRet = SQLDisconnect (m_hdbc);
	nRet = SQLFreeConnect (m_hdbc);
	m_hdbc = 0;
	ms_ndbc--;
	if (!ms_ndbc)
	{
		nRet = SQLFreeEnv (ms_henv);
		ms_henv = 0;
	}
}
//---------------------------------------------------------------------------
HENV CDatabase::GetHENV()
{
	SQLRETURN nRet;
	if (!ms_henv)
	{
		nRet = SQLAllocEnv (&ms_henv);
		nRet = SQLSetEnvAttr (ms_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, SQL_NTS);
	}
	return ms_henv;
}
//---------------------------------------------------------------------------
void CDatabase::ExecuteSQL (LPCTSTR szSQL)
{
	SQLRETURN nRet;
	HSTMT hstmt;
	nRet = SQLAllocStmt (m_hdbc, &hstmt);
	nRet = SQLExecDirect (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
}
//---------------------------------------------------------------------------
BOOL CDatabase::Open (LPCTSTR szDSN
, BOOL bExclusive
, BOOL bReadOnly
, LPCTSTR szConnect
, BOOL bUseCursorLib)
{
	HWND hWnd = ::GetDesktopWindow();
	SQLCHAR szOut[8192];
	const SQLSMALLINT nOut = sizeof(szOut);
	const char * szODBCTrail = "ODBC;";
	SQLSMALLINT iOut;
	memset (szOut, 0, nOut);
	if (false)
	if (0 == strnicmp (szConnect, szODBCTrail, strlen(szODBCTrail)))
		szConnect += strlen(szODBCTrail);
	SQLRETURN nRet;
//	nRet = SQLSetConnectOption(m_hdbc, SQL_ODBC_CURSORS, SQL_CUR_USE_ODBC);
//	nRet = SQLSetConnectOption(m_hdbc, SQL_ACCESS_MODE, SQL_MODE_READ_ONLY);
	nRet = SQLDriverConnect (m_hdbc
	, hWnd
	, (SQLCHAR*)szDSN
	, SQL_NTS
	, szOut
	, nOut
	, &iOut
	, SQL_DRIVER_COMPLETE);
	m_bOpen = (nRet == SQL_SUCCESS) || (nRet == SQL_SUCCESS_WITH_INFO);
	m_strConnect = (const char *)szOut;
	return nRet;
}
//---------------------------------------------------------------------------
// DB-Eexecption
//---------------------------------------------------------------------------
CDBException::CDBException()
{
	memset (m_szState, 0, sizeof(m_szState));
	m_szText[0] = 0;
	m_nError = 0;
	m_szFile = 0;
	m_nLine = 0;
}
//---------------------------------------------------------------------------
CDBException::CDBException (SQLSMALLINT t, SQLHANDLE h, const char * szFile, int nLine)
{
	memset (m_szState, 0, sizeof(m_szState));
	m_szText[0] = 0;
	m_nError = 0;
	m_szFile = 0;
	m_nLine = 0;
	get (t, h, szFile, nLine);
}
//---------------------------------------------------------------------------
SQLRETURN 
CDBException::get (SQLSMALLINT t, SQLHANDLE h, const char * szFile, int nLine)
{
	static const SQLSMALLINT nText = sizeof(m_szText) - 1;
	SQLRETURN nDbg;
	SQLSMALLINT i;
	switch (t)
	{
	case SQL_HANDLE_ENV:
		nDbg = SQLError (h, NULL, NULL, (SQLCHAR*)m_szState, &m_nError, (SQLCHAR*)m_szText, nText, &i);
		break;
	case SQL_HANDLE_DBC:
		nDbg = SQLError (NULL, h, NULL, (SQLCHAR*)m_szState, &m_nError, (SQLCHAR*)m_szText, nText, &i);
		break;
	case SQL_HANDLE_STMT:
		nDbg = SQLError (NULL, NULL, h, (SQLCHAR*)m_szState, &m_nError, (SQLCHAR*)m_szText, nText, &i);
		break;
	default:
		assert (false);
	}
	m_szText[i] = 0;
	return nDbg;
}
//---------------------------------------------------------------------------
void
CDBException::trace()
{
	char szText[8000];
	sprintf (szText, "ODBC: [#%d %s]  %s\n", m_nError, m_szState, m_szText);
	OutputDebugString (szText);
	if (!m_szFile)
		return;
	sprintf (szText, "ODBC [%s]:%d\n", m_szFile, m_nLine);
	OutputDebugString (szText);
}
//---------------------------------------------------------------------------
void
CDBException::trace (SQLSMALLINT t, SQLHANDLE h, const char * szFile, int nLine)
{
	CDBException(t, h, szFile, nLine).trace();
}
//---------------------------------------------------------------------------
