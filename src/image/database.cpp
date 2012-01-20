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

#include <set>
#include <time.h>
#include <io.h>
#include "image.h"
#include "driver.h"
#include "sqlplib/index.h"
#include "sqlplib/yac.h"
#include "database.h"

using namespace std;
using namespace idx;
using namespace sqlp;


#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))
#define SYM(id) #id
//---------------------------------------------------------------------------
CTableBase::CTableBase (const column_t aDesc[], size_t nCols)
{
	char szBuff[8000];
	wsprintf (szBuff, "DB: %s\n", SYM(SQL_HANDLE_ENV));
	OutputDebugString (szBuff);
	m_aDesc = aDesc;
	m_nCols = nCols;
	m_nRows = 0;
}
//---------------------------------------------------------------------------
CTableBase::~CTableBase()
{
}
//---------------------------------------------------------------------------
CValue
CTableBase::desc (int iAttr, size_t iCol) const
{
	if (iAttr == SQL_DESC_COUNT)
		return CValue (TermINT, m_nCols);
	ASSUME (iCol < m_nCols);
	const column_t & aColumn = m_aDesc[iCol];
	if (iAttr == SQL_DESC_NAME)
		return CValue (aColumn.name);
	if (iAttr == SQL_DESC_TYPE)
	{
		int nType = CDriver::sql_type (aColumn.type);
		return CValue (TermINT, nType);
	}
	return CValue();
}
//---------------------------------------------------------------------------
void
CTableBase::clear (CRecord & raRecord) const
{
	size_t i;
	const size_t nCols = cols();
	raRecord.resize (nCols);
	for (i=0; i<nCols; i++)
	{
		raRecord[i].clear (col_type(i));
	}
}
//---------------------------------------------------------------------------
const column_t
CTableTST::ms_aDesc[] =
{	{"a1", TermVCHAR}
,	{"t1", TermTIME}
,	{"n1", TermINT}
,	{"d1", TermFLOAT}
};
//---------------------------------------------------------------------------
CTableTST::CTableTST (const char * szName)
: CTableBase (ms_aDesc, DIM(ms_aDesc))
{
	if (szName)
		m_strName = szName;
	m_nRows = 10;
}
//---------------------------------------------------------------------------
bool
CTableTST::fetch (size_t iRec, CRecord & raRecord) const
{
	if (iRec >= m_nRows)
		return false;
	const size_t nCols = cols();
	const char * szTable = m_strName.c_str();
	for (int j=0; j<nCols; j++)
	{
		const char * szColumn = col_name (j);
		CValue & raValue = raRecord[j];
		if (strcmp (szTable, "test"))
		if (((iRec+j) % 2) && (szTable[0] % 2 == 0))
		{
			raValue = CValue::null;
			continue;
		}
		term_t eType = raValue.type();
		if (CTerm::isString (eType))
		{
			char szValue[256];
			memset (szValue, 0, sizeof (szValue));
			const int i= (int)iRec;
			sprintf (szValue, "%s[%02d,%02d]", szColumn, i, j);
			raValue = szValue;
		}
		else if (CTerm::isDateTime (eType))
		{
			struct tm aTime;
			memset (&aTime, 0, sizeof(aTime));
			aTime.tm_mday = 1;
			aTime.tm_mon = 1 - 1;
			aTime.tm_year = 2000 - 1900;
			static time_t nTime = mktime (&aTime);
			raValue = nTime + iRec * 24 * 3600;
		}
		else if (eType == TermFLOAT)
		{
			raValue = 1.0 / ((double) iRec + 1);
		}
		else if (CTerm::isNumber (eType))
		{
			raValue = -(double)iRec;
		}
		else
		{
			raValue = CValue::null;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
