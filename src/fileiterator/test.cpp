// test.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
//#include <windows.h>
//#include <afx.h>
#include <assert.h>
#include <stdio.h>
#include "CFileIter.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if 1
#include <sstream>
#include <locale>
string narrow( const wstring& str )
{
	ostringstream stm;
	const ctype<char>& ctfacet = use_facet< ctype<char> >( stm.getloc(), 0, true ) ;
	for( size_t i=0 ; i<str.size() ; ++i )
		stm << ctfacet.narrow( str[i], 0 ) ;
	return stm.str() ;
}
#endif
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
//	CFileIterator aFileIter (".");
	time_t t0, t1;
	size_t nCount;
	const char * szDir = argv[argc-1];
	const char * szLoc = setlocale (LC_ALL, NULL);
//	szLoc = setlocale (LC_ALL, "Russian");
//	szLoc = setlocale (LC_ALL, "german");
//	szLoc = setlocale (LC_ALL, "");

//	szDir = "V:\\GACPDD (Gazprom)\\5_SysErstell\\2_Design\\Systemspezifikation (Phase 2)\\sorted by SUBSYSTEMS\\fSubSystem ODU\\OLD";
//	szDir = "V:\\GACPDD (Gazprom)\\5_SysErstell\\2_Design\\Systemspezifikation (Phase 2)\\sorted by SUBSYSTEMS";
#ifdef _UNICODE
	wchar_t wcDir[MAXPATHLENGTH];
	memset (wcDir, 0, sizeof(wcDir));
	mbstowcs (wcDir, szDir, strlen(szDir));
	//wprintf( L"%s\n", L"?????" );
//	return 0;
	CFileIterator aFileIter (wcDir);
#else
	CFileIterator aFileIter (szDir);
#endif

	t0 = time(0);
	long nDepth = 0;
	for (nCount=0; aFileIter.current(); ++aFileIter)
	{
		nCount++;
		const _tfinddata_t & aItem = *aFileIter.current();
		const _TCHAR * szName = aItem.name;
		const _TCHAR * szPath = aFileIter.path();
//		if (!strcmp(szPath, "c:\\System Volume Information\\temp\\Temporary Internet Files\\TMP\\WINDOWS"))
//		if (aFileIter.depth() == 1)
//			printf ("mist!\n");

//		_tprintf (_T("%s%s\n"), _T("Test"), _T("mist"));
//		if (_tprintf (_T("%s%s\n"), szPath, szName) < 0)
		if (false)
		if (aFileIter.depth() > nDepth)
			_tprintf (_T("%s\n"), szPath);
		nDepth = aFileIter.depth();
		if (false)
		if (_tprintf (_T("\t%s\n"), szName) < 0)
		{
			puts ("?!");
			fprintf (stderr, "io-error %d: %s\n", errno, strerror (errno));
		}
//		_tcsprintf (_T("%s%s\n"), szPath, szName);
//		TRACE2	("File: %s%s\n",  szPath, szName);
//		wprintf( L" File %d = %s \n", (int) nCount, aItem.name);
//		string strName = narrow (szName);
//		fwprintf(pFile, _T("%s%s\n"), szPath, szName);
//		printf ("%S%s\n", szPath, strName.c_str());
	}
	t1 = time(0);
	fprintf (stderr, "timimng [%d]:  %ds\n", (int)nCount, (int)(t1-t0));

#ifdef _UNICODE
	CFileIteratorWin32 aFileIterWin32 (wcDir);
#else
	CFileIteratorWin32 aFileIterWin32 (szDir);
#endif
	t0 = time(0);
	nDepth = 0;
	for (nCount=0; aFileIterWin32.current(); ++aFileIterWin32)
	{
		nCount++;
		const WIN32_FIND_DATA & aItem = *aFileIterWin32.current();
		const TCHAR * szName = aItem.cFileName;
		const TCHAR * szPath = aFileIterWin32.path();
//		if (!strcmp(szPath, "c:\\System Volume Information\\temp\\Temporary Internet Files\\TMP\\WINDOWS"))
//		if (aFileIter.depth() == 1)
//			printf ("mist!\n");

//		_tprintf (_T("%s%s\n"), _T("Test"), _T("mist"));
//		if (_tprintf (_T("%s%s\n"), szPath, szName) < 0)
//		if (false)
		if (aFileIterWin32.depth() > nDepth)
			_tprintf (_T("%s\n"), szPath);
		nDepth = aFileIterWin32.depth();
		if (false)
		if (_tprintf (_T("\t%s\n"), szName) < 0)
		{
			puts ("?!");
			fprintf (stderr, "io-error %d: %s\n", errno, strerror (errno));
		}
//		_tcsprintf (_T("%s%s\n"), szPath, szName);
//		TRACE2	("File: %s%s\n",  szPath, szName);
//		wprintf( L" File %d = %s \n", (int) nCount, aItem.name);
//		string strName = narrow (szName);
//		fwprintf(pFile, _T("%s%s\n"), szPath, szName);
//		printf ("%S%s\n", szPath, strName.c_str());
	}
	t1 = time(0);
	fprintf (stderr, "timimng [%d]:  %ds\n", (int)nCount, (int)(t1-t0));
	
	return 0;	
}
//---------------------------------------------------------------------------
