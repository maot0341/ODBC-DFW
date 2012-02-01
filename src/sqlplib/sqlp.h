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

#ifndef __SQLP_H__
#define __SQLP_H__

#define YYFPRINTF yyfprintf

#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <time.h>

#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>


using namespace std;
namespace sqlp {

class CDesc;
//---------------------------------------------------------------------------
// DEFINITIONS
//---------------------------------------------------------------------------
#define EXC_BEGIN try{
#define EXC_END } catch (const string & err) { printf ("%s\n", err.c_str()); }

//---------------------------------------------------------------------------
#define CASE_SQL_BOOL case SQL_BIT

#define CASE_SQL_STRING \
	case SQL_CHAR: \
	case SQL_VARCHAR: \
	case SQL_LONGVARCHAR

#define CASE_SQL_INTEGER \
	case SQL_INTEGER: \
	case SQL_SMALLINT: \
	case SQL_BIGINT: \
	case SQL_TINYINT: \
	case SQL_GUID

#define CASE_SQL_FLOAT \
	case SQL_NUMERIC: \
	case SQL_DECIMAL: \
	case SQL_FLOAT: \
	case SQL_REAL: \
	case SQL_DOUBLE

#define CASE_SQL_DATETIME \
	case SQL_DATETIME: \
	case SQL_TYPE_DATE: \
	case SQL_TYPE_TIME: \
	case SQL_TYPE_TIMESTAMP: \
	case SQL_TIME: \
	case SQL_TIMESTAMP

#define CASE_SQL_BINARY \
	case SQL_BINARY: \
	case SQL_VARBINARY: \
	case SQL_LONGVARBINARY

#define CASE_SQL_NUMBER \
	CASE_SQL_BOOL: \
	CASE_SQL_INTEGER: \
	CASE_SQL_FLOAT: \
	CASE_SQL_DATETIME
 
//---------------------------------------------------------------------------

class CObject;
class CTerm;
class CColumn;
class CCondition;
class CTable;
class CValue;
class CRsQuery;
class CStatement;
class CException;
class CTableSet;

//---------------------------------------------------------------------------
//enum term_t { TermBOOL, TermINT, TermFLOAT, TermTIME, TermVCHAR, TermTYP};
//---------------------------------------------------------------------------
string id(const CObject*);
string stringf (const char * szFormat, ...);
void format (vector<CValue> & record, const vector<CTerm*> & terms);
//---------------------------------------------------------------------------
bool match (const char * str, const char * pat, const char * esc);
bool match (const char * str, const char * pat, char del, char esc);
bool match (const char * str, const vector<string> aToken);
vector<string> match (const char * pat, const char * esc);
vector<string> match (const char * pat, char del, char esc);
//---------------------------------------------------------------------------

//// table name or null-ptr (if more then one tables involed)
CTerm* filter (CStatement & stmt, CTerm* pTerm, const char * szTable);
//---------------------------------------------------------------------------
typedef vector<CTerm*> CTermList;
typedef vector<const char *> CNameList;
typedef std::auto_ptr<CObject> CVariant;
//---------------------------------------------------------------------------
int compare (const CTerm& ,const CTerm&);
//---------------------------------------------------------------------------
// Parser-Objekte
//---------------------------------------------------------------------------
class CObject
{
public:
	virtual ~CObject() {}
};
//---------------------------------------------------------------------------
class CTerm : public CObject
{
public:
	CTerm();
	virtual ~CTerm();

	static bool isBool(short);
	static bool isFloat(short);
	static bool isString(short);
	static bool isInteger(short);
	static bool isDateTime(short);
	static bool isBinary (short);
	static bool isNumber (short);

#if 0
	virtual bool isBool() const     { return isBool(type()); }
	virtual bool isNumber() const   { return isNumber(type()); }
	virtual bool isFloat() const    { return isFloat(type()); }
	virtual bool isString() const   { return isString(type()); }
	virtual bool isInteger() const  { return isInteger(type()); }
	virtual bool isDateTime() const { return isDateTime(type()); }
#endif
	bool operator== (const CTerm & v) const { return compare(*this, v) == 0; }
	bool operator!= (const CTerm & v) const { return compare(*this, v) != 0; }
	bool operator<  (const CTerm & v) const { return compare(*this, v) <  0; }
	bool operator<= (const CTerm & v) const { return compare(*this, v) <= 0; }
	bool operator>  (const CTerm & v) const { return compare(*this, v) >  0; }
	bool operator>= (const CTerm & v) const { return compare(*this, v) >= 0; }

	virtual bool isNull() const = 0;
	virtual short type() const { return SQL_UNKNOWN_TYPE; }
	virtual double asDouble() const = 0;
	virtual const char * asString() const = 0;

	virtual long asInteger() const { return asDouble(); }
	virtual bool asBool() const    { return asInteger() != 0; }
	virtual time_t asTime() const { return asDouble(); }

	virtual bool isTrue() const    { return asInteger() != 0; }
	virtual bool isFalse() const   { return asInteger() == 0; }

