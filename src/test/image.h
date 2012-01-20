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
#ifndef TPPRO_CIMAGE_H
#define TPPRO_CIMAGE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

//---------------------------------------------------------------------------
enum iptc_t
{	IPTC_BY_LINE                         = 0x50
,	IPTC_BY_LINE_TITLE                   = 0x55
,	IPTC_CREDIT                          = 0x6e
,	IPTC_SOURCE                          = 0x73
,	IPTC_OBJECT_NAME                     = 0x05
,	IPTC_DATE_CREATED                    = 0x37
,	IPTC_CITY                            = 0x5a
,	IPTC_PROVINCE_STATE                  = 0x5f
,	IPTC_COUNTRY_NAME                    = 0x65
,	IPTC_ORIGINAL_TRANSMISSION_REFERENCE = 0x67
,	IPTC_COPYRIGHT                       = 0x74
,	IPTC_CAPTION                         = 0x78
,	IPTC_WRITER_EDITOR                   = 0x7a
,	IPTC_HEADLINE                        = 0x69
,	IPTC_SPECIAL_INSTRUCTIONS            = 0x28
,	IPTC_CATEGORY                        = 0x0f
,	IPTC_SUPPLEMENTAL_CATEGORY           = 0x14  //VECTOR
,	IPTC_KEYWORDS                        = 0x19  //VECTOR
};
//---------------------------------------------------------------------------
enum jfif_t
{	JFIF_SOI   = 0xd8
,	JFIF_EOI   = 0xd9
,	JFIF_SOS   = 0xda

,	JFIF_APP0  = 0xe0
,	JFIF_APP13 = 0xed

,	JFIF_EXIF  = JFIF_APP0
,	JFIF_IPTC  = JFIF_APP13
};
//---------------------------------------------------------------------------
extern const char * iptc_field (iptc_t);
extern const char * jfif_segm_name (short);
//---------------------------------------------------------------------------
class CIPTC
{
public:

	virtual ~CIPTC() {}
	bool parse (const unsigned char * buff, long size);
	bool x1C02 (const unsigned char * pBuff, long nBuff);

	virtual void set (int type, const unsigned char * buff, long size) = 0;
	virtual const char * field (iptc_t) const;

};
//---------------------------------------------------------------------------
class CIPTCCharArray : public CIPTC
{
public:

	CIPTCCharArray();
	void set (int type, const unsigned char * buff, long size);

	char headline[256];
	char caption[2000];
	vector<string> keywords;
	char by_line[32];
	char by_line_title[32];
	char copyright[128];
	char credit[32];
	char contact[128];
	char object_name[64];
	char date_created[8];
	char time_createt[11];
	char digital_creation_date[8];
	char digital_creation_time[11];
	char city[32];
	char sublocation[32];
	char province_state[32];
	char country_code[3];
	char country_name[64];
	char special_instructions[256];
	char source[32];
	char program[32];
	char edit_status[64];
	char envelope_priority;
	char object_cycle;
	char original_transmission_reference[32];
	char fixture_identifier[32];
	char writer_editor[128];
	char category[3];
	vector<string> category_sub;
};
//---------------------------------------------------------------------------
class CIPTCString : public CIPTC
{
public:
	typedef unsigned char uchar;

	CIPTCString();
	virtual ~CIPTCString();
	void set (unsigned char type, const uchar * buff, long size);

protected:
	map<uchar, string> m_aAttr;

	vector<string> m_aKeywords;
	vector<string> m_aCategorys;
};
//---------------------------------------------------------------------------
class CJPEG
{
public:
	CJPEG();
	CJPEG (const char * file, const char *mode="rb");
	virtual ~CJPEG();


	virtual bool open (const char * file, const char * mode="rb");
	virtual void assign (FILE*, bool owner=true);
	virtual void close() { assign (0); }
	virtual void segment (short id, size_t size);


//protected:
	FILE * m_pFile;
	bool m_bOwner;
	vector<long> m_aStruct;
	long m_nJPEG_Start;
	long m_nJPEG_Scan;
	long m_nJPEG_End;
	bool m_bProceed;
	CIPTCCharArray m_aIPTC;

};
//---------------------------------------------------------------------------
inline
CJPEG::CJPEG (const char * file, const char *mode)
{
	m_pFile = 0;
	m_nJPEG_Start = 0;
	m_nJPEG_Scan = 0;
	m_nJPEG_End = 0;
	open (file, mode); 
}					 
//---------------------------------------------------------------------------
void jpeg_test();
//---------------------------------------------------------------------------
#endif
