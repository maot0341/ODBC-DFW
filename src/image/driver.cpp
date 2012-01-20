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

#include <stdio.h>
#include <assert.h>
#include <crtdbg.h>
#include <time.h>

#include <string>
#include <map>
#include "driver.h"
#include "database.h"
#include "CTableIMG.h"
#include "export.h"


using namespace std;
//---------------------------------------------------------------------------
// Main -- DLL-Initialisierung
//---------------------------------------------------------------------------
BOOL WINAPI DllMain (HINSTANCE hInstanceDLL, DWORD dwReason, LPVOID pReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH || dwReason == DLL_THREAD_ATTACH)
	{
//		_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
	}
//	if (dwReason == DLL_PROCESS_DETACH || dwReason == DLL_THREAD_DETACH)
	if (dwReason == DLL_PROCESS_DETACH)
	{
		//_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
//		_CrtDumpMemoryLeaks();
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);
//		_CrtCheckMemory();
		_CrtSetDbgFlag (_CRTDBG_LEAK_CHECK_DF);
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// Test Umgebung:
// D:\jvater\odbc test\OdbcTE32.Exe
//---------------------------------------------------------------------------
bool 
SetString (const char * szValue, SQLPOINTER pBuffer, SQLSMALLINT nSize, SQLSMALLINT * pLength)
{
	if (!pBuffer) return false;
	if (!pLength) return false;
	if (!szValue) szValue = "";
	char * szBuffer = (char*)pBuffer;
	const int nLength = strlen (szValue);
	strncpy (szBuffer, szValue, nSize);
	szBuffer [nSize - 1] = 0;
	*pLength = strlen (szBuffer);
	return (nLength < nSize);
};
//---------------------------------------------------------------------------
bool SetShort (int nValue, SQLPOINTER pBuffer, SQLSMALLINT nSize)
{
	if (!pBuffer) return false;
	if (nSize != sizeof(SQLUSMALLINT)) return false;
	*((SQLUSMALLINT*) pBuffer) = (SQLUSMALLINT) nValue;
	return true;
}
//---------------------------------------------------------------------------
// DRIVER
//---------------------------------------------------------------------------
CDriver CDriver::ms_aInstance;
//---------------------------------------------------------------------------
CDriver::~CDriver()
{
}
//---------------------------------------------------------------------------
const char * CDriver::driver_odbc_ver()
{
	return "02.00";
	return "03.00";
	return SQL_SPEC_STRING;
}
//---------------------------------------------------------------------------
int CDriver::sql_type (term_t eType)
{
	int nType 
	= eType == TermBOOL  ? SQL_BIT
	: eType == TermINT   ? SQL_INTEGER
	: eType == TermFLOAT ? SQL_FLOAT
//	: eType == TermTIME  ? SQL_DATETIME
//	: eType == TermTIME  ? SQL_TYPE_DATE
	: eType == TermTIME  ? SQL_TYPE_TIMESTAMP
	: eType == TermVCHAR ? SQL_CHAR
//	: eType == TermVCHAR ? SQL_VARCHAR
	: SQL_UNKNOWN_TYPE;
	return nType;
}
//---------------------------------------------------------------------------
// ENVIRONMENT
//---------------------------------------------------------------------------
CEnvironment::CEnvironment (CDriver * paDriver)
: CHandle (paDriver)
{
}
//---------------------------------------------------------------------------
CEnvironment::~CEnvironment()
{
}
//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
CDatabase::CDatabase (CEnvironment * paEnv)
: IDatabase (paEnv)
{
	assert (paEnv);
}
//---------------------------------------------------------------------------
CDatabase::~CDatabase()
{
}
//---------------------------------------------------------------------------
CTable * 
CDatabase::table (const char * szCatalog, const char * szSchema, const char * szName)
{
	CTable * pTable = 0;
	if (stricmp(szName, "reg") == 0)
		pTable = 0;
	else if (strlen(szName) == 1)
		pTable = new CTableTST (szName);
	else if (strcmp(szName, "test") == 0)
		pTable = new CTableTST (szName);
	else if (strncmp (szName, "iptc@", 5) == 0)
		pTable = new CTableIMG (szName+5);
	else
		throw MSG("42S02", __LINE__, "Base table or view [%s] not found", szName);
	ASSUME (pTable);
	return pTable;
//	return new CTableTST (name);
}
//---------------------------------------------------------------------------
CResult 
CDatabase::query (const char * sql)
{
	return CResult(0);
}
//---------------------------------------------------------------------------
CResult 
CDatabase::columns (const char * table)
{
	return CResult(0);
}
//---------------------------------------------------------------------------
CResult 
CDatabase::type_info (int datatype)
{
	return CResult(0);
}
//---------------------------------------------------------------------------
CResult 
CDatabase::tables (const char * filter)
{
	return CResult(0);
}
//---------------------------------------------------------------------------
class CStmtDesc : public CStmtData
{
public:
	CStmtDesc (CStmtHandle * h, const char * table);
	virtual ~CStmtDesc();
	short fields() { return m_aFields.size(); }
	CField & fields (short index);
	virtual SQLRETURN close() { return SQL_SUCCESS; }

	virtual SQLRETURN getData (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	);

	virtual SQLRETURN bind (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	);

	virtual SQLRETURN fetch();

