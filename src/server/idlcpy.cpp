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
#include "idlcpy.h"

//---------------------------------------------------------------------------
void idlnull (idl::typVariant & crbValue, short nType)
{
	crbValue.isNull = true;
	const char * szNull = "";
	switch (nType)
	{
	case SQL_BIT:
		crbValue.aValue.bBoolean(0);
		break;
	case SQL_INTEGER:
	case SQL_SMALLINT:
	case SQL_BIGINT:
	case SQL_TINYINT:
	case SQL_GUID:
		crbValue.aValue.nInteger(0);
		break;
	case SQL_NUMERIC:
	case SQL_DECIMAL:
	case SQL_FLOAT:
	case SQL_REAL:
	case SQL_DOUBLE:
		crbValue.aValue.dNumber(0);
		break;
	case SQL_DATETIME:
	case SQL_TYPE_DATE:
	case SQL_TYPE_TIME:
	case SQL_TYPE_TIMESTAMP:
	case SQL_TIME:
	case SQL_TIMESTAMP:
		crbValue.aValue.dTime(0);
		break;
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_LONGVARCHAR:
		crbValue.aValue.aString(szNull);
		break;
	case SQL_BINARY:
	case SQL_VARBINARY:
	case SQL_LONGVARBINARY:
		crbValue.aValue._default();
		break;
	default:
		crbValue.aValue._default();
		break;
	}
//	crbValue.aValue.aString (CORBA::string_dup (""));
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, bool bValue)
{
	crbValue.isNull = false;
	crbValue.aValue.bBoolean (bValue);
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, long nValue)
{
	crbValue.isNull = false;
	crbValue.aValue.nInteger (nValue);
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, short nValue)
{
	crbValue.isNull = false;
	crbValue.aValue.nInteger (nValue);
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, double dValue)
{
	crbValue.isNull = false;
	crbValue.aValue.dNumber (dValue);
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, const string & strValue)
{
	idlcpy (crbValue, strValue.c_str());
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, const char * szValue)
{
	crbValue.isNull = false;
	crbValue.aValue.aString (CORBA::string_dup (szValue));
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, const CTerm & aValue)
{
	idlcpy (crbValue, &aValue);
}
//---------------------------------------------------------------------------
void idlcpy (idl::typVariant & crbValue, const CTerm * pValue)
{
	assert (pValue);
	const short nType = pValue->type();
	crbValue.isNull = pValue->isNull();
	if (CTerm::isBool (nType))
	{
		const bool bBoolean = pValue->asBool();
		crbValue.aValue.bBoolean (bBoolean);
		return;
	}
	if (CTerm::isInteger (nType))
	{
		const long nInteger = pValue->asInteger();
		crbValue.aValue.nInteger (nInteger);
		return;
	}
	if (CTerm::isFloat (nType))
	{
		const double dNumber = pValue->asDouble();
		crbValue.aValue.dNumber (dNumber);
		return;
	}
	if (CTerm::isDateTime (nType))
	{
		double dTime = pValue->asDouble();
		crbValue.aValue.dTime (dTime);
		return;
	}
	if (CTerm::isString (nType))
	{
//		const char * szValue = CORBA::string_dup (pValue->asString());
		const char * szValue = pValue->asString();
		crbValue.aValue.aString (szValue);
		return;
	}
	assert (false);
}
//---------------------------------------------------------------------------
#if 0
void idlcpy (const idl::typVariant & vValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (vValue.isNull)
	{
		if (pInd)
			*pInd = SQL_NULL_DATA;
		return;
	}
	switch (vValue.aValue._d())
	{
	case idl::TermSTRING:
		idlcpy (vValue.aValue.aString(), nType, pValue, nLen, pInd);
		break;
	case idl::TermTIME:
		idltime (vValue.aValue.dTime(), nType, pValue, nLen, pInd);
		break;
	case idl::TermNUMBER:
		idlcpy (vValue.aValue.dNumber(), nType, pValue, nLen, pInd);
		break;
	case idl::TermINTEGER:
		idlcpy (vValue.aValue.nInteger(), nType, pValue, nLen, pInd);
		break;
	case idl::TermBOOLEAN:
//		idlcpy (vValue.bBoolean(), nType, pValue, nLen, pInd);
		idlcpy (vValue.aValue.nInteger(), nType, pValue, nLen, pInd);
		break;
	default:
		break;
	}
};



