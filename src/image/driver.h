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

#ifndef DRIVER_H
#define DRIVER_H

#include <windows.h>
#include <sqlext.h>
#include <assert.h>
#include <crtdbg.h>
#include <list>
#include <vector>
#include "database.h"


//---------------------------------------------------------------------------
#define TRACE(msg) _RPT3(_CRT_WARN, "%s[%d]: %s\n", __FILE__, __LINE__, (const char *)(msg))
//#define DIM(arr) (sizeof(arr) / sizeof((arr)[0]))

class CDriver;
class CEnvironment;
class CDatabase;
class CStmtHandle;
class CStmtData;
class CField;

using namespace std;
//---------------------------------------------------------------------------
bool SetString (const char * value, SQLPOINTER buffer, SQLSMALLINT size, SQLSMALLINT * length);
bool SetShort (int value, SQLPOINTER buffer, SQLSMALLINT size=sizeof(SQLUSMALLINT));
//---------------------------------------------------------------------------
typedef struct
{
	const char * name;
	int type;
} typColumn;
//---------------------------------------------------------------------------
int str_name_cmp (const char * s1, const char * s2, const char esc);
//---------------------------------------------------------------------------
// DRIVER
//---------------------------------------------------------------------------
class CDriver : CHandle
{
	friend CEnvironment;
public:
	CDriver() : CHandle(0) {}
	virtual ~CDriver();
	static const char * driver_odbc_ver();
	static int sql_type (term_t);
	static CDriver * Instance() { return & ms_aInstance; }

protected:
	static CDriver ms_aInstance;
};
//---------------------------------------------------------------------------
// ENVIRONMENT
//---------------------------------------------------------------------------
class CEnvironment : public CHandle
{
	friend CDatabase;
public:
	CEnvironment (CDriver *);
	virtual ~CEnvironment();

};
//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
class CDatabase : public IDatabase
{
	friend CEnvironment;
	friend CStmtHandle;
public:
	CDatabase (CEnvironment*);
	virtual ~CDatabase();
	virtual CTable * table (const char * catalog, const char * schema, const char * name);

	virtual CResult query (const char * sql);
	virtual CResult columns (const char * table);
	virtual CResult type_info (int datatype);
	virtual CResult tables (const char * filter);

	virtual void desc (CStmtHandle *, const char * table);

};
//---------------------------------------------------------------------------
// STATEMENT
//---------------------------------------------------------------------------
class CStmtData
{
public:
	CStmtData (CStmtHandle*h) : m_pHandle(h) {}
	virtual ~CStmtData() {};
	virtual short fields() = 0;
	virtual CField & fields (short index) = 0;

	virtual SQLRETURN getData (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	) = 0; // { ASSUME(false); return SQL_ERROR; }

	virtual SQLRETURN bind (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	) = 0;

	SQLRETURN unbind();

	virtual SQLRETURN close() = 0;
	virtual SQLRETURN fetch() = 0;
	virtual SQLRETURN exec() { return SQL_SUCCESS; }
	CStmtHandle * m_pHandle;
};
//---------------------------------------------------------------------------
// FIELD
//---------------------------------------------------------------------------
class CField
{
public:
	CField();
	virtual ~CField();
	CField & operator= (const CTerm &);
	CField & operator= (double val) { return (*this) = CValue(TermFLOAT, val); }
	CField & operator= (const char * val) { return (*this) = CValue(val); }
	void bind (short typ, char* val, long len, long* ind);
	void unbind() { bind (SQL_UNKNOWN_TYPE, 0, 0, 0) ; }
	static size_t displaysize (short sql_type);

	void (*export)(void *, long, long*, const CTerm *);


