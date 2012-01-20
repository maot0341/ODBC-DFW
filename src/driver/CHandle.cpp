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

#include "CHandle.h"
#include <stdx/debug.h>
#include <assert.h>

using namespace stdx;
//---------------------------------------------------------------------------
// CHandle
//---------------------------------------------------------------------------
CHandle::CHandle (CHandle * paParent)
{
	if (paParent)
		paParent->m_aChildList.push_back (this);
	m_paParent = paParent;
	m_iError = 0;
}
//---------------------------------------------------------------------------
CHandle::~CHandle()
{
	if (m_paParent)
		m_paParent->m_aChildList.remove (this);
	clear();
}
//---------------------------------------------------------------------------
void CHandle::clear()
{
	list<CHandle*>::iterator iChild = m_aChildList.begin();
	for (; iChild != m_aChildList.end(); iChild++)
	{
		CHandle * pChild = *iChild;
		trace ("Handle(0x%x): destroy child 0x%x\n", this, pChild);
		pChild->m_paParent = 0;
		delete pChild;
	}
	m_aChildList.clear();
}
//---------------------------------------------------------------------------
// DRIVER
//---------------------------------------------------------------------------
auto_ptr<CDriver> CDriver::ms_aInstance;
//---------------------------------------------------------------------------
CDriver *
CDriver::Instance()
{
	CDriver * pDriver = ms_aInstance.get();
	if (pDriver)
		return pDriver;
	pDriver = new CDriver();
	assert (pDriver);
	ms_aInstance = auto_ptr<CDriver>(pDriver);
	return pDriver;
}
//---------------------------------------------------------------------------
CDriver::CDriver()
: CHandle(0)
{
	m_argv = 0;
	m_argc = 0;
	char ** argv = m_argv;
	int & argc = m_argc;
	argc = 0;

}
//---------------------------------------------------------------------------
CDriver::~CDriver()
{
}
//---------------------------------------------------------------------------
const char * CDriver::driver_odbc_ver()
{
	return "02.00";
	return "03.00";
	return SQL_SPEC_STRING;
}
//---------------------------------------------------------------------------
// ENVIRONMENT
//---------------------------------------------------------------------------
CEnvironment::CEnvironment (CDriver * paParent)
: CHandle (paParent)
{
}
//---------------------------------------------------------------------------
CEnvironment::~CEnvironment()
{
}
//---------------------------------------------------------------------------