//---------------------------------------------------------------------------
void idlcpy (long * pnValue, const idl::typVariant &vValue)
{
	assert (pnValue);
	switch (vValue.aValue._d())
	{
	case idl::TermBOOLEAN:
		*pnValue = (long) vValue.aValue.bBoolean();
		break;
	case idl::TermINTEGER:
		*pnValue = (long) vValue.aValue.nInteger();
		break;
	case idl::TermNUMBER:
		*pnValue = (long) vValue.aValue.dNumber();
		break;
	case idl::TermTIME:
		*pnValue = (long) vValue.aValue.dTime();
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
void idlcpy (SQLPOINTER pStrVal, SQLSMALLINT nStrLen, SQLSMALLINT * pStrLen, const idl::typVariant & vValue)
{
	assert (pStrVal);
	assert (vValue.aValue._d() == idl::TermSTRING);
	char * szStrVal = (char*) pStrVal;
	const char * szValue = vValue.aValue.aString();
	if (pStrLen)
		*pStrLen = strlen(szValue);
	strncpy (szStrVal, szValue, nStrLen);
}
//---------------------------------------------------------------------------
void idlcpy (const char * szValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_CHAR)
	{
		strncpy ((char*)pValue, szValue, nLen);
		if (pInd)
			*pInd = strlen (szValue);
		((char*)pValue)[nLen-1] = 0;
		return;
	}
}
//---------------------------------------------------------------------------
void idlcpy (double dValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		*((long*)pValue) = (long) dValue;
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		*((short*)pValue) = (short) dValue;
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		*((float*)pValue) = (float) dValue;
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		*((double*)pValue) = (double) dValue;
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		sprintf ((char*)pValue, "%*f", nLen-2, dValue);
		((char*)pValue)[nLen-1] = 0;
		return;
	}
//	assert (false);
}
//---------------------------------------------------------------------------
void idlcpy (long nValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		*((long*)pValue) = (long) nValue;
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		*((short*)pValue) = (short) nValue;
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		*((float*)pValue) = (float) nValue;
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		*((double*)pValue) = (double) nValue;
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		sprintf ((char*)pValue, "%*d", nLen-2, nValue);
		((char*)pValue)[nLen-1] = 0;
		return;
	}
//	assert (false);
}
//---------------------------------------------------------------------------
void idltime (double dValue, short nType, void* pValue, long nLen, long * pInd)
{
	if (nType == SQL_C_LONG)
	{
		*((long*)pValue) = (long) dValue;
		return;
	}
	if (nType == SQL_C_SHORT)
	{
		*((short*)pValue) = (short) dValue;
		return;
	}
	if (nType == SQL_C_FLOAT)
	{
		*((float*)pValue) = (float) dValue;
		return;
	}
	if (nType == SQL_C_DOUBLE)
	{
		*((double*)pValue) = (double) dValue;
		return;
	}
	if (nType == SQL_C_CHAR)
	{
		const time_t nTime = dValue;
		struct tm * pTime = localtime (&nTime);
		char * szValue = (char*) pValue;
		size_t n = strftime (szValue, nLen, "%Y-%m-%d %H:%M:%S", pTime);
		szValue[nLen-1] = 0;
		if (pInd)
			*pInd = n;
		return;
	}
//	assert (false);
}
#endif
//---------------------------------------------------------------------------
void idlcpy (CValue & raValue, const idl::typVariant & crbVariant)
{
	const idl::typValue & crbValue = crbVariant.aValue;
	const idl::typTerm crbType = crbValue._d();
	if (!crbVariant.isNull)
		idlcpy (raValue, crbValue);
	else
	switch (crbType)
	{
	case idl::TermSTRING:
		raValue.clear (SQL_VARCHAR);
		break;
	case idl::TermBOOLEAN:
		raValue.clear (SQL_BIT);
		break;
	case idl::TermINTEGER:
		raValue.clear (SQL_INTEGER);
		break;
	case idl::TermNUMBER:
		raValue.clear (SQL_DOUBLE);
		break;
	case idl::TermTIME:
		raValue.clear (SQL_DATETIME);
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
void idlcpy (CValue & raValue, const idl::typValue & crbValue)
{
	const idl::typTerm crbType = crbValue._d();
	switch (crbType)
	{
	case idl::TermSTRING:
		raValue.set (SQL_CHAR, crbValue.aString());
		break;
	case idl::TermBOOLEAN:
		raValue.set (SQL_BIT, crbValue.bBoolean());
		break;
	case idl::TermINTEGER:
		raValue.set (SQL_INTEGER, crbValue.nInteger());
		break;
	case idl::TermNUMBER:
		raValue.set (SQL_DOUBLE, crbValue.dNumber());
		break;
	case idl::TermTIME:
		raValue.set (SQL_DATETIME, crbValue.dTime());
		break;
	default:
		assert (false);
	}

}
//---------------------------------------------------------------------------
void idlcpy (CParam & raParam, const idl::typParam & crbParam)
{
	short nType = crbParam.m_nParameterType;
	const idl::typVariant & crbVariant = crbParam.m_aValue;
	const idl::typValue & crbValue = crbVariant.aValue;
	const idl::typTerm crbType = crbValue._d();
	if (crbVariant.isNull)
		raParam.clear (nType);
	else
	switch (crbType)
	{
	case idl::TermSTRING:
		raParam.set (nType, crbValue.aString());
		break;
	case idl::TermBOOLEAN:
		raParam.set (nType, crbValue.bBoolean());
		break;
	case idl::TermINTEGER:
		raParam.set (nType, crbValue.nInteger());
		break;
	case idl::TermNUMBER:
		raParam.set (nType, crbValue.dNumber());
		break;
	case idl::TermTIME:
		raParam.set (nType, crbValue.dTime());
		break;
	default:
		assert (false);
	}
}
//---------------------------------------------------------------------------
void idlcpy (idl::typDesc & raDst, const CDesc & raSrc)
{
	raDst.m_SQL_DESC_AUTO_UNIQUE_VALUE = raSrc.m_SQL_DESC_AUTO_UNIQUE_VALUE;
	raDst.m_SQL_DESC_BASE_COLUMN_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_BASE_COLUMN_NAME.c_str());
	raDst.m_SQL_DESC_BASE_TABLE_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_BASE_TABLE_NAME.c_str());
	raDst.m_SQL_DESC_CASE_SENSITIVE = raSrc.m_SQL_DESC_CASE_SENSITIVE;
	raDst.m_SQL_DESC_CATALOG_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_CATALOG_NAME.c_str());
	raDst.m_SQL_DESC_CONCISE_TYPE = raSrc.m_SQL_DESC_CONCISE_TYPE;
