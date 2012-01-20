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

//#include <vld.h>
//#include <stdio.h>
#include <crtdbg.h>
#include <fstream>
#include <iostream>

#include <OB/CORBA.h>
#include "../sqlplib/sqlp.h"
#include "../sqlplib/yac.h"
#include "IRemote_idl.h"

using namespace std;
using namespace sqlp;

//---------------------------------------------------------------------------
CORBA::ORB_var orb;
//---------------------------------------------------------------------------
// main
int main(int argc, char * argv[])
{
	CORBA::Object_var vObj;
	idl::IRemote_var aRemoteStub;
	try 
	{

		orb = CORBA::ORB_init (argc, argv);
		//vObj = orb->string_to_object ("corbaloc:iiop:localhost:5000/dblnk");
		vObj = orb->string_to_object ("corbaloc:iiop:gontpc47:5000/dblnk");
		aRemoteStub = idl::IRemote::_narrow(vObj);
		if(CORBA::is_nil (aRemoteStub))
			return SQL_ERROR;
		if (argc < 2)
		{
			idl::IQuery_var vQuery = aRemoteStub->prepare ("SELECT * FROM test");
			vQuery->ping();
			aRemoteStub->ping();
			return EXIT_SUCCESS;
		}
		if (strcmp(argv[1], "ping") == 0)
			aRemoteStub->ping();
		if (strcmp(argv[1], "shutdown") == 0)
			aRemoteStub->shutdown (false);

	}
	catch(const CORBA::Exception& aError)
	{
		cerr << aError << endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		cerr << "unkonwn exception!" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
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

