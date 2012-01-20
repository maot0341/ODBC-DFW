// test.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//

#ifndef __ARCHIV_H__
#define __ARCHIV_H__

#include <assert.h>
#include <stdio.h>
#include "database.h"
#include "archiv1.h"

//---------------------------------------------------------------------------
class CArchiv
{
public:
	static const time_t MINUTE;
	static const time_t HOUR;
	static const time_t DAY;
	static const time_t YEAR;

	CArchiv (CDatabase*db=0);
	~CArchiv() { close(); }
	bool select (long tid);
	void create (long tid, const char * knz, long takt, long slots);
	void create (long tid, long slots=0, bool erase=true);
	void open (CDatabase &);
	void set (long nTid, time_t t, double dValue, const char * szMark);
	void set (long nTid, time_t t, long slots, double *pValue, char * szMark);
	bool get (long nTid, time_t t, long slots, double *pValue, char * szMark);
	void close();

	CDatabase * m_pDatabase;
	HSTMT m_hCtrl;
	HSTMT m_hData;
	long m_nTid;
	long m_nSlots;
	time_t m_nTakt;
	time_t m_nDate;
};
//---------------------------------------------------------------------------
#endif