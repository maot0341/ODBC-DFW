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

#ifndef __SQLERROR_H__
#define __SQLERROR_H__


#include <string>
#include <stdx/debug.h>

using namespace std;
//---------------------------------------------------------------------------
// SQLTables QUERY
//---------------------------------------------------------------------------
class CSQLException
{
public:
	CSQLException (const char * file, size_t line);
	virtual ~CSQLException();
	const CSQLException & set (const char * szState, long nError, const char * szMessage, ...);
	const char * text() const { return m_strText.c_str(); }
	const char * state() const { return m_szState; }
	long code() const { return m_nCode; }

protected:
	char m_szState[6];
	long m_nCode;
	string m_strText;
	const char * m_szFile;
	size_t m_nLine;
};
#define EXC CSQLException(__FILE__,__LINE__).set
//---------------------------------------------------------------------------
class CSQLError
{
public:
	CSQLError();
	virtual ~CSQLError();

	struct record_t
	{
		const char * szSQL_DIAG_CLASS_ORIGIN;
		long nSQL_DIAG_COLUMN_NUMBER;
		const char * szSQL_DIAG_CONNECTION_NAME;
		char * szSQL_DIAG_MESSAGE_TEXT;
		long nSQL_DIAG_NATIVE;
		long nSQL_DIAG_ROW_NUMBER;
		const char * szSQL_DIAG_SERVER_NAME;
		char szSQL_DIAG_SQLSTATE[6];
		const char * szSQL_DIAG_SUBCLASS_ORIGIN;
		const char * szFile;
		size_t nLine;
	};

	void set (const CSQLException &);
	long set (long nRet, const char * szState, long nError, const char * szMessage, ...);
	void set (const char * szState, long nError, const char * szMessage, ...);
	void add (const char * szState, long nError, const char * szMessage, ...);
	void clear(bool bfree=false);
	long count() const { return m_nSQL_DIAG_ROW_COUNT; }
	const record_t & record(long i) const { ASSUME (i < m_nSQL_DIAG_ROW_COUNT); return m_aData[i]; }
	const record_t * fetch();


	long m_nSQL_DIAG_CURSOR_ROW_COUNT;
	const char * m_szSQL_DIAG_DYNAMIC_FUNCTION;
	long m_nSQL_DIAG_DYNAMIC_FUNCTION_CODE;
	long m_nSQL_DIAG_NUMBER;
	long m_nSQL_DIAG_RETURNCODE;
	long m_nSQL_DIAG_ROW_COUNT;

protected:
	record_t m_aData[10];
	long m_iRecord;
};
//---------------------------------------------------------------------------
#endif
