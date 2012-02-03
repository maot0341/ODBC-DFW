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
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdx/debug.h>
#include "CHandle.h"

using namespace stdx;
//---------------------------------------------------------------------------
#define INFON(i,t,v) \
if (nInfoType == (i)) \
{ \
	*(t*)pInfoValue = (v); \
	if (pStringLength) \
		*pStringLength = sizeof(t);	\
	return SQL_SUCCESS; \
}
//---------------------------------------------------------------------------
#define INFOS(i,v) \
if (nInfoType == (i)) \
{ \
	strncpy ((char*)pInfoValue, (const char*)(v), nBufferLength); \
	((char*)pInfoValue)[nBufferLength-1] = 0; \
	if (pStringLength) \
		*pStringLength = strlen((const char*)v);	\
	return SQL_SUCCESS; \
}
//---------------------------------------------------------------------------
SQLRETURN SQL_API
SQLGetInfo (SQLHDBC hdbc, SQLUSMALLINT nInfoType, SQLPOINTER pInfoValue
, SQLSMALLINT nBufferLength, SQLSMALLINT *pStringLength)
{
	TRACELN ("SQLGetInfo");
	if (!hdbc)
		return SQL_INVALID_HANDLE;
	if (!pInfoValue)
		return SQL_ERROR;

	INFOS (SQL_ACCESSIBLE_PROCEDURES, "N");
	INFOS (SQL_ACCESSIBLE_TABLES, "Y");
	INFON (SQL_ACTIVE_ENVIRONMENTS, SQLUSMALLINT, 0);
	INFON (SQL_AGGREGATE_FUNCTIONS, SQLUINTEGER, SQL_AF_ALL);
	INFON (SQL_ALTER_DOMAIN, SQLUINTEGER, 0);
	INFON (SQL_ALTER_TABLE, SQLUINTEGER, 0);
	INFON (SQL_ALTER_TABLE, SQLUINTEGER, 0);
	INFON (SQL_ASYNC_MODE, SQLUINTEGER, SQL_AM_NONE);
	INFON (SQL_BATCH_ROW_COUNT, SQLUINTEGER, 0);
	INFON (SQL_BATCH_SUPPORT, SQLUINTEGER, 0);
	INFON (SQL_BOOKMARK_PERSISTENCE, SQLUINTEGER, 0);
	INFON (SQL_CATALOG_LOCATION, SQLUSMALLINT, SQL_CL_START);
	INFOS (SQL_CATALOG_NAME, "N");
	INFOS (SQL_CATALOG_NAME_SEPARATOR, ".");
	INFOS (SQL_CATALOG_TERM, "database");
	INFON (SQL_CATALOG_USAGE, SQLUINTEGER, SQL_CU_DML_STATEMENTS);
	INFOS (SQL_COLLATION_SEQ, "ISO 8859-1");
	INFOS (SQL_COLUMN_ALIAS, "Y");
	INFON (SQL_CONCAT_NULL_BEHAVIOR, SQLUSMALLINT, SQL_CB_NULL);
/*
	INFON (SQL_CONVERT_BIGINT, SQLUINTEGER, SQL_CVT_BIGINT);
	...
	INFON (SQL_CONVERT_VARCHAR, SQLUINTEGER, SQL_CVT_VARCHAR);
*/
//	INFON (SQL_CONVERT_FUNCTIONS, SQLUINTEGER, SQL_FN_CVT_CAST | SQL_FN_CVT_CONVERT);
	INFON (SQL_CONVERT_FUNCTIONS, SQLUINTEGER, 0);
	INFON (SQL_CORRELATION_NAME, SQLUSMALLINT, SQL_CN_ANY);
	INFON (SQL_CREATE_ASSERTION, SQLUINTEGER, 0);
	INFON (SQL_CREATE_CHARACTER_SET, SQLUINTEGER, 0);
	INFON (SQL_CREATE_COLLATION, SQLUINTEGER, 0);
	INFON (SQL_CREATE_DOMAIN, SQLUINTEGER, 0);
	INFON (SQL_CREATE_SCHEMA, SQLUINTEGER, 0);
	INFON (SQL_CREATE_TABLE, SQLUINTEGER, 0);
	INFON (SQL_CREATE_TRANSLATION, SQLUINTEGER, 0);
	INFON (SQL_CREATE_VIEW, SQLUINTEGER, 0);
	INFON (SQL_CURSOR_COMMIT_BEHAVIOR, SQLUSMALLINT, SQL_CB_DELETE);
	INFON (SQL_CURSOR_ROLLBACK_BEHAVIOR, SQLUSMALLINT, SQL_CB_DELETE);
	INFON (SQL_CURSOR_SENSITIVITY, SQLUINTEGER, SQL_INSENSITIVE);
	INFOS (SQL_DATA_SOURCE_NAME, "ODBC Framework Source");
	INFOS (SQL_DATA_SOURCE_READ_ONLY, "Y");
	INFOS (SQL_DATABASE_NAME, "database");
	INFON (SQL_DATETIME_LITERALS, SQLUINTEGER
	, SQL_DL_SQL92_DATE 
	| SQL_DL_SQL92_TIME 
	| SQL_DL_SQL92_TIMESTAMP);
	INFOS (SQL_DBMS_NAME, "ODBC Framework DB");
	INFOS (SQL_DBMS_VER, "v1.1.00");
	INFON (SQL_DDL_INDEX, SQLUINTEGER, 0);
	INFON (SQL_DEFAULT_TXN_ISOLATION, SQLUINTEGER, 0);
	INFOS (SQL_DESCRIBE_PARAMETER, "Y");
//	INFOS (SQL_DM_VER, "");
	INFOS (SQL_DRIVER_NAME, "ODBC Template");
	INFOS (SQL_DRIVER_ODBC_VER, CDriver::driver_odbc_ver());
	INFOS (SQL_DRIVER_VER, "01.01.0000");
	INFON (SQL_DROP_ASSERTION, SQLUINTEGER, 0);
	INFON (SQL_DROP_CHARACTER_SET, SQLUINTEGER, 0);
	INFON (SQL_DROP_COLLATION, SQLUINTEGER, 0);
	INFON (SQL_DROP_DOMAIN, SQLUINTEGER, 0);
	INFON (SQL_DROP_SCHEMA, SQLUINTEGER, 0);
	INFON (SQL_DROP_TABLE, SQLUINTEGER, 0);
	INFON (SQL_DROP_TRANSLATION, SQLUINTEGER, 0);
	INFON (SQL_DROP_VIEW, SQLUINTEGER, 0);
	INFON (SQL_DYNAMIC_CURSOR_ATTRIBUTES1, SQLUINTEGER, 0);
	INFON (SQL_DYNAMIC_CURSOR_ATTRIBUTES2, SQLUINTEGER, 0);
	INFOS (SQL_EXPRESSIONS_IN_ORDERBY, "Y");
	INFON (SQL_FILE_USAGE, SQLUSMALLINT, SQL_FILE_NOT_SUPPORTED);
	INFON (SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1, SQLUINTEGER, SQL_CA1_NEXT);
	INFON (SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1, SQLUINTEGER, SQL_CA2_READ_ONLY_CONCURRENCY);
	INFON (SQL_GETDATA_EXTENSIONS, SQLUINTEGER
	, SQL_GD_ANY_COLUMN 
	| SQL_GD_ANY_ORDER 
	| SQL_GD_BOUND);
	INFON (SQL_GROUP_BY, SQLUSMALLINT, SQL_GB_GROUP_BY_CONTAINS_SELECT);
	INFON (SQL_IDENTIFIER_CASE, SQLUSMALLINT, SQL_IC_MIXED);
	INFOS (SQL_IDENTIFIER_QUOTE_CHAR, "\"");
	INFON (SQL_INDEX_KEYWORDS, SQLUINTEGER, SQL_IK_ALL);
	INFON (SQL_INFO_SCHEMA_VIEWS, SQLUINTEGER, 0);
	INFON (SQL_INSERT_STATEMENT, SQLUINTEGER, 0);
	INFOS (SQL_INTEGRITY, "N");
	INFON (SQL_KEYSET_CURSOR_ATTRIBUTES1, SQLUINTEGER, 0);
	INFON (SQL_KEYSET_CURSOR_ATTRIBUTES2, SQLUINTEGER, 0);
	INFOS (SQL_KEYWORDS, SQL_ODBC_KEYWORDS);
	INFOS (SQL_LIKE_ESCAPE_CLAUSE, "N");
	INFON (SQL_MAX_ASYNC_CONCURRENT_STATEMENTS, SQLUINTEGER, 0);
	INFON (SQL_MAX_BINARY_LITERAL_LEN, SQLUINTEGER, 0);
	INFON (SQL_MAX_CATALOG_NAME_LEN, SQLUSMALLINT, 128);
	INFON (SQL_MAX_CHAR_LITERAL_LEN, SQLUINTEGER, 0);
	INFON (SQL_MAX_COLUMN_NAME_LEN, SQLUSMALLINT, 128);
	INFON (SQL_MAX_COLUMNS_IN_GROUP_BY, SQLUSMALLINT, 0);
	INFON (SQL_MAX_COLUMNS_IN_INDEX, SQLUSMALLINT, 0);
	INFON (SQL_MAX_COLUMNS_IN_ORDER_BY, SQLUSMALLINT, 0);
	INFON (SQL_MAX_COLUMNS_IN_SELECT, SQLUSMALLINT, 0);
	INFON (SQL_MAX_COLUMNS_IN_TABLE, SQLUSMALLINT, 0);
	INFON (SQL_MAX_CONCURRENT_ACTIVITIES, SQLUSMALLINT, 0);
	INFON (SQL_MAX_CURSOR_NAME_LEN, SQLUSMALLINT, 128);
	INFON (SQL_MAX_DRIVER_CONNECTIONS, SQLUSMALLINT, 0);
	INFON (SQL_MAX_IDENTIFIER_LEN, SQLUSMALLINT, 128);
	INFON (SQL_MAX_INDEX_SIZE, SQLUINTEGER, 0);
	INFON (SQL_MAX_PROCEDURE_NAME_LEN, SQLUSMALLINT, 0);
	INFON (SQL_MAX_ROW_SIZE, SQLUINTEGER, 0);
	INFOS (SQL_MAX_ROW_SIZE_INCLUDES_LONG, "Y");
	INFON (SQL_MAX_SCHEMA_NAME_LEN, SQLUSMALLINT, 128);
	INFON (SQL_MAX_STATEMENT_LEN, SQLUINTEGER, 128);
	INFON (SQL_MAX_TABLE_NAME_LEN, SQLUSMALLINT, 128);
	INFON (SQL_MAX_TABLES_IN_SELECT, SQLUSMALLINT, 50);
	INFON (SQL_MAX_USER_NAME_LEN, SQLUSMALLINT, 0);
	INFOS (SQL_MULT_RESULT_SETS, "N");
	INFOS (SQL_MULTIPLE_ACTIVE_TXN, "N");
	INFOS (SQL_NEED_LONG_DATA_LEN, "Y");
	INFON (SQL_NON_NULLABLE_COLUMNS, SQLUSMALLINT, SQL_NNC_NON_NULL);
	INFON (SQL_NULL_COLLATION, SQLUSMALLINT, SQL_NC_START);
	INFON (SQL_NUMERIC_FUNCTIONS, SQLUINTEGER, SQL_FN_NUM_ABS|SQL_FN_NUM_ROUND);
	INFON (SQL_ODBC_INTERFACE_CONFORMANCE, SQLUINTEGER, SQL_OIC_CORE);
	INFON (SQL_OJ_CAPABILITIES, SQLUINTEGER, SQL_OJ_LEFT | SQL_OJ_RIGHT | SQL_OJ_FULL);
	INFOS (SQL_ORDER_BY_COLUMNS_IN_SELECT, "N");
	INFON (SQL_PARAM_ARRAY_ROW_COUNTS, SQLUINTEGER, SQL_PARC_NO_BATCH);
	INFON (SQL_PARAM_ARRAY_SELECTS, SQLUINTEGER, SQL_PAS_NO_SELECT);
	INFOS (SQL_PROCEDURE_TERM, "procedure");
	INFOS (SQL_PROCEDURES, "N");
	INFON (SQL_QUOTED_IDENTIFIER_CASE, SQLUSMALLINT, SQL_IC_MIXED);
	INFOS (SQL_ROW_UPDATES, "N");
	INFOS (SQL_SCHEMA_TERM, "owner");
	INFON (SQL_SCHEMA_USAGE, SQLUINTEGER
	, SQL_SU_DML_STATEMENTS 
	| SQL_SU_TABLE_DEFINITION 
	| SQL_SU_PRIVILEGE_DEFINITION);
//	INFON (SQL_SCROLL_OPTIONS, SQLUINTEGER, SQL_SO_FORWARD_ONLY | SQL_SO_STATIC)
	INFON (SQL_SCROLL_OPTIONS, SQLUINTEGER
	, SQL_SO_FORWARD_ONLY 
	| SQL_SO_STATIC 
	| SQL_SO_DYNAMIC 
	| SQL_SO_MIXED 
	| SQL_SO_KEYSET_DRIVEN);
	INFOS (SQL_SEARCH_PATTERN_ESCAPE, "\\");
	INFOS (SQL_SERVER_NAME, "");
	INFOS (SQL_SPECIAL_CHARACTERS, "\"'");
	INFON (SQL_SQL_CONFORMANCE, SQLUINTEGER, SQL_SC_SQL92_ENTRY);
	INFON (SQL_SQL92_DATETIME_FUNCTIONS, SQLUINTEGER
	, SQL_SDF_CURRENT_DATE 
	| SQL_SDF_CURRENT_TIME 
	| SQL_SDF_CURRENT_TIMESTAMP);
	INFON (SQL_SQL92_FOREIGN_KEY_DELETE_RULE, SQLUINTEGER
	, SQL_SFKD_CASCADE 
	| SQL_SFKD_NO_ACTION 
	| SQL_SFKD_SET_DEFAULT 
	| SQL_SFKD_SET_NULL);
	INFON (SQL_SQL92_FOREIGN_KEY_UPDATE_RULE, SQLUINTEGER
	, SQL_SFKU_CASCADE 
	| SQL_SFKU_NO_ACTION 
	| SQL_SFKU_SET_DEFAULT 
	| SQL_SFKU_SET_NULL);
	INFON (SQL_SQL92_GRANT, SQLUINTEGER, 0);
	INFON (SQL_SQL92_NUMERIC_VALUE_FUNCTIONS, SQLUINTEGER, 0);
	INFON (SQL_SQL92_PREDICATES, SQLUINTEGER
	, SQL_SP_BETWEEN
	| SQL_SP_COMPARISON
	| SQL_SP_EXISTS
	| SQL_SP_IN
	| SQL_SP_ISNOTNULL
	| SQL_SP_ISNULL
	| SQL_SP_LIKE
	| SQL_SP_QUANTIFIED_COMPARISON
	| SQL_SP_UNIQUE);
	INFON (SQL_SQL92_RELATIONAL_JOIN_OPERATORS, SQLUINTEGER, 0);
	INFON (SQL_SQL92_REVOKE, SQLUINTEGER, 0);
	INFON (SQL_SQL92_ROW_VALUE_CONSTRUCTOR, SQLUINTEGER
	, SQL_SRVC_VALUE_EXPRESSION 
	| SQL_SRVC_NULL 
	| SQL_SRVC_DEFAULT);
	INFON (SQL_SQL92_STRING_FUNCTIONS, SQLUINTEGER
	, SQL_SSF_LOWER 
	| SQL_SSF_UPPER 
	| SQL_SSF_SUBSTRING);
	INFON (SQL_SQL92_VALUE_EXPRESSIONS, SQLUINTEGER, 0);
	INFON (SQL_STANDARD_CLI_CONFORMANCE, SQLUINTEGER
	, SQL_SCC_XOPEN_CLI_VERSION1 
	| SQL_SCC_ISO92_CLI);
	INFON (SQL_STATIC_CURSOR_ATTRIBUTES1, SQLUINTEGER, 0);
	INFON (SQL_STATIC_CURSOR_ATTRIBUTES2, SQLUINTEGER, 0);
	INFON (SQL_STRING_FUNCTIONS, SQLUINTEGER, 0);
	INFON (SQL_SUBQUERIES, SQLUINTEGER, 0);
	INFON (SQL_SYSTEM_FUNCTIONS, SQLUINTEGER, 0);
	INFOS (SQL_TABLE_TERM, "table");
	INFON (SQL_TIMEDATE_ADD_INTERVALS, SQLUINTEGER, 0);
	INFON (SQL_TIMEDATE_DIFF_INTERVALS, SQLUINTEGER, 0);
	INFON (SQL_TIMEDATE_FUNCTIONS, SQLUINTEGER, 0);
	INFON (SQL_TXN_CAPABLE, SQLUSMALLINT, SQL_TC_NONE);
	INFON (SQL_TXN_ISOLATION_OPTION, SQLUINTEGER, SQL_TXN_SERIALIZABLE);
	INFON (SQL_UNION, SQLUINTEGER, SQL_U_UNION | SQL_U_UNION_ALL);
	INFOS (SQL_USER_NAME, "<user>");
	INFOS (SQL_XOPEN_CLI_YEAR, "<unknown>");


	INFOS (SQL_OUTER_JOINS, "Y");
	INFON (SQL_OJ_CAPABILITIES || nInfoType == 65003, SQLUINTEGER, 0);
	INFON (SQL_FETCH_DIRECTION, SQLINTEGER, SQL_FD_FETCH_NEXT);
	INFON (SQL_LOCK_TYPES, SQLINTEGER, 0);
	INFON (SQL_ODBC_API_CONFORMANCE, SQLSMALLINT, SQL_OAC_LEVEL1);
	INFON (SQL_ODBC_SQL_CONFORMANCE, SQLSMALLINT, SQL_OSC_CORE);
//	INFON (SQL_ODBC_SQL_CONFORMANCE, SQLUINTEGER, SQL_SC_SQL92_ENTRY);
	INFON (SQL_POS_OPERATIONS, SQLINTEGER, 0);
	INFON (SQL_POSITIONED_STATEMENTS, SQLINTEGER, 0);
	INFON (SQL_SCROLL_CONCURRENCY, SQLINTEGER, SQL_SCCO_READ_ONLY);
	INFON (SQL_STATIC_SENSITIVITY, SQLINTEGER, 0);
	INFOS (SQL_SPECIAL_CHARACTERS, "\"'");
	INFOS (SQL_SEARCH_PATTERN_ESCAPE, "");
//---------------
return SQL_ERROR;
//---------------

	if (nInfoType == SQL_ODBC_API_CONFORMANCE)
	{
		*(SQLSMALLINT *)pInfoValue = SQL_OAC_LEVEL1;
		if (pStringLength)
			*pStringLength = sizeof(SQLSMALLINT);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 7 -- SQL_DRIVER_VER
	////-----------------------------------------
	if (nInfoType == SQL_DRIVER_VER)
	{
		const char * szValue = "01.01.0000";
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	if (nInfoType == SQL_DRIVER_ODBC_VER)
	{
//		const char * szValue = CDriver::driver_odbc_ver();
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;

	}
	////-----------------------------------------
	//// 2 -- SQL_DATA_SOURCE_NAME
	////-----------------------------------------
	if (nInfoType == SQL_DATA_SOURCE_NAME)
	{
		const char * szValue = "ODBC Test";
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 23
	////-----------------------------------------
	if (nInfoType == SQL_CURSOR_COMMIT_BEHAVIOR)
	{
//		SetShort (0, pInfoValue);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 24
	////-----------------------------------------
	if (nInfoType == SQL_CURSOR_ROLLBACK_BEHAVIOR)
	{
//		SetShort (0, pInfoValue);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 1
	////-----------------------------------------
	if (nInfoType == SQL_ACTIVE_STATEMENTS)
	{
//		SetShort (1, pInfoValue);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 25
	////-----------------------------------------
	if (nInfoType == SQL_DATA_SOURCE_READ_ONLY)
	{
//		SetString ("Y", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 6
	////-----------------------------------------
	if (nInfoType == SQL_DRIVER_NAME)
	{
//		SetString ("Klausi", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 14
	////-----------------------------------------
	if (nInfoType == SQL_SEARCH_PATTERN_ESCAPE)
	{
//		SetString ("\\", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_IDENTIFIER_QUOTE_CHAR)
	{
//		SetString ("\"", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 38
	////-----------------------------------------
	if (nInfoType == SQL_OUTER_JOINS)
	{
//		SetString ("Y", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	//// 115
	////-----------------------------------------
	if (nInfoType == SQL_OJ_CAPABILITIES)
	{
		*((SQLUINTEGER*)pInfoValue) = SQL_OJ_LEFT | SQL_OJ_RIGHT | SQL_OJ_FULL;
		return SQL_SUCCESS;
	}
	////-----------------------------------------
	if (nInfoType == SQL_TXN_CAPABLE)
	{
		*(SQLUSMALLINT*)pInfoValue = SQL_TC_NONE;
		if (pStringLength)
			*pStringLength = sizeof(SQLUSMALLINT);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_DBMS_NAME)
	{
//		SetString ("ODBC-Test", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_ACTIVE_CONNECTIONS)
	{
		*(SQLUSMALLINT*)pInfoValue = 2;
		if (pStringLength)
			*pStringLength = sizeof(SQLUSMALLINT);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_STRING_FUNCTIONS)
	{
		*(SQLUINTEGER *)pInfoValue = 0;
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_NUMERIC_FUNCTIONS)
	{
		*(SQLUINTEGER *)pInfoValue = 0;
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_TIMEDATE_FUNCTIONS)
	{
		*(SQLUINTEGER *)pInfoValue = 0;
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_SYSTEM_FUNCTIONS)
	{
		*(SQLUINTEGER *)pInfoValue = 0;
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_CONVERT_FUNCTIONS)
	{
		*(SQLUINTEGER *)pInfoValue = 0;
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_OJ_CAPABILITIES || nInfoType == 65003)
	{
		*(SQLUINTEGER *)pInfoValue = 0;
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_EXPRESSIONS_IN_ORDERBY)
	{
//		SetString ("Y", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_ORDER_BY_COLUMNS_IN_SELECT)
	{
//		SetString ("Y", pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_CONCAT_NULL_BEHAVIOR)
	{
		*(SQLUSMALLINT*)pInfoValue = SQL_CB_NULL; // SQL_CB_NON_NULL 
		if (pStringLength)
			*pStringLength = sizeof(SQLUSMALLINT);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_ODBC_SQL_CONFORMANCE)
	{
		*(SQLUINTEGER*)pInfoValue = SQL_SC_SQL92_ENTRY;
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_SCROLL_OPTIONS)
	{
		*(SQLUINTEGER*)pInfoValue = SQL_SO_DYNAMIC | SQL_SO_STATIC;
		memset (pInfoValue, 0xff, sizeof(SQLUINTEGER));
		if (pStringLength)
			*pStringLength = sizeof(SQLUINTEGER);
		return SQL_SUCCESS;
	}
//---------------
return SQL_ERROR;
//---------------
	if (nInfoType == SQL_DRIVER_NAME)
	{
		const char * szValue = "Klausi";
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_SEARCH_PATTERN_ESCAPE)
	{
		const char * szValue = "\\";
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}
	if (nInfoType == SQL_CATALOG_TERM)
	{
		const char * szValue = "database";
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;

	}
	if (nInfoType == SQL_DATABASE_NAME)
	{
		const char * szValue = "database";
//		SetString (szValue, pInfoValue, nBufferLength, pStringLength);
		return SQL_SUCCESS;
	}

	return SQL_ERROR;
}
//---------------------------------------------------------------------------
