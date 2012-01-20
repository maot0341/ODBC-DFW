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

#include <windows.h>
#include <sqlext.h>
#include <sql.h>
#include <assert.h>
#include <crtdbg.h>

//---------------------------------------------------------------------------
#ifdef  NDEBUG
#define TRACE(msg) ((void)0)
#else
#define TRACE(msg) _RPT3(_CRT_WARN, "%s[%d]: %s\n", __FILE__, __LINE__, (const char *)(msg))
#endif
//---------------------------------------------------------------------------
#undef DIM
#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))
//---------------------------------------------------------------------------
static const SQLUSMALLINT
gs_aSuportedFunc[] =
{	SQL_API_SQLALLOCCONNECT
,	SQL_API_SQLALLOCENV
,	SQL_API_SQLALLOCSTMT
,	SQL_API_SQLBINDCOL
//,	SQL_API_SQLCANCEL
,	SQL_API_SQLCOLATTRIBUTES
,	SQL_API_SQLCONNECT
,	SQL_API_SQLDESCRIBECOL
,	SQL_API_SQLDISCONNECT
,	SQL_API_SQLERROR
,	SQL_API_SQLEXECDIRECT
,	SQL_API_SQLEXECUTE
,	SQL_API_SQLFETCH
,	SQL_API_SQLFREECONNECT
,	SQL_API_SQLFREEENV
,	SQL_API_SQLFREESTMT
//,	SQL_API_SQLGETCURSORNAME
,	SQL_API_SQLNUMRESULTCOLS
,	SQL_API_SQLPREPARE
,	SQL_API_SQLROWCOUNT
//,	SQL_API_SQLSETCURSORNAME
,	SQL_API_SQLSETPARAM
//,	SQL_API_SQLTRANSACT
,	SQL_API_SQLCOLUMNS
,	SQL_API_SQLDRIVERCONNECT
,	SQL_API_SQLGETCONNECTOPTION
,	SQL_API_SQLGETDATA
,	SQL_API_SQLGETFUNCTIONS
,	SQL_API_SQLGETINFO
//,	SQL_API_SQLGETSTMTOPTION
,	SQL_API_SQLGETTYPEINFO
//,	SQL_API_SQLPARAMDATA
//,	SQL_API_SQLPUTDATA
,	SQL_API_SQLSETCONNECTOPTION
,	SQL_API_SQLSETSTMTOPTION
,	SQL_API_SQLSPECIALCOLUMNS
,	SQL_API_SQLSTATISTICS
,	SQL_API_SQLTABLES
//,	SQL_API_SQLBROWSECONNECT
//,	SQL_API_SQLCOLUMNPRIVILEGES
//,	SQL_API_SQLDATASOURCES
//,	SQL_API_SQLDESCRIBEPARAM
,	SQL_API_SQLEXTENDEDFETCH
,	SQL_API_SQLFOREIGNKEYS
//,	SQL_API_SQLMORERESULTS
//,	SQL_API_SQLNATIVESQL
,	SQL_API_SQLNUMPARAMS
//,	SQL_API_SQLPARAMOPTIONS
//,	SQL_API_SQLPRIMARYKEYS
//,	SQL_API_SQLPROCEDURECOLUMNS
//,	SQL_API_SQLPROCEDURES
//,	SQL_API_SQLSETPOS
//,	SQL_API_SQLSETSCROLLOPTIONS
//,	SQL_API_SQLTABLEPRIVILEGES
//,	SQL_API_SQLDRIVERS
,	SQL_API_SQLBINDPARAMETER
,	SQL_API_SQLCOLATTRIBUTE

/*
// X/Open standards compliance level
,	SQL_API_SQLCOLUMNS
,	SQL_API_SQLSPECIALCOLUMNS
,	SQL_API_SQLSTATISTICS
,	SQL_API_SQLTABLES

// ODBC standards compliance level
,	SQL_API_SQLBINDPARAMETER
,	SQL_API_SQLBROWSECONNECT
,	SQL_API_SQLBULKOPERATIONS
,	SQL_API_SQLCOLUMNPRIVILEGES
,	SQL_API_SQLDESCRIBEPARAM
,	SQL_API_SQLDRIVERCONNECT
,	SQL_API_SQLFOREIGNKEYS
,	SQL_API_SQLMORERESULTS
,	SQL_API_SQLNATIVESQL
,	SQL_API_SQLNUMPARAMS
,	SQL_API_SQLPRIMARYKEYS
,	SQL_API_SQLPROCEDURECOLUMNS
,	SQL_API_SQLPROCEDURES
,	SQL_API_SQLSETPOS
,	SQL_API_SQLTABLEPRIVILEGES

// Vom Header
,	SQL_API_SQLALLOCHANDLESTD
//,	SQL_API_SQLBULKOPERATIONS
,	SQL_API_SQLBINDPARAMETER
,	SQL_API_SQLCOLATTRIBUTES
//,	SQL_API_SQLCOLUMNPRIVILEGES
,	SQL_API_SQLDESCRIBEPARAM
,	SQL_API_SQLDRIVERCONNECT
,	SQL_API_SQLDRIVERS
,	SQL_API_SQLEXTENDEDFETCH
,	SQL_API_SQLFOREIGNKEYS
,	SQL_API_SQLMORERESULTS
,	SQL_API_SQLNATIVESQL
,	SQL_API_SQLNUMPARAMS
//,	SQL_API_SQLPARAMOPTIONS
,	SQL_API_SQLPRIMARYKEYS
,	SQL_API_SQLPROCEDURECOLUMNS
,	SQL_API_SQLPROCEDURES
,	SQL_API_SQLSETPOS
,	SQL_API_SQLSETSCROLLOPTIONS
,	SQL_API_SQLTABLEPRIVILEGES
*/
};
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLGetFunctions (SQLHDBC hdbc, SQLUSMALLINT nFunctionId, SQLUSMALLINT *pnSupported)
{
	TRACE ("SQLGetFunctions");
	if (!hdbc)
		return SQL_INVALID_HANDLE;
	if (!pnSupported)
		return SQL_ERROR;

	int i;

	const int nFunc = DIM (gs_aSuportedFunc);
	////-----------------------------------------
	//// ODBC2
	////-----------------------------------------
	if (nFunctionId == SQL_API_ALL_FUNCTIONS)
	{

		const int nSize = 100;
		memset (pnSupported, 0x00, nSize * sizeof(SQLUSMALLINT));
		for (i=0; i<nFunc; i++)
		{
			const int id = gs_aSuportedFunc[i];
			if (0 > id || id >= nSize)
				continue;
			pnSupported[id] = SQL_TRUE;
		}
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// ODBC3 -- Bit-Array (16bit * nSize)
	////-----------------------------------------
	if (nFunctionId == SQL_API_ODBC3_ALL_FUNCTIONS)
	{
		const int nSize = SQL_API_ODBC3_ALL_FUNCTIONS_SIZE;
		memset (pnSupported, 0x00, nSize * sizeof(SQLUSMALLINT));
		for (i=0; i<nFunc; i++)
		{
			const int id = gs_aSuportedFunc[i];
			//// idx = id div 16
			const int j = id >> 4; 
			if (0 > j || j >= nSize)
				continue;
			//// bit = id mod 16
			pnSupported[j] |= 1 << (id & 0xf);
		}
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// Genau eine Fkt. (*pnSupported als BOOL)
	////-----------------------------------------
	else
	{
		*pnSupported = SQL_FALSE;
		for (i=0; i<nFunc; i++)
		{	
			const int j = (int) gs_aSuportedFunc[i];
			if (j == (int)nFunctionId)
			{
				*pnSupported = SQL_TRUE;
				break;
			}	
			
		}	
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
