#pragma warning(disable : 4786)

#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <vector>

using namespace std;
#define ASSUME(expr) if (!(expr)) throw CString().Format ("%s[%ld]: %s  assertion failed!", __FILE__, __LINE__,  #expr )
//---------------------------------------------------------------------------
void trace (const char * szFormat, ...);
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
	CString & Format (const char * szFormat, ...);
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
class CStringList : public vector<CString>
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
inline
time_t Time (int y, int m, int d, int hh=0, int mi=0, int ss=0, bool dst=false)
{
	struct tm aTime;
	memset (&aTime, 0, sizeof(aTime));
	aTime.tm_sec = ss;
	aTime.tm_min = mi;
	aTime.tm_hour = hh;
	aTime.tm_mday = d;
	aTime.tm_mon = m - 1;
	aTime.tm_year = y - 1900;
	aTime.tm_isdst = dst;
	time_t t = mktime (&aTime);
	return t;
}
//---------------------------------------------------------------------------
inline
struct tm *  CETime (time_t t)
{
	t += 3600;
	return gmtime (&t);
}
//---------------------------------------------------------------------------
#endif