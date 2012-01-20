

#ifndef __CFILEITER_H__
#define __CFILEITER_H__

#include <io.h>
#include <tchar.h>
#include <vector>

using namespace std;
#define MAXPATHLENGTH 900
//---------------------------------------------------------------------------
class CFileIteratorA
{
public:
	CFileIteratorA (const char * path=0);

	void operator++ ();
	const _finddata_t * current() const;
	const char * path() const                                                { return m_szPath; }
	const long depth() const                                                 { return m_aStack.size(); }

protected:
	bool push (const char * dir);
	bool pop();

	struct item_t
	{
		long h;
		long i;
	};

	char* m_szName;
	char m_szPath [MAXPATHLENGTH];
	struct _finddata_t m_aFileInfo;
	vector<item_t> m_aStack;
};
//---------------------------------------------------------------------------
class CFileIteratorW
{
public:
	CFileIteratorW (const wchar_t * path=0);

	void operator++ ();
	const _wfinddata_t * current() const;
	const wchar_t * path() const                                             { return m_szPath; }
	const long depth() const                                                 { return m_aStack.size(); }

protected:
	bool push (const wchar_t * dir);
	bool pop();

	struct item_t
	{
		long h;
		long i;
	};

	wchar_t* m_szName;
	wchar_t m_szPath [MAXPATHLENGTH];
	struct _wfinddata_t m_aFileInfo;
	vector<item_t> m_aStack;
};
//---------------------------------------------------------------------------
#ifdef _WIN32

#ifndef _WINDOWS_
#ifdef _UNICODE
#define UNICODE
#endif
#endif

#include <windows.h>


class CFileIteratorWin32
{
public:
	CFileIteratorWin32 (const TCHAR * path=0);

	void operator++ ();
	const WIN32_FIND_DATA * current() const;
	const TCHAR * path() const                                               { return m_szPath; }
	const long depth() const                                                 { return m_aStack.size(); }

protected:
	bool push (const TCHAR * dir);
	bool pop();

	struct item_t
	{
		HANDLE h;
		long i;
	};

	TCHAR* m_szName;
	TCHAR m_szPath [MAXPATHLENGTH];
	WIN32_FIND_DATA m_aFileInfo;
	vector<item_t> m_aStack;
};
#endif
//---------------------------------------------------------------------------
#ifdef _UNICODE
	typedef CFileIteratorW CFileIterator;
#else
	typedef CFileIteratorA CFileIterator;
#endif
//---------------------------------------------------------------------------
#endif
