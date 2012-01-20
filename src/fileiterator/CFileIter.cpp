#include <stdio.h>
#include <string.h>

#include "CFileIter.h"
//---------------------------------------------------------------------------
// ASCII
//---------------------------------------------------------------------------
CFileIteratorA::CFileIteratorA (const char * szPath)
{
	m_szPath[0] = 0;
	m_szName = 0;
	m_aStack.reserve(20);
	if (!szPath)
		return;
	strcpy (m_szPath, szPath);
	long nPath = strlen(szPath);
	long i = nPath - 1;
	if (i >= 0 && szPath[i] != '\\')
		strcat (m_szPath, "\\");
	m_szName = m_szPath + strlen(m_szPath);
	push(0);
}
//---------------------------------------------------------------------------
void CFileIteratorA::operator++()
{
	const size_t nStack = m_aStack.size();
	if (!nStack)
		return;
	//// current: subdir ==> change dir
	const char * szName = m_aFileInfo.name;
	if (m_aFileInfo.attrib & _A_SUBDIR)
	if (strcmp (szName, ".") && strcmp (szName, ".."))
	if (push (szName))
		return;
	//// current: file ==> next file/dir
	const item_t & aItem = m_aStack.back();
	if (_findnext (aItem.h, &m_aFileInfo) == 0)
		return;
	//// current: eof ==> pop dir
	while (pop())
	{
		const item_t & aItem = m_aStack.back();
		if (_findnext (aItem.h, &m_aFileInfo) == 0)
			return;
	}
}
//---------------------------------------------------------------------------
const _finddata_t * CFileIteratorA::current() const
{
	const size_t nStack = m_aStack.size();
	if (!nStack)
		return 0;
	return &m_aFileInfo;
}
//---------------------------------------------------------------------------
bool CFileIteratorA::push (const char * szDir)
{
	char * szName = m_szName;
	if (szDir)
	{
		strcpy (m_szName, szDir);
		strcat (m_szName, "\\");
		m_szName = m_szPath + strlen(m_szPath);
	}
	item_t aItem;
	strcpy (m_szName, "*");
	aItem.h = _findfirst (m_szPath, &m_aFileInfo);
	*m_szName = 0;
	aItem.i = strlen(m_szPath);
	if (aItem.h == -1)
	{
		m_szName = szName; 
		*m_szName = 0;
		return false;
	}
	m_aStack.push_back (aItem);
	return true;
}
//---------------------------------------------------------------------------
bool CFileIteratorA::pop()
{
	const size_t nStack = m_aStack.size();
	if (nStack > 0)
	{
		const item_t & aItem = m_aStack.back();
		_findclose (aItem.h);
		m_aStack.pop_back();
	}
	if (nStack < 2)
		return false;
	const item_t & aItem = m_aStack.back();
	const long i = aItem.i;
	m_szName = m_szPath + i;
	*m_szName = 0;
	return true;
}
//---------------------------------------------------------------------------
// UNICODE
//---------------------------------------------------------------------------
CFileIteratorW::CFileIteratorW (const wchar_t * szPath)
{
	m_szPath[0] = 0;
	m_szName = 0;
	m_aStack.reserve(20);
	if (!szPath)
		return;
	long nPath = wcslen(szPath);
	wcscpy  (m_szPath, szPath);
	long i = nPath - 1;
	if (i >= 0 && szPath[i] != '\\')
		wcscat (m_szPath, L"\\");
	m_szName = m_szPath + wcslen(m_szPath);
	push(0);
}
//---------------------------------------------------------------------------
void CFileIteratorW::operator++()
{
	const size_t nStack = m_aStack.size();
	if (!nStack)
		return;
	//// current: subdir ==> change dir
	const wchar_t * szName = m_aFileInfo.name;
	if (m_aFileInfo.attrib & _A_SUBDIR)
	if (wcscmp (szName, L".") && wcscmp (szName, L".."))
	if (push (szName))
		return;
	//// current: file ==> next file/dir
	const item_t & aItem = m_aStack.back();
	if (_wfindnext (aItem.h, &m_aFileInfo) == 0)
		return;
	//// current: eof ==> pop dir
	while (pop())
	{
		const item_t & aItem = m_aStack.back();
		if (_wfindnext (aItem.h, &m_aFileInfo) == 0)
			return;
	}
}
//---------------------------------------------------------------------------
const _wfinddata_t * CFileIteratorW::current() const
{
	const size_t nStack = m_aStack.size();
	if (!nStack)
		return 0;
	return &m_aFileInfo;
}
//---------------------------------------------------------------------------
bool CFileIteratorW::push (const wchar_t * szDir)
{
	wchar_t * szName = m_szName;
	if (szDir)
	{
		wcscpy (m_szName, szDir);
		wcscat (m_szName, L"\\");
		m_szName = m_szPath + wcslen(m_szPath);
	}
	item_t aItem;
	wcscpy (m_szName, L"*");
	aItem.h = _wfindfirst (m_szPath, &m_aFileInfo);
	*m_szName = 0;
	aItem.i = wcslen(m_szPath);
	if (aItem.h == -1)
	{
		m_szName = szName; 
		*m_szName = 0;
		return false;
	}
	m_aStack.push_back (aItem);
	return true;
}
//---------------------------------------------------------------------------
bool CFileIteratorW::pop()
{
	const size_t nStack = m_aStack.size();
	if (nStack > 0)
	{
		const item_t & aItem = m_aStack.back();
		_findclose (aItem.h);
		m_aStack.pop_back();
	}
	if (nStack < 2)
		return false;
	const item_t & aItem = m_aStack.back();
	const long i = aItem.i;
	m_szName = m_szPath + i;
	*m_szName = 0;
	return true;
}
//---------------------------------------------------------------------------
// Win32
//---------------------------------------------------------------------------
#ifdef _WIN32
CFileIteratorWin32::CFileIteratorWin32 (const TCHAR * szPath)
{
	m_szPath[0] = 0;
	m_szName = 0;
	m_aStack.reserve(20);
	if (!szPath)
		return;
	_tcscpy (m_szPath, szPath);
	long nPath = _tcslen(szPath);
	long i = nPath - 1;
	if (i >= 0 && szPath[i] != '\\')
		_tcscat (m_szPath, _T("\\"));
	m_szName = m_szPath + _tcslen(m_szPath);
	push(0);
}
//---------------------------------------------------------------------------
void CFileIteratorWin32::operator++()
{
	const size_t nStack = m_aStack.size();
	if (!nStack)
		return;
	//// current: subdir ==> change dir
	const TCHAR * szName = m_aFileInfo.cFileName;
	if (m_aFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	if (_tcscmp (szName, _T(".")) && _tcscmp (szName, _T("..")))
	if (push (szName))
		return;
	//// current: file ==> next file/dir
	const item_t & aItem = m_aStack.back();
	if (FindNextFile (aItem.h, &m_aFileInfo))
		return;
	//// current: eof ==> pop dir
	while (pop())
	{
		const item_t & aItem = m_aStack.back();
		if (FindNextFile (aItem.h, &m_aFileInfo))
			return;
	}
}
//---------------------------------------------------------------------------
const WIN32_FIND_DATA * CFileIteratorWin32::current() const
{
	const size_t nStack = m_aStack.size();
	if (!nStack)
		return 0;
	return &m_aFileInfo;
}
//---------------------------------------------------------------------------
bool CFileIteratorWin32::push (const TCHAR * szDir)
{
	TCHAR * szName = m_szName;
	if (szDir)
	{
		_tcscpy (m_szName, szDir);
		_tcscat (m_szName, _T("\\"));
		m_szName = m_szPath + _tcslen(m_szPath);
	}
	item_t aItem;
	_tcscpy (m_szName, _T("*"));
	//aItem.h = FindFirstFileEx (m_szPath, FindExInfoStandard, &m_aFileInfo,  FindExSearchLimitToDirectories, NULL, 0);
	aItem.h = FindFirstFile (m_szPath, &m_aFileInfo);
	*m_szName = 0;
	aItem.i = _tcslen(m_szPath);
	if (aItem.h == INVALID_HANDLE_VALUE)
	{
		m_szName = szName; 
		*m_szName = 0;
		return false;
	}
	m_aStack.push_back (aItem);
	return true;
}
//---------------------------------------------------------------------------
bool CFileIteratorWin32::pop()
{
	const size_t nStack = m_aStack.size();
	if (nStack > 0)
	{
		const item_t & aItem = m_aStack.back();
		FindClose (aItem.h);
		m_aStack.pop_back();
	}
	if (nStack < 2)
		return false;
	const item_t & aItem = m_aStack.back();
	const long i = aItem.i;
	m_szName = m_szPath + i;
	*m_szName = 0;
	return true;
}
#endif
//---------------------------------------------------------------------------
