/****************************************************************************

    Projekt:    Hasy                                          (C) 2000 PSI AG
    Komponente: tpprocl
    Modul:      IStmt_impl.cpp

    Implementierung der IDL-Schnittstelle IStmt.
    Implementationsmodul.

    Systeme: Alpha, Linux, NT 4

    $Author: hasyvh $
    $Date: 2007/03/03 17:46:13 $
    $Revision: 1.10 $
    $State: Exp $

    �nderung  Datum     Autor  Bemerkung
    -------------------------------------------------------------------------
              27.02.02  jv     Erstellung
              17.06.03  vh     aufger�umt
              13.06.06  vh     meldeProtokollmeldung neu.
              09.11.06  vh     externeNavigation() neu.
*****************************************************************************/

#pragma warning (disable:4786)

#include "idlcpy.h"
#include "IRemote_impl.h"
#include "database.h"
#include "SQLQuery.h"
#include "SQLTables.h"
#include "SQLColumns.h"
#include "SQLSpecialColumns.h"
#include "SQLStatistics.h"
#include "SQLGetTypeInfo.h"
#include <stdx/debug.h>


using namespace stdx;
//---------------------------------------------------------------------------
extern CORBA::ORB_var orb;
static time_t ta;
//---------------------------------------------------------------------------
#define TRACEFN(s) trace("%-20s  %s:%d\n", (char*)s, __FILE__, __LINE__) 
//---------------------------------------------------------------------------
#ifdef  NDEBUG
#	define TRY(s) try {
#else
#	define TRY(s) try { TRACELN(s)
#endif
//---------------------------------------------------------------------------
#	define EXCEPTIONS } \
 	catch (const CException & aExc)	\
	{ \
		throw IDL(aExc); \
	} \
	catch (const string & strErr) \
	{ \
		throw IDL(EXC("42000", 900, strErr.c_str())); \
	} \
	catch (...) \
	{ \
		throw IDL(EXC("42000", 900, "Syntax error or access violation")); \
	} \

