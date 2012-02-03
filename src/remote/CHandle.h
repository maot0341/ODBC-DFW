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

#ifndef CHANDLE_H
#define CHANDLE_H

#include <OB/CORBA.h>
#include <idl/IRemote_idl.h>
#include "SQLException.h"

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include <list>

class CDriver;
class CEnvironment;
class CDriverDB;
class CStmtHandle;
class CLink;

//#define JV_DBG 1
//---------------------------------------------------------------------------
// exception handling
//---------------------------------------------------------------------------
#define TRY(s) try { if (s) TRACELN(s)
//#define TRY(s) try {
#define EXCEPTION(h) \
	} \
	catch (const idl::typException & aExc) \
	{ \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->state (aExc.aDiag); \
	}\
	catch (const idl::typDiagItem & aExc) \
	{ \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->state (aExc); \
	} \
	catch (const std::string & strText) \
	{ \
		const char * szText = strText.c_str(); \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->state (EXC("08S01", 1999, szText)); \
		TRACELN (szText); \
	} \
	catch (const ::CORBA::Exception& aExc) \
	{ \
		const char * szText = aExc._to_string(); \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->state (EXC("08S01", 1999, szText)); \
		TRACELN (szText); \
	} \
	catch (...) \
	{ \
		const char * szText = "unknown exception!"; \
		CHandle * p = static_cast<CHandle*>(h); \
		if (p) p->state (EXC("08S01", 1999, szText)); \
		TRACELN (szText); \
	} \

//---------------------------------------------------------------------------
// HANDLE
//---------------------------------------------------------------------------
class CHandle
{
public:
	CHandle (CHandle * parent);
	virtual ~CHandle();
	void clear();
	void state (const idl::typDiagItem &);
	void state (const idl::typDiagSeq &);

	SQLRETURN SQLError (CORBA::String_var & crbState, CORBA::Short &crbErrorCode, CORBA::String_var & crbMessage);
	short RETN() const                            
	{ 
		return (&m_vRetn == 0) ? SQL_SUCCESS : m_vRetn->nRetn; 
	}

	short RETN (idl::RETN * ret);
	short RETN (const idl::typException & aExc);
	short RETN (const ::CORBA::Exception& aExc, const char * file, long line);


protected:
	CHandle * m_paParent;
	std::list<CHandle*> m_aChildList;

private:
	idl::RETN_var m_vRetn;
	int m_iDiag;
};
//---------------------------------------------------------------------------
// DRIVER
//---------------------------------------------------------------------------
class CDriver : CHandle
{
	friend CEnvironment;
public:
	virtual ~CDriver();
	static const char * driver_odbc_ver();
	static const SQLUSMALLINT ms_aSuportedFunc[];
	static CDriver * Instance();
	static CORBA::ORB_ptr ORB();

	static HINSTANCE ms_hInstance;

protected:
	CDriver();

#ifndef JV_DBG
	static auto_ptr<CDriver> ms_aInstance;
#else
	static CDriver* ms_pInstance;
#endif
	char * m_argv[99];
	int m_argc;
	CORBA::ORB_var m_orb;
};
//---------------------------------------------------------------------------
// ENVIRONMENT
//---------------------------------------------------------------------------
class CEnvironment : public CHandle
{
	friend CDriverDB;
public:
	CEnvironment (CDriver *);
	virtual ~CEnvironment();

};
//---------------------------------------------------------------------------
class CLogin
{
public:
	CLogin (const char * connect=0);
	CLogin (const char * host, const char * usr, const char * pwd);
	CLogin & operator= (const CLogin&);
	bool operator== (const CLogin&) const;
	bool operator!= (const CLogin& l) const  { return !operator==(l); }
	bool read (const char * connect); 

	char m_szDSN[256];
	char m_szHost[256];
	char m_szUser[256];
	char m_szPassword[256];
	char m_szDisplay[256];
};
//---------------------------------------------------------------------------
class CDatabase : public CHandle
{
public:
	CDatabase (CEnvironment*);
	virtual ~CDatabase();
	SQLRETURN connect (const char * connection);
//	SQLRETURN connect (const char * szSrvt, const char * szAuth);
	SQLRETURN connect (const CLogin &);
	void close();
	bool connected();
	bool fetch (long query);
	idl::IConnection_ptr operator->();

protected:
	string m_strConnect;
	idl::IConnection_var m_aConnectionStub;
	CLogin m_aLogin;
};
//---------------------------------------------------------------------------
// CTarget
//---------------------------------------------------------------------------
class CTarget
{
public:
	CTarget(): nCol(0), nTyp(SQL_C_DEFAULT), pVal(0), nLen(0), pInd(0) {}


	SQLUSMALLINT nCol;
	SQLSMALLINT nTyp;
	SQLPOINTER pVal;
	SQLINTEGER nLen;
	SQLINTEGER * pInd;
};
//---------------------------------------------------------------------------
inline
bool operator!= (const idl::RETN_var & vRetn, short nRetn)
{
	if (&vRetn == 0)
		return false;
	return vRetn->nRetn != nRetn;
}
//---------------------------------------------------------------------------
#endif
