/***************************************************************************************

    Projekt:    Hasy                                                     (C) 2000 PSI AG
    Komponente: tppro
    Modul:      CStorage.h

    Online Datei

    Systeme: Alpha, Linux, NT 4

    $Author: hasyjv $
    $Date: 2005/08/19 10:43:15 $
    $Revision: 1.7 $
    $State: Exp $

    Änderung     Datum     Autor  Bemerkung
    ------------------------------------------------------------------------------------
                 24.06.03  jv     Ausgelagert aus CDataBase
***************************************************************************************/
#ifndef TPPRO_CSTORAGE_H
#define TPPRO_CSTORAGE_H

#include <assert.h>
#include <stdio.h>
#include <string>
#include <map>

namespace tppro
{
//---------------------------------------------------------------------------
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef long int INT32;
typedef unsigned char INT8;

typedef std::string QString;
//---------------------------------------------------------------------------
// DB-Tabelle (Online-Dateien)
//---------------------------------------------------------------------------
class CStorage
{
public:
	////-----------------------------------------
	typedef struct
	{
		UINT8  nSize;
		char   ID[8];
		UINT8  nVersion;
		UINT8  nRelease;
		UINT8  nBuild;
		char   szLabel[20];
	} typHeader;
	////-----------------------------------------
	static const typHeader ms_aDefaultHeader;
	////-----------------------------------------
	typedef struct
	{
		UINT8  nSize;
		char   szKey[33];
		char   szArc[20];
		UINT16 nLen;            // reservierte Anz. von Bytes pro Record
		UINT16 nRec;			// reservierte Anz. von Records
	} typItem;
	////-----------------------------------------
	typedef std::map<long,typItem> typIndex;
	////-----------------------------------------

	CStorage (const char * path=0, const char * name=0);
	virtual ~CStorage ();

	void setName (const char * name);
	void setPath (const char * path, const char * name=0);
	const QString & name() const                   { return m_strName; }
	const QString & path() const                   { return m_strPath; }
	QString absFilePath() const;
	const typHeader & header()                     { if(!open()) assert(false); return m_aHeader; }

	bool create();
	bool open (bool force=false);
	bool close();
	bool copy (const char * dest);
	bool isOpen() const { return (m_paFile != 0); }

	bool alloc (const char * key, const char * arc, int nLen, int nRec);
	bool seek (const char * key, const char * arc);
	void remove (const char * key, const char * arc=0);

	void rewind();
	bool next();

	const typItem * current() const;
	long currentKey() const;
	long read (int iRec, void * record, int size);
	long write (int iRec, const void * record, int size);

	static bool match (const char * szKey, const char * szArc, const typItem &);
	static bool check (const typItem &);
	static bool isValid (const typItem &);
	static void setValid (typItem &, bool);
	static void setItem (typItem &, const char * key, const char * arc=0);
	static int cmp (const CStorage::typItem &, const CStorage::typItem &);

	bool EOT() const { return static_cast<typIndex::const_iterator>(m_aCursor) == m_aIndex.end(); }

protected:
	static long key (typIndex::const_iterator i) { return (*i).first; }
	static const typItem & data (typIndex::const_iterator i) { return (*i).second; }
	static typItem & data (typIndex::iterator i) { return (*i).second; }
	FILE * open (const char * file, FILE * prev=0);
	FILE * create (const char * file, FILE * prev=0);

	typHeader m_aHeader;
	QString   m_strPath;
	QString   m_strName;
	FILE *    m_paFile;
	typIndex  m_aIndex;
	typIndex::iterator m_aCursor;
};
//---------------------------------------------------------------------------
inline bool
CStorage::check (const typItem & aIndex)
{
	const int nIdx1 = aIndex.nSize & 0x7F;
	const int nIdx2 = sizeof(aIndex) & 0x7F;
	return nIdx1 == nIdx2;
}
//---------------------------------------------------------------------------
bool operator== (const CStorage::typItem &, const CStorage::typItem &);
bool operator< (const CStorage::typItem &, const CStorage::typItem &);
bool operator> (const CStorage::typItem &, const CStorage::typItem &);
//---------------------------------------------------------------------------



////-----------------------------------------
// Tabellen
////-----------------------------------------
typedef enum
{
	P_ARCHIV,
	REGMODEL,
	HRGMODEL,
	ZTRMODEL,
	TAUMODEL,
	TableTYP
} typTable;
////-----------------------------------------
//// P-Archiv - Record (Stundenwerte)
////-----------------------------------------
typedef struct
{
	INT32 start;
	float val[240];
} typErgArchiv;
////-----------------------------------------
//// Reg.Modell - Record
////-----------------------------------------
typedef struct
{
	INT8 isValid;
	float a;
	float b;
	float k;
	float P[3];
} typRegModell;
////-----------------------------------------
//// Hrg.Modell - Record
////-----------------------------------------
typedef struct
{
	INT8 isValid;
	float profile[24];
} typHrgModell;
////-----------------------------------------
//// Ztr.Modell - Record
////-----------------------------------------
typedef struct
{
	INT8 isValid;
	float P[(24+1)*24/2];	// symetrische Matrix (gepackt)
	float b[24];
} typZtrModell;
////-----------------------------------------
//// Tau.Modell - Record
////-----------------------------------------
typedef struct
{
	double dQdg;			// Tagesgrundlast
	double dTau;			// Parameter
	double aProfile[24];	// Tagesprofil
} typTauModell;

//---------------------------------------------------------------------------
}		// ende namespace
#endif