#define DIAG(p) ((p) ? &(p)->diag() : (CDiagInfo*)0)
//#define DIAG(s,t) IDL (s, (t) ? &(t)->diag() : (CDiagInfo*)0) 
//---------------------------------------------------------------------------
// Host
//---------------------------------------------------------------------------
IHost_impl::IHost_impl()
{
}
//---------------------------------------------------------------------------
IHost_impl::~IHost_impl()
{
}
//---------------------------------------------------------------------------
void
IHost_impl::shutdown (bool wait_for_completion)
throw(::CORBA::SystemException)
{
	TRY("shutdown");
	printf ("CORBA-Call: shutdown\n");
	orb->shutdown (wait_for_completion);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IHost_impl::ping()
throw(::CORBA::SystemException)
{
	TRY("IHost::ping");
//	printf ("CORBA-Call (IHost 0x%x): ping\n", this);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IHost_impl::exec (const char * szCmd)
throw(::CORBA::SystemException)
{
	TRY("exec");
//	printf ("CORBA-Call: %s\n", szCmd);
	if (strcmp (szCmd, "test") == 0)
	{
		CDatabase * pDB = CDatabase::Instance();
		ASSUME (pDB);
		return;
	}
	if (strcmp (szCmd, "shutdown") == 0)
	{
		shutdown (false);
		return;
	}
	puts("exec <cmd> ?!");
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::IConnection_ptr
IHost_impl::login (const char * szAuth)
throw(::CORBA::SystemException)
{
	TRY("login");
//	printf ("CORBA-Call (IHost): login\n");
	IConnection_impl * pConnection = new IConnection_impl();
	PortableServer::ServantBase_var vImpl = pConnection;
	idl::IConnection_var vConnection = pConnection->_this();
	return vConnection._retn();
	EXCEPTIONS
}

//---------------------------------------------------------------------------
// Connection
//---------------------------------------------------------------------------
IConnection_impl::IConnection_impl()
{
	TRY("IConnection_impl");
//	printf ("0x%x  IConnection_impl\n", this);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
IConnection_impl::~IConnection_impl()
{
	TRY("~IConnection_impl");
//	printf ("0x%x  ~IConnection_impl\n", this);
	list<IStmt_impl*>::iterator iStmt = m_aStmtList.begin();
	for (; iStmt != m_aStmtList.end(); ++iStmt)
	{
		IStmt_impl * pStmt = *iStmt;
		if (!pStmt)
			continue;
		pStmt->m_pDBC = 0;
		pStmt->destroy();
	}
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IConnection_impl::logout()
throw(::CORBA::SystemException)
{
	TRY("logout");
//	printf ("0x%x  IConnection: logout\n", this);
	CORBA::ULong nRef = _OB_getRefCount();
	PortableServer::POA_var poa = _default_POA();
	PortableServer::ObjectId_var oid = poa->servant_to_id(this);
	poa->deactivate_object(oid);
	oid._retn();
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IConnection_impl::ping()
throw(::CORBA::SystemException)
{
	TRY("ping");
//	printf ("0x%x  IConnection: ping\n", this);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::IStmt_ptr
IConnection_impl::SQLAllocStmt()
throw(::CORBA::SystemException)
{
	TRY("SQLAllocStmt");
//	printf ("0x%x  IConnection: SQLAllocStmt\n", this);
	IStmt_impl * pStmt = new IStmt_impl (this);
	PortableServer::ServantBase_var vImpl = pStmt;
	idl::IStmt_var vStmt = pStmt->_this();
	return vStmt._retn();
	EXCEPTIONS
}
//---------------------------------------------------------------------------
// Stmt -- Interface
//---------------------------------------------------------------------------
IStmt_impl::IStmt_impl(IConnection_impl *pDBC)
{
	TRY("IStmt_impl");
//	printf ("0x%x  IStmt_impl\n", this);
	m_pDBC = pDBC;
	if (m_pDBC)
		m_pDBC->m_aStmtList.push_back(this);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
IStmt_impl::~IStmt_impl()
{
	TRY("~IStmt_impl");
//	printf ("0x%x  ~IStmt_impl\n", this);
	if (m_pDBC)
		m_pDBC->m_aStmtList.remove (this);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IStmt_impl::ping()
throw(::CORBA::SystemException)
{
	TRY("ping");
//	printf ("0x%x  IStmt: ping\n", this);
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IStmt_impl::destroy()
throw(::CORBA::SystemException)
{
	TRY("destroy");
//	printf ("0x%x  IStmt: destroy\n", this);
//	return;
	CORBA::ULong nRef = _OB_getRefCount();
	PortableServer::POA_var poa = _default_POA();
	PortableServer::ObjectId_var oid = poa->servant_to_id(this);
	poa->deactivate_object(oid);
	oid._retn();
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IStmt_impl::exec (const char * szCmd)
throw(::CORBA::SystemException)
{
	TRY("exec");
//	printf ("CORBA-Call: %s\n", szCmd);
	if (strcmp (szCmd, "test") == 0)
	{
		CDatabase * pDB = CDatabase::Instance();
		ASSUME (pDB);
		return;
	}
	puts("exec <cmd> ?!");
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void 
IStmt_impl::desc (idl::typHeader & crbHeader)
throw(::CORBA::SystemException)
{
	TRY("desc");
	CTableImpl * pTable = m_aTablePtr.get();
	if (!pTable)
	{
		crbHeader.length(0);
		return;
	}
	const ULONG nCols = pTable->cols();
	ULONG i;
	crbHeader.length (nCols);
	for (i=0; i<nCols; i++)
	{
		const CDesc * pDesc = pTable->desc(i);
		ASSUME (pDesc);
		idlcpy (crbHeader[i], *pDesc);
	}
	EXCEPTIONS
}
//---------------------------------------------------------------------------
void
IStmt_impl::clear()
{
	m_aTablePtr = auto_ptr<CTableImpl> (0);
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::RETN (short nRetn, const CDiagInfo * pInfo)
{
	return IDL (nRetn, pInfo);
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLTables
( const char* szCatalog
, const char* szSchema
, const char* szTable
, const char* szType)
throw(::CORBA::SystemException)
{
	TRY("SQLTables");
	CDatabase * pDatabase = CDatabase::Instance();
	if (!pDatabase)
		return RETN (SQL_INVALID_HANDLE);
	CTableImpl * pTable = pDatabase->SQLTables (szCatalog, szSchema, szTable, szType);
	m_aTablePtr = auto_ptr<CTableImpl> (pTable);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLColumns
( const char* szCatalog
, const char* szSchema
, const char* szTable
, const char* szColumn)
throw(::CORBA::SystemException)
{
	TRY("SQLColumns");
	CDatabase * pDatabase = CDatabase::Instance();
	if (!pDatabase)
		return RETN (SQL_INVALID_HANDLE);
	CTableImpl * pTable = new CSQLColumns(szCatalog, szSchema, szTable, szColumn);
	m_aTablePtr = auto_ptr<CTableImpl> (pTable);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLSpecialColumns
( const char* szCatalog
, const char* szSchema
, const char* szTable
, short nScope
, short nNullable
)
throw(::CORBA::SystemException)
{
	TRY("SQLSpecialColumns");
	CDatabase * pDatabase = CDatabase::Instance();
	if (!pDatabase)
		return RETN (SQL_INVALID_HANDLE);
	CTableImpl * pTable = new CSQLSpecialColumns(0, szCatalog, szSchema, szTable, nScope, nNullable);
	m_aTablePtr = auto_ptr<CTableImpl> (pTable);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLStatistics
( const char* szCatalog
, const char* szSchema
, const char* szTable
, short nUnique
, short nReserved
)
throw(::CORBA::SystemException)
{
	TRY("SQLStatistics");
	CDatabase * pDatabase = CDatabase::Instance();
	if (!pDatabase)
		return RETN (SQL_INVALID_HANDLE);
	CTableImpl * pTable = new CSQLStatistics (szCatalog, szSchema, szTable, nUnique, nReserved);
	m_aTablePtr = auto_ptr<CTableImpl> (pTable);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLParams (const idl::typParamset & crbParamset)
throw(::CORBA::SystemException)
{
	TRY("SQLParams");
	CSQLQuery * pTable = dynamic_cast<CSQLQuery*> (m_aTablePtr.get());
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	vector<CParam*> & raParamset =  pTable->m_aStmt.m_aParam;
	const short nParam = crbParamset.length();
	short i;
	for (i=0; i<nParam; i++)
	{
		const idl::typParam & crbParam = crbParamset[i];
		const idl::typValue & crbValue = crbParam.m_aValue;
		sqlp::CParam * pParam = raParamset[i];
		idlcpy (*pParam, crbParam);
	}
	for (; i<raParamset.size(); i++)
	{
		sqlp::CParam * pParam = raParamset[i];
		pParam->setNull();
	}
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLPrepare(const char* szSQL)
throw(::CORBA::SystemException)
{
	TRY("SQLPrepare");
	clear();
	CSQLQuery * pTable = new CSQLQuery(szSQL);
	ASSUME (pTable);
	m_aTablePtr = auto_ptr<CTableImpl> (pTable);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLExecute ()
throw(::CORBA::SystemException, idl::typException)
{
	TRY("SQLExecute");
	CSQLQuery * pTable = dynamic_cast<CSQLQuery*> (m_aTablePtr.get());
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	pTable->open();
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLNumResultCols (short & rnColumns)
throw(::CORBA::SystemException)
{
	TRY("SQLNumResultCols");
	CTableImpl * pTable = m_aTablePtr.get();
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	pTable->diag(0);
	rnColumns = pTable->cols();
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLGetTypeInfo (short nDataType)
throw(::CORBA::SystemException)
{
	TRY("SQLGetTypeInfo");
	CDatabase * pDatabase = CDatabase::Instance();
	if (!pDatabase)
		return RETN (SQL_INVALID_HANDLE);
	CSQLGetTypeInfo * pTable = pDatabase->SQLGetTypeInfo(nDataType);
	m_aTablePtr = auto_ptr<CTableImpl> (pTable);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLFetch (ULONG iRow, ULONG nRow, idl::typRecord_out pRecord)
throw(::CORBA::SystemException)
{
	TRY(0);
	ta = time(0);
	pRecord = new idl::typRecord;
	assert (pRecord != 0);
	CTableImpl * pTable = m_aTablePtr.get();
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	if (nRow == 0)
		nRow = pTable->rows();
	if (!pTable->read (iRow, nRow, *pRecord))
		return RETN (SQL_NO_DATA, DIAG(pTable));
	time_t te = time(0);
	time_t td = te - ta;
//	trace ("SQLFetch [%d] timing: %d s\n", iRow, td);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLFetchRef (ULONG iRow, ULONG nRow, idl::typRecord & raRecord)
throw(::CORBA::SystemException)
{
	TRY("SQLFetchRef");
	ta = time(0);
	CTableImpl * pTable = m_aTablePtr.get();
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	if (!pTable->read (iRow, nRow, raRecord))
		return RETN (SQL_NO_DATA, DIAG(pTable));
	time_t te = time(0);
	time_t td = te - ta;
//	trace ("SQLFetchRef [%d] timing: %d s\n", iRow, td);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLDescribeParams (idl::typParamset & raParamset)
throw(::CORBA::SystemException)
{
	TRY("SQLDescribeParams");
	CSQLQuery * pTable = dynamic_cast<CSQLQuery*> (m_aTablePtr.get());
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	const vector<CParam*> & aParamset =  pTable->m_aStmt.m_aParam;
	const short nParam = aParamset.size();
//	if (raParamset.length() < nParam)
	raParamset.length (nParam);
	short i;
	for (i=0; i<nParam; i++)
	{
		const CParam * pParam = aParamset[i];
		assert (pParam);
		idl::typParam & raParam = raParamset[i];
		//assert (raParam.m_nParam == i);
		//raParam.m_nParam;
		//raParam.m_nInputOutputType;
		short  nType = pParam->type();
		raParam.m_nParam = i+1;
		raParam.m_nIOType = pParam->IOType();
		raParam.m_nType = nType;
		raParam.m_nColumnSize = pParam->ColumnSize();
		raParam.m_nDecimalDigits = pParam->DecimalDigits();;
		raParam.m_nNullable = pParam->Nullable();
		idlnull (raParam.m_aValue, nType);
	}
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLColAttribute
( CORBA::UShort crbColumn
, CORBA::UShort crbAttr
, idl::typValue_out crbValue)
throw(::CORBA::SystemException)
{
	TRY("SQLColAttribute");
	crbValue = new idl::typValue;
	CTableImpl * pTable = m_aTablePtr.get();
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	CValue aValue = pTable->attr (crbAttr);
	idlcpy (crbValue, aValue);
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------
idl::RETN*
IStmt_impl::SQLDescribeCol
( CORBA::UShort crbColumn
, CORBA::String_out crbName
, CORBA::Short_out crbDataType
, CORBA::ULong_out crbColumnSize
, CORBA::Short_out crbDecimalDigits
, CORBA::Short_out crbNullable)
throw(::CORBA::SystemException)
{
	TRY("SQLDescribeCol");
	CTableImpl * pTable = m_aTablePtr.get();
	if (!pTable)
		return RETN (SQL_INVALID_HANDLE);
	const CDesc * pDesc = pTable->desc (crbColumn);
	if (!pDesc)
		return RETN (SQL_INVALID_HANDLE, DIAG(pTable));
	crbName = CORBA::string_dup (pDesc->name());
	crbDataType = pDesc->type();
	crbColumnSize = pDesc->size();
	crbDecimalDigits = pDesc->digits();
	crbNullable = pDesc->nullable();
	return RETN (SQL_SUCCESS, DIAG(pTable));
	EXCEPTIONS
}
//---------------------------------------------------------------------------