//	raDst.m_SQL_DESC_DATA_PTR = raSrc.m_SQL_DESC_DATA_PTR;
	raDst.m_SQL_DESC_DATETIME_INTERVAL_CODE = raSrc.m_SQL_DESC_DATETIME_INTERVAL_CODE;
	raDst.m_SQL_DESC_DATETIME_INTERVAL_PRECISION = raSrc.m_SQL_DESC_DATETIME_INTERVAL_PRECISION;
	raDst.m_SQL_DESC_DISPLAY_SIZE = raSrc.m_SQL_DESC_DISPLAY_SIZE;
	raDst.m_SQL_DESC_FIXED_PREC_SCALE = raSrc.m_SQL_DESC_FIXED_PREC_SCALE;
//	raDst.m_SQL_DESC_INDICATOR_PTR = raSrc.m_SQL_DESC_INDICATOR_PTR;
	raDst.m_SQL_DESC_LABEL = CORBA::string_dup (raSrc.m_SQL_DESC_LABEL.c_str());
	raDst.m_SQL_DESC_LENGTH = raSrc.m_SQL_DESC_LENGTH;
	raDst.m_SQL_DESC_LITERAL_PREFIX = CORBA::string_dup (raSrc.m_SQL_DESC_LITERAL_PREFIX.c_str());
	raDst.m_SQL_DESC_LITERAL_SUFFIX = CORBA::string_dup (raSrc.m_SQL_DESC_LITERAL_SUFFIX.c_str());
	raDst.m_SQL_DESC_LOCAL_TYPE_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_LOCAL_TYPE_NAME.c_str());
	raDst.m_SQL_DESC_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_NAME.c_str());
	raDst.m_SQL_DESC_NULLABLE = raSrc.m_SQL_DESC_NULLABLE;
	raDst.m_SQL_DESC_NUM_PREC_RADIX = raSrc.m_SQL_DESC_NUM_PREC_RADIX;
	raDst.m_SQL_DESC_OCTET_LENGTH = raSrc.m_SQL_DESC_OCTET_LENGTH;
//	raDst.m_SQL_DESC_OCTET_LENGTH_PTR = raSrc.m_SQL_DESC_OCTET_LENGTH_PTR;
	raDst.m_SQL_DESC_PARAMETER_TYPE = raSrc.m_SQL_DESC_PARAMETER_TYPE;
	raDst.m_SQL_DESC_PRECISION = raSrc.m_SQL_DESC_PRECISION;
	raDst.m_SQL_DESC_ROWVER = raSrc.m_SQL_DESC_ROWVER;
	raDst.m_SQL_DESC_SCALE = raSrc.m_SQL_DESC_SCALE;
	raDst.m_SQL_DESC_SCHEMA_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_SCHEMA_NAME.c_str());
	raDst.m_SQL_DESC_SEARCHABLE = raSrc.m_SQL_DESC_SEARCHABLE;
	raDst.m_SQL_DESC_TABLE_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_TABLE_NAME.c_str());
	raDst.m_SQL_DESC_TYPE = raSrc.m_SQL_DESC_TYPE;
	raDst.m_SQL_DESC_TYPE_NAME = CORBA::string_dup (raSrc.m_SQL_DESC_TYPE_NAME.c_str());
	raDst.m_SQL_DESC_UNSIGNED = raSrc.m_SQL_DESC_UNSIGNED;
	raDst.m_SQL_DESC_UPDATABLE = raSrc.m_SQL_DESC_UPDATABLE;

	raDst.size = raSrc.size();
	raDst.digits = raSrc.digits();
}
//---------------------------------------------------------------------------
#if 0
idl::typTerm IDL (sqlp::term_t eTerm)
{
	return (idl::typTerm) eTerm;
}
#endif
//---------------------------------------------------------------------------
idl::typException  IDL(const CException & aExc)
{
	idl::typException crbExc;
	crbExc.aDiag.length(1);
	idl::typDiagItem & raDiag = crbExc.aDiag[0];
	raDiag.nCode = aExc.nId;
	strncpy (raDiag.szState, aExc.szState , 6);
	raDiag.strText = (const char*)aExc.strText.c_str();
	raDiag.strFile = (const char*)aExc.szFile;
	raDiag.nLine = aExc.nLine;
	return crbExc;

}
//---------------------------------------------------------------------------
