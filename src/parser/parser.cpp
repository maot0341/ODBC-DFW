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

static const char* id_tp_pro_tprodump_cpp = "@(#) $Header: /gow/hasy/cvs/src/tp/tls/prodump/tpprodump.cpp,v 1.4 2005/08/19 10:43:18 hasyjv Exp $";

#include <vld.h>
#include <stdio.h>
#include <crtdbg.h>
#include <sqlplib/sqlp.h>
#include <sqlplib/yac.h>
#include "database.h"

using namespace std;
using namespace sqlp;

//---------------------------------------------------------------------------
// main
int main(void)
{
	int n = 1;
	CDatabase  * pDatabase = CDatabase::Instance();
	CStatement aStmt(pDatabase);

	//sqlp::m_stmt = &aStmt;
	yyinit (&aStmt, stdin);
	puts ("SQL>");
	n = yyparse();
	while (n)
	try
	{
		n = yyparse();
	}
	catch (const CException & aError)
	{
		printf ("ODBC-Error [%s]: %ld %s\n%s[%ld]\n"
		, aError.szState
		, aError.nId
		, aError.strText.c_str()
		, aError.szFile
		, aError.nLine);
	}
	catch (const std::string & strError)
	{
		printf ("ODBC-Error [HY000]: 0001 %s", strError.c_str());
	}
	catch (...)
	{
		printf ("ODBC-Error [HY000]: 0001 unknown exception");
	}
	// Send all reports to STDOUT
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	_CrtCheckMemory();
	_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
	//SET_CRT_DEBUG_FIELD( _CRTDBG_LEAK_CHECK_DF );
	return n;
}
//---------------------------------------------------------------------------
/*
int main(int argc, char ** argv)
{
	SQLCHAR* szServer = (SQLCHAR*)"hasy.world";
	SQLCHAR* szUser = (SQLCHAR*)"tpm";
	SQLCHAR* szPasswd = (SQLCHAR*)"tpm";
	SQLRETURN rc;
	HENV henv;
	HDBC hdbc;
	HSTMT hstmt;
	char szText1[200];
	long nText1;

	rc = SQLAllocEnv (&henv);
	rc = SQLAllocConnect (henv, &hdbc);
	rc = SQLConnect (hdbc, szServer, SQL_NTS, szUser, SQL_NTS, szPasswd, SQL_NTS);
	rc = SQLAllocStmt (hdbc, &hstmt);
	rc = SQLBindCol (hstmt, 0, SQL_CHAR, szText1, sizeof(szText1), &nText1);
	while ((rc = SQLFetch(hstmt)) == SQL_SUCCESS)
	{
		printf ("now %s\n", szText1);
	}
	rc = SQLFreeStmt (hstmt, SQL_DROP);
	rc = SQLDisconnect (hdbc);
	rc = SQLFreeConnect (hdbc);
	rc = SQLFreeEnv (henv);

	return 0;
}
*/
//---------------------------------------------------------------------------