	void bind (const CValue & v)   { m_paValue = &v; }
	void bind (const CValue * v)   { m_paValue = v; }
	const CValue * value() const        { return m_paValue; }

	string m_strDebug;

protected:
	const CValue * m_paValue;
};
//---------------------------------------------------------------------------
class CValue : public CTerm
{
public:
	CValue (short t=SQL_UNKNOWN_TYPE);
	CValue (short t, double val);
	CValue (const char * val);
	CValue (const string & val);
	CValue (const CTerm & t);
	~CValue();

	void clear (short t=SQL_UNKNOWN_TYPE);

	const CValue & operator= (const CTerm &);
	const CValue & operator= (const CValue &);
	const CValue & operator= (double value);
	const CValue & operator= (const char * value);
	const CValue & operator= (const string & value);

	short type() const  { return m_nType; }
	bool isNull() const  { return !m_bValid; }
	bool isValid() const { return m_bValid; }
	void setNull() { m_bValid = false; }

	double        asDouble() const;
	const char *  asString() const;
	long          asInteger() const;
	time_t        asTime() const;

	char*         alloc (size_t);
	const char *  set (const char * val);
	void          valid (bool b) {m_bValid = b; }

	void fetch (const CTerm*);
	CValue & set (short, double val);
	CValue & set (short, const char * val);
	static bool isTrue (const CTerm *t) { return t==0 || t->asBool(); }
	
	bool   m_bValid;
	short  m_nType;
	double m_dNumber;
//	string m_aString;
	char * m_pString;
	size_t m_nString;
};

//---------------------------------------------------------------------------
typedef vector<CValue> CRecord;
//---------------------------------------------------------------------------
class CUnary : public CTerm
{
public:
	CUnary (int head, CTerm *term) : m_nHead(head), m_pTerm(term)  { m_strDebug = id(this); }
	int head() const { return m_nHead; }
	const CTerm* arg() const { return m_pTerm; }
	short type() const;

	bool isNull() const;
	double asDouble() const;
	const char * asString() const ;

	int m_nHead;
	CTerm * m_pTerm;
};
//---------------------------------------------------------------------------
class CFunction : public CTerm
{
public:
	friend class CStatement;
	CFunction (int head, const vector<CTerm*> & args);
	CFunction (int head, va_list &);
	~CFunction();

	static short type (const vector<CTerm*> &);
	int head() const { return m_nHead; }
	const vector<CTerm*> & args() const { return m_aArgs; }
	short type() const;
	bool isNull() const;
	double asDouble() const;
	const char * asString() const ;
	void arg (CTerm*);
	bool isAssociative (CFunction *);
	bool isComparison() const;
	void append (CTerm* t) { m_aArgs.push_back(t); }
	CTerm * arg(int);

protected:

	int m_nHead;
	mutable std::string  m_strValue;
	vector<CTerm*> m_aArgs;
	double (*m_pNumber)(const vector<CTerm*>&);
	double (*m_pString)(const vector<CTerm*>&);
};
//---------------------------------------------------------------------------
class CColumn : public CTerm
{
public:
	CColumn (const char * t=0, const char * c=0);
	static string key (const char * table, const char * column);
	string key() const { return key(m_szTable, m_szColumn); }
	const char * table() const { return m_szTable; }
	const char * column() const { return m_szColumn; }
	void assign (CTable * t) { m_pTable = t; }

	const CDesc * desc() const;
	short type() const;
	bool isNull() const;
	double asDouble() const ;
	const char * asString() const;

protected:
	const char * m_szTable;
	const char * m_szColumn;

	CTable * m_pTable;
};
//---------------------------------------------------------------------------
class CParam : public CValue
{
public:
	friend class CStatement;
	CParam();
	void assign (const CColumn *);

	short IOType() const        { return m_IOType;   }
	short Nullable() const      { return m_nNullable; }
	short DecimalDigits() const { return m_nDecimalDigits; }
	ULONG ColumnSize() const    { return m_nColumnSize; }

protected:
	short			m_IOType;
	short			m_nNullable;
	short			m_nDecimalDigits;
	ULONG			m_nColumnSize;
};
//---------------------------------------------------------------------------
class CTable : public CObject
{
public:
	CTable() {}
	virtual ~CTable();
	virtual CValue attr (int attr) const = 0;
	virtual const CDesc * desc (ULONG col) const = 0;

	virtual ULONG cols() const = 0;
	virtual ULONG rows() const = 0;

	virtual void clear (CRecord &) const = 0;
	virtual bool read (ULONG row, CRecord &) const = 0;

	virtual ULONG colidx (const char * name) const;
	virtual ULONG colidx (const string& name) const;

	virtual bool get (vector<ULONG> & index, int cond, int col, CTerm*) const;
	virtual bool get (vector<ULONG> & index, int cond, const char* col, CTerm*) const;

