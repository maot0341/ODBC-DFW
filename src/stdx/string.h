#pragma warning(disable : 4786)

#ifndef __STDXSTRING_H__
#define __STDXSTRING_H__

#include <time.h>
#include <vector>

namespace stdx
{
//---------------------------------------------------------------------------
class CString
{
public:
	CString (const char * str=0);
	CString (const CString &);
	
	virtual ~CString();

	const char & operator[] (int i) const                                     { return m_pchData[i]; }
	char & operator[] (int i)                                                 { return m_pchData[i]; }
	operator const char * () const                                            { return m_pchData; }
	CString & operator = (const CString &);
	CString & operator = (const char *);
	CString & operator += (const char *);
	CString & operator += (char);
	bool empty() const;
	size_t length() const;
	void resize (size_t len, bool preserve=true);
	void clear (size_t len)                                                   { resize (len, false); }
	size_t GetAllocLength() const                                             { return m_c; }
	bool IsEmpty() const                                                      { return empty(); }
	size_t GetLength() const                                                  { return length(); }
	int Count (const char*) const;
	void Format (const char * szFormat, ...);
	CString & Remove (char c);
	CString & Replace (char c1, char c2);
	CString & Replace (const char * s1, const char * s2);
	char * GetBufferSetLength (int len);
	char * GetBuffer (size_t);
	void ReleaseBuffer();
	void TrimRight();

protected:
	char * m_pchData;
	size_t m_c;
};
//---------------------------------------------------------------------------
CString operator+ (const char *, const CString&);
//---------------------------------------------------------------------------
class CStringList : public std::vector<CString>
{
public:
	void RemoveAll()                                                         { clear(); }
};
//---------------------------------------------------------------------------
class CTime
{
public:
	CTime (time_t utc=0);
	CString Format (const char *) const;

	time_t m_time;
};
//---------------------------------------------------------------------------
} // namespace stdx
#endif