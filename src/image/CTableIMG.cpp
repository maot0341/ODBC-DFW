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

#include <time.h>
#include <stdlib.h>
#include <io.h>
#include "image.h"
#include "driver.h"

#include "CTableIMG.h"

using namespace std;
//using namespace idx;
using namespace sqlp;


#define DIM(arr) (sizeof(arr) / sizeof(arr[0]))
#define SYM(id) #id
//---------------------------------------------------------------------------
// Version 0
//---------------------------------------------------------------------------
const column_t
CTableIMG0::ms_aDesc[] =
{	{"filename", TermVCHAR}
,	{"path", TermVCHAR}
,	{"headline", TermVCHAR}
,	{"caption", TermVCHAR}
,	{"keywords", TermVCHAR}
,	{"copyright", TermVCHAR}
,	{"date created", TermTIME}
};
//---------------------------------------------------------------------------
CTableIMG0::CTableIMG0 (const char * szName)
: CTableBase (ms_aDesc, DIM(ms_aDesc))
{
	if (szName)
		m_strName = szName;
	m_nRows = 0;
	_finddata_t aFileInfo;
	long nFind = _findfirst(szName, &aFileInfo);
	if  (nFind < 0)
		return;
	if ((aFileInfo.attrib & _A_SUBDIR) == 0)
		m_aFile.push_back (aFileInfo);

	while (true)
	{
		int nNext = _findnext (nFind, &aFileInfo);
		if (nNext < 0)
			break;
		if ((aFileInfo.attrib & _A_SUBDIR) == _A_SUBDIR)
			continue;
		m_aFile.push_back (aFileInfo);
	}
	_findclose (nFind);
	m_nRows = m_aFile.size();
}
//---------------------------------------------------------------------------
bool
CTableIMG0::fetch (size_t iRec, CRecord & raRecord) const
{
	if (iRec >= m_nRows)
		return false;
	const size_t nCols = cols();
	const char * szTable = m_strName.c_str();
	char szPath[8000];
	*szPath = 0;
	const char * p = strrchr (szTable, '\\');
	if (p == 0)
		p = strrchr (szTable, '/');
	if (p)
	{
		string strPath (szTable, p - szTable);
		_fullpath (szPath, strPath.c_str(), sizeof(szPath));
	}
	int nPath = strlen(szPath);
	strcat (szPath, "\\");
	char * szFile = szPath + nPath + 1;
	const _finddata_t & aFile = m_aFile[iRec];
	try
	{
		strcpy (szFile, aFile.name);
		CJPEG aJPG (szPath);
		CIPTCCharArray & aIPTC = aJPG.m_aIPTC;
		raRecord[0] = aFile.name;
		raRecord[1] = szPath;
		raRecord[2] = aIPTC.headline;
		raRecord[3] = aIPTC.caption;
		raRecord[4] = (aIPTC.keywords.size() > 0) ? aIPTC.keywords[0].c_str() : "";
		raRecord[5] = aIPTC.copyright;
		raRecord[6] = aFile.time_create;
	}
	catch(...)
	{
		return false;
	}
	return true;

}
//---------------------------------------------------------------------------
// Version 1
//---------------------------------------------------------------------------
const column_t
CTableIMG1::ms_aDesc[] =
{	{"filename", TermVCHAR}
,	{"path", TermVCHAR}
,	{"headline", TermVCHAR}
,	{"caption", TermVCHAR}
,	{"keywords", TermVCHAR}
,	{"copyright", TermVCHAR}
,	{"time created", TermTIME}
,	{"file size", TermINT}
};
//---------------------------------------------------------------------------
CTableIMG1::CTableIMG1 (const char * szName)
: CTableBase (ms_aDesc, DIM(ms_aDesc))
{
	if (szName)
		m_strName = szName;
	m_nRows = 0;
	m_pKeys = 0;
	const char * p = strrchr (szName, '\\');
	if (p == 0)
		return;
	p++;
	string strDir (szName, p - szName);
	string strExp (p);
	p = strchr (p, '*');
	if (p == 0)
		recurse (szName, 0);
	else
		recurse (strDir.c_str(), strExp.c_str());
	m_nRows = m_aFile.size();
}
//---------------------------------------------------------------------------
CTableIMG1::~CTableIMG1()
{
	delete [] m_pKeys;
}
//---------------------------------------------------------------------------
void
CTableIMG1::recurse (const char * szDir, const char * szFilter)
{
	char szPath[8000];
	_finddata_t aFile;
	long nFind = -1;
	long nNext;

	_fullpath (szPath, szDir, sizeof(szPath));
	strcat (szPath, "\\*");
	char * szFile = szPath + strlen(szPath) - 1;
	while (true)
	{
		if (nFind < 0)
		{
			nFind = _findfirst (szPath, &aFile);
			if  (nFind < 0)
				return;
		}
		else
		{
			nNext = _findnext (nFind, &aFile);
			if (nNext < 0)
				break;
		}
		if (strcmp (aFile.name, ".") == 0)
			continue;
		if (strcmp (aFile.name, "..") == 0)
			continue;
		if ((aFile.attrib & _A_SUBDIR) == 0)
			continue;
		strcpy (szFile, aFile.name);
		recurse (szPath, szFilter);
	}
	_findclose (nFind);
	filter (szDir, szFilter);
}
//---------------------------------------------------------------------------
// scan one directory with filter [optional]
//---------------------------------------------------------------------------
void
CTableIMG1::filter (const char * szDir, const char * szFilter)
{
	char szPath[8000];
	_finddata_t aFile;
	long nFind = -1;
	long nNext;

	if (!szFilter || *szFilter==0)
		szFilter = "*";
	_fullpath (szPath, szDir, sizeof(szPath));
	int i = strlen(szPath) - 1;
	if (szPath[i] != '\\')
		strcat (szPath, "\\");
	char * szFile = szPath + strlen(szPath);
	if (szFilter)
		strcat (szPath, szFilter);
	while (true)
	{
		if (nFind < 0)
		{
			nFind = _findfirst (szPath, &aFile);
			if  (nFind < 0)
				return;
		}
		else
		{
			nNext = _findnext (nFind, &aFile);
			if (nNext < 0)
				break;
		}
		if ((aFile.attrib & _A_SUBDIR) == _A_SUBDIR)
			continue;
		strcpy (szFile, aFile.name);
		m_aFile[szPath] = aFile;
	}
	_findclose (nFind);
}
//---------------------------------------------------------------------------
int keycmp(const void * p1, const void * p2)
{
	const char * c1 = *(const char **)p1;
	const char * c2 = *(const char **)p2;
	return strcmp (c1, c2);
}
//---------------------------------------------------------------------------
bool
CTableIMG1::fetch (size_t iRec, CRecord & raRecord) const
{
	if (iRec >= m_nRows)
		return false;

	const size_t nCols = cols();
	const char * szTable = m_strName.c_str();

	if (iRec == 0)
	{
		delete [] m_pKeys;
		m_pKeys = new const char * [m_nRows];
		assert (m_pKeys);
		long i;
		CFileMap::const_iterator iFile = m_aFile.begin();
		for (i=0; iFile != m_aFile.end(); ++iFile, i++)
		{
			const char * szKey = (*iFile).first.c_str();
			m_pKeys[i] = szKey;
		}
		qsort (m_pKeys, m_nRows, sizeof(char*), keycmp);
	}
	const char * szPath = m_pKeys[iRec];
	CFileMap::const_iterator iFile = m_aFile.find(szPath);
	assert (iFile != m_aFile.end());

	const _finddata_t & aFile = (*iFile).second;
	try
	{
		CJPEG aJPG (szPath);
		CIPTCCharArray & aIPTC = aJPG.m_aIPTC;
		raRecord[0] = aFile.name;
		raRecord[1] = szPath;
		raRecord[2] = aIPTC.headline;
		raRecord[3] = aIPTC.caption;
		raRecord[4] = aIPTC.keywords.size() ? aIPTC.keywords.front().c_str() : "";
		raRecord[5] = aIPTC.copyright;
		raRecord[6] = aFile.time_create;
		raRecord[7] = aFile.size;
	}
	catch(...)
	{
		return false;
	}
	return true;

}
//---------------------------------------------------------------------------
// Version 2
//---------------------------------------------------------------------------
const column_t
CTableIMG2::ms_aDesc[] =
{	{"filename", TermVCHAR}
,	{"path", TermVCHAR}
,	{"date created", TermTIME}
};
//---------------------------------------------------------------------------
CTableIMG2::CTableIMG2 (const char * szName)
: CTableBase (ms_aDesc, DIM(ms_aDesc))
{
	if (szName)
		m_strName = szName;
	m_nRows = 0;
	m_pKeys = 0;
	const char * p = strrchr (szName, '\\');
	if (p == 0)
		return;
	p++;
	string strDir (szName, p - szName);
	string strExp (p);
	p = strchr (p, '*');
	if (p == 0)
		recurse (szName, 0);
	else
		recurse (strDir.c_str(), strExp.c_str());
	m_nRows = m_aFile.size();
}
//---------------------------------------------------------------------------
CTableIMG2::~CTableIMG2()
{
	delete [] m_pKeys;
}
//---------------------------------------------------------------------------
void
CTableIMG2::recurse (const char * szDir, const char * szFilter)
{
	char szPath[8000];
	_finddata_t aFile;
	long nFind = -1;
	long nNext;

	_fullpath (szPath, szDir, sizeof(szPath));
	strcat (szPath, "\\*");
	char * szFile = szPath + strlen(szPath) - 1;
	while (true)
	{
		if (nFind < 0)
		{
			nFind = _findfirst (szPath, &aFile);
			if  (nFind < 0)
				return;
		}
		else
		{
			nNext = _findnext (nFind, &aFile);
			if (nNext < 0)
				break;
		}
		if (strcmp (aFile.name, ".") == 0)
			continue;
		if (strcmp (aFile.name, "..") == 0)
			continue;
		if ((aFile.attrib & _A_SUBDIR) == 0)
			continue;
		strcpy (szFile, aFile.name);
		recurse (szPath, szFilter);
	}
	_findclose (nFind);
	filter (szDir, szFilter);
}
//---------------------------------------------------------------------------
// scan one directory with filter [optional]
//---------------------------------------------------------------------------
void
CTableIMG2::filter (const char * szDir, const char * szFilter)
{
	char szPath[8000];
	_finddata_t aFile;
	long nFind = -1;
	long nNext;

	_fullpath (szPath, szDir, sizeof(szPath));
	strcat (szPath, "\\");
	char * szFile = szPath + strlen(szPath);
	if (szFilter)
		strcat (szPath, szFilter);
	while (true)
	{
		if (nFind < 0)
		{
			nFind = _findfirst (szPath, &aFile);
			if  (nFind < 0)
				return;
		}
		else
		{
			nNext = _findnext (nFind, &aFile);
			if (nNext < 0)
				break;
		}
		if ((aFile.attrib & _A_SUBDIR) == _A_SUBDIR)
			continue;
		strcpy (szFile, aFile.name);
		m_aFile[szPath] = aFile;
	}
	_findclose (nFind);
}
//---------------------------------------------------------------------------
bool
CTableIMG2::fetch (size_t iRec, CRecord & raRecord) const
{
	if (iRec >= m_nRows)
		return false;

	const size_t nCols = cols();
	const char * szTable = m_strName.c_str();

	if (iRec == 0)
	{
		delete [] m_pKeys;
		m_pKeys = new const char * [m_nRows];
		assert (m_pKeys);
		long i;
		CFileMap::const_iterator iFile = m_aFile.begin();
		for (i=0; iFile != m_aFile.end(); ++iFile, i++)
		{
			const char * szKey = (*iFile).first.c_str();
			m_pKeys[i] = szKey;
		}
		qsort (m_pKeys, m_nRows, sizeof(char*), keycmp);
	}
	const char * szPath = m_pKeys[iRec];
	CFileMap::const_iterator iFile = m_aFile.find(szPath);
	assert (iFile != m_aFile.end());

	const _finddata_t & aFile = (*iFile).second;
	try
	{
		CJPEG aJPG (szPath);
		CIPTCCharArray & aIPTC = aJPG.m_aIPTC;
		raRecord[0] = aFile.name;
		raRecord[1] = szPath;
		raRecord[2] = aFile.time_create;
	}
	catch(...)
	{
		return false;
	}
	return true;

}
//---------------------------------------------------------------------------
