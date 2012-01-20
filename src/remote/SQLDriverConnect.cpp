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

#include "driver.h"
#include "resource.h"
#include "CHandle.h"
#include <stdx/debug.h>
#include <windows.h>
#include <sqlext.h>
#include <sql.h>

//---------------------------------------------------------------------------
BOOL CALLBACK DialogProc (HWND hWnd, UINT nMessage, WPARAM wParam, LPARAM lParam);
CLogin ms_aLogin;
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
//	CLogin aLogin ("localhost:5000", "hasyjv", "Superuse");
	CLogin aLogin (szConnect);
	if (aLogin == ms_aLogin)
	if (pDatabase->connected())
		return SQL_SUCCESS;
	if ((fDriverCompletion == SQL_DRIVER_PROMPT)
	||  (fDriverCompletion == SQL_DRIVER_COMPLETE)
	||  (fDriverCompletion == SQL_DRIVER_COMPLETE_REQUIRED))
	{
		ms_aLogin = aLogin;
		if (!*ms_aLogin.m_szHost)
			strcpy (ms_aLogin.m_szHost, "localhost:5000");
		strcpy (ms_aLogin.m_szUser, "hasyjv");
		strcpy (ms_aLogin.m_szPassword, "Superuse");
		HINSTANCE hInst = CDriver::ms_hInstance;
		int nBtn = DialogBox (hInst, MAKEINTRESOURCE (IDD_DIALOG_CONNECT), hwnd, DialogProc);
		if (nBtn == IDOK)
	//		wsprintf (szConnect, "DNS=ODBC Test;HOST=%s;UID=%s;PWD=%s;DRIVER={ODBC Test}", ms_aLogin.m_szHost, ms_aLogin.m_szUser, ms_aLogin.m_szPassword);
			wsprintf (szConnect, "DSN=%s;HOST=%s;UID=%s;PWD=%s;"
			, ms_aLogin.m_szDSN, ms_aLogin.m_szHost, ms_aLogin.m_szUser, ms_aLogin.m_szPassword);
		else
			return SQL_NO_DATA;
		aLogin = ms_aLogin;
	}
//	SQLRETURN nState = pDatabase->connect ("db=localhost:5000;usr=hasyjv;pwd=Superuse;display=gontpc47");
	SQLRETURN nState = pDatabase->connect (aLogin);
	if (nState == SQL_SUCCESS || nState == SQL_SUCCESS_WITH_INFO)
		SQLWrite (szConnect, (char*)szConnStrOut, cbConnStrOutMax, pcbConnStrOut);
//	SQLWrite ((char*)szConnStrIn, (char*)szConnStrOut, cbConnStrOutMax, pcbConnStrOut);
	return nState;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
BOOL CALLBACK DialogProc (HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	char * szHost = ms_aLogin.m_szHost;
	char * szUsr = ms_aLogin.m_szUser;
	char * szPwd = ms_aLogin.m_szPassword;
	const size_t nHost = sizeof (ms_aLogin.m_szHost);
	const size_t nUsr = sizeof (ms_aLogin.m_szUser);
	const size_t nPwd = sizeof (ms_aLogin.m_szPassword);
    switch (nMsg)
    {
    case WM_INITDIALOG:
	{
		SetDlgItemText (hwnd, IDC_EDIT_HOST, szHost);
		SetDlgItemText (hwnd, IDC_EDIT_USR, szUsr);
		SetDlgItemText (hwnd, IDC_EDIT_PWD, szPwd);
        return TRUE;
	}
    case WM_COMMAND:
		switch (LOWORD(wParam)) 
		{
		case IDOK:
			GetDlgItemText (hwnd, IDC_EDIT_HOST, szHost, nHost);
			GetDlgItemText (hwnd, IDC_EDIT_USR, szUsr, nUsr);
			GetDlgItemText (hwnd, IDC_EDIT_PWD, szPwd, nPwd);
		case IDCANCEL:
			EndDialog(hwnd, wParam);
			break;
		}
        return TRUE;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
//        DestroyWindow (hwnd);
        return TRUE;
    }
    return FALSE;
}
//---------------------------------------------------------------------------
