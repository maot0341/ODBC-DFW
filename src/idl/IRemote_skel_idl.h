// **********************************************************************
//
// Generated by the ORBacus IDL-to-C++ Translator
//
// Copyright (c) 2004
// IONA Technologies, Inc.
// Waltham, MA, USA
//
// All Rights Reserved
//
// **********************************************************************

// Version: 4.2.2

#ifndef ___IRemote_skel_idl_h__
#define ___IRemote_skel_idl_h__

#include <IRemote_idl.h>

#ifndef OB_INTEGER_VERSION
#   error No ORBacus version defined! Is <OB/CORBA.h> included?
#endif

#ifndef OB_NO_VERSION_CHECK
#   if (OB_INTEGER_VERSION != 4020200L)
#       error ORBacus version mismatch!
#   endif
#endif

//
// IDL:go.psi.de/idl:1.0
//
namespace POA_idl
{

//
// IDL:go.psi.de/idl/IHost:1.0
//
class IHost : virtual public PortableServer::ServantBase
{
    IHost(const IHost&);
    void operator=(const IHost&);

protected:

    //
    // IDL:go.psi.de/idl/IHost/shutdown:1.0
    //
    void _OB_op_shutdown(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IHost/login:1.0
    //
    void _OB_op_login(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IHost/ping:1.0
    //
    void _OB_op_ping(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IHost/exec:1.0
    //
    void _OB_op_exec(OB::Upcall_ptr);

public:

    IHost() { }

    virtual ::CORBA::Boolean _is_a(const char*)
        throw(::CORBA::SystemException);

    virtual ::CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId&,
                                                     PortableServer::POA_ptr);

    idl::IHost_ptr _this();

    virtual OB::DirectStubImpl_ptr _OB_createDirectStubImpl(PortableServer::POA_ptr,
                                                            const PortableServer::ObjectId&);
    virtual void _OB_dispatch(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IHost/shutdown:1.0
    //
    virtual void shutdown(::CORBA::Boolean wait_for_completion)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IHost/login:1.0
    //
    virtual ::idl::IConnection_ptr login(const char* auth)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IHost/ping:1.0
    //
    virtual void ping()
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IHost/exec:1.0
    //
    virtual void exec(const char* cmd)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;
};

//
// IDL:go.psi.de/idl/IConnection:1.0
//
class IConnection : virtual public PortableServer::ServantBase
{
    IConnection(const IConnection&);
    void operator=(const IConnection&);

protected:

    //
    // IDL:go.psi.de/idl/IConnection/SQLAllocStmt:1.0
    //
    void _OB_op_SQLAllocStmt(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IConnection/logout:1.0
    //
    void _OB_op_logout(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IConnection/ping:1.0
    //
    void _OB_op_ping(OB::Upcall_ptr);

public:

    IConnection() { }

    virtual ::CORBA::Boolean _is_a(const char*)
        throw(::CORBA::SystemException);

    virtual ::CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId&,
                                                     PortableServer::POA_ptr);

    idl::IConnection_ptr _this();

    virtual OB::DirectStubImpl_ptr _OB_createDirectStubImpl(PortableServer::POA_ptr,
                                                            const PortableServer::ObjectId&);
    virtual void _OB_dispatch(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IConnection/SQLAllocStmt:1.0
    //
    virtual ::idl::IStmt_ptr SQLAllocStmt()
        throw(::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IConnection/logout:1.0
    //
    virtual void logout()
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IConnection/ping:1.0
    //
    virtual void ping()
        throw(idl::typException,
              ::CORBA::SystemException) = 0;
};

//
// IDL:go.psi.de/idl/IStmt:1.0
//
class IStmt : virtual public PortableServer::ServantBase
{
    IStmt(const IStmt&);
    void operator=(const IStmt&);

protected:

    //
    // IDL:go.psi.de/idl/IStmt/ping:1.0
    //
    void _OB_op_ping(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/exec:1.0
    //
    void _OB_op_exec(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/destroy:1.0
    //
    void _OB_op_destroy(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/desc:1.0
    //
    void _OB_op_desc(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLTables:1.0
    //
    void _OB_op_SQLTables(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLColumns:1.0
    //
    void _OB_op_SQLColumns(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLSpecialColumns:1.0
    //
    void _OB_op_SQLSpecialColumns(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLStatistics:1.0
    //
    void _OB_op_SQLStatistics(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLParams:1.0
    //
    void _OB_op_SQLParams(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLPrepare:1.0
    //
    void _OB_op_SQLPrepare(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLExecute:1.0
    //
    void _OB_op_SQLExecute(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLNumResultCols:1.0
    //
    void _OB_op_SQLNumResultCols(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLGetTypeInfo:1.0
    //
    void _OB_op_SQLGetTypeInfo(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLFetch:1.0
    //
    void _OB_op_SQLFetch(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLDescribeParams:1.0
    //
    void _OB_op_SQLDescribeParams(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLFetchRef:1.0
    //
    void _OB_op_SQLFetchRef(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLColAttribute:1.0
    //
    void _OB_op_SQLColAttribute(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/SQLDescribeCol:1.0
    //
    void _OB_op_SQLDescribeCol(OB::Upcall_ptr);

public:

    IStmt() { }

    virtual ::CORBA::Boolean _is_a(const char*)
        throw(::CORBA::SystemException);

    virtual ::CORBA::RepositoryId _primary_interface(const PortableServer::ObjectId&,
                                                     PortableServer::POA_ptr);

    idl::IStmt_ptr _this();

    virtual OB::DirectStubImpl_ptr _OB_createDirectStubImpl(PortableServer::POA_ptr,
                                                            const PortableServer::ObjectId&);
    virtual void _OB_dispatch(OB::Upcall_ptr);

    //
    // IDL:go.psi.de/idl/IStmt/ping:1.0
    //
    virtual void ping()
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/exec:1.0
    //
    virtual void exec(const char* cmd)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/destroy:1.0
    //
    virtual void destroy()
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/desc:1.0
    //
    virtual void desc(::idl::typHeader& header)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLTables:1.0
    //
    virtual ::idl::RETN* SQLTables(const char* catalog,
                                   const char* schema,
                                   const char* table,
                                   const char* type)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLColumns:1.0
    //
    virtual ::idl::RETN* SQLColumns(const char* catalog,
                                    const char* schema,
                                    const char* table,
                                    const char* column)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLSpecialColumns:1.0
    //
    virtual ::idl::RETN* SQLSpecialColumns(const char* catalog,
                                           const char* schema,
                                           const char* table,
                                           ::CORBA::Short scope,
                                           ::CORBA::Short nullable)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLStatistics:1.0
    //
    virtual ::idl::RETN* SQLStatistics(const char* catalog,
                                       const char* schema,
                                       const char* table,
                                       ::CORBA::Short nUnique,
                                       ::CORBA::Short nReserved)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLParams:1.0
    //
    virtual ::idl::RETN* SQLParams(const ::idl::typParamset& params)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLPrepare:1.0
    //
    virtual ::idl::RETN* SQLPrepare(const char* sql)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLExecute:1.0
    //
    virtual ::idl::RETN* SQLExecute()
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLNumResultCols:1.0
    //
    virtual ::idl::RETN* SQLNumResultCols(::CORBA::Short_out columns)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLGetTypeInfo:1.0
    //
    virtual ::idl::RETN* SQLGetTypeInfo(::CORBA::Short datatyp)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLFetch:1.0
    //
    virtual ::idl::RETN* SQLFetch(::idl::ULONG iRow,
                                  ::idl::ULONG nRow,
                                  ::idl::typRecord_out record)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLDescribeParams:1.0
    //
    virtual ::idl::RETN* SQLDescribeParams(::idl::typParamset& params)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLFetchRef:1.0
    //
    virtual ::idl::RETN* SQLFetchRef(::idl::ULONG iRow,
                                     ::idl::ULONG nRow,
                                     ::idl::typRecord& record)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLColAttribute:1.0
    //
    virtual ::idl::RETN* SQLColAttribute(::CORBA::UShort col,
                                         ::CORBA::UShort attr,
                                         ::idl::typValue_out value)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;

    //
    // IDL:go.psi.de/idl/IStmt/SQLDescribeCol:1.0
    //
    virtual ::idl::RETN* SQLDescribeCol(::CORBA::UShort col,
                                        ::CORBA::String_out name,
                                        ::CORBA::Short_out datatype,
                                        ::CORBA::ULong_out columnsize,
                                        ::CORBA::Short_out decimaldigits,
                                        ::CORBA::Short_out nullable)
        throw(idl::typException,
              ::CORBA::SystemException) = 0;
};

} // End of namespace POA_idl

//
// IDL:go.psi.de/idl:1.0
//
namespace OBDirectStubImpl_idl
{

//
// IDL:go.psi.de/idl/IHost:1.0
//
class IHost : virtual public OBStubImpl_idl::IHost,
              virtual public OB::DirectStubImpl
{
    IHost(const IHost&);
    void operator=(const IHost&);

protected:

#ifdef HAVE_VCPLUSPLUS_BUGS

    IHost() { }

#endif

    IHost(PortableServer::POA_ptr,
          const PortableServer::ObjectId&,
          PortableServer::ServantBase*);

    friend class POA_idl::IHost;

public:

    //
    // IDL:go.psi.de/idl/IHost/shutdown:1.0
    //
    virtual void shutdown(::CORBA::Boolean wait_for_completion);

    //
    // IDL:go.psi.de/idl/IHost/login:1.0
    //
    virtual ::idl::IConnection_ptr login(const char* auth);

    //
    // IDL:go.psi.de/idl/IHost/ping:1.0
    //
    virtual void ping();

    //
    // IDL:go.psi.de/idl/IHost/exec:1.0
    //
    virtual void exec(const char* cmd);
};

//
// IDL:go.psi.de/idl/IConnection:1.0
//
class IConnection : virtual public OBStubImpl_idl::IConnection,
                    virtual public OB::DirectStubImpl
{
    IConnection(const IConnection&);
    void operator=(const IConnection&);

protected:

#ifdef HAVE_VCPLUSPLUS_BUGS

    IConnection() { }

#endif

    IConnection(PortableServer::POA_ptr,
                const PortableServer::ObjectId&,
                PortableServer::ServantBase*);

    friend class POA_idl::IConnection;

public:

    //
    // IDL:go.psi.de/idl/IConnection/SQLAllocStmt:1.0
    //
    virtual ::idl::IStmt_ptr SQLAllocStmt();

    //
    // IDL:go.psi.de/idl/IConnection/logout:1.0
    //
    virtual void logout();

    //
    // IDL:go.psi.de/idl/IConnection/ping:1.0
    //
    virtual void ping();
};

//
// IDL:go.psi.de/idl/IStmt:1.0
//
class IStmt : virtual public OBStubImpl_idl::IStmt,
              virtual public OB::DirectStubImpl
{
    IStmt(const IStmt&);
    void operator=(const IStmt&);

protected:

#ifdef HAVE_VCPLUSPLUS_BUGS

    IStmt() { }

#endif

    IStmt(PortableServer::POA_ptr,
          const PortableServer::ObjectId&,
          PortableServer::ServantBase*);

    friend class POA_idl::IStmt;

public:

    //
    // IDL:go.psi.de/idl/IStmt/ping:1.0
    //
    virtual void ping();

    //
    // IDL:go.psi.de/idl/IStmt/exec:1.0
    //
    virtual void exec(const char* cmd);

    //
    // IDL:go.psi.de/idl/IStmt/destroy:1.0
    //
    virtual void destroy();

    //
    // IDL:go.psi.de/idl/IStmt/desc:1.0
    //
    virtual void desc(::idl::typHeader& header);

    //
    // IDL:go.psi.de/idl/IStmt/SQLTables:1.0
    //
    virtual ::idl::RETN* SQLTables(const char* catalog,
                                   const char* schema,
                                   const char* table,
                                   const char* type);

    //
    // IDL:go.psi.de/idl/IStmt/SQLColumns:1.0
    //
    virtual ::idl::RETN* SQLColumns(const char* catalog,
                                    const char* schema,
                                    const char* table,
                                    const char* column);

    //
    // IDL:go.psi.de/idl/IStmt/SQLSpecialColumns:1.0
    //
    virtual ::idl::RETN* SQLSpecialColumns(const char* catalog,
                                           const char* schema,
                                           const char* table,
                                           ::CORBA::Short scope,
                                           ::CORBA::Short nullable);

    //
    // IDL:go.psi.de/idl/IStmt/SQLStatistics:1.0
    //
    virtual ::idl::RETN* SQLStatistics(const char* catalog,
                                       const char* schema,
                                       const char* table,
                                       ::CORBA::Short nUnique,
                                       ::CORBA::Short nReserved);

    //
    // IDL:go.psi.de/idl/IStmt/SQLParams:1.0
    //
    virtual ::idl::RETN* SQLParams(const ::idl::typParamset& params);

    //
    // IDL:go.psi.de/idl/IStmt/SQLPrepare:1.0
    //
    virtual ::idl::RETN* SQLPrepare(const char* sql);

    //
    // IDL:go.psi.de/idl/IStmt/SQLExecute:1.0
    //
    virtual ::idl::RETN* SQLExecute();

    //
    // IDL:go.psi.de/idl/IStmt/SQLNumResultCols:1.0
    //
    virtual ::idl::RETN* SQLNumResultCols(::CORBA::Short_out columns);

    //
    // IDL:go.psi.de/idl/IStmt/SQLGetTypeInfo:1.0
    //
    virtual ::idl::RETN* SQLGetTypeInfo(::CORBA::Short datatyp);

    //
    // IDL:go.psi.de/idl/IStmt/SQLFetch:1.0
    //
    virtual ::idl::RETN* SQLFetch(::idl::ULONG iRow,
                                  ::idl::ULONG nRow,
                                  ::idl::typRecord_out record);

    //
    // IDL:go.psi.de/idl/IStmt/SQLDescribeParams:1.0
    //
    virtual ::idl::RETN* SQLDescribeParams(::idl::typParamset& params);

    //
    // IDL:go.psi.de/idl/IStmt/SQLFetchRef:1.0
    //
    virtual ::idl::RETN* SQLFetchRef(::idl::ULONG iRow,
                                     ::idl::ULONG nRow,
                                     ::idl::typRecord& record);

    //
    // IDL:go.psi.de/idl/IStmt/SQLColAttribute:1.0
    //
    virtual ::idl::RETN* SQLColAttribute(::CORBA::UShort col,
                                         ::CORBA::UShort attr,
                                         ::idl::typValue_out value);

    //
    // IDL:go.psi.de/idl/IStmt/SQLDescribeCol:1.0
    //
    virtual ::idl::RETN* SQLDescribeCol(::CORBA::UShort col,
                                        ::CORBA::String_out name,
                                        ::CORBA::Short_out datatype,
                                        ::CORBA::ULong_out columnsize,
                                        ::CORBA::Short_out decimaldigits,
                                        ::CORBA::Short_out nullable);
};

} // End of namespace OBDirectStubImpl_idl

#endif