	/* EVERYTHING YOU WOULD EVER WANT TO KNOW ABOUT THE COLUMN. INIT THIS BY CALLING 			*/
	/* _NativeToSQLColumnHeader AS SOON AS YOU HAVE COLUMN INFO. THIS MAKES THE COL HDR LARGER	*/
	/* BUT GENERALIZES MORE CODE. see SQLColAttribute()											*/
	int		bSQL_DESC_AUTO_UNIQUE_VALUE;			/* IS AUTO INCREMENT COL?					*/
	char	*pszSQL_DESC_BASE_COLUMN_NAME;			/* empty string if N/A						*/
	char	*pszSQL_DESC_BASE_TABLE_NAME;			/* empty string if N/A						*/
	int		bSQL_DESC_CASE_SENSITIVE;				/* IS CASE SENSITIVE COLUMN?				*/
	char	*pszSQL_DESC_CATALOG_NAME;				/* empty string if N/A						*/
	int		nSQL_DESC_CONCISE_TYPE;					/* ie SQL_CHAR, SQL_TYPE_TIME...			*/
	int		nSQL_DESC_DISPLAY_SIZE;					/* max digits required to display			*/
	int		bSQL_DESC_FIXED_PREC_SCALE;				/* has data source specific precision?		*/
	char	*pszSQL_DESC_LABEL;						/* display label, col name or empty string	*/
	int		nSQL_DESC_LENGTH;						/* strlen or bin size						*/
	char	*pszSQL_DESC_LITERAL_PREFIX;			/* empty string if N/A						*/
	char	*pszSQL_DESC_LITERAL_SUFFIX;			/* empty string if N/A						*/
	char	*pszSQL_DESC_LOCAL_TYPE_NAME;			/* empty string if N/A						*/
	char	*pszSQL_DESC_NAME;						/* col alias, col name or empty string		*/
	int		nSQL_DESC_NULLABLE;						/* SQL_NULLABLE, _NO_NULLS or _UNKNOWN		*/
	int		nSQL_DESC_NUM_PREC_RADIX;				/* 2, 10, or if N/A... 0 					*/
	int		nSQL_DESC_OCTET_LENGTH;					/* max size									*/
	int		nSQL_DESC_PRECISION;					/*											*/
	int		nSQL_DESC_SCALE;						/*											*/
	char	*pszSQL_DESC_SCHEMA_NAME;				/* empty string if N/A						*/
	int		nSQL_DESC_SEARCHABLE;					/* can be in a filter ie SQL_PRED_NONE...	*/
	char	*pszSQL_DESC_TABLE_NAME;                /* empty string if N/A						*/
	int		nSQL_DESC_TYPE;							/* SQL data type ie SQL_CHAR, SQL_INTEGER..	*/
	char	*pszSQL_DESC_TYPE_NAME;					/* DBMS data type ie VARCHAR, MONEY...		*/
	int		nSQL_DESC_UNNAMED;						/* qualifier for SQL_DESC_NAME ie SQL_NAMED	*/
	int		bSQL_DESC_UNSIGNED;						/* if signed FALSE else TRUE				*/
	int		nSQL_DESC_UPDATABLE;					/* ie SQL_ATTR_READONLY, SQL_ATTR_WRITE...	*/

	/* BINDING INFO		*/
	short			m_nTargetTyp;			/* BIND: C DATA TYPE ie SQL_C_CHAR			*/
	char* 			m_pTargetVal;			/* BIND: POINTER FROM APPLICATION TO COPY TO*/
	long			m_nTargetLen;			/* BIND: MAX SPACE IN pTargetValue			*/
	long*			m_pTargetInd;			/* BIND: TO RETURN LENGTH OR NULL INDICATOR	*/
};
//---------------------------------------------------------------------------
class CStmtHandle : public CHandle
{
public:
	CStmtHandle (CDatabase*);
	virtual ~CStmtHandle();

	CStmtHandle & operator= (CStmtData * result);
	CStmtData * operator->() { return m_paResult;}
	static CStmtData * ptr (SQLHSTMT);
	static CStmtHandle & get (void *);
	CDatabase * Database() const { return dynamic_cast<CDatabase*>(m_paParent); }

//protected:
	CStmtData * m_paResult;

	vector<CField> m_aFields;
};
//---------------------------------------------------------------------------
class CStmtTables : public CStmtData
{
public:
	typedef struct
	{
		string catalog;
		string schema;
		string tablename;
		string tabletype;
		string remarks;
	} entry_t;
//	static const entry_t ms_aList[];

	std::vector<entry_t> m_aData;

	CStmtTables(CStmtHandle*h);
	virtual ~CStmtTables();

