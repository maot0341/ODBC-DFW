// test.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#include <assert.h>
#include <stdio.h>
#include <float.h>
#include "archiv.h"
#include "image.h"
#include <algorithm>
#include <stdx/svector.h>
#include <stdx/btree.h>
#include <math.h>
#include <set>
#include <vld.h>
//---------------------------------------------------------------------------

//#define ARCHIV0
using namespace stdx;
//---------------------------------------------------------------------------
bool match (const char * str, const char * pat, const char * szEsc)
{
	vector<string> aToken;
	string strToken;
	int nEsc = strlen(szEsc);
	const char * a = pat;
	const char * b = a;
	while (true)
	{
		b = strstr (b, szEsc);
		if (b == 0)
			break;
		if (strncmp (b+nEsc, szEsc, nEsc) == 0)
		{
			b += nEsc;
			strToken += string (a, b-a);
		}
		else
		{
			strToken += string (a, b-a);
			aToken.push_back (strToken);
			strToken.erase();
		}
		a = b + nEsc;
		b = a;
	}
	aToken.push_back (a);
	a = str;
	const int nToken = aToken.size();
	const int e = nToken-1;
	int i;
	for (i=0; i<nToken; i++)
	{
		const char * szToken = aToken[i].c_str();
		if (*szToken == 0)
			continue;
		if (i == e)
		{
			int n = strlen(szToken);
			b = a + strlen(a) - n;
			if (b < a)
				return false;
			return strcmp (b, szToken) == 0;
		}
		b = strstr (a, szToken);
		if (b == 0)
			return false;
		if (i == 0)
		if (a != b)
			return false;
		a = b + strlen (szToken);
	}
	return true;
}
//---------------------------------------------------------------------------
template<class T>
void insert (vector<T> & raVec, const T & aVal)
{
	size_t a = 0;
	size_t b = raVec.size();
	size_t i;
	while (a < b)
	{
		i = (a + b) >> 1;
		const T & aItem = raVec[i];
		if (aItem < aVal)
			a = i + 1;
		else
		if (aVal < aItem)
			b = i;
		else
			a = b = i;
	}
	vector<T>::iterator iVec = raVec.begin() + a;
	raVec.insert (iVec, aVal);
}
//---------------------------------------------------------------------------
unsigned long get_data (std::vector<unsigned long> & raVector)
{
	srand(time(0));
	const char * szFile = "C:\\temp\\data.txt";
//	const unsigned long n = 3000700;
	const unsigned long n = 3000000 + rand();
	unsigned long nValue;
	unsigned long i;
	raVector.reserve (n);
	raVector.clear();
#if 1
	printf ("generating random data...\n");
	FILE * pFile = fopen (szFile, "wt");
	for (i=0; i<n; i++)
	{
		nValue = rand() * rand();
		if (pFile)
			fprintf (pFile, "%lu\n", nValue);
		raVector.push_back (nValue);
	}
	if (pFile)
		fclose (pFile);
	pFile = 0;
#else
	printf ("reading data...\n");
	FILE * pFile = fopen (szFile, "rt");
	assert (pFile);
	while (fscanf (pFile, "%lu\n", &nValue)==1)
		raVector.push_back (nValue);
	fclose (pFile);
#endif
	return raVector.size();
}
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
#ifndef DIM
#	define DIM(a) (sizeof(a)/sizeof(a[0]))
#endif
typedef double test_t;
int main(int argc, char* argv[])
{
#if 1
	size_t nValue;
	size_t i,j;
	time_t ta, tb, td;

	if (false)
	{
		test_t data[] = {0,1,2,6,5,4,3};
		size_t n = DIM(data);
		btree<test_t,1> aBTree(false);
		for (i=0; i<n; i++)
			aBTree.insert (data[i]);
		ULONG nCount = aBTree.count();
		return 0;
	}

	std::vector<unsigned long> aValue;
	ta = time(0);
	size_t n = get_data (aValue);
	tb = time(0);
	td = tb - ta;
	printf ("timing: %d s\n", td);
	if (true)
	{
		printf ("\ntest B-Tree >>>>\n");
		ta = time(0);
		btree<test_t,128> aBTree(false);
		for (i=0; i<n; i++)
		{
			nValue = aValue[i];
			aBTree.insert (nValue);
		}
		tb = time(0);
		td = tb - ta;
		printf ("timing for %d inserts: %d s\n", n, td);
		ULONG nCount = aBTree.count();
		return 0;
	}
	if (false)
	{
		printf ("\ntest 1 >>>>\n");
		svec<test_t> aVec1(false,500);
		ta = time(0);
		for (i=0; i<n; i++)
		{
			nValue = aValue[i];
			if (false)
			if (i > 100)
//			if ((i % 2) == 0)
			{
				aVec1.check();
			}
			aVec1.insert (nValue);
		}
		tb = time(0);
		td = tb - ta;
		aVec1.check();
		if (false)
		{
			aVec1.reorg();
			aVec1.check();
		}
		double dPerc = (double)aVec1.count() / ((double)aVec1.pages() * (double)aVec1.cluster()) * 100.0;
		printf ("page=%u size=%u count=%lu blks=%u full=%.2f%%\n", aVec1.pages(), aVec1.size(), aVec1.count(), aVec1.cluster(), 
			dPerc);
		printf ("timing for %d inserts: %d s\n", n, td);
		const size_t n1 = aVec1.count();
		aVec1.index();
		ta = time(0);
		for (i=0; i<n; i++)
		{
			j = ((unsigned long)aValue[i]) % n1;
			nValue = aVec1[j];
		}
		tb = time(0);
		td = tb - ta;
		printf ("timing for %d reads: %d s\n", n, td);
		ULONG x,y;
		ta = time(0);
		const test_t * pValue = aVec1.find (27748, 0, &x, &y);
		//x = y = 0;
		pValue = aVec1.read (x,y);
		while ((pValue = aVec1.next (x,y)) != 0)
		{
			double dValue = *pValue;
		}
		tb = time(0);
		td = tb - ta;
		printf ("timing for %d fast reads: %d s\n", n, td);
		aVec1.end (x,y);
		while ((pValue = aVec1.prev(x,y)) != 0)
		{
			double dValue = *pValue;
		}
		aVec1.clear();
	}
	if (false)
	{
		printf ("\ntest 2 >>>>\n");
		std::vector<test_t> aVec2;
		aVec2.reserve (n);
		ta = time(0);
		for (i=0; i<n; i++)
		{
			nValue = aValue[i];
			//insert (aVec2, nValue);
			aVec2.push_back (nValue);
		}
		std::sort (aVec2.begin(), aVec2.end());
		tb = time(0);
		td = tb - ta;

		printf ("timing for %d inserts: %d s\n", n, td);
		const size_t n2 = aVec2.size();
		ta = time(0);
		for (i=0; i<n; i++)
		{
			ULONG j = aValue[i] % n2;
			nValue = aVec2[j];
		}
		tb = time(0);
		td = tb - ta;
		printf ("timing for %d reads: %d s\n", n, td);
		aVec2.clear();
		aVec2 = std::vector<test_t>();
	}
	if (false)
	{
		printf ("\ntest 3 >>>>\n");
		std::set<test_t> aVec;
//		aVec.reserve (n);
		ta = time(0);
		for (i=0; i<n; i++)
		{
			nValue = aValue[i];
			//insert (aVec2, nValue);
			aVec.insert (nValue);
		}
		tb = time(0);
		td = tb - ta;
		printf ("timing for %d inserts: %d s\n", n, td);

//		aVec.clear();
	}
	return 0;






#else
	bool bMist;
	bMist = match ("Erich Weinert Str", "*ch*Str", "*");
	bMist = match ("Erich Weinert Str", "*", "*");
	bMist = match ("Erich Weinert Str", "*Weinert", "*");
	bMist = match ("Erich Weinert Str", "*Weinert*", "*");
	bMist = match ("Erich Weinert Str", "Weinert*", "*");

	bMist = match ("Erich Weinert Str", "*Erich", "*");
	bMist = match ("Erich Weinert Str", "*Erich*", "*");
	bMist = match ("Erich Weinert Str", "Erich*", "*");

	bMist = match ("Erich Weinert Str", "*Str", "*");
	bMist = match ("Erich Weinert Str", "*Str*", "*");
	bMist = match ("Erich Weinert Str", "Str*", "*");
	
	
	jpeg_test();
	const char * szDBC1 = "ODBC;"
	"DSN=Microsoft Access-Datenbank;"
	"Driver={Microsoft Access Driver (*.mdb)};"
	"DBQ=G:\\Access\\Test.mdb;"
	"DefaultDir=G:\\Access;"
	;
	const char * szDBC = "ODBC;"
	"DSN=Microsoft Access-Datenbank;"
	"Driver={Microsoft Access Driver (*.mdb)};"
	"DBQ=D:\\jvater\\odbc\\Test.mdb;"
	"DefaultDir=D:\\jvater\\odbc;"
	;

	////-----------------------------------------
	CDatabase aDB;
	aDB.Open (szDBC);
	printf("Hallo Welt!\n");
	const char * szConnect = aDB.m_strConnect;
	printf ("DB:%s\n", szConnect);


	const time_t nTakt = CArchiv::HOUR;
	double dValue = 0.0;
	long nTid = 1;
	long i,j,k,n;

	time_t ta = Time (2009,3,1,12);
	time_t t0 = time(0);
	long nSlots = (t0 - ta) / nTakt + 1;
	time_t t;


	const long nBuffer = 100;
	double aValue[18000];
	char   szMark[18000] = " ";

#ifndef ARCHIV0
	CArchiv1 aArchiv (&aDB);
	aArchiv.purge();
	int nTid0 = 200;
#else
	CArchiv aArchiv;
	int nTid0 = 100;
#endif
	aArchiv.open (aDB);
	nTid = 1;
	const time_t ts = time(0);
	////-----------------------------------------
	//// Create Archiv-Entry for TID
	////-----------------------------------------
	if (false)
	{
		const nSlots = 24 * 366 * 3;
//		aArchiv.create (1, nSlots);
		aArchiv.create (2, nSlots);
	}
	////-----------------------------------------
	//// Create Archiv-Entry for TID
	////-----------------------------------------
	if (false)
	{
		for (k=0; k<100; k++)
			aArchiv.create (nTid, 100);
		printf ("TID %d created\n", nTid);
	}
	////-----------------------------------------
	//// Create Archiv for KNZ
	////-----------------------------------------
	if (false)
	{
		char szKNZ[256];
		long nTid;
		const long nTakt = CArchiv::HOUR;
		const long nSlots = 2 * CArchiv::YEAR / CArchiv::HOUR;
		for (k=0; k<100; k++)
		{
			nTid = nTid0 + k;
			sprintf (szKNZ, "VBR.VBR%03d.MNG.IST.H", nTid);
			try { aArchiv.create (nTid, szKNZ, nTakt, nSlots); }
			catch (...) {}
			printf ("TID %d[%s] created\n", nTid, szKNZ);
		}
	}
	////-----------------------------------------
	//// Set Single Value
	////-----------------------------------------
#ifdef ARCHIV0
	if (false)
	for (i=0; i<n; i++)
	{
		t = t0 + i * nTakt;
		dValue = rand() / 100.00;
		aArchiv.set (nTid, t, dValue, szMark);
		printf ("[%05d]\t", i);
	}
#endif
	////-----------------------------------------
	//// Set Value Vector
	////-----------------------------------------
	if (true)
	for (k=1; k<=4; k++)
	{
		nTid = nTid0 + k;
		t = t0;
		printf ("tid[%d]\t", nTid);
		for (i=0; i<nSlots; i++)
		{
			dValue = rand() / 100.00;
			//// Write-Trigger (j == 0)
			j = i % nBuffer;
			if (i && !j)
			{
				aArchiv.set (nTid, t, -nBuffer, aValue, szMark);
				memset (&aValue, 0, sizeof(aValue));
				memset (&szMark, 'N', sizeof(szMark));
				printf (".");
//				printf ("\n");
			}
			t = ta + i * nTakt;
			aValue[j] = dValue;
			szMark[j] = ' ';
//			printf ("[%05d]\t", i);
		}
		if (j)
		{
			aArchiv.set (nTid, t, -(j+1), aValue, szMark);
			printf ("\nlast record s=%d t=%s", j+1, ctime(&t));
		}
	}
	t = Time (2010, 6, 23);
	printf ("fetching: ", i);
	////-----------------------------------------
	//// Read Value Vector
	////-----------------------------------------
	if (false)
	for (i=0; i<10; i++)
	{
		memset (aValue, 0, sizeof(aValue));
		memset (szMark, 'N', sizeof(szMark));
		aArchiv.get (nTid0+1, t, -10000, aValue, szMark);
		memset (aValue, 0, sizeof(aValue));
		memset (szMark, 'N', sizeof(szMark));
		aArchiv.get (nTid0+2, t, -10000, aValue, szMark);
		putchar ('.');
	}
	aArchiv.close();
	time_t td = time(0) - ts;
	printf ("\ntiming [%d]: %d s\n", nSlots, td);
	return 0;
#endif
}

