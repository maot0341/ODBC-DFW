/***************************************************************************************

    Projekt:    Hasy                                                     (C) 2000 PSI AG
    Komponente: tppro
    Modul:      CStorage.h

    Online Datei

    Systeme: Alpha, Linux, NT 4

    $Author: hasyjv $
    $Date: 2006/04/28 15:04:26 $
    $Revision: 1.18 $
    $State: Exp $

    Änderung     Datum     Autor  Bemerkung
    ------------------------------------------------------------------------------------
                 24.06.03  jv     Ausgelagert aus CDataBase
***************************************************************************************/
#pragma warning (disable:4786)

static const char* id_tp_pro_CStorage_cpp = "@(#) $Header: /gow/hasy/cvs/src/tp/pro/CStorage.cpp,v 1.18 2006/04/28 15:04:26 hasyjv Exp $";

#include "CStorage.h"
#include <stdarg.h>

//---------------------------------------------------------------------------
namespace tppro
{
//---------------------------------------------------------------------------
QString format (const char * szFormat, ...)
{
	char szBuff[8000];
	va_list aArgs;
	va_start (aArgs, szFormat);
	vsprintf (szBuff, szFormat, aArgs);
	va_end(aArgs);
	QString str = szBuff;
	return str;
//	return std::string (szBuff);
}
//---------------------------------------------------------------------------
static const char * gs_szStorageException = "Storage <%s> failure! (line %d:%s)";
#define STORAGE_ASSUME(exp) if (!(exp)) throw format (gs_szStorageException, m_strName.c_str(), __LINE__, #exp)
//---------------------------------------------------------------------------
// Aktueller Header
//---------------------------------------------------------------------------
const CStorage::typHeader CStorage::ms_aDefaultHeader =
{
	sizeof(typHeader),
	{'P','S','I','t','p','p','r','o'},  // ID, never change!
	1, 0, 1,                            // Version
	"PSI(c) "__DATE__                   // Label
};
//---------------------------------------------------------------------------
CStorage::CStorage (const char * szPath, const char * szName)
{
	m_paFile = 0;
	setPath (szPath);
	setName (szName);
	m_aIndex.clear();
	m_aCursor = m_aIndex.end();
}
//---------------------------------------------------------------------------
CStorage::~CStorage ()
{
	close();
}
//---------------------------------------------------------------------------
void
CStorage::setName (const char * szName)
{
	close();
	m_strName = szName;
}
//---------------------------------------------------------------------------
void
CStorage::setPath (const char * szPath, const char * szName)
{
	char szBuff[8000];
	close();
	_fullpath (szBuff, szPath, sizeof(szBuff));
	m_strPath = szBuff;
	if (szName) setName (szName);
}
//---------------------------------------------------------------------------
// LOWLEVEL I/O
//---------------------------------------------------------------------------
FILE *
CStorage::open (const char * szFile, FILE * paPrev)
{
	const char * szMode = "r+b";
	typHeader & aHeader = m_aHeader;
	//// File oeffnen
	FILE * paFile = 0;
	if (paPrev)
		paFile = freopen (szFile, szMode, paPrev);
	else
		paFile = fopen (szFile, szMode);
	if (paFile == 0)
		return paFile;
	//// Check Header
	if (fread (&aHeader, sizeof(aHeader), 1, paFile) == 1)
	if (strncmp (aHeader.ID, "PSItppro", 8) == 0)
	if (aHeader.nSize == sizeof(aHeader))
		return paFile;
	//@@@@ qWarning ("Header?! check failure!");
	fclose (paFile);
	return 0;
}
//---------------------------------------------------------------------------
FILE *
CStorage::create (const char * szFile, FILE * paPrev)
{
	const char * szMode = "w+b";
	FILE * paFile = 0;
	if (paPrev)
		paFile = freopen (szFile, szMode, paPrev);
	else
		paFile = fopen (szFile, szMode);
	if (paFile == 0)
		return paFile;
	//// Write Header
	m_aHeader = ms_aDefaultHeader;
//	tpbas::CMeldung("Creating File %s", szFile).ausgeben();
	fwrite (&m_aHeader, sizeof(m_aHeader), 1, paFile);
	fflush (m_paFile);
	return paFile;
}
//---------------------------------------------------------------------------
QString
CStorage::absFilePath() const
{
	QString strFilePath (m_strPath);
	strFilePath += "\\";
	strFilePath += m_strName;
	return strFilePath;
}
//---------------------------------------------------------------------------
// Export I/O
//---------------------------------------------------------------------------
bool
CStorage::create()
{
//	QFileInfo aFileInfo = getFileInfo();
//	QString strFile = aFileInfo.absFilePath();
	QString strFile = absFilePath();
	m_paFile = create (strFile.c_str(), m_paFile);
	m_aIndex.clear();
	return isOpen();
}
//---------------------------------------------------------------------------
bool
CStorage::open (bool bForce)
{
	////-----------------------------------------
	//// File oeffnen
	////-----------------------------------------
	if (isOpen()) return true;
	QString strFile = absFilePath();
#ifdef TPGLO_OS_WIN32
	strFile.replace ('/', '\\');
#endif
	if (m_paFile == 0)
		m_paFile = open (strFile.c_str());
	if (m_paFile == 0 && bForce)
		m_paFile = create (strFile.c_str());
	if (m_paFile == 0)
		return false;
	////-----------------------------------------
	//// Index erstellen
	////-----------------------------------------
	m_aIndex.clear();
	long nIndex = ftell (m_paFile);
	typItem aItem;
	while (fread (&aItem, sizeof(aItem), 1, m_paFile) == 1)
	{
		STORAGE_ASSUME (check (aItem));
		const int nData = aItem.nLen * aItem.nRec;
		STORAGE_ASSUME (nData > 0);
		m_aIndex[nIndex] = aItem;
		nIndex += sizeof(aItem) + nData;
		STORAGE_ASSUME (fseek (m_paFile, nIndex, SEEK_SET) == 0);
	}
	m_aCursor = m_aIndex.end();
	return isOpen();
}
//---------------------------------------------------------------------------
bool
CStorage::close ()
{
	if (m_paFile != 0)
		fclose (m_paFile);
	m_paFile = 0;
	m_aIndex.clear();
	m_aCursor = m_aIndex.end();
	return true;
}
//---------------------------------------------------------------------------
bool
CStorage::copy (const char * szDest)
{
	FILE * paDest = fopen (szDest, "wb");
	if (paDest == 0)
		return false;
	if (!open(false))
		return false;
	char szBuffer[8192];
	::rewind (m_paFile);
	while (true)
	{
		const int nRead = fread (szBuffer, 1, sizeof(szBuffer), m_paFile);
		if (nRead < 1)
			break;
		const int nWrite = fwrite (szBuffer, 1, nRead, paDest);
		STORAGE_ASSUME (nRead == nWrite);
	}
	fclose (paDest);
	return true;
}
//---------------------------------------------------------------------------
bool
CStorage::match (const char * szKey, const char * szArc, const typItem & aItem)
{
	if (!isValid (aItem))
		return false;
	if (szKey != 0 && strcmp(aItem.szKey, szKey) != 0)
		return false;
	if (szArc != 0 && strcmp(aItem.szArc, szArc) != 0)
		return false;
	return true;
}
//---------------------------------------------------------------------------
bool
CStorage::alloc (const char * szKey
, const char * szArc
, int nLen
, int nRec)
{
	STORAGE_ASSUME (szKey != 0);
	STORAGE_ASSUME (nRec > 0);
	long nFree = 0;
	int i;

	////-----------------------------------------
	//// Eintrag suchen
	////-----------------------------------------
	STORAGE_ASSUME (open(true));
	rewind();
	while (next())
	{
		long nIndex = key(m_aCursor);
		typItem & aItem = data(m_aCursor);
		const bool bSize = (nRec <= aItem.nRec);
		STORAGE_ASSUME (nLen == aItem.nLen);
		////-----------------------------------------
		//// Eintrag gefunden
		////-----------------------------------------
		if (match (szKey, szArc, aItem))
		{
			////-----------------------------------------
			//// Groesse ok ==> fertig
			////-----------------------------------------
			if (bSize)
				return true;
			////-----------------------------------------
			//// Eintrag als frei markieren (loeschen)
			//// (wenn nicht genuegend Platz)
			////-----------------------------------------
			setValid (aItem, false);
			STORAGE_ASSUME (fseek (m_paFile, nIndex, SEEK_SET) == 0);
			STORAGE_ASSUME (fwrite (&aItem, sizeof(aItem), 1, m_paFile));
			fflush (m_paFile);
		}
		////-----------------------------------------
		//// Ersten freien Block <nFree>
		//// passender Groesse merken
		////-----------------------------------------
		if (!nFree && !isValid(aItem) && bSize)
			nFree = nIndex;
	}
	////-----------------------------------------
	//// Neuen Eintrag anhaengen (ggf.)
	////-----------------------------------------
	if (!nFree)
	{
		fseek (m_paFile, 0, SEEK_END);
		nFree = ftell (m_paFile);
		typItem & aItem = m_aIndex [nFree];
		aItem.nRec = nRec;
		aItem.nLen = nLen;
	}
	////-----------------------------------------
	//// Index-Eintrag  schreiben
	////-----------------------------------------
	m_aCursor = m_aIndex.find (nFree); 
	typItem & aItem = data(m_aCursor);
	setItem (aItem, szKey, szArc);
	setValid (aItem, true);
	fseek (m_paFile, nFree, SEEK_SET);
	fwrite (&aItem, sizeof(aItem), 1, m_paFile);
	////-----------------------------------------
	//// Neuen Record-Block schreiben
	////-----------------------------------------
	const int nSize = aItem.nRec * aItem.nLen;
	for (i=0; i<nSize; i++)
		fputc (0, m_paFile);
	fflush (m_paFile);
	return true;
}
//---------------------------------------------------------------------------
bool
CStorage::seek (const char * szKey, const char * szArc)
{
	if (!open())
		return false;
	////-----------------------------------------
	//// Eintrag suchen
	////-----------------------------------------
	rewind();
	while (next())
	{
		const typItem & aItem = *current();
		if (match (szKey, szArc, aItem))
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------
void
CStorage::rewind()
{
	m_aCursor = m_aIndex.begin();
}
//---------------------------------------------------------------------------
bool
CStorage::next()
{
	if (m_aCursor == m_aIndex.end())
		return false;
	m_aCursor++;
	return m_aCursor != m_aIndex.end();
}
//---------------------------------------------------------------------------
const CStorage::typItem *
CStorage::current() const
{
	if (EOT())
		return 0;
	const typItem * paItem = & data(m_aCursor);
	return paItem;
}
//---------------------------------------------------------------------------
long
CStorage::currentKey() const
{
	if (!isOpen())
		return -1L;
	STORAGE_ASSUME (!EOT());
	return key (m_aCursor);
}
//---------------------------------------------------------------------------
long
CStorage::read (int nIdx, void * paRec, int nSize)
{
	STORAGE_ASSUME (isOpen());
	STORAGE_ASSUME (current());
	const typItem & aItem = *current();
	STORAGE_ASSUME (nIdx < aItem.nRec);
	STORAGE_ASSUME (nSize == aItem.nLen);
	long nOffset = currentKey();
	nOffset += sizeof(aItem);
	nOffset += nIdx * aItem.nLen;
	fseek (m_paFile, nOffset , SEEK_SET);
	return fread (paRec, 1, nSize, m_paFile);
}
//---------------------------------------------------------------------------
long
CStorage::write (int nIdx, const void * paRec, int nSize)
{
	STORAGE_ASSUME (isOpen());
	STORAGE_ASSUME (current());
	const typItem & aItem = *current();
	STORAGE_ASSUME (nIdx < aItem.nRec);
	STORAGE_ASSUME (nSize == aItem.nLen);
	long nOffset = currentKey();
	nOffset += sizeof(aItem);
	nOffset += nIdx * aItem.nLen;
	fseek (m_paFile, nOffset , SEEK_SET);
	const int nWrite = fwrite (paRec, 1, nSize, m_paFile);
	fflush (m_paFile);
	return nWrite;
}
//---------------------------------------------------------------------------
void
CStorage::remove (const char * szKey, const char * szArc)
{
	STORAGE_ASSUME (open());
	typIndex & aIndex = m_aIndex;
	typIndex::iterator aIter;
	for (aIter = aIndex.begin(); aIter != aIndex.end(); ++aIter)
	{
		long nItem = key (aIter);
		typItem & aItem = data (aIter);
		if (!match (szKey, szArc, aItem))
			continue;
		setValid (aItem, false);
		STORAGE_ASSUME (fseek (m_paFile, nItem, SEEK_SET) == 0);
		STORAGE_ASSUME (fwrite (&aItem, sizeof(aItem), 1, m_paFile));
	}
	fflush (m_paFile);
}
//---------------------------------------------------------------------------
bool
CStorage::isValid (const typItem & aItem)
{
	assert (check (aItem));
	return aItem.nSize >= 0x80;
}
//---------------------------------------------------------------------------
void
CStorage::setValid (typItem & aItem, bool bValid)
{
	aItem.nSize = sizeof (aItem) & 0x7F;
	if (bValid)	aItem.nSize |= 0x80;
}
//---------------------------------------------------------------------------
void
CStorage::setItem (typItem & aItem, const char * szKey, const char * szArc)
{
	memset (aItem.szKey, 0, sizeof(aItem.szKey));
	memset (aItem.szArc, 0, sizeof(aItem.szArc));
	if (szKey)
		strncpy (aItem.szKey, szKey, sizeof(aItem.szKey));
	if (szArc)
		strncpy (aItem.szArc, szArc, sizeof(aItem.szArc));
}
//---------------------------------------------------------------------------
int
CStorage::cmp (const typItem & aI1, const typItem & aI2)
{
	int nRet = strcmp (aI1.szKey, aI2.szKey);
	if (nRet == 0)
		nRet = strcmp (aI1.szArc, aI2.szArc);
	nRet = memcmp (&aI1, &aI2, sizeof(typItem));
	return nRet;
}
//---------------------------------------------------------------------------
bool operator== (const CStorage::typItem & aI1, const CStorage::typItem & aI2)
{
	return CStorage::cmp (aI1, aI2) == 0;
}
//---------------------------------------------------------------------------
bool operator< (const CStorage::typItem & aI1, const CStorage::typItem & aI2)
{
	return CStorage::cmp (aI1, aI2) < 0;
}
//---------------------------------------------------------------------------
bool operator> (const CStorage::typItem & aI1, const CStorage::typItem & aI2)
{
	return CStorage::cmp (aI1, aI2) > 0;
}
//---------------------------------------------------------------------------
}		// ende namespace