	virtual const char * catalog() const = 0;
	virtual const char * schema() const = 0;
	virtual const char * name() const = 0;

};
//---------------------------------------------------------------------------
class CJoin: public CObject
{
public:
	CJoin (int head, const char * left, const char * right, CTerm* cond)
		: m_nHead(head), m_szLeft(left), m_szRight(right), m_pCond(cond) {}

	int m_nHead;
	const char * m_szLeft;
	const char * m_szRight;
	CTerm  * m_pCond;
};
//---------------------------------------------------------------------------
// Recordset
//---------------------------------------------------------------------------
class CRecordset
{
public:
	virtual ~CRecordset() {}
	vector<ULONG> filter (const CTerm * filter=0);
	vector<ULONG> sort (const vector<CTerm*> & order, const CTerm * filter=0);
	void add (CTerm * item) { m_aTerms.push_back (item); }
	bool EOT(ULONG);

	virtual bool move (ULONG i) = 0;
	virtual ULONG rows() const = 0;

	const vector<CTerm*> & terms;

protected:
	CRecordset() : terms (m_aTerms) {}
	CRecordset(const vector<CTerm*>& list) : m_aTerms(list), terms(m_aTerms) {}
	vector<CTerm*> m_aTerms;
};
//---------------------------------------------------------------------------
typedef auto_ptr<CRecordset> CResult;
//---------------------------------------------------------------------------
// Exception
//---------------------------------------------------------------------------
#define EXC CException(__FILE__,__LINE__).set
//#define ERR(h,s,id,t) (h).push(s,id,t,__FILE__,__LINE__)
class CException
{
public:
	typedef vector<CException*> CMsgList;

	CException (const CException & src) { (*this) = src; }
	CException (const char * file, ULONG line): szFile(file), nLine(line) {}

	CException (const char * state, ULONG id, const char * text, const char * file, ULONG line)
	: nId(id), strText(text), szFile(file), nLine(line)
	{
		memcpy (szState, state, 5);
	}

	CException & operator=(const CException & src)
	{
		if (&src == this)
			return *this;
		nId = src.nId;
		memcpy (szState, src.szState, sizeof(szState));
		strText = src.strText;
		szFile = src.szFile;
		nLine = src.nLine;
		return *this;
	}

	const CException& set (const char * state, ULONG id, const char * text, ...)
	{
		nId = id;
		strncpy (szState, state, sizeof(szState));
		char szBuff[8000];
		va_list aArgs;
		va_start (aArgs, text);
		vsprintf (szBuff, text, aArgs);
		va_end(aArgs);
		strText = szBuff;
		return *this;
	}

	ULONG nId;
	char szState[6];
	std::string strText;
	const char * szFile;
	ULONG  nLine;
};
//---------------------------------------------------------------------------
// DATABASE
//---------------------------------------------------------------------------
class IDatabase
{
public:
	IDatabase() {}
	virtual ~IDatabase();
	virtual CTable * table (const char * catalog, const char * schema, const char * name) = 0;
};
//---------------------------------------------------------------------------
typedef map<const char *,CTable*> CTableMap;
//---------------------------------------------------------------------------
class CStatement
{
public:
	CStatement (IDatabase *);
	virtual ~CStatement();
	void clear();
	const char * atom(const char *);
	const char * atom(const char *, long n);
	const char * sql(int len, bool quoted);
	const char * table (const char * catalog, const char * schema, const char * name, const char * alias);
	CTerm * column (const char * schema, const char * table, const char * column);
	CValue * value (double);
	CValue * value (long int);
	CValue * value (const char *);
	CValue * time  (const char *);
	CUnary * unary (int head, CTerm *);
	CFunction * func (int head, va_list args);
	CParam * param();
	const char * join (int head, const char * left, const char * right, CTerm * cond);

	void prepare (CFunction &);
	void prepare();
	void execute();
	ULONG rows() const;
	bool move (ULONG recno);

	void setSQL (const char *, int len=SQL_NTS);

	void dump();
	string exec (const char * command);

	vector<CTerm*> select (const CTableMap::iterator &);
	void select (const char * table, int column);
	void select();
	CTerm* result (CTerm*);

	void setFormat (CTerm*, const char * format);
	const char * getFormat (CTerm*) const;

	IDatabase * m_pDatabase;

	string m_strSQL;
	ULONG m_iSQL;

	bool m_bDistinct;
	CTableMap m_aTables;
	map<string,CTerm*> m_aNames;
	vector<CTerm*> m_aSelection;
	vector<CTerm*> m_aGroup;
	vector<CTerm*> m_aOrder;
	vector<CUnary*> m_aAggr;
	CTerm* m_pWhere;
	CTerm* m_pHaving;

	CRecordset * m_pResult;
	CTableSet*   m_pTableSet;
	CResult      m_aTmpTbl;
	map<ULONG, string> m_aFormat;
	vector<CJoin*> m_aJoins;
	vector<CObject*> m_aMemory;
	vector<const char *> m_aAtoms;
	vector<ULONG> m_aIndex;
	vector<CParam*> m_aParam;
//	vector<CParam*> m_aBind;
};
//---------------------------------------------------------------------------
} // namespace
#endif
