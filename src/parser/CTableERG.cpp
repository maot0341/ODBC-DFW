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
#include <sql.h>
#include <sqlext.h>
#include <sqlplib/index.h>
#include <vector>


#include "CTableERG.h"

using namespace std;
//---------------------------------------------------------------------------
time_t UTCTime (size_t i)
{
	return i ? (i - 613609) * 3600 : 0;
	return i ? (i - 58574113)* 3600 : 0;
//		const size_t nTime = aRecord.start ? (aRecord.start - 58574113)* 3600 : 0;
}
//---------------------------------------------------------------------------
CTableERG::CTableERG (const char * szSchema, const char * szPath)
: CTableImpl (szSchema, "p-archiv.dat")
, m_aFile (szPath, "p-archiv.dat")
{
//	m_szName = m_strName.c_str();
	column (SQL_CHAR,      "knz", 128, 0, SQL_NO_NULLS);
	column (SQL_CHAR,      "arc", 128, 0, SQL_NO_NULLS);
	column (SQL_INTEGER,   "rid", 128, 0, SQL_NO_NULLS);
	column (SQL_INTEGER,   "val", 128, 0, SQL_NO_NULLS);
	column (SQL_DATETIME, "time", 128, 0, SQL_NULLABLE);
	column (SQL_DOUBLE,    "reg", 128, 0, SQL_NULLABLE);
	column (SQL_DOUBLE,    "hrg", 128, 0, SQL_NULLABLE);
	column (SQL_DOUBLE,    "ztr", 128, 0, SQL_NULLABLE);

	m_nRows = 0;
}
//---------------------------------------------------------------------------
ULONG
CTableERG::rows (const tppro::CStorage::typItem * pItem) const
{
	if (!pItem || !tppro::CStorage::isValid (*pItem))
		return 0;
	const size_t nVal = (pItem->nLen - sizeof(time_t)) / sizeof(float);
	return nVal * (nVal + 24);
}
//---------------------------------------------------------------------------
ULONG
CTableERG::rows() const
{
	if (m_nRows)
		return m_nRows;

	//// create recordset
	tppro::CStorage & aFile = m_aFile;
	aFile.open();
	aFile.rewind();
	while (aFile.current())
	{
		const tppro::CStorage::typItem * pItem = aFile.current();
		assert (pItem);
		m_nRows += rows (pItem);
		aFile.next();
	}
	return m_nRows;
}
//---------------------------------------------------------------------------
void 
CTableERG::clear (CRecord & raRecord) const
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
CTableERG::read (ULONG iRec, CRecord & raRecord) const
{
	const int nVerf = TPPRO_VERF;
//	if ((iRec % 10000) == 0)
//		_RPT2(_CRT_WARN, "Table ERG: %ld of %ld\n", iRec, m_nRows); 
	if (iRec >= rows())
		return false;
	tppro::typErgArchiv aRecord;
	tppro::CStorage & aFile = const_cast<tppro::CStorage &>(m_aFile);

	for (aFile.rewind(); aFile.current(); aFile.next())
	{
		const tppro::CStorage::typItem * pItem = aFile.current();
		assert (pItem);
		if (!aFile.isValid (*pItem))
			continue;
		const size_t nRec = rows(pItem);
		if (iRec < nRec)
			break;
		iRec -= nRec;
	}
	const tppro::CStorage::typItem * pItem = aFile.current();
	if (!pItem)
		return false;
	const size_t nLen = pItem->nLen;
	const size_t nRec = pItem->nRec;
	const size_t nVal = (nLen - sizeof(aRecord.start)) / sizeof(aRecord.val[0]);
	const size_t nErg = nVal * nVal;
	const size_t nAbw = nVal * 24;
	const size_t iRun = iRec / nVal;
	const size_t iVal = iRec % nVal;
	const size_t iRow = iRun * nVerf;
	const char * szKnz = pItem->szKey;
	const char * szArc = (iRec < nErg) ? "KPRG" : "FREL";
	const size_t iBlk = (iRec < nErg) ? iRun : iRun - nVal;

	int i;

	raRecord[0] = szKnz;
	raRecord[1] = szArc;
	raRecord[2] = iBlk;
	raRecord[3] = iVal;
	raRecord[4].setNull();
	raRecord[5].setNull();
	raRecord[6].setNull();
	raRecord[7].setNull();
	for (i=0; i<3; i++)
	{
		aFile.read (iRow+i, &aRecord, nLen);
		const size_t nTime = UTCTime (aRecord.start);
		const double dValue = aRecord.val[iVal];
		if (!nTime)
			continue;
		if (dValue <= 0)
			continue;
		raRecord[4] = nTime;
		raRecord[5+i] = dValue;
	}
	return true;
}
//---------------------------------------------------------------------------
bool
CTableERG::get (vector<ULONG> & raIndex, int nHeader, int nCol, CTerm* pValue) const
{
	if (!pValue)
		return false;

	static vector< vector<ULONG>*> ms_aData(4,0);
	if (nCol >= ms_aData.size())
		return false;
	if (ms_aData[nCol])
	{
		raIndex = *ms_aData[nCol];
		return true;
	}

	tppro::CStorage & aFile = const_cast<tppro::CStorage &>(m_aFile);
	const char * szValue = pValue->asString();
	const double dValue = pValue->asDouble();
	size_t iRec = 0;
	aFile.open();
	for (aFile.rewind(); aFile.current(); aFile.next())
	{
		const tppro::CStorage::typItem * pItem = aFile.current();
		if (!aFile.isValid(*pItem))
			continue;
		const size_t nVal = (pItem->nLen - sizeof(time_t)) / sizeof(float);
 		const size_t nErg = nVal * nVal;
		const size_t nAbw = 24 * nVal;
		const size_t nRec = rows (pItem);
		if (nCol == 0)
		{
			const char * szKey = pItem->szKey;
			const int nCmp = strcmp (szKey, szValue);
			if ((nHeader == '=' && nCmp == 0)
			||  (nHeader == '<' && nCmp < 0)
			||  (nHeader == '>' && nCmp > 0))
			{
				idx::append (raIndex, iRec, nRec);
			}
		}
		else if (nCol == 1)
		{
			int nCmp;
			nCmp = strcmp ("KPRG", szValue);
			if ((nHeader == '=' && nCmp == 0)
			||  (nHeader == '<' && nCmp < 0)
			||  (nHeader == '>' && nCmp > 0))
			{
				idx::append (raIndex, iRec, nErg);
			}
			nCmp = strcmp ("FREL", szValue);
			if ((nHeader == '=' && nCmp == 0)
			||  (nHeader == '<' && nCmp < 0)
			||  (nHeader == '>' && nCmp > 0))
			{
				idx::append (raIndex, iRec + nErg, nAbw);
			}
		}
		else if (nCol == 2)
		{
			if (nHeader == '=')
			{
				size_t iErg = iRec + nVal * dValue;
				size_t iAbw = iRec + nErg + nVal * dValue;
				if (dValue < 168)
				idx::append (raIndex, iErg, nVal);
				if (dValue < 24)
				idx::append (raIndex, iAbw, nVal);
			}
		}
		else if (nCol == 3)
		{
			size_t j;
			const size_t nRec = nVal + 24;
			const size_t iVal0 = iRec + dValue;
			if (nHeader == '=')
			for (j=0; j<nRec; j++)
			{
				size_t iVal = iVal0 + j * nVal;
				idx::append (raIndex, iVal);
			}
		}
		iRec += nRec;
	}
//	ms_aData[nCol] = new vector<size_t> (raIndex);
	return true;
}
//---------------------------------------------------------------------------
