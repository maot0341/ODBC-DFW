/****************************************************************************
 
    Projekt:    Transport
    Komponente: idl
    Modul:      IPrognose.idl
 
    IPrognose definiert die Schnittstelle zum Anstoss der Prognose
    des Transport-Servers.
 
    Systeme: Alpha, Linux, Win32
 
    $Author: hasyjv $
    $Date: 2007/10/10 11:41:14 $
    $Revision: 1.76 $
    $State: Exp $
 
    Datum     Autor  Bemerkung
    -------------------------------------------------------------------------
    16.07.01  hh/mtk Erstellung
    07.08.01  hks    Hasy-Programmierkonventionen, ustring
    07.08.01  hks    Ausnahmen
    08.08.01  hks    Aktualisierungsinterface ausgelagert 
*****************************************************************************/

#ifndef TPDBLNK_IREMOTE_IMPL_H
#define TPDBLNK_IREMOTE_IMPL_H

#include <OB/CORBA.h>
#include <idl/IRemote_skel_idl.h>

#include <sqlplib/sqlp.h>
#include <list>
#include "CTableImpl.h"

using namespace std;
using namespace sqlp;
class IHost_impl;
class IConnection_impl;
class IStmt_impl;
//---------------------------------------------------------------------------
class IHost_impl
: virtual public POA_idl::IHost
, public PortableServer::RefCountServantBase
{
public:
	IHost_impl();
	~IHost_impl();
	
	void shutdown (bool wait_for_completion)
	throw(::CORBA::SystemException);

	void ping()
	throw(::CORBA::SystemException);

	void exec (const char * cmd)
	throw(::CORBA::SystemException);

    idl::IConnection_ptr login (const char* auth)
	throw(::CORBA::SystemException);
};
//---------------------------------------------------------------------------
class IConnection_impl
: virtual public POA_idl::IConnection
, public PortableServer::RefCountServantBase
{
public:
	IConnection_impl();
	~IConnection_impl();
	////-----------------------------------------
	void logout()
	throw(::CORBA::SystemException);
	////-----------------------------------------
    idl::IStmt_ptr SQLAllocStmt()
	throw(::CORBA::SystemException);
	////-----------------------------------------
	void ping()
	throw(::CORBA::SystemException);


	list<IStmt_impl*> m_aStmtList;
};
//---------------------------------------------------------------------------
class IStmt_impl
: public POA_idl::IStmt
, public PortableServer::RefCountServantBase
{
public:
	friend class IConnection_impl;

	IStmt_impl(IConnection_impl*);
	~IStmt_impl();
	
	void destroy()
	throw(::CORBA::SystemException);

	void ping()
	throw(::CORBA::SystemException);

	void exec (const char * cmd)
	throw(::CORBA::SystemException);

	void desc(idl::typHeader& header)
	throw(::CORBA::SystemException);

	void clear();

//	idl::RETN* RETN(short n, const CException* exc=0);
	idl::RETN* RETN(short n, const CDiagInfo* info=0);
	////-----------------------------------------
	idl::RETN*
	SQLTables
	( const char* catalog
	, const char* schema
	, const char* table
	, const char* type
	)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN*
	SQLSpecialColumns
	( const char* catalog
	, const char* schema
	, const char* table
	, short scope
	, short nullable
	)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN*
	SQLStatistics
	( const char* catalog
	, const char* schema
	, const char* table
	, short nUnique
	, short nReserved
	)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN*
	SQLColumns
	( const char* catalog
	, const char* schema
	, const char* table
	, const char* column
	)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLParams (const idl::typParamset& params)
	throw(::CORBA::SystemException);
	idl::RETN* SQLPrepare (const char* sql)
	throw(::CORBA::SystemException);
	idl::RETN* SQLExecute()
	throw(::CORBA::SystemException, idl::typException);
	idl::RETN* SQLNumResultCols (short & columns)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLGetTypeInfo (short datatyp)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLFetch (ULONG iRow, ULONG nRow, idl::typRecord_out)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLFetchRef (ULONG iRow, ULONG nRow, idl::typRecord &)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLDescribeParams(idl::typParamset& params)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLColAttribute
	( CORBA::UShort col
	, CORBA::UShort attr
	, idl::typVariant_out value
	)
	throw(::CORBA::SystemException);
	////-----------------------------------------
	idl::RETN* SQLDescribeCol
	( CORBA::UShort column
	, CORBA::String_out name
	, CORBA::Short_out datatype
	, CORBA::ULong_out columnsize
	, CORBA::Short_out decimaldigits
	, CORBA::Short_out nullable
	)
	throw(::CORBA::SystemException);
//	static vector<PortableServer::ObjectId*> ms_aDeleted;

protected:
	IConnection_impl * m_pDBC;
	auto_ptr<CTableImpl> m_aTablePtr;
};
//---------------------------------------------------------------------------
#endif // TPDBLNK_IREMOTE_IMPL_H
