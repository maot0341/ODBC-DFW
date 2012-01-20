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

#include <assert.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqlplib/index.h>

#define CTableTST_GENERIC

#include "CTableTST.h"
//---------------------------------------------------------------------------
CTableTST::CTableTST (const char * szSchema, const char * szName)
: CTableImpl(szSchema, szName)
{
//	column (SQL_GUID,     "rid", 128, 0, SQL_NO_NULLS);
	column (SQL_INTEGER,  "rid", 128, 0, SQL_NO_NULLS);
	column (SQL_CHAR,     "a1",  128, 0, SQL_NULLABLE);
	column (SQL_DATETIME, "t1",  128, 0, SQL_NULLABLE);
	column (SQL_INTEGER,  "n1",  128, 0, SQL_NULLABLE);
	column (SQL_DOUBLE,   "d1",  128, 0, SQL_NULLABLE);
	m_nRows = 100;
	if (szName[0] == 'c')
		m_nRows = 100000;

}
//---------------------------------------------------------------------------
ULONG 
CTableTST::rows() const
{
	return m_nRows;
}
//---------------------------------------------------------------------------
void 
CTableTST::clear (CRecord & raRecord) const
{
	const ULONG nCols = m_aDesc.size();
	ULONG i;
	raRecord.resize (nCols);
	for (i=0; i<nCols; i++)
	{
		const CDesc & aDesc = m_aDesc[i];
		const short nType = aDesc.aSQL_DESC_TYPE();
		raRecord[i].clear (nType);
	}
}
//---------------------------------------------------------------------------
bool
CTableTST::read (ULONG iRec, CRecord & raRecord) const
{
	if (iRec >= m_nRows)
		return false;
	const ULONG nCols = cols();
	assert (nCols);
	const char * szTable = m_strName.c_str();
	raRecord[0] = iRec;
	for (int j=1; j<nCols; j++)
	{
		const CDesc * pDesc = desc(j);
		assert (pDesc);
		const char * szColumn = pDesc->name();
		CValue & raValue = raRecord[j];
		if (((iRec+j) % 2) != 0 && (szTable[0] % 2 == 0))
		{
			raValue.setNull();
			continue;
		}
		short nType = raValue.type();
		if (CTerm::isString (nType))
		{
			char szValue[256];
			memset (szValue, 0, sizeof (szValue));
			const int i= (int)iRec;
			sprintf (szValue, "%s[%02d,%02d]", szColumn, i, j);
			raValue = szValue;
		}
		else if (CTerm::isDateTime (nType))
		{
			struct tm aTime;
			memset (&aTime, 0, sizeof(aTime));
			aTime.tm_mday = 1;
			aTime.tm_mon = 1 - 1;
			aTime.tm_year = 2000 - 1900;
			static time_t nTime = mktime (&aTime);
			raValue = nTime + iRec  * 3600;
		}
		else if (CTerm::isFloat (nType))
		{
			raValue = 1.0 / ((double) iRec + 1);
		}
		else if (CTerm::isNumber (nType))
		{
			raValue = (double) iRec;
		}
		else
		{
			raValue.setNull();
		}
	}
	return true;
}
//---------------------------------------------------------------------------
bool
CTableTST::get (vector<ULONG> & raIndex, int nHeader, int nCol, CTerm* pValue) const
{
	if (nHeader == '=')
	if (nCol == 0)
	{
		int nValue = pValue->asInteger();
		raIndex.clear();
		idx::append (raIndex, nValue);
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------
