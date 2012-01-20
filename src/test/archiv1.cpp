// test.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#include <assert.h>
#include <stdio.h>
#include "archiv1.h"

//---------------------------------------------------------------------------
CArchiv1::CArchiv1 (CDatabase* pDB)
{
	m_pDatabase = 0;
	if (pDB)
		open (*pDB);
	m_szTable[0] = 0;
	m_hCtrl = 0;
	m_hData = 0;
	m_nTid = 0;
	m_nSlots = 0;
	m_nTakt = 0;
	m_nDate = 0;

}
//---------------------------------------------------------------------------
void CArchiv1::open (CDatabase & aDB)
{
	if (m_pDatabase)
		close();
	m_pDatabase = &aDB;
	HSTMT & hCtrl = m_hCtrl;
	SQLRETURN nRet;
	const char * szSQL = "UPDATE TArchivCtrl SET [Date]=?, [RID]=? WHERE [TID]=? AND ([Date] IS NULL OR [Date] < ?)";
	nRet = SQLAllocStmt (aDB.m_hdbc, &hCtrl);
	nRet = SQLPrepare (hCtrl, (SQLCHAR*)szSQL, SQL_NTS);
}
//---------------------------------------------------------------------------
void CArchiv1::close()
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
bool CArchiv1::select (long nTid)
{
	assert (m_pDatabase);
	if (nTid <= 0)
		return false;
	if (m_nTid == nTid)
		return true;
	sprintf (m_szTable, "XA%06d", nTid);
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

	if (true)
	{
		char szSQL[8000];
		sprintf (szSQL, "UPDATE [%s] SET [VALUE]=?, [MARK]=? WHERE [RID]=?", m_szTable);
		if (m_hData)
		nRet = SQLFreeStmt (m_hData, SQL_DROP);
		nRet = SQLAllocStmt (hdbc, &m_hData);
		nRet = SQLPrepare (m_hData, (SQLCHAR*)szSQL, SQL_NTS);
	}
	return m_nTid > 0;
}
//---------------------------------------------------------------------------
void CArchiv1::create (long nTid, const char * szKNZ, long nTakt, long nSlots, bool bErase)
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
	SQLCHECK (nRet, hstmt, false);
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
	create (nTid, nSlots, bErase);
}
//---------------------------------------------------------------------------
void CArchiv1::create (long nTid, long nSlots, bool bDelete)
{
	assert (m_pDatabase);
	SQLRETURN nRet;
	HDBC & hdbc = m_pDatabase->m_hdbc;
	HSTMT hstmt;
	long nRid;
	char szDYN[8000];
	const char * szSQL;

	if (!select(nTid))
		return;

	nRet = SQLAllocStmt (hdbc, &hstmt);
	if (bDelete)
	{
		szSQL = "DROP TABLE [%s]";
		sprintf (szDYN, szSQL, m_szTable);
		nRet = SQLExecDirect (hstmt, (SQLCHAR*)szDYN, SQL_NTS);
	}

	szSQL = 
	"CREATE TABLE [%s] ("
	"[RID]    INTEGER NOT NULL,"
	"[VALUE]  DOUBLE,"
	"[MARK]   VARCHAR(4),"
	"PRIMARY KEY ([RID]))"
	;
	sprintf (szDYN, szSQL, m_szTable);
	nRet = SQLFreeStmt (hstmt, SQL_CLOSE);
	nRet = SQLExecDirect (hstmt, (SQLCHAR*)szDYN, SQL_NTS);
	SQLCHECK (nRet, hstmt, false);

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
	szSQL = "INSERT INTO [%s] ([RID],[VALUE],[MARK]) VALUES (?,0,'N')";
	sprintf (szDYN, szSQL, m_szTable);
	nRet = SQLPrepare (hstmt, (SQLCHAR*)szDYN, SQL_NTS);
	nRet = SQLBindParam (hstmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);
	for (nRid=0; nRid<nSlots; nRid++)
	{
		nRet = SQLExecute (hstmt);
	}
	nRet = SQLFreeStmt (hstmt, SQL_DROP);
}
//---------------------------------------------------------------------------
void CArchiv1::set (long nTid, time_t t, long nSlots, double * pValue, char * szMark)
{
	assert (nSlots);
	assert (pValue);
	assert (szMark);
	HSTMT & hCtrl = m_hCtrl;
	HSTMT & hData = m_hData;
	SQLINTEGER nWertInd;
	SQLINTEGER nMarkInd = 1;
	SQLRETURN nRet;
	double dValue;
	char cMark;
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
	//// UPDATE [%s] SET [VALUE]=?, [MARK]=? WHERE [RID]=?
	nRet = SQLBindParam (hData, 1, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &dValue, &nWertInd);
	nRet = SQLBindParam (hData, 2, SQL_C_CHAR, SQL_CHAR, 255, 0, (SQLCHAR*)&cMark, &nMarkInd);
	nRet = SQLBindParam (hData, 3, SQL_C_LONG, SQL_INTEGER, 0, 0, &nRid, 0);
	SQLCHECK (nRet, hData, true);
	long i0 = (t / m_nTakt + 1 - nSlots) % m_nSlots;
	for (i=0; i<nSlots; i++)
	{
		nRid = (i0 + i) % m_nSlots;
		nWertInd = (szMark[i] == 'N') ? SQL_NULL_DATA : sizeof(double);
		nMarkInd = (szMark[i] == ' ') ? SQL_NULL_DATA : 1;
		dValue = pValue[i];
		cMark = szMark[i];
		nRet = SQLExecute (hData);
		SQLCHECK (nRet, hData, true);
	}

	//// Ctrl-Table aktualisieren
	t -= t % m_nTakt;
//	printf ("\nrecord [%d]=%s", nRid, ctime(&t));
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
}
//---------------------------------------------------------------------------
bool CArchiv1::get (long nTid, time_t t, long nSlots, double * pWert, char * pMark)
{
	assert (nSlots);
	assert (pWert);
	assert (pMark);
	HSTMT hstmt;
	HDBC & hdbc = m_pDatabase->m_hdbc;
	double dWert;
	char szMark[256];
	char szDYN[8000];
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
	"SELECT TVal.VALUE, TVal.MARK, TVal.RID "
	"FROM TArchivCtrl TCtrl, [%s] TVal "
	"WHERE TCtrl.TID = ? "
//	"AND TArchivCtrl.Date+((TArchivValue.RID-TArchivCtrl.RID-TArchivCtrl.Slots) Mod TArchivCtrl.Slots)*TArchivCtrl.Takt/86400 >= ? "
	"AND ((TVal.RID-TCtrl.RID-TCtrl.Slots) Mod TCtrl.Slots) >= ? "
	"ORDER BY (TVal.RID - TCtrl.RID-1 + TCtrl.Slots) Mod TCtrl.Slots"
	;
	sprintf (szDYN, szSQL, m_szTable);

	long nSlot0 = (t - m_nDate) / m_nTakt;
//	SQL_TIMESTAMP_STRUCT aTime = SQLTime(t);
	nRet = SQLAllocStmt (hdbc, &hstmt);
	nRet = SQLPrepare (hstmt, (SQLCHAR*)szDYN, SQL_NTS);
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
void CArchiv1::purge()
{
	assert (m_pDatabase);
	HDBC & hdbc = m_pDatabase->m_hdbc;
	HSTMT hStmt;
	HSTMT hQuery;
	SQLRETURN nRet;
	char szTable[256];
	SQLINTEGER nTableInd;

	nRet = SQLAllocStmt (hdbc, &hQuery);
	nRet = SQLAllocStmt (hdbc, &hStmt);
	nRet = SQLTables (hQuery
	, (SQLCHAR*)0, 0
	, (SQLCHAR*)0, 0
	, (SQLCHAR*)0, 0
	, (SQLCHAR*)0, 0);
	SQLCHECK(nRet, hQuery, true);
	nRet = SQLBindCol (hQuery, 3, SQL_C_CHAR, (SQLPOINTER)(szTable), sizeof(szTable), &nTableInd);
	const char * szSQL = "DROP TABLE [%s]";
	char szDYN[8000];
	long nTid = 0;
	while (true)
	{
		nRet = SQLFreeStmt (hStmt, SQL_CLOSE);
		nRet = SQLFetch (hQuery);
		if (nRet == SQL_NO_DATA)
			break;
		if (nTableInd == SQL_NULL_DATA)
			continue;
		if (strlen(szTable) != 8)
			continue;
		if (strncmp ("XA", szTable, 2))
			continue;
		if (!sscanf (szTable, "XA%d", &nTid))
			continue;;
		nRet = SQLPrepare (hStmt, (SQLCHAR*)"SELECT * FROM TArchivCtrl WHERE TID=?", SQL_NTS);
		nRet = SQLBindParam (hStmt, 1, SQL_C_LONG, SQL_INTEGER, 0, 0, &nTid, 0);
		nRet = SQLExecute (hStmt);
		nRet = SQLFetch (hStmt);
		if (nRet != SQL_NO_DATA)
			continue;
		printf ("deleting table [%s]...\n", szTable);
		nRet = SQLFreeStmt (hStmt, SQL_CLOSE);
		sprintf (szDYN, szSQL, szTable);
		nRet = SQLExecDirect (hStmt, (SQLCHAR*)szDYN, SQL_NTS);
		SQLCHECK(nRet, hStmt, true);
	}
	nRet = SQLFreeStmt (hStmt, SQL_DROP);
	nRet = SQLFreeStmt (hQuery, SQL_DROP);
}
//---------------------------------------------------------------------------
