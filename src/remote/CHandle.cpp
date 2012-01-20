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

#include "CHandle.h"
#include <stdx/debug.h>

using namespace stdx;
//---------------------------------------------------------------------------
// CHandle
//---------------------------------------------------------------------------
CHandle::CHandle (CHandle * paParent)
{
	if (paParent)
		paParent->m_aChildList.push_back (this);
	m_paParent = paParent;
}
//---------------------------------------------------------------------------
CHandle::~CHandle()
{
	if (m_paParent)
		m_paParent->m_aChildList.remove (this);
	clear();
}
//---------------------------------------------------------------------------
void CHandle::clear()
{
	list<CHandle*>::iterator iChild = m_aChildList.begin();
	for (; iChild != m_aChildList.end(); iChild++)
	{
		CHandle * pChild = *iChild;
		trace ("Handle(0x%x): destroy child 0x%x\n", this, pChild);
		pChild->m_paParent = 0;
		delete pChild;
	}
	m_aChildList.clear();
}
//---------------------------------------------------------------------------
// DRIVER
//---------------------------------------------------------------------------
HINSTANCE CDriver::ms_hInstance = 0;
auto_ptr<CDriver> CDriver::ms_aInstance;
//---------------------------------------------------------------------------
CDriver *
CDriver::Instance()
{
	CDriver * pDriver = ms_aInstance.get();
	if (pDriver)
		return pDriver;
	pDriver = new CDriver();
	assert (pDriver);
	ms_aInstance = auto_ptr<CDriver>(pDriver);
	return pDriver;
}
//---------------------------------------------------------------------------
CDriver::CDriver()
: CHandle(0)
{
	CORBA::ORB_var & orb = m_orb;
	CORBA::Object_var obj;
	char ** argv = m_argv;
	int & argc = m_argc;
	argc = 0;

	argv[argc++] = "rundll32.exe";
	argv[argc++] = "-ORBreactive";
	try 
	{
		orb = CORBA::ORB_init (argc, argv);
	}
	catch(const CORBA::Exception&)
	{
	}
	catch (...)
	{
	}
}
//---------------------------------------------------------------------------
CDriver::~CDriver()
{
//	clear();
	////-----------------------------------------
	//// destroy the ORB
	////-----------------------------------------
	CORBA::ORB_ptr orb = m_orb;
	if(!CORBA::is_nil(orb))
	try
	{
		orb->shutdown (false);
		orb->destroy();
	}
	catch(const CORBA::Exception&)
	{
	}
	catch (...)
	{
	}
}
//---------------------------------------------------------------------------
CORBA::ORB_ptr
CDriver::ORB()
{
	CDriver * pDriver = Instance();
	assert (pDriver);
	int & argc = pDriver->m_argc;
	char ** argv =pDriver->m_argv;
	CORBA::ORB_var & orb = pDriver->m_orb;
	if (CORBA::is_nil(orb))
		orb = CORBA::ORB_init (argc, argv);
	return CORBA::ORB::_duplicate (orb);
}
//---------------------------------------------------------------------------
const char * CDriver::driver_odbc_ver()
{
	return "02.00";
	return "03.00";
	return SQL_SPEC_STRING;
}
//---------------------------------------------------------------------------
// ENVIRONMENT
//---------------------------------------------------------------------------
CEnvironment::CEnvironment (CDriver * paParent)
: CHandle (paParent)
{
}
//---------------------------------------------------------------------------
CEnvironment::~CEnvironment()
{
}
//---------------------------------------------------------------------------
// Login
//---------------------------------------------------------------------------
CLogin::CLogin (const char * szHost, const char * szUsr, const char * szPwd)
{
	m_szDSN[0] = 0;
	m_szHost[0] = 0;
	m_szUser[0] = 0;
	m_szPassword[0] = 0;
	m_szDisplay[0] = 0;
	if (szHost)
		strcpy (m_szHost, szHost);
	if (szUsr)
		strcpy (m_szUser, szUsr);
	if (szPwd)
		strcpy (m_szPassword, szPwd);
}
//---------------------------------------------------------------------------
CLogin::CLogin (const char * szConnect)
{
	m_szHost[0] = 0;
	m_szUser[0] = 0;
	m_szPassword[0] = 0;
	m_szDisplay[0] = 0;
	if (!szConnect)
		return;
	int n = 0;
	char szAttr[256];
	const char * a = szConnect;
	const char * b;
	while (a)
	{
		while (*a && isspace(*a))
			a++;
		if (!*a)
			break;
		b = strchr (a, ';');
		memset (szAttr, 0, sizeof(szAttr));
		if (b)
			strncpy (szAttr, a, b - a);
		else
			strcpy (szAttr, a);
		if (!strnicmp (szAttr, "DSN=", 4))
			strcpy (m_szDSN, szAttr+4);
		if (!strnicmp (szAttr, "HOST=", 5))
			strcpy (m_szHost, szAttr+5);
		if (!strnicmp (szAttr, "UID=", 4))
			strcpy (m_szUser, szAttr+4);
		if (!strnicmp (szAttr, "PWD=", 4))
			strcpy (m_szPassword, szAttr+4);
		a = b ? b + 1 : 0;
	}
}
//---------------------------------------------------------------------------
CLogin &
CLogin::operator= (const CLogin & aLogin)
{
	if (&aLogin == this)
		return *this;
	strcpy (m_szDSN, aLogin.m_szDSN);
	strcpy (m_szHost, aLogin.m_szHost);
	strcpy (m_szUser, aLogin.m_szUser);
	strcpy (m_szPassword, aLogin.m_szPassword);
	strcpy (m_szDisplay, aLogin.m_szDisplay);
	return *this;
}
//---------------------------------------------------------------------------
bool
CLogin::operator== (const CLogin & aLogin) const
{
	if (strcmp (m_szHost, aLogin.m_szHost))
		return false;
	if (strcmp (m_szUser, aLogin.m_szUser))
		return false;
	if (strcmp (m_szPassword, aLogin.m_szPassword))
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool
CLogin::read  (const char * szConnect)
{
	if (!szConnect)
		return false;
	int n = 0;
	n += sscanf (szConnect, "db=%[^;]s", m_szHost);
	n += sscanf (szConnect, "usr=%[^;]s", m_szUser);
	n +=sscanf (szConnect, "pwd=%[^;]s", m_szPassword);
	return n == 3;
}
//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
CDatabase::CDatabase (CEnvironment * paParent)
: CHandle (paParent)
{
}
//---------------------------------------------------------------------------
CDatabase::~CDatabase()
{
	OutputDebugString ("destructor databse\n");
	close();
}
//---------------------------------------------------------------------------
void
CDatabase::close()
{
	if (!CORBA::is_nil(m_aConnectionStub))
		m_aConnectionStub->logout();
	m_aConnectionStub = idl::IConnection::_nil();
}
//---------------------------------------------------------------------------
bool
CDatabase::fetch (long nQuery)
{
	return false;
}
//---------------------------------------------------------------------------
SQLRETURN
CDatabase::connect (const char * szConnect)
{
#if 1
	CLogin aLogin (szConnect);
	return connect (aLogin);
#else
	const char * szOptions = szConnect;
	char szObject[8000] = "corbaloc:iiop:";
	char * szHost = szObject + strlen(szObject);

	sscanf (szConnect, "db=%[^;]s", szHost);
	szOptions = strchr (szConnect, ';');
	if (szOptions)
		szOptions++;
	const char * szAuth = "usr=suser;pwd=Superuse,dsp=default";
	strcat (szObject, "/dbhost");
	return connect (szObject, szAuth);
#endif

}
//---------------------------------------------------------------------------
SQLRETURN
CDatabase::connect (const CLogin & aLogin)
{
	CORBA::ORB_var orb = CDriver::ORB();
	CORBA::Object_var obj;
	try 
	{
		char szSrvt[8000] = "corbaloc:iiop:";
		const char * szHost = aLogin.m_szHost;
		const char * szAuth = aLogin.m_szUser;
		strcat (szSrvt, szHost);
		strcat (szSrvt, "/dbhost");
		idl::IHost_var aHostStub;
		obj = orb->string_to_object (szSrvt);
		aHostStub = idl::IHost::_narrow(obj);
		if(CORBA::is_nil (aHostStub))
			throw EXC("08S01", 2500, "driver is not connected!");
		m_aConnectionStub = aHostStub->login(szAuth);
		if(CORBA::is_nil (m_aConnectionStub))
			throw EXC("08S01", 2500, "driver is not connected!");
		m_aConnectionStub->ping();
		if (&aLogin != &m_aLogin)
		if (aLogin != m_aLogin)
			m_aLogin = aLogin;
	}
	catch(const CORBA::Exception& aExc)
	{
		trace ("CORBA: %s\n", aExc._to_string());
		return SQL_ERROR;
	}
	catch (...)
	{
		return SQL_ERROR;
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
bool
CDatabase::connected()
{
	if (!CORBA::is_nil (m_aConnectionStub))
	if (!m_aConnectionStub->_non_existent())
		return true;
	return false;
}
//---------------------------------------------------------------------------
idl::IConnection_ptr
CDatabase::operator->()
{
	ASSUME(!CORBA::is_nil (m_aConnectionStub));
	if (m_aConnectionStub->_non_existent())
		connect (m_aLogin);
	return m_aConnectionStub;
//	assert(!CORBA::is_nil (m_aStmtStub));
//	return m_aStmtStub;
}
//---------------------------------------------------------------------------
