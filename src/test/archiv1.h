// test.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//

#ifndef __ARCHIV1_H__
#define __ARCHIV1_H__

#include <assert.h>
#include <stdio.h>
#include "database.h"

//---------------------------------------------------------------------------
class CArchiv1
{
public:
	CArchiv1 (CDatabase* db=0);
	void open (CDatabase &);
	void close();
	void purge();
	void create (long tid, const char * knz, long takt, long slots, bool erase=false);
	void create (long tid, long slots=0, bool erase=true);
	bool select (long tid);
	void set (long nTid, time_t t, long slots, double *pValue, char * szMark);
	bool get (long nTid, time_t t, long slots, double *pValue, char * szMark);

	CDatabase * m_pDatabase;
	char m_szTable[256];
	HSTMT m_hCtrl;
	HSTMT m_hData;
	long m_nTid;
	long m_nSlots;
	time_t m_nTakt;
	time_t m_nDate;
};
//---------------------------------------------------------------------------
#endif