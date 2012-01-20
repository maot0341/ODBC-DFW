// test.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#include <assert.h>
#include <stdio.h>
#include "archiv.h"

//---------------------------------------------------------------------------
const time_t CArchiv::MINUTE = 60;
const time_t CArchiv::HOUR = 3600;
const time_t CArchiv::DAY = 24 * 3600;
const time_t CArchiv::YEAR 
= 365 * CArchiv::DAY
+ 6 * CArchiv::HOUR
+ 9 * CArchiv::MINUTE
+ 10;
//---------------------------------------------------------------------------
CArchiv::CArchiv (CDatabase * pDB)
{
	m_pDatabase = 0;
	m_nTid = 0;
	m_nSlots = 0;
	m_nTakt = 0;
	m_nDate = 0;
	if (pDB)
		open (*pDB);
}
//---------------------------------------------------------------------------
bool CArchiv::select (long nTid)
{
	assert (m_pDatabase);
	if (nTid <= 0)
		return false;
	if (m_nTid == nTid)
		return true;
	SQLRETURN nRet;
	HDBC & hdbc = m_pDatabase->m_hdbc;
	HSTMT hstmt;
	SQL_TIMESTAMP_STRUCT aDate;
	SQLINTEGER nSlotsInd = SQL_NULL_DATA;
	SQLINTEGER nTaktInd  = SQL_NULL_DATA;
	SQLINTEGER nDateInd  = SQL_NULL_DATA;
	nRet = SQLAllocStmt (hdbc, &hstmt);

	const char * szSQL = "SELECT slots,takt,[date] FROM TArchivCtrl WHERE [TID]=?";
	nRet = SQLPrepare (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
	nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
	nRet = SQLExecute (hstmt);
	nRet = SQLBindCol (hstmt, 1, SQL_C_LONG, &m_nSlots, 0, &nSlotsInd);
	nRet = SQLBindCol (hstmt, 2, SQL_C_LONG, &m_nTakt , 0, &nTaktInd);
	nRet = SQLBindCol (hstmt, 3, SQL_C_TYPE_TIMESTAMP, &aDate, 0, &nDateInd);
	nRet = SQLFetch (hstmt);
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
	if (nSlotsInd == SQL_NULL_DATA)
		m_nSlots = 0;
	if (nTaktInd == SQL_NULL_DATA)
		m_nTakt = 0;
	if (nDateInd  == SQL_NULL_DATA)
		m_nDate = 0;
	else
		m_nDate = SQLTime (aDate);
	m_nTid = nTid;
	if (m_nSlots == 0 || m_nTakt == 0)
		m_nTid = 0;
	return m_nTid > 0;
}
//---------------------------------------------------------------------------
void CArchiv::open (CDatabase & aDB)
{
	if (m_pDatabase)
		close();
	m_pDatabase = &aDB;
	HSTMT & hCtrl = m_hCtrl;
	HSTMT & hData = m_hData;
	SQLRETURN nRet;
	const char * szSQL1 = "UPDATE TArchivCtrl SET [Date]=?, [RID]=? WHERE [TID]=? AND ([Date] IS NULL OR [Date] < ?)";
	nRet = SQLAllocStmt (aDB.m_hdbc, &hCtrl);
	nRet = SQLPrepare (hCtrl, (SQLCHAR*)szSQL1, SQL_NTS);

	const char * szSQL2 = "UPDATE TArchivValue SET [VALUE]=?, [MARK]=? WHERE [TID]=? AND [RID]=?";
	nRet = SQLAllocStmt (aDB.m_hdbc, &hData);
	nRet = SQLPrepare (hData, (SQLCHAR*)szSQL2, SQL_NTS);
}
//---------------------------------------------------------------------------
void CArchiv::close()
{
	HSTMT & hCtrl = m_hCtrl;
	HSTMT & hData = m_hData;
	SQLRETURN nRet;
	if (hCtrl)
	nRet = SQLFreeStmt (hCtrl, SQL_DROP);
	if (hData)
	nRet = SQLFreeStmt (hData, SQL_DROP);
	hCtrl = 0;
	hData = 0;
}
//---------------------------------------------------------------------------
void CArchiv::create (long nTid, const char * szKNZ, long nTakt, long nSlots)
{
	assert (m_pDatabase);
	assert (nTid > 0);
	assert (nTakt > 0);
	assert (nSlots > 0);
	assert (szKNZ && *szKNZ);
	SQLINTEGER cbNTS = SQL_NTS;
	SQLRETURN nRet;
	HDBC & hdbc = m_pDatabase->m_hdbc;
	HSTMT hstmt;
	const char * szSQL = "INSERT INTO TArchivCtrl (TID,KNZ,TAKT,SLOTS,[DATE],RID) VALUES (?,?,?,?,NULL,NULL)";
	nRet = SQLAllocStmt (hdbc, &hstmt);
	nRet = SQLPrepare (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
	nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid,   0);
	nRet = SQLBindParam (hstmt, 2, SQL_C_CHAR, SQL_CHAR,  255, 0, (SQLCHAR*)szKNZ, &cbNTS);
	nRet = SQLBindParam (hstmt, 3, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTakt, 0);
	nRet = SQLBindParam (hstmt, 4, SQL_C_LONG, SQL_INTEGER, 0, 0, &nSlots, 0);
	nRet = SQLExecute (hstmt);
	SQLCHECK (nRet, hstmt, true);
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
	create (nTid, nSlots, false);
}
//---------------------------------------------------------------------------
void CArchiv::create (long nTid, long nSlots, bool bDelete)
{
	assert (m_pDatabase);
	SQLRETURN nRet;
	HDBC & hdbc = m_pDatabase->m_hdbc;
	HSTMT hstmt;
	long nRid;
	const char * szSQL;

	if (!select(nTid))
		return;

	nRet = SQLAllocStmt (hdbc, &hstmt);
	if (bDelete)
	{
		szSQL = "DELETE FROM TArchivValue WHERE [TID]=?";
		nRet = SQLPrepare (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
		nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
		nRet = SQLExecute (hstmt);
	}
	if (nSlots <= 0)
		nSlots = m_nSlots;
	if (m_nSlots != nSlots)
	{
		szSQL = "UPDATE TArchivCtrl SET slots=?, [date]=NULL, RID=NULL WHERE TID=?";
		nRet = SQLPrepare (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
		nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nSlots, 0);
		nRet = SQLBindParam (hstmt, 2, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid,   0);
		nRet = SQLExecute (hstmt);
	}
	szSQL = "INSERT INTO TArchivValue ([TID],[RID],[VALUE],[MARK]) VALUES (?,?,0,'N')";
	nRet = SQLPrepare (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
	nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
	nRet = SQLBindParam (hstmt, 2, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);
	for (nRid=0; nRid<nSlots; nRid++)
	{
		nRet = SQLExecute (hstmt);
	}
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
}
//---------------------------------------------------------------------------
void CArchiv::set (long nTid, time_t t, double dValue, const char * szMark)
{
	select (nTid);
	SQL_TIMESTAMP_STRUCT aTime = SQLTime(t);
	long nRid;
	assert (m_nSlots);
	assert (m_nTakt);
	HSTMT & hCtrl = m_hCtrl;
	HSTMT & hData = m_hData;
	SQLINTEGER cbNTS = SQL_NTS;
	SQLRETURN nRet;

	nRet = SQLBindParam (hCtrl, 1, SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, &aTime, 0);
	nRet = SQLBindParam (hCtrl, 2, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);
	nRet = SQLBindParam (hCtrl, 3, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
	nRet = SQLBindParam (hCtrl, 4, SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, &aTime, 0);

	nRet = SQLBindParam (hData, 1, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &dValue, 0);
	nRet = SQLBindParam (hData, 2, SQL_C_CHAR, SQL_CHAR, 255, 0, (SQLCHAR*)szMark, &cbNTS);
	nRet = SQLBindParam (hData, 3, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
	nRet = SQLBindParam (hData, 4, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);

	nRid = (t / m_nTakt) % m_nSlots;

	nRet = SQLExecute (hCtrl);
	nRet = SQLExecute (hData);
}
//---------------------------------------------------------------------------
void CArchiv::set (long nTid, time_t t, long nSlots, double * pValue, char * szMark)
{
	assert (nSlots);
	assert (pValue);
	assert (szMark);
	HSTMT & hCtrl = m_hCtrl;
	HSTMT & hData = m_hData;
	SQLINTEGER nWertInd;
	SQLINTEGER nMarkInd = 1;
	SQLRETURN nRet;
	long nRid;
	long i;

	select (nTid);
	ASSUME (m_nSlots);
	ASSUME (m_nTakt);

	//// t = start oder ende?
	if (nSlots < 0)
		nSlots = -nSlots;
	else
		t += (nSlots-1) * m_nTakt;
	//// t hinreichend neu?
	if (m_nDate)
	if (t < m_nDate - (m_nSlots-1) * m_nTakt)
		return;

	//// Value-Table aktualisieren
	nRet = SQLBindParam (hData, 3, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
	SQLCHECK (nRet, hData, true);
	nRet = SQLBindParam (hData, 4, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);
	long i0 = (t / m_nTakt + 1 - nSlots) % m_nSlots;
	for (i=0; i<nSlots; i++)
	{
		nRid = (i0 + i) % m_nSlots;
		nWertInd = (szMark[i] == 'N') ? SQL_NULL_DATA : sizeof(double);
		nMarkInd = (szMark[i] == ' ') ? SQL_NULL_DATA : 1;
		nRet = SQLBindParam (hData, 1, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, (pValue+i), &nWertInd);
		nRet = SQLBindParam (hData, 2, SQL_C_CHAR, SQL_CHAR, 255, 0, (SQLCHAR*)(szMark+i), &nMarkInd);
		nRet = SQLExecute (hData);
		SQLCHECK (nRet, hData, true);
	}

	//// Ctrl-Table aktualisieren
	t -= t % m_nTakt;
	printf ("\nrecord [%d]=%s", nRid, ctime(&t));
	if (t <= m_nDate)
		return;
	m_nDate = t;
	SQL_TIMESTAMP_STRUCT aTime = SQLTime(t);
	nRet = SQLBindParam (hCtrl, 1, SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, &aTime, 0);
	nRet = SQLBindParam (hCtrl, 2, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);
	nRet = SQLBindParam (hCtrl, 3, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
	nRet = SQLBindParam (hCtrl, 4, SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, &aTime, 0);
	nRet = SQLExecute (hCtrl);
	SQLCHECK (nRet, hCtrl, true);
//	CDBException (SQL_HANDLE_STMT, hCtrl).trace();
}
//---------------------------------------------------------------------------
bool CArchiv::get (long nTid, time_t t, long nSlots, double * pWert, char * pMark)
{
	assert (nSlots);
	assert (pWert);
	assert (pMark);
	HSTMT hstmt;
	HDBC & hdbc = m_pDatabase->m_hdbc;
	double dWert;
	char szMark[256];
	SQLINTEGER nWertInd;
	SQLINTEGER nMarkInd;
	SQLINTEGER nRidInd;
	SQLRETURN nRet;
	long i;

	if (!select (nTid))
		return false;
	assert (m_nTakt);
	assert (m_nSlots);
	assert (m_nDate);
	long nRid;
	if (nSlots < 0)
	{
		nSlots = -nSlots;
		t -= (nSlots - 1) * m_nTakt;
	}
	if (nSlots > m_nSlots)
		nSlots = m_nSlots;
	const char * szSQL = 
	"SELECT TArchivValue.VALUE, TArchivValue.MARK, TArchivValue.RID "
	"FROM TArchivCtrl INNER JOIN TArchivValue ON TArchivCtrl.TID = TArchivValue.TID "
	"WHERE TArchivValue.TID = ? "
//	"AND TArchivCtrl.Date+((TArchivValue.RID-TArchivCtrl.RID-TArchivCtrl.Slots) Mod TArchivCtrl.Slots)*TArchivCtrl.Takt/86400 >= ? "
	"AND ((TArchivValue.RID-TArchivCtrl.RID-TArchivCtrl.Slots) Mod TArchivCtrl.Slots) >= ? "
	"ORDER BY (TArchivValue.RID - TArchivCtrl.RID-1 + TArchivCtrl.Slots) Mod TArchivCtrl.Slots"
	;

	long nSlot0 = (t - m_nDate) / m_nTakt;
//	SQL_TIMESTAMP_STRUCT aTime = SQLTime(t);
	nRet = SQLAllocStmt (hdbc, &hstmt);
	nRet = SQLPrepare (hstmt, (SQLCHAR*)szSQL, SQL_NTS);
	nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
//	nRet = SQLBindParam (hstmt, 2, SQL_C_TYPE_TIMESTAMP, SQL_TIMESTAMP, 0, 0, &aTime, 0);
	nRet = SQLBindParam (hstmt, 2, SQL_C_LONG, SQL_INTEGER, 0, 0, &nSlot0, 0);
	nRet = SQLExecute (hstmt);
	nRet = SQLBindCol (hstmt, 1, SQL_C_DOUBLE, (SQLPOINTER)(&dWert), 0, &nWertInd);
	nRet = SQLBindCol (hstmt, 2, SQL_C_CHAR, (SQLPOINTER)(szMark), 2, &nMarkInd);
	nRet = SQLBindCol (hstmt, 3, SQL_C_LONG, &nRid, 0, &nRidInd);

	for (i=0; i<nSlots; i++)
	{
		nRet = SQLFetch (hstmt);
		if (nRet == SQL_NO_DATA)
			break;
		if (nRet != SQL_SUCCESS)
		if (nRet != SQL_SUCCESS_WITH_INFO)
			break;
		pWert[i] = dWert;
		pMark[i] = szMark[0];
		if (nWertInd == SQL_NULL_DATA)
		{
			pWert[i] =  0.0;
			pMark[i] = 'N';
		}
		if (nMarkInd == SQL_NULL_DATA)
			pMark[i] = ' ';
	}
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
	return true;
}
//---------------------------------------------------------------------------
#if 0
void CArchiv::open (CDatabase & aDB)
{
	if (m_pDatabase)
		close();
	m_pDatabase = &aDB;
	HSTMT & hCtrl = m_hCtrl;
	HSTMT & hData = m_hData;
	SQLRETURN nRet;
	const char * szSQL1 = 
	"UPDATE TArchivCtrl SET [Date]=? [RID]= (? / [Takt] * 86400) MOD [Slots] "
	"WHERE [TID]=? AND ([Date] IS NULL OR [Date] < ?)";
	nRet = SQLAllocStmt (aDB.m_hdbc, &hCtrl);
	nRet = SQLPrepare (hCtrl, (SQLCHAR*)szSQL1, SQL_NTS);

	const char * szSQL2 = 
	"UPDATE TArchivValue T1 INNER JOIN TArchivCtrl T2 ON T1.TID = T2.TID "
	"SET T1.[VALUE]=?, T1.[MARK]=? "
	"WHERE T1.[TID]=? AND T1.[RID] = (? / T2.[Takt] * 86400) MOD T2.[Slots]";
	nRet = SQLAllocStmt (aDB.m_hdbc, &hData);
	nRet = SQLPrepare (hData, (SQLCHAR*)szSQL2, SQL_NTS);
}
#endif
//---------------------------------------------------------------------------