	short fields()  { return 5; }
	CField & fields (short i) { return m_aFields[i]; }

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

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();
	
	char m_szTable[256];
	int m_nTable;
	int m_iTable;
	CField m_aFields[5];
};
//---------------------------------------------------------------------------
class CStmtColumns : public CStmtData
{
public:
	CStmtColumns(CStmtHandle*h, const char * table);

	short fields()  { return 18; }
	CField & fields (short i) { return m_aFields[i]; }

	virtual SQLRETURN bind (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	);

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();
	
	static const typColumn ms_aItems[];
	CField m_aFields[18];
	int m_iCol;
};
//---------------------------------------------------------------------------
class CStmtTypeInfo : public CStmtData
{
public:
	struct column_t
	{
		const char * name;
		int type;
		int size;
	};

	struct record_t
	{
		const char * name;
		short datatype;
		long columnsize;
		char * prefix;
		char * suffix;
		short nullabel;
		short searchabel;
		short maximumscale;
	};

	static const record_t ms_aData[];
	static const column_t ms_aDesc[];

	static const record_t * at (int sqltype);

	CStmtTypeInfo (CStmtHandle*h, int datatype);

	short fields()  { return 19; }
	CField & fields (short i) { return m_aFields[i]; }

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

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();
	
	CField m_aFields[19];
	int m_nDataType;
	static const int ms_nRecord;
	int m_iRecord;
	const record_t * m_pRecord;

};
//---------------------------------------------------------------------------
class CStmtQuery : public CStmtData
{
public:
	CStmtQuery (CStmtHandle*);

	short fields()  { return m_aFields.size(); }
	CField & fields (short i) { return m_aFields[i]; }

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

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();
	virtual SQLRETURN exec();
	
	vector<CField> m_aFields;
	CStatement m_aStatement;
	size_t m_iRecord;
	size_t m_nRecord;
};
//---------------------------------------------------------------------------
class CStmtTest : public CStmtData
{
public:
	CStmtTest (CStmtHandle*);

	short fields()  { return 3; }
	CField & fields (short i) { return m_aFields[i]; }

	virtual SQLRETURN bind (SQLUSMALLINT nCol
	, SQLSMALLINT nTargetTyp
	, SQLPOINTER pTargetVal
	, SQLINTEGER nTargetLen
	, SQLINTEGER * pnTargetInd
	);

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();
	
	CField m_aFields[3];
	int m_nRecord;
	int m_iRecord;
};
//---------------------------------------------------------------------------
class CStmtSpecialColumns : public CStmtData
{
public:
	struct column_t
	{
		const char * name;
		int type;
	};

	struct record_t
	{
		int scope;
		string column_name;
		int data_type;
		string type_name;
		int column_size;
		int buffer_length;
		int descimal_digits;
		int pseudo_column;
	};

	static const column_t ms_aDesc[];
	std::vector<record_t> m_aData;

	CStmtSpecialColumns(CStmtHandle*, const char * table);
	virtual ~CStmtSpecialColumns();

	short fields() { return DIM(m_aFields); }
	CField & fields (short i) { return m_aFields[i]; }

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

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();

protected:	
	size_t m_nRecord;
	size_t m_iRecord;
	CField m_aFields[8];
	CTable * m_pTable;
};
//---------------------------------------------------------------------------
class CStmtStatistics : public CStmtData
{
public:
	struct column_t
	{
		const char * name;
		int type;
	};

	struct record_t
	{
		int scope;
		string column_name;
		int data_type;
		string type_name;
		int column_size;
		int buffer_length;
		int descimal_digits;
		int pseudo_column;
	};

	static const column_t ms_aDesc[];
	std::vector<record_t> m_aData;

	CStmtStatistics (CStmtHandle*);

	short fields() { return DIM(m_aFields); }
	CField & fields (short i) { return m_aFields[i]; }

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

	virtual SQLRETURN close() { return SQL_SUCCESS; }
	virtual SQLRETURN fetch();

protected:	
	size_t m_nRecord;
	size_t m_iRecord;
	CField m_aFields[13];
};
//---------------------------------------------------------------------------
#endif
