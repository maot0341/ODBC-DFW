
#ifndef DATABASE_H
#define DATABASE_H

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "utils.h"

//---------------------------------------------------------------------------
#define SQLCHECK(ret,stmt,exc) SQLCheck (ret, stmt, exc, __FILE__, __LINE__)
void SQLCheck (SQLRETURN nRet, HSTMT hstmt, bool bExc, const char * szFile, int nLine);
//---------------------------------------------------------------------------
SQL_TIMESTAMP_STRUCT SQLTime (time_t t);
time_t SQLTime (const SQL_TIMESTAMP_STRUCT & aTime);
//---------------------------------------------------------------------------
class CDatabase
{
public:
	CDatabase();
	virtual ~CDatabase();

	static HENV GetHENV();

	void ExecuteSQL(LPCTSTR lpszSQL);

	virtual BOOL Open (LPCTSTR lpszDSN
	, BOOL bExclusive = FALSE
	, BOOL bReadOnly = FALSE
	, LPCTSTR lpszConnect = "ODBC;"
	, BOOL bUseCursorLib = TRUE);

	CString m_strConnect;
	static HENV ms_henv;
	static int  ms_ndbc;
	HDBC m_hdbc;
	bool m_bOpen;
};
//---------------------------------------------------------------------------
class CDBException
{
public:
	CDBException();
	CDBException (SQLSMALLINT type, SQLHANDLE h, const char * file=0, int line=0);

	SQLRETURN get (SQLSMALLINT type, SQLHANDLE h, const char * file=0, int line=0);

	static void trace (SQLSMALLINT type, SQLHANDLE h, const char * file=0, int line=0);
	void trace();

	char         m_szState[6];
	char         m_szText[8000];
	SQLINTEGER   m_nError;

	const char * m_szFile;
	int m_nLine;
};
//---------------------------------------------------------------------------
#endif