	CTable * m_pTable;
	static const typColumn ms_aItems[];
	vector<CField> m_aFields;
	size_t m_iRecord;
	size_t m_nRecord;
};
//---------------------------------------------------------------------------
void
CDatabase::desc (CStmtHandle * h, const char * table)
{
	*h = new CStmtDesc (h, table);
//	*h = new CStmtColumns (h);
}
//---------------------------------------------------------------------------
const typColumn CStmtDesc::ms_aItems[] =
{ {"TABLE_CAT", SQL_CHAR}
, {"TABLE_SCHEM", SQL_CHAR}
, {"TABLE_NAME", SQL_CHAR}
, {"COLUMN_NAME", SQL_CHAR}
, {"DATA_TYPE", SQL_SMALLINT}
, {"TYPE_NAME", SQL_CHAR}
, {"COLUMN_SIZE", SQL_INTEGER}
, {"BUFFER_LENGTH", SQL_INTEGER}
, {"DECIMAL_DIGITS", SQL_SMALLINT}
, {"NUM_PREC_RADIX", SQL_SMALLINT}
, {"NULLABLE", SQL_SMALLINT}
, {"REMARKS", SQL_CHAR}
, {"COLUMN_DEF", SQL_CHAR}
, {"SQL_DATA_TYPE", SQL_SMALLINT}
, {"SQL_DATETIME_SUB", SQL_SMALLINT}
, {"CHAR_OCTET_LENGTH", SQL_INTEGER}
, {"ORDINAL_POSITION", SQL_INTEGER}
, {"IS_NULLABLE", SQL_CHAR}
};
//---------------------------------------------------------------------------
CStmtDesc::CStmtDesc(CStmtHandle * h, const char * szTable)
: CStmtData (h)
, m_aFields(DIM(ms_aItems))
{
	const size_t n = m_aFields.size();
	size_t i;
	for (i=0; i<n; i++)
	{
		short nSQLType = ms_aItems[i].type;
		size_t nSize = m_aFields[i].displaysize (nSQLType);
		m_aFields[i].pszSQL_DESC_NAME = strdup (ms_aItems[i].name);
		m_aFields[i].nSQL_DESC_TYPE = nSQLType;
		m_aFields[i].nSQL_DESC_DISPLAY_SIZE = nSize;
	}
	m_iRecord = 0;
	CDatabase * pDatabase = h->Database();
	m_pTable = pDatabase->table ("", "", szTable);
	m_nRecord = m_pTable->cols();
}
//---------------------------------------------------------------------------
CStmtDesc::~CStmtDesc()
{
	delete m_pTable;
}
//---------------------------------------------------------------------------
CField &
CStmtDesc::fields (short index)
{
	size_t i = index;
	assert (i < fields());
	return m_aFields[i];
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtDesc::fetch()
{
	if (m_iRecord >= m_nRecord)
		return SQL_NO_DATA;

	CValue aType = m_pTable->desc(SQL_DESC_TYPE, m_iRecord);
	CValue aName = m_pTable->desc(SQL_DESC_NAME, m_iRecord);
	const int nType = aType.asInteger();
	const int nTypeInfo = CStmtTypeInfo::ms_nRecord;
	int i;
	for (i=0; i<nTypeInfo; i++)
	{
		const CStmtTypeInfo::record_t & aRecord = CStmtTypeInfo::ms_aData[i];
		if (aRecord.datatype == nType)
			break;
	}
	const char * szType = (i<nTypeInfo) ? CStmtTypeInfo::ms_aData[i].name : "unknown";
	const char * szName = aName.asString();
	char szColumn[256];
	for (i=0; i<strlen(szName); i++)
		szColumn[i] = toupper(szName[i]);
	szColumn[i] = 0;

	m_aFields[ 0] = m_pTable->catalog.c_str();
	m_aFields[ 1] = m_pTable->schema.c_str();
	m_aFields[ 2] = m_pTable->name.c_str();
	m_aFields[ 3] = aName;
	m_aFields[ 4] = aType;
	m_aFields[ 5] = szType;
	m_aFields[ 6] = 10;
	m_aFields[ 7] = 10;
	m_aFields[ 8] = 10;
	m_aFields[ 9] = 0.0;
	m_aFields[10] = (int) true;
	m_aFields[11] = "remarks:";
	m_aFields[12] = CValue::null;
	m_aFields[13] = aType;
	m_aFields[14] = aType;
	m_aFields[15] = CValue::null;
	m_aFields[16] = m_iRecord+1;
	m_aFields[17] = "YES";
	m_iRecord++;
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
SQLRETURN
CStmtDesc::getData (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (0 <= nCol && nCol < m_aFields.size());
	assert (pTargetVal);
	CField aField;
	aField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	const size_t iRecord = m_iRecord - 1;
	if (iRecord >= m_nRecord)
		return SQL_NO_DATA;

	CValue aType = m_pTable->desc(SQL_DESC_TYPE, iRecord);
	CValue aName = m_pTable->desc(SQL_DESC_NAME, iRecord);
	const int nType = aType.asInteger();
	const int nTypeInfo = CStmtTypeInfo::ms_nRecord;
	int i;
	for (i=0; i<nTypeInfo; i++)
	{
		const CStmtTypeInfo::record_t & aRecord = CStmtTypeInfo::ms_aData[i];
		if (aRecord.datatype == nType)
			break;
	}
	const char * szType = (i<nTypeInfo) ? CStmtTypeInfo::ms_aData[i].name : "unknown";
	const char * szName = aName.asString();
	char szColumn[256];
	for (i=0; i<strlen(szName); i++)
		szColumn[i] = toupper(szName[i]);
	szColumn[i] = 0;
	const size_t nSize = CField::displaysize (nType);

	if (nCol == 0)
		aField = ""; // catalog
	if (nCol == 1)
		aField = ""; // schema
	if (nCol == 2)
		aField = m_pTable->name.c_str();
	if (nCol == 3)
		aField = aName;
	if (nCol == 4)
		aField = aType;
	if (nCol == 5)
		aField = szType;
	if (nCol == 6)
		aField = nSize;
	if (nCol == 7)
		aField = 20;
	if (nCol == 8)
		aField = 20;
	if (nCol == 9)
		aField = 0.0;
	if (nCol == 10)
		aField = (int) true;
	if (nCol == 11)
		aField = "remarks:";
	if (nCol == 12)
		aField = CValue::null;
	if (nCol ==13)
		aField = aType;
	if (nCol == 14)
		aField = aType;
	if (nCol == 15)
		aField = CValue::null;
	if (nCol == 16)
		aField = m_iRecord+1;
	if (nCol == 17)
		aField = "YES";

	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtDesc::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < m_aFields.size());
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
#if 0
	raField.m_nTargetTyp = nTargetTyp;
	raField.m_pTargetVal = (char*)pTargetVal;
	raField.m_nTargetLen = nTargetLen;
	raField.m_pTargetInd = pnTargetInd;
#endif
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
// STATEMENT
//---------------------------------------------------------------------------
CStmtHandle::CStmtHandle (CDatabase * paDatabase)
: CHandle (paDatabase)
{
	assert (paDatabase);
	m_paResult = 0;
}
//---------------------------------------------------------------------------
CStmtHandle::~CStmtHandle()
{
	delete m_paResult;
}
//---------------------------------------------------------------------------
CStmtHandle &
CStmtHandle::operator= (CStmtData * paData)
{
	delete m_paResult;
	m_paResult = paData;
	return *this;
}
//---------------------------------------------------------------------------
CStmtData*
CStmtHandle::ptr (SQLHSTMT hstmt)
{
	CStmtHandle * paStmt = (CStmtHandle*) hstmt;
	if (!paStmt) return 0;
	return paStmt->m_paResult;
}
//---------------------------------------------------------------------------
CStmtHandle&
CStmtHandle::get (SQLHSTMT hstmt)
{
	return *(CStmtHandle*) hstmt;
}
//---------------------------------------------------------------------------
// FIELD
//---------------------------------------------------------------------------
CField::CField()
{
	bSQL_DESC_AUTO_UNIQUE_VALUE = false;
	pszSQL_DESC_BASE_COLUMN_NAME = 0;
	pszSQL_DESC_BASE_TABLE_NAME = 0;
	bSQL_DESC_CASE_SENSITIVE = false;
	pszSQL_DESC_CATALOG_NAME = 0;
	nSQL_DESC_CONCISE_TYPE = 0;
	nSQL_DESC_DISPLAY_SIZE = 0;
	bSQL_DESC_FIXED_PREC_SCALE = false;
	pszSQL_DESC_LABEL = 0;
	nSQL_DESC_LENGTH = 0;
	pszSQL_DESC_LITERAL_PREFIX = 0;
	pszSQL_DESC_LITERAL_SUFFIX = 0;
	pszSQL_DESC_LOCAL_TYPE_NAME = 0;
	pszSQL_DESC_NAME = 0;
	nSQL_DESC_NULLABLE = SQL_NULLABLE;
	nSQL_DESC_NUM_PREC_RADIX = 0;
	nSQL_DESC_OCTET_LENGTH = 0;
	nSQL_DESC_PRECISION = 0;
	nSQL_DESC_SCALE = 0;
	pszSQL_DESC_SCHEMA_NAME = 0;
	nSQL_DESC_SEARCHABLE = 0;
	pszSQL_DESC_TABLE_NAME = 0;
	nSQL_DESC_TYPE = 0;
	pszSQL_DESC_TYPE_NAME = 0;
	nSQL_DESC_UNNAMED = 0;
	bSQL_DESC_UNSIGNED = false;
	nSQL_DESC_UPDATABLE = 0;

	/* BINDING INFO		*/
	bind (SQL_CHAR, 0, 0, 0);
	export = 0;
#if 0
	m_nTargetTyp = SQL_CHAR;
	m_pTargetVal = 0;
	m_nTargetLen = 0;
	m_pTargetInd = 0;
#endif
};
//---------------------------------------------------------------------------
CField::~CField()
{
	free (pszSQL_DESC_NAME);
}
//---------------------------------------------------------------------------
#if 0
CField &
CField::operator= (double dValue)
{
	if (m_nTargetTyp == SQL_C_CHAR)
		ExpCHAR (m_pTargetVal, m_nTargetLen, m_pTargetInd, dValue);
	return *this;
}
#endif
//---------------------------------------------------------------------------
CField &
CField::operator= (const CTerm & aTerm)
{
	ExportTerm (m_nTargetTyp, m_pTargetVal, m_nTargetLen, m_pTargetInd, aTerm);
	return *this;

	long * pLength = m_pTargetInd;
	int nSize = m_nTargetLen;
	if (!m_pTargetVal)
		return *this;
	if (export == 0)
	{
		term_t eTerm = aTerm.type();
		if (m_nTargetTyp == SQL_C_DEFAULT)
		{
			if (eTerm == TermBOOL || eTerm == TermINT || eTerm == TermFLOAT)
			{
				if (nSize >= sizeof(short))
					export = ExpTermSHORT;
				if (nSize >= sizeof(long))
					export = ExpTermLONG;
				if (nSize >= sizeof(double))
					export = ExpTermDOUBLE;
			}
			if (eTerm == TermVCHAR)
				export = ExpTermCHAR;
			if (eTerm == TermTIME)
				export = ExpTermTIMESTAMP;
		}
		if (m_nTargetTyp == SQL_C_CHAR)
		if (eTerm == TermINT)
			export = ExpCHAR_TermINT;
		if (m_nTargetTyp == SQL_C_CHAR)
		if (eTerm == TermFLOAT)
			export = ExpCHAR_TermFLOAT;
		if (m_nTargetTyp == SQL_C_CHAR)
		if (eTerm == TermVCHAR)
			export = ExpCHAR_TermVCHAR;
		if (m_nTargetTyp == SQL_C_CHAR)
		if (eTerm == TermTIME)
			export = ExpCHAR_TermTIME;
		if (m_nTargetTyp == SQL_C_TIMESTAMP)
		if (eTerm == TermTIME)
			export = ExpTermTIMESTAMP;
		if (m_nTargetTyp == SQL_C_LONG)
			export = ExpTermLONG;
		if (m_nTargetTyp == SQL_C_SHORT)
			export = ExpTermSHORT;
	}
	if (export)
	{
		export (m_pTargetVal, m_nTargetLen, m_pTargetInd, &aTerm);
		return *this;
	}
//	assert (false);

	if (aTerm.isNull() && m_pTargetInd)
	{
		*m_pTargetInd = SQL_NULL_DATA;
		return *this;
	}
	if (m_nTargetTyp == SQL_C_BIT)
	{
		unsigned char * pField = (unsigned char *)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_LONG)
	{
		long* pField = (long*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_ULONG)
	{
		unsigned long* pField = (unsigned long*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_USHORT)
	{
		unsigned short* pField = (unsigned short*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_SLONG)
	{
		signed long* pField = (signed long*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_SSHORT)
	{
		signed short* pField = (signed short*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_FLOAT)
	{
		float* pField = (float*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_DOUBLE)
	{
		double* pField = (double*)m_pTargetVal;
		if (pLength)
			*pLength = sizeof(*pField);
		*pField = aTerm.asDouble();
		return *this;
	}
	if (m_nTargetTyp == SQL_C_CHAR)
	{
		const char * szValue = aTerm.asString();
		char * szField = m_pTargetVal;
		assert (pLength);
		memset (szField, 0, nSize);
		strncpy (szField, szValue, nSize-1);
		*pLength = strlen (szField);
		return *this;
	}
	if (m_nTargetTyp == SQL_C_DEFAULT)
	{
		if (aTerm.isInteger())
		{
			if (nSize > sizeof(long))
				nSize = sizeof(long);
			if (nSize == sizeof(short))
				*(short*)m_pTargetVal = aTerm.asInteger();
			if (nSize == sizeof(long))
				*(long*)m_pTargetVal = aTerm.asDouble();
			if (m_pTargetInd)
				*m_pTargetInd = nSize;
			return *this;
		}
		else
		if (aTerm.isNumber())
		{
			if (nSize > sizeof(double))
				nSize = sizeof(double);
			if (nSize == sizeof(short))
				*(short*)m_pTargetVal = aTerm.asInteger();
			if (nSize == sizeof(long))
				*(long*)m_pTargetVal = aTerm.asDouble();
			if (nSize == sizeof(double))
				*(double*)m_pTargetVal = aTerm.asDouble();
			if (m_pTargetInd)
				*m_pTargetInd = m_nTargetLen;
			return *this;
		}
		else
		if (aTerm.isString())
		{
			const char * szValue = aTerm.asString();
			char * szField = m_pTargetVal;
			assert (pLength);
			memset (szField, 0, nSize);
			strncpy (szField, szValue, nSize-1);
			*pLength = strlen (szField);
			return *this;
		}
	}
	if (m_nTargetTyp == SQL_C_TIMESTAMP)
	{
		const time_t nTime = aTerm.asInteger();
		TIMESTAMP_STRUCT & aTarget = *((TIMESTAMP_STRUCT*)m_pTargetVal);
		tm * pTime = localtime (&nTime);
		ASSUME (pTime);
		aTarget.year = pTime->tm_year + 1900;
		aTarget.month = pTime->tm_mon + 1;
		aTarget.day = pTime->tm_mday;
		aTarget.hour = pTime->tm_hour;
		aTarget.minute = pTime->tm_min;
		aTarget.second = pTime->tm_sec;
		return *this;
	}
	char szBuff[256];
	sprintf (szBuff, "unknown taget type: %d (0x%x)", m_nTargetTyp);
	MessageBox (0, szBuff, "ODBC Error", MB_OK);
	assert (false);
	return *this;
}
//---------------------------------------------------------------------------
size_t
CField::displaysize (short nType)
{
	if (nType == SQL_VARCHAR)
		return 128;
	if (nType == SQL_SMALLINT)
		return 5;
	if (nType == SQL_INTEGER)
		return 10;
	if (nType == SQL_CHAR)
		return 255;
	if (nType == SQL_TIMESTAMP)
		return 19;
	if (nType == SQL_TYPE_TIMESTAMP)
		return 19;
	if (nType == SQL_TYPE_TIME)
		return 8;
	if (nType == SQL_TYPE_DATE)
		return 11;
	if (nType == SQL_DATETIME)
		return 19;
	if (nType == SQL_DOUBLE)
		10;
	return 25;
}
//---------------------------------------------------------------------------
void
CField::bind (short nTyp, char*pVal, long nLen, long* pInd)
{
	m_nTargetTyp = nTyp;
	m_pTargetVal = pVal;
	m_nTargetLen = nLen;
	m_pTargetInd = pInd;
	export = 0;
}
//---------------------------------------------------------------------------
// Statement - Data
//---------------------------------------------------------------------------
SQLRETURN
CStmtData::unbind()
{
	const int nFields = fields();
	int i;
	for (i=0; i<nFields; i++)
	{
		CField & raField = fields(i);
		raField.unbind();
	}
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
CStmtTables::CStmtTables (CStmtHandle * h)
: CStmtData(h)
//, m_nTable(DIM(ms_aList))
{
	m_iTable = 0;
	memset (m_szTable, 0, sizeof(m_szTable));

	m_aFields[0].pszSQL_DESC_NAME = strdup ("TABLE_CAT");
	m_aFields[0].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[1].pszSQL_DESC_NAME = strdup ("TABLE_SCHEM");
	m_aFields[1].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[2].pszSQL_DESC_NAME = strdup ("TABLE_NAME");
	m_aFields[2].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[3].pszSQL_DESC_NAME = strdup ("TABLE_TYPE");
	m_aFields[3].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[4].pszSQL_DESC_NAME = strdup ("REMARKS");
	m_aFields[4].nSQL_DESC_TYPE = SQL_CHAR;

	entry_t aRecord;
	CDatabase * pDatabase = dynamic_cast<CDatabase*>(h->m_paParent);
	assert (pDatabase);
	m_aData.reserve (100);
	aRecord.tabletype = "TABLE";
//	aRecord.catalog = aStorage.path();
//	aRecord.schema = "archiv";
	aRecord.tablename = "reg";
	aRecord.remarks = "Modelle der Regression";
//	m_aData.push_back (aRecord);
//	aRecord.catalog = aStorage.path();
//	aRecord.schema = "archiv";
	aRecord.tablename = "erg";
	aRecord.remarks = "Prognose Archiv (Ergebnisse/Rel.Fehler)";
//	m_aData.push_back (aRecord);
//	aRecord.catalog = "<static>";
//	aRecord.schema = "sys";
	aRecord.tablename = "a";
	aRecord.remarks = "Test Tabelle";
	m_aData.push_back (aRecord);
	aRecord.tablename = "test";
	aRecord.remarks = "Test Tabelle";
	m_aData.push_back (aRecord);
	m_nTable = m_aData.size();
//	aRecord.catalog = "<static>";
//	aRecord.schema = "sys";
	aRecord.tablename = "b";
	aRecord.remarks = "Test Tabelle";
	m_aData.push_back (aRecord);
	m_nTable = m_aData.size();
};
//---------------------------------------------------------------------------
CStmtTables::~CStmtTables()
{
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtTables::getData (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	const size_t iTable = m_iTable - 1;
	if (iTable >= m_nTable)
		return SQL_NO_DATA;
	const entry_t aTable = m_aData[iTable];
	CField aField;
	aField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	if (nCol == 0)
		aField = CValue (aTable.catalog.c_str());
	if (nCol == 1)
		aField = CValue (aTable.schema.c_str());
	if (nCol == 2)
		aField = CValue (aTable.tablename.c_str());
	if (nCol == 3)
		aField = CValue (aTable.tabletype.c_str());
	if (nCol == 4)
		aField = CValue (aTable.remarks.c_str());
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtTables::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < 5);
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtTables::fetch()
{
	if (m_iTable >= m_nTable)
		return SQL_NO_DATA;
	const entry_t aTable = m_aData[m_iTable++];
	m_aFields[0] = CValue (aTable.catalog.c_str());
	m_aFields[1] = CValue (aTable.schema.c_str());
	m_aFields[2] = CValue (aTable.tablename.c_str());
	m_aFields[3] = CValue (aTable.tabletype.c_str());
	m_aFields[4] = CValue (aTable.remarks.c_str());
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
CStmtColumns::CStmtColumns (CStmtHandle * h, const char * szTable)
: CStmtData(h)
{
	assert (h);
	m_iCol = 0;
	//memset (m_aCol, 0, sizeof(m_aCol));

	m_aFields[0].pszSQL_DESC_NAME = strdup ("TABLE_CAT");
	m_aFields[0].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[1].pszSQL_DESC_NAME = strdup ("TABLE_SCHEM");
	m_aFields[1].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[2].pszSQL_DESC_NAME = strdup ("TABLE_NAME");
	m_aFields[2].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[3].pszSQL_DESC_NAME = strdup ("COLUMN_NAME");
	m_aFields[3].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[4].pszSQL_DESC_NAME = strdup ("DATA_TYPE");
	m_aFields[4].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[5].pszSQL_DESC_NAME = strdup ("TYPE_NAME");
	m_aFields[5].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[6].pszSQL_DESC_NAME = strdup ("COLUMN_SIZE");
	m_aFields[6].nSQL_DESC_TYPE = SQL_INTEGER;
	m_aFields[7].pszSQL_DESC_NAME = strdup ("BUFFER_LENGTH");
	m_aFields[7].nSQL_DESC_TYPE = SQL_INTEGER;
	m_aFields[8].pszSQL_DESC_NAME = strdup ("DECIMAL_DIGITS");
	m_aFields[8].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[9].pszSQL_DESC_NAME = strdup ("NUM_PREC_RADIX");
	m_aFields[9].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[10].pszSQL_DESC_NAME = strdup ("NULLABLE");
	m_aFields[10].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[11].pszSQL_DESC_NAME = strdup ("REMARKS");
	m_aFields[11].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[12].pszSQL_DESC_NAME = strdup ("COLUMN_DEF");
	m_aFields[12].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[13].pszSQL_DESC_NAME = strdup ("SQL_DATA_TYPE");
	m_aFields[13].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[14].pszSQL_DESC_NAME = strdup ("SQL_DATETIME_SUB");
	m_aFields[14].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[15].pszSQL_DESC_NAME = strdup ("CHAR_OCTET_LENGTH");
	m_aFields[15].nSQL_DESC_TYPE = SQL_INTEGER;
	m_aFields[16].pszSQL_DESC_NAME = strdup ("ORDINAL_POSITION");
	m_aFields[16].nSQL_DESC_TYPE = SQL_INTEGER;
	m_aFields[17].pszSQL_DESC_NAME = strdup ("IS_NULLABLE");
	m_aFields[17].nSQL_DESC_TYPE = SQL_CHAR;

};
//---------------------------------------------------------------------------
SQLRETURN
CStmtColumns::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtColumns::fetch()
{
	const char * aName[] = { "Name", "Anschrift", "Adresse"};
	int i;
	if (m_iCol >= DIM(aName))
		return SQL_NO_DATA;
	for (i=0; i<DIM(m_aFields); i++)
	{
		CField & aField = m_aFields[i];
		if (!aField.m_pTargetVal) continue;

		//// COLUMN_NAME
		if (aField.m_nTargetTyp == SQL_CHAR)
		if (i == 3)
		{
			char * szValue = aField.m_pTargetVal;
			int nSize = aField.m_nTargetLen;
			long * pLength = aField.m_pTargetInd;
			assert (szValue && pLength);
			const int j = m_iCol;
			strncpy (szValue, aName[j], nSize);
			szValue[nSize - 1] = 0;
			*pLength = strlen (szValue);
		}
		if (i == 4)
		{
			*(SQLSMALLINT*)aField.m_pTargetVal = SQL_VARCHAR;
		}
		//strncpy (szValue, aColumn[i].name, nSize);
	}
	m_iCol++;
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
const CStmtTypeInfo::record_t
CStmtTypeInfo::ms_aData[] =
{	{ "CHAR",              SQL_CHAR,            128, "\"", "\"", TRUE, TRUE, 16 }
,	{ "INTEGER",           SQL_INTEGER,          10, "", "", TRUE, TRUE, 16 }
,	{ "FLOAT",             SQL_FLOAT,            13, "", "", TRUE, TRUE, 16 }
,	{ "DATETIME",          SQL_DATETIME,         16, "#", "#", TRUE, TRUE, 16 }
,	{ "TIMESTAMP",         SQL_TYPE_TIMESTAMP,   19, "#", "#", TRUE, TRUE, 16 }
,	{ "BIT",               SQL_BIT,               1, "", "", TRUE, TRUE, 16 }
,	{ "GUID",              SQL_GUID,             10, "", "", TRUE, TRUE, 16 }
/*
,	{ "NUMERIC",           SQL_NUMERIC,         16, "", "", TRUE, TRUE, 16 }
,	{ "DECIMAL",           SQL_DECIMAL,         16, "", "", TRUE, TRUE, 16 }
,	{ "SAMLLINT",          SQL_SMALLINT,        16, "", "", TRUE, TRUE, 16 }
,	{ "REAL",              SQL_REAL,            16, "", "", TRUE, TRUE, 16 }
,	{ "DOUBLE",            SQL_DOUBLE,          16, "", "", TRUE, TRUE, 16 }
,	{ "VARCHAR",           SQL_VARCHAR,         16, "", "", TRUE, TRUE, 16 }
,	{ "LONGVARCHAR",       SQL_LONGVARCHAR,     16, "", "", TRUE, TRUE, 16 }
,	{ "DATE",              SQL_TYPE_DATE,       16, "#", "#", TRUE, TRUE, 16 }
,	{ "TIME",              SQL_TYPE_TIME,       16, "#", "#", TRUE, TRUE, 16 }
,	{ "TINYINT",           SQL_TINYINT,         16, "", "", TRUE, TRUE, 16 }
,	{ "BIGINT",            SQL_BIGINT,          16, "", "", TRUE, TRUE, 16 }
,	{ "BINARY",            SQL_BINARY,          16, "", "", TRUE, TRUE, 16 }
,	{ "VARBINARY",         SQL_VARBINARY,       16, "", "", TRUE, TRUE, 16 }
,	{ "LONGVARBINARY",     SQL_LONGVARBINARY,   16, "", "", TRUE, TRUE, 16 }
,	{ "DATE",              SQL_TYPE_DATE ,      16, "#", "#", TRUE, TRUE, 16 }
,	{ "TIME",              SQL_TYPE_TIME ,      16, "#", "#", TRUE, TRUE, 16 }
,	{ "TIMESTAMP",         SQL_TYPE_TIMESTAMP , 16, "#", "#", TRUE, TRUE, 16 }
*/
};
//---------------------------------------------------------------------------
const CStmtTypeInfo::column_t
CStmtTypeInfo::ms_aDesc[] =
{ {"TYPE_NAME",          SQL_CHAR,            128}
, {"DATA_TYPE",          SQL_SMALLINT,          5}
, {"COLUMN_SIZE",        SQL_INTEGER,          10}
, {"LITERAL_PREFIX",     SQL_CHAR,            128}
, {"LITERAL_SUFFIX",     SQL_CHAR,            128}
, {"CREATE_PARAMS",      SQL_CHAR,            128}
, {"NULLABLE",           SQL_SMALLINT,          5}
, {"CASE_SENSITIVE",     SQL_SMALLINT,          5}
, {"SEARCHABLE",         SQL_SMALLINT,          5}
, {"UNSIGNED_ATTRIBUTE", SQL_CHAR,            128}
, {"AUTO_UNIQUE_VALUE",  SQL_SMALLINT,          5}
, {"LOCAL_TYPE_NAME",    SQL_CHAR,            128}
, {"MINIMUM_SCALE",      SQL_SMALLINT,          5}
, {"MAXIMUM_SCALE",      SQL_SMALLINT,          5}
, {"SQL_DATA_TYPE",      SQL_SMALLINT,          5}
, {"SQL_DATETIME_SUB",   SQL_SMALLINT,          5}
, {"NUM_PREC_RADIX",     SQL_INTEGER,          10}
, {"INTERVAL_PRECISION", SQL_SMALLINT,          5}
};
const int CStmtTypeInfo::ms_nRecord = DIM(ms_aData);
//---------------------------------------------------------------------------
CStmtTypeInfo::CStmtTypeInfo (CStmtHandle * h, int nDataType)
: CStmtData(h)
{
	m_pRecord = 0;
	m_iRecord = 0;
	m_nDataType = nDataType;
	//memset (m_aCol, 0, sizeof(m_aCol));
	const int nCol = DIM(ms_aDesc);
	int i;

	for (i=0; i<nCol; i++)
	{
		const column_t & aDesc = ms_aDesc[i];
		m_aFields[i].pszSQL_DESC_NAME = strdup (aDesc.name);
		m_aFields[i].nSQL_DESC_TYPE = aDesc.type;
		m_aFields[i].nSQL_DESC_DISPLAY_SIZE = aDesc.size;
	}
	return;
	m_aFields[0].pszSQL_DESC_NAME = strdup ("TYPE_NAME");
	m_aFields[0].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[0].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[1].pszSQL_DESC_NAME = strdup ("DATA_TYPE");
	m_aFields[1].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[1].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[2].pszSQL_DESC_NAME = strdup ("COLUMN_SIZE");
	m_aFields[2].nSQL_DESC_TYPE = SQL_INTEGER;
	m_aFields[2].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[3].pszSQL_DESC_NAME = strdup ("LITERAL_PREFIX");
	m_aFields[3].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[3].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[4].pszSQL_DESC_NAME = strdup ("LITERAL_SUFFIX");
	m_aFields[4].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[4].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[5].pszSQL_DESC_NAME = strdup ("CREATE_PARAMS");
	m_aFields[5].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[5].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[6].pszSQL_DESC_NAME = strdup ("NULLABLE");
	m_aFields[6].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[6].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[7].pszSQL_DESC_NAME = strdup ("CASE_SENSITIVE");
	m_aFields[7].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[7].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[8].pszSQL_DESC_NAME = strdup ("SEARCHABLE");
	m_aFields[8].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[8].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[9].pszSQL_DESC_NAME = strdup ("UNSIGNED_ATTRIBUTE");
	m_aFields[9].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[9].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[10].pszSQL_DESC_NAME = strdup ("FIXED_PREC_SCALE");
	m_aFields[10].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[10].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[11].pszSQL_DESC_NAME = strdup ("AUTO_UNIQUE_VALUE");
	m_aFields[11].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[11].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[12].pszSQL_DESC_NAME = strdup ("LOCAL_TYPE_NAME");
	m_aFields[12].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[12].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[13].pszSQL_DESC_NAME = strdup ("MINIMUM_SCALE");
	m_aFields[13].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[13].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[14].pszSQL_DESC_NAME = strdup ("MAXIMUM_SCALE");
	m_aFields[14].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[14].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[15].pszSQL_DESC_NAME = strdup ("SQL_DATA_TYPE");
	m_aFields[15].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[15].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[16].pszSQL_DESC_NAME = strdup ("SQL_DATETIME_SUB");
	m_aFields[16].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[16].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[17].pszSQL_DESC_NAME = strdup ("NUM_PREC_RADIX");
	m_aFields[17].nSQL_DESC_TYPE = SQL_INTEGER;
	m_aFields[17].nSQL_DESC_DISPLAY_SIZE = 16;
	m_aFields[18].pszSQL_DESC_NAME = strdup ("INTERVAL_PRECISION");
	m_aFields[18].nSQL_DESC_TYPE = SQL_SMALLINT;
	m_aFields[18].nSQL_DESC_DISPLAY_SIZE = 16;
};
//---------------------------------------------------------------------------
const CStmtTypeInfo::record_t *
CStmtTypeInfo::at (int nSQLType)
{
	const int n = DIM (ms_aData);
	int i;
	for (i=0; i<n; i++)
	{
		const record_t & aRecord = ms_aData[i];
		if (aRecord.datatype == nSQLType)
			return & aRecord;
	}
	return 0;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtTypeInfo::getData (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	CField aField;
	aField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	if (!m_pRecord)
		return SQL_NO_DATA;
	const record_t & aRecord = *m_pRecord;
	switch (nCol)
	{
	case 0:
		aField = aRecord.name;
		break;
	case 1:
		aField = aRecord.datatype;
		break;
	case 2:
		aField = aRecord.columnsize;
		break;
	case 3:
		aField = aRecord.prefix;
		break;
	case 4:
		aField = aRecord.suffix;
		break;
	case 5:
		aField = ""; //CREATE_PARAMS
		break;
	case 6:
		aField = aRecord.nullabel;
		break;
	case 7:
		aField = (double) false; // case sensitive
		break;
	case 8:
		aField = aRecord.searchabel;
		break;
	case 9:
		aField = (double) false;
		break;
	case 10:
		aField = (double) false;
		break;
	case 11:
		aField = (double) false;
		break;
	case 12:
		aField = aRecord.name;
		break;
	case 13:
		aField = (double) 0; // minimumscale
		break;
	case 14:
		aField = aRecord.maximumscale;
		break;
	case 15:
		aField = aRecord.datatype;
		break;
	case 16:
		aField = CValue::null;
		break;
	case 17:
		aField = CValue::null;
		break;
	case 18:
		aField = CValue::null;
		break;
	default:
		ASSUME (false);
	}

	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtTypeInfo::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtTypeInfo::fetch()
{
	return SQL_NO_DATA;
	if (m_iRecord >= ms_nRecord)
		return SQL_NO_DATA;
	const record_t & aRecord = ms_aData[m_iRecord++];
	const int nType = aRecord.datatype;
	m_pRecord = &aRecord;

	m_aFields[ 0] = aRecord.name;
	m_aFields[ 1] = nType;
	m_aFields[ 2] = aRecord.columnsize;
	m_aFields[ 3] = aRecord.prefix;
	m_aFields[ 4] = aRecord.suffix;
	m_aFields[ 5] = ""; //CREATE_PARAMS
	m_aFields[ 6] = aRecord.nullabel;
	m_aFields[ 7] = (double) false; // case sensitive
	m_aFields[ 8] = aRecord.searchabel;
	m_aFields[ 9] = (double) false;
	m_aFields[10] = (double) false;
	m_aFields[11] = (double) false;
	m_aFields[12] = aRecord.name;
	m_aFields[13] = (double) 0; // minimumscale
	m_aFields[14] = aRecord.maximumscale;
	m_aFields[15] = nType;
	m_aFields[16] = CValue::null;
	m_aFields[17] = CValue::null;
	m_aFields[18] = CValue::null;
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
CStmtTest::CStmtTest (CStmtHandle * h)
: CStmtData(h)
{
	m_nRecord = 5;
	m_iRecord = 0;

	m_aFields[0].pszSQL_DESC_NAME = strdup ("Name");
	m_aFields[0].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[0].nSQL_DESC_DISPLAY_SIZE = 10;
	m_aFields[1].pszSQL_DESC_NAME = strdup ("Vorname");
	m_aFields[1].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[1].nSQL_DESC_DISPLAY_SIZE = 10;
	m_aFields[2].pszSQL_DESC_NAME = strdup ("Anschrift");
	m_aFields[2].nSQL_DESC_TYPE = SQL_CHAR;
	m_aFields[2].nSQL_DESC_DISPLAY_SIZE = 20;
};
//---------------------------------------------------------------------------
SQLRETURN
CStmtTest::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (0 <= nCol && nCol < 5);
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.m_nTargetTyp = nTargetTyp;
	raField.m_pTargetVal = (char*)pTargetVal;
	raField.m_nTargetLen = nTargetLen;
	raField.m_pTargetInd = pnTargetInd;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtTest::fetch()
{
	char szBuffer[256];
	int i;
	if (m_iRecord >= m_nRecord)
		return SQL_NO_DATA;
	for (i=0; i<DIM(m_aFields); i++)
	{
		sprintf (szBuffer, "A[%02d,%02d]", m_iRecord, i);
		CField & aField = m_aFields[i];
		if (!aField.m_pTargetVal) continue;
		if (aField.m_nTargetTyp != SQL_CHAR) continue;
		char * szValue = aField.m_pTargetVal;
		int nSize = aField.m_nTargetLen;
		long * pLength = aField.m_pTargetInd;
		assert (szValue && pLength);
		strncpy (szValue, szBuffer, nSize);
		szValue[nSize - 1] = 0;
		*pLength = strlen (szValue);
	}
	m_iRecord++;
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
CStmtQuery::CStmtQuery (CStmtHandle* h)
: CStmtData(h)
, m_aStatement (h->Database())
{
	m_iRecord = 0;
	m_nRecord = 0;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtQuery::exec()
{
	m_aStatement.execute();
	m_iRecord = 0;
	m_nRecord = m_aStatement.rows();
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtQuery::getData (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (0 <= nCol && nCol < m_aFields.size());
	assert (pTargetVal);
	CField aField;
	aField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	const CTerm * pTerm = m_aStatement.m_aSelection[nCol];
	assert (pTerm);
	aField = *pTerm;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtQuery::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (0 <= nCol && nCol < m_aFields.size());
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.m_nTargetTyp = nTargetTyp;
	raField.m_pTargetVal = (char*)pTargetVal;
	raField.m_nTargetLen = nTargetLen;
	raField.m_pTargetInd = pnTargetInd;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN 
CStmtQuery::fetch()
{
	const size_t n = m_aFields.size();
	size_t i;
	if (m_iRecord >= m_nRecord)
		return SQL_NO_DATA;
	m_aStatement.move (m_iRecord);
	for (i=0; i<n; i++)
	{
		CField & aField = m_aFields[i];
		if (!aField.m_pTargetVal) continue;
		const CTerm * pTerm = m_aStatement.m_aSelection[i];
		assert (pTerm);
		aField = *pTerm;
	}
	m_iRecord++;
	return SQL_SUCCESS;
};
//---------------------------------------------------------------------------
const CStmtSpecialColumns::column_t
CStmtSpecialColumns::ms_aDesc[] =
{	{"SCOPE", SQL_SMALLINT}
,	{"COLUMN_NAME", SQL_VARCHAR}
,	{"DATA_TYPE", SQL_SMALLINT}
,	{"TYPE_NAME", SQL_VARCHAR}
,	{"PRECISION", SQL_INTEGER}
,	{"LENGTH", SQL_INTEGER}
,	{"SCALE", SQL_SMALLINT}
,	{"PSEUDO_COLUMN", SQL_SMALLINT}
};
//---------------------------------------------------------------------------
CStmtSpecialColumns::CStmtSpecialColumns (CStmtHandle * hstmt, const char *szTable)
: CStmtData (hstmt)
{
	m_nRecord = 0;
	m_iRecord = 0;
	int i;
	for (i=0; i<DIM(ms_aDesc); i++)
	{
		const column_t & aDesc = ms_aDesc[i];
		assert (i < DIM(m_aFields));
		m_aFields[i].pszSQL_DESC_NAME = strdup (aDesc.name);
		m_aFields[i].nSQL_DESC_TYPE = aDesc.type;
		m_aFields[i].nSQL_DESC_DISPLAY_SIZE = 10;
	}
	CDatabase * pDatabase = hstmt->Database();
	m_pTable = pDatabase->table ("", "", szTable);
	m_nRecord = m_pTable->cols();
	m_iRecord = 0;
}
//---------------------------------------------------------------------------
CStmtSpecialColumns::~CStmtSpecialColumns()
{
	delete m_pTable;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtSpecialColumns::fetch()
{
	return SQL_NO_DATA;
	if (m_iRecord >= m_nRecord)
		return SQL_NO_DATA;
	m_iRecord++;
	CValue aType = m_pTable->desc(SQL_DESC_TYPE, m_iRecord);
	CValue aName = m_pTable->desc(SQL_DESC_NAME, m_iRecord);
	const int nType = aType.asInteger();
	const int nTypeInfo = CStmtTypeInfo::ms_nRecord;
	int i;
	for (i=0; i<nTypeInfo; i++)
	{
		const CStmtTypeInfo::record_t & aRecord = CStmtTypeInfo::ms_aData[i];
		if (aRecord.datatype == nType)
			break;
	}
	const CStmtTypeInfo::record_t & aTypeInfo = CStmtTypeInfo::ms_aData[i];
	const char * szType = (i<nTypeInfo) ? CStmtTypeInfo::ms_aData[i].name : "unknown";
	const char * szName = aName.asString();
	char szColumn[256];
	for (i=0; i<strlen(szName); i++)
		szColumn[i] = toupper(szName[i]);
	szColumn[i] = 0;
	
	m_aFields[ 0] = SQL_SCOPE_SESSION;
	m_aFields[ 1] = aName;
	m_aFields[ 2] = nType;
	m_aFields[ 3] = szType;
	m_aFields[ 4] = aTypeInfo.columnsize;
	m_aFields[ 5] = aTypeInfo.columnsize;
	m_aFields[ 6] = aTypeInfo.maximumscale;
	m_aFields[ 7] = SQL_PC_NOT_PSEUDO;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtSpecialColumns::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.m_nTargetTyp = nTargetTyp;
	raField.m_pTargetVal = (char*)pTargetVal;
	raField.m_nTargetLen = nTargetLen;
	raField.m_pTargetInd = pnTargetInd;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtSpecialColumns::getData (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	CField aField;
	aField.bind (nTargetTyp, (char*)pTargetVal, nTargetLen, pnTargetInd);
	const size_t iRecord = m_iRecord - 1;
	if (iRecord >= m_nRecord)
		return SQL_NO_DATA;

	CValue aType = m_pTable->desc(SQL_DESC_TYPE, iRecord);
	CValue aName = m_pTable->desc(SQL_DESC_NAME, iRecord);
	const int nType = aType.asInteger();
	const int nTypeInfo = CStmtTypeInfo::ms_nRecord;
	int i;
	for (i=0; i<nTypeInfo; i++)
	{
		const CStmtTypeInfo::record_t & aRecord = CStmtTypeInfo::ms_aData[i];
		if (aRecord.datatype == nType)
			break;
	}
	const CStmtTypeInfo::record_t & aTypeInfo = CStmtTypeInfo::ms_aData[i];
	const char * szType = (i<nTypeInfo) ? CStmtTypeInfo::ms_aData[i].name : "unknown";
	const char * szName = aName.asString();
	char szColumn[256];
	for (i=0; i<strlen(szName); i++)
		szColumn[i] = toupper(szName[i]);
	szColumn[i] = 0;

	if (nCol == 0)
		aField = SQL_SCOPE_SESSION;
	if (nCol == 1)
		aField = aName;
	if (nCol == 2)
		aField = nType;
	if (nCol == 3)
		aField = szType;
	if (nCol == 4)
		aField = aTypeInfo.columnsize;
	if (nCol == 5)
		aField = aTypeInfo.columnsize;
	if (nCol == 6)
		aField = aTypeInfo.maximumscale;
	if (nCol == 7)
		aField = SQL_PC_NOT_PSEUDO;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
const CStmtStatistics::column_t
CStmtStatistics::ms_aDesc[] =
{	{"TABLE_QUALIFIER", SQL_VARCHAR}
,	{"TABLE_OWNER", SQL_VARCHAR}
,	{"TABLE_NAME", SQL_VARCHAR}
,	{"NON_UNIQUE", SQL_SMALLINT}
,	{"INDEX_QUALIFIER", SQL_VARCHAR}
,	{"INDEX_NAME", SQL_VARCHAR}
,	{"TYPE", SQL_SMALLINT}
,	{"SEQ_IN_INDEX", SQL_SMALLINT}
,	{"COLUMN_NAME", SQL_VARCHAR}
,	{"COLLATION", SQL_CHAR}
,	{"CARDINALITY", SQL_INTEGER}
,	{"PAGES", SQL_INTEGER}
,	{"FILTER_CONDITION", SQL_VARCHAR}
};
//---------------------------------------------------------------------------
CStmtStatistics::CStmtStatistics (CStmtHandle * hstmt)
: CStmtData (hstmt)
{
	m_nRecord = 0;
	m_iRecord = 0;
	int i;
	for (i=0; i<DIM(ms_aDesc); i++)
	{
		const column_t & aDesc = ms_aDesc[i];
		assert (i < DIM(m_aFields));
		m_aFields[i].pszSQL_DESC_NAME = strdup (aDesc.name);
		m_aFields[i].nSQL_DESC_TYPE = aDesc.type;
		m_aFields[i].nSQL_DESC_DISPLAY_SIZE = 10;
	}
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtStatistics::fetch()
{
	if (m_iRecord >= m_nRecord)
		return SQL_NO_DATA;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtStatistics::bind (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	CField & raField = m_aFields [nCol];
	raField.m_nTargetTyp = nTargetTyp;
	raField.m_pTargetVal = (char*)pTargetVal;
	raField.m_nTargetLen = nTargetLen;
	raField.m_pTargetInd = pnTargetInd;
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
SQLRETURN
CStmtStatistics::getData (SQLUSMALLINT nCol
, SQLSMALLINT nTargetTyp
, SQLPOINTER pTargetVal
, SQLINTEGER nTargetLen
, SQLINTEGER * pnTargetInd
)
{
	nCol--;
	assert (nCol < DIM(m_aFields));
	assert (pTargetVal);
	return SQL_SUCCESS;
}
//---------------------------------------------------------------------------
