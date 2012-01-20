

#include <windows.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include "utils.h"

//-----------------------------------------------------------------------------
void trace (const char * szFormat, ...)
{
	char szBuff[8096];
	va_list aList;
	va_start (aList, szFormat);
	vsprintf (szBuff, szFormat, aList);
	va_end (aList);
	//strcat (szBuff, "\n");
	//fprintf (stderr, szBuff);
	OutputDebugString(szBuff);
}
//---------------------------------------------------------------------------
CString::CString (const char * szText)
{
	m_pchData = 0;
	m_c = 0;
	(*this) = szText;
}
//---------------------------------------------------------------------------
CString::CString (const CString & strText)
{
	if (&strText == this)
		return;
	m_pchData = 0;
	m_c = 0;
	(*this) = strText;
}
//---------------------------------------------------------------------------
CString::~CString()
{
	delete [] m_pchData;
	m_pchData = 0;
	m_c = 0;
}
//---------------------------------------------------------------------------
void CString::resize (size_t n, bool bPreserve)
{
	if (++n <= m_c)
		return;
	n = ((n >> 8) + 1) << 8;
	char * p = new char[n];
	assert (p);
	if (m_c && bPreserve)
		strcpy (p ,m_pchData);
	else
		p[0] = 0;
	delete [] m_pchData;
	m_pchData = p;
	m_c = n;
}
//---------------------------------------------------------------------------
CString & CString::operator= (const CString & strText)
{
	const char * szText = strText;
	return (*this) = szText;
}
//---------------------------------------------------------------------------
CString & CString::operator= (const char * szText)
{
	if (!szText)
		szText = "";
	size_t nLen = strlen (szText);
	clear (nLen);
	strcpy (m_pchData, szText);
	return *this;
}
//---------------------------------------------------------------------------
CString operator+ (const char * szText, const CString & strText)
{
	CString strResult (szText);
	strResult += strText;
	return strResult;
}
//---------------------------------------------------------------------------
CString & CString::operator+= (char c)
{
	const size_t n = length();
	resize (n+1);
	m_pchData[n] = c;
	m_pchData[n+1] = 0;
	return *this;
}
//---------------------------------------------------------------------------
CString & CString::operator+= (const char * szText)
{
	if (!szText)
		return *this;
	const size_t n = length() + strlen(szText);
	resize (n);
	strcat (m_pchData, szText);
	return *this;
}
//---------------------------------------------------------------------------
bool CString::empty() const
{
	if (!m_pchData)
		return true;
	return *m_pchData == 0;
}
//---------------------------------------------------------------------------
size_t CString::length() const
{
	return m_pchData ? strlen(m_pchData) : 0;
}
//---------------------------------------------------------------------------
int CString::Count (const char * s) const
{
	if (!s)
		return 0;
	const int n = strlen (s);
	int nCount = 0;
	const char* p = strstr (m_pchData, s);
	while (p)
	{
		nCount++;
		p = strstr (p+n, s);
	}
	return nCount;
}
//---------------------------------------------------------------------------
CString & CString::Replace (const char * s1, const char * s2)
{
	int nCount = Count (s1);
	if (!nCount)
		return *this;
	int n1 = strlen(s1);
	int n2 = strlen(s2);
	int nLen = length() + (n2 - n1) * nCount;
	resize (nLen);
	char * p = strstr (m_pchData, s1);
	while (p)
	{
		const int n = strlen (p + n1) + 1;
		memmove (p+n2, p+n1, n);
		memcpy (p, s2, n2);
		p = strstr (p+n2, s1);
	}
	return *this;
}
//---------------------------------------------------------------------------
CString & CString::Replace (char c1, char c2)
{
	char * p = m_pchData;
	while (*p)
	{
		if (*p == c1)
			*p = c2;
		p++;
	}
	return *this;
}
//---------------------------------------------------------------------------
CString & CString::Remove (char c)
{
	const char * pSrc = m_pchData;
	const char * pEnd = m_pchData + strlen (m_pchData);
	char * pDst = m_pchData;
	for (; pSrc < pEnd; pSrc++)
	{
		if (*pSrc == c)
			continue;
		*pDst = *pSrc;
		pDst++;
	}
	*pDst = 0;
	return *this;
}
//---------------------------------------------------------------------------
char * CString::GetBuffer (size_t nLen)
{
	clear (nLen);
	return m_pchData;
}
//---------------------------------------------------------------------------
char * CString::GetBufferSetLength (int nLen)
{
	resize (nLen);
	return m_pchData;
}
//---------------------------------------------------------------------------
void CString::ReleaseBuffer()
{
}
//---------------------------------------------------------------------------
CString &
CString::Format (const char * szFormat, ...)
{
	va_list aArgs;
	va_start (aArgs, szFormat);

	assert (m_c > 0);
	size_t nSize = m_c - 1;
	long nWrite;
	while (true) 
	{
		nWrite = _vsnprintf (m_pchData, nSize, szFormat, aArgs);
		if (nWrite >= 0)
			break;
		nSize = nSize<<1;
		clear (nSize);
	}
	m_pchData[nWrite] = 0;
	return *this;
}
//---------------------------------------------------------------------------
void CString::TrimRight()
{
	const int n = length();
	if (n < 1)
		return;
	char * p = m_pchData + n;
	while (m_pchData < p--)
	{
		if (!isspace(*p))
			break;
		*p = 0;
	}
}
//---------------------------------------------------------------------------
// CTime
//---------------------------------------------------------------------------
CTime::CTime (time_t nUTC)
{
	m_time = nUTC;
}
//---------------------------------------------------------------------------
CString CTime::Format (const char * szFormat) const
{
	if (m_time < 0)
		return CString();
	const size_t nSize = 255;
	const struct tm * pTime = localtime (&m_time);
	CString strTime;
	char * szTime = strTime.GetBufferSetLength (nSize);
	strftime (szTime, nSize, szFormat, pTime);
	return strTime;
}
//---------------------------------------------------------------------------
