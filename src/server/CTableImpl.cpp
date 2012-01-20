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


#include "CTableImpl.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <stdx/utils.h>

using namespace stdx;
//---------------------------------------------------------------------------
CTableImpl::CTableImpl(const char * szName)
{
	if (szName)
		m_strName = szName;
}
//---------------------------------------------------------------------------
// TableImpl -- Neue Spalte hinzufuegen
//---------------------------------------------------------------------------
void CTableImpl::column (short nType
, const char * szName
, unsigned nSize
, short nDigits
, short nNullable)
{
	CDesc aDesc (nType, szName, nSize, nDigits, nNullable);
	aDesc.m_SQL_DESC_BASE_COLUMN_NAME = STRNVL(szName);
	aDesc.m_SQL_DESC_BASE_TABLE_NAME = m_strName;
	aDesc.m_SQL_DESC_CATALOG_NAME = "";
	aDesc.m_SQL_DESC_LABEL = STRNVL(szName);
	aDesc.m_SQL_DESC_SCHEMA_NAME = "";
	aDesc.m_SQL_DESC_SEARCHABLE = SQL_SEARCHABLE;
	aDesc.m_SQL_DESC_TABLE_NAME = m_strName;
	aDesc.m_SQL_DESC_UNSIGNED = SQL_FALSE;
	aDesc.m_SQL_DESC_UPDATABLE = SQL_FALSE;
	m_aDesc.push_back (aDesc);
};
//---------------------------------------------------------------------------
ULONG
CTableImpl::cols() const
{
	return m_aDesc.size();
};
//---------------------------------------------------------------------------
CValue 
CTableImpl::attr (int iAttr) const
{
	const ULONG nCol = m_aDesc.size();
	if (iAttr == SQL_DESC_COUNT)
		return CValue (SQL_INTEGER, nCol);
	CValue aValue;
	return aValue;
};
//---------------------------------------------------------------------------
const CDesc *
CTableImpl::desc (ULONG i) const
{
	const ULONG nCol = m_aDesc.size();
	if (0 <= i && i < nCol)
		return &m_aDesc[i];
	return 0;
};
//---------------------------------------------------------------------------
void
CTableImpl::clear (CRecord & raRecord) const
{
	const ULONG nCols = cols();
	raRecord.resize (nCols);
	int i;
	for (i=0; i<nCols; i++)
	{
		const CDesc & aDesc = m_aDesc[i];
		short nType = aDesc.type();
		raRecord[i].clear (nType);
	}
}
//---------------------------------------------------------------------------
