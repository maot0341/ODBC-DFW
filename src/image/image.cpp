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

#include <stdarg.h>
#include <assert.h>
#include "utils.h"
#include "image.h"

//---------------------------------------------------------------------------
// IPTC -- Base Class
//---------------------------------------------------------------------------
const char * CIPTC::field (iptc_t i) const
{
	return iptc_field (i);
}
//---------------------------------------------------------------------------
// IPTC --- Header [0x1C 0x02]
//---------------------------------------------------------------------------
bool CIPTC::x1C02 (const unsigned char * pBuff, long nBuff)
{
	const unsigned char * p = pBuff;
	const unsigned char * e = pBuff + nBuff;
	short nType;
	long nSize;
	while ((p+5 < e) && (p[0] == 0x1C) && (p[1] == 0x02))
	{
		nType = p[2];
		nSize = (p[3] << 8) + p[4];
		p += 5;
		set (nType, p, nSize);
		p += nSize;
	}
	return p == e;
}
//---------------------------------------------------------------------------
// IPTC -- Start at APP13 (0xed)
//---------------------------------------------------------------------------
bool CIPTC::parse (const unsigned char * pBuff, long nBuff)
{
	const char * szPS3 = "Photoshop 3.0";
	const char * szBIM = "8BIM";
	int nType = 0;
	int nSize = 0;
	const unsigned char * p = pBuff;
	const unsigned char * e = pBuff + nBuff;
	assert (pBuff);
	assert (nBuff > 0);
	if (memcmp (pBuff, szPS3, 14))
		return false;
	p += 14;
	////-----------------------------------------
	//// 8BIM - Header
	////-----------------------------------------
	while ((p+11 < e) && !memcmp (p, szBIM, 4))
	{
		p += 4;
		if (p + 8 >= e)
			return false;
		nType = (p[0] << 8) + p[1];
		nSize = (p[6] << 8) + p[7];
		p += 8;
		x1C02 (p, nSize);
		p += nSize;
	}
	return p == e;
}
//---------------------------------------------------------------------------
// IPTC -- Version: char[]
//---------------------------------------------------------------------------
CIPTCCharArray::CIPTCCharArray()
{
	*headline = 0;
	*caption = 0;
	//keywords = 0;
	*by_line = 0;
	*by_line_title = 0;
	*copyright = 0;
	*credit = 0;
	*contact = 0;
	*object_name = 0;
	*date_created = 0;
	*time_createt = 0;
	*digital_creation_date = 0;
	*digital_creation_time = 0;
	*city = 0;
	*sublocation = 0;
	*province_state = 0;
	*country_code = 0;
	*country_name = 0;
	*country_name = 0;
	*special_instructions = 0;
	*source = 0;
	*program = 0;
	*edit_status = 0;
	envelope_priority = 0;
	object_cycle = 0;
	*original_transmission_reference = 0;
	*fixture_identifier = 0;
	*writer_editor = 0;
	*category = 0;
}
//---------------------------------------------------------------------------
#ifndef MIN
#	define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#	define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#define IPTC_SET(arr,buf,len)		{ \
	size_t i = MIN((size_t)len,sizeof(arr)-1); \
	memcpy (arr,buf,i); \
	arr[i] = 0; \
}
//---------------------------------------------------------------------------
void CIPTCCharArray::set (int nType, const unsigned char * buff, long size)
{
	string strValue ((const char *)buff,size);
	switch (nType)
	{
	//credits
	case '\x50':
		IPTC_SET (by_line, buff, size);
		break;
	case '\x55':
		IPTC_SET (by_line_title, buff, size);
		break;
	case '\x6E':
		IPTC_SET (credit, buff, size);
		break;
	case '\x73':
		IPTC_SET (source, buff, size);
		break;
	//origin
	case '\x05':
		IPTC_SET (object_name, buff, size);
		break;
	case '\x37':
		IPTC_SET (date_created, buff, size);
		break;
	case '\x5A':
		IPTC_SET (city, buff, size);
		break;
	case '\x5F':
		IPTC_SET (province_state, buff, size);
		break;
	case '\x65':
		IPTC_SET (country_name, buff, size);
		break;
	case '\x67':
		IPTC_SET (original_transmission_reference, buff, size);
		break;
	//copyright
	case '\x74':
		IPTC_SET (copyright, buff, size);
		break;
	//caption
	case '\x78':
		IPTC_SET (caption, buff, size);
		break;
	case '\x7A':
		IPTC_SET (writer_editor, buff, size);
		break;
	case '\x69':
		IPTC_SET (headline, buff, size);
		break;
	case '\x28':
		IPTC_SET (special_instructions, buff, size);
		break;
	//categories
	case '\x0F':
		IPTC_SET (category, buff, size);
		break;
	case '\x14':
		category_sub.push_back (strValue);
		break;
	//Keywords
	case '\x19':
		keywords.push_back (strValue);
		break;
	default:
		trace ("%s[%d]: IPTC unknown type 0x%02X\n", __FILE__, __LINE__, (int)nType);
	}
}
//---------------------------------------------------------------------------
// IPTC -- Version: String
//---------------------------------------------------------------------------
CIPTCString::CIPTCString()
{
}
//---------------------------------------------------------------------------
CIPTCString::~CIPTCString()
{
}
//---------------------------------------------------------------------------
void CIPTCString::set (unsigned char cType, const unsigned char * buff, long size)
{
	string strValue ((const char *)buff,size);
	if (cType == '\x14')
		m_aCategorys.push_back (strValue);
	else
	if (cType == '\x19')
		m_aKeywords.push_back (strValue);
	else
		m_aAttr[cType] = strValue;
}
//---------------------------------------------------------------------------
// CJPEG - JFIF File Parser
//---------------------------------------------------------------------------
CJPEG::CJPEG()
{
	m_pFile = 0;
	m_nJPEG_Start = 0;
	m_nJPEG_Scan = 0;
	m_nJPEG_End = 0;
	m_nJPEG_StartOffsetMax = 0xff;
}
//---------------------------------------------------------------------------
CJPEG::~CJPEG()
{
	if (m_pFile && m_bOwner)
		fclose (m_pFile);
	m_pFile = 0;
}
//---------------------------------------------------------------------------
bool CJPEG::open (const char * szFile, const char * szMode)
{
	FILE * pFile = fopen (szFile, szMode);
	if (!pFile)
		return false;
	assign (pFile);
	return true;
}
//---------------------------------------------------------------------------
void CJPEG::segment (short nSegm, size_t nSize)
{
	assert (m_pFile);
	if (nSegm == 0xed)
	{
		unsigned char * paBuff = new unsigned char [nSize];
		long nIPTC = fread (paBuff, 1, nSize-2, m_pFile);
		assert (nIPTC == nSize-2);
		m_aIPTC.parse (paBuff, nIPTC);
		delete [] paBuff;
		m_bProceed = false;
	}
}
//---------------------------------------------------------------------------
// JPEG / JFIF -- Parser
//---------------------------------------------------------------------------
enum jpeg_state_t
{ JPEG_S0=0
, JPEG_FF
, JPEG_FFD8
, JPEG_FFxx
, JPEG_FFs1
, JPEG_FFs2
, JPEG_SCAN
, JPEG_SCAN_FF
, JPEG_EOI
};
//---------------------------------------------------------------------------
void CJPEG::assign (FILE * pFile, bool bOwner)
{
	if (m_pFile && m_bOwner)
		fclose (m_pFile);
	m_pFile = pFile;
	m_bOwner = bOwner;
	m_aStruct.clear();
	m_nJPEG_Start = 0;
	m_nJPEG_Scan = 0;
	m_nJPEG_End = 0;
	if (!pFile)
		return;
	long nSize;
	long i;
	int c;
	short nSegm;

	m_bProceed = true;
	jpeg_state_t nMode = JPEG_S0;
	while (m_bProceed)
	{
		c = fgetc (pFile);
		if (c == EOF)
			break;
		i = ftell(pFile);

		if (!m_nJPEG_Start)
		if (m_nJPEG_StartOffsetMax)
		if (i > m_nJPEG_StartOffsetMax)
			break;
		
		if(nMode == JPEG_SCAN_FF)
		{
			nMode = (c == 0x00) ? JPEG_SCAN : JPEG_FF;
		}

		if (nMode == JPEG_S0 && c == 0xff)
		{
			nMode = JPEG_FF;
		}
		else
		if(nMode == JPEG_SCAN)
		{
			if (c == 0xff)
				nMode = JPEG_SCAN_FF;
		}
		else
		if(nMode == JPEG_FF && c == 0xd8)
		{
			nMode = JPEG_S0;
			m_nJPEG_Start = ftell (pFile);
			long nPos = ftell (m_pFile) - 2;
			m_aStruct.push_back (nPos);
		}
		else
		if(nMode == JPEG_FF && c == 0xd9)
		{
			nMode = JPEG_EOI;
			m_nJPEG_End = ftell (pFile) - 2;
			long nPos = ftell (m_pFile) - 4;
			m_aStruct.push_back (nPos);
		}
		else
		if(nMode == JPEG_FF && c == 0xda)
		{
			nMode = JPEG_SCAN;
			m_nJPEG_Scan = ftell (pFile) - 2;
			long nPos = ftell (m_pFile) - 4;
			m_aStruct.push_back (nPos);
			m_bProceed = false;
		}
		else
		if (nMode == JPEG_FF)
		{
			nSegm = c;
			nMode = JPEG_FFxx;
		}
		else
		if (nMode == JPEG_FFxx)
		{
			nMode = JPEG_FFs1;
			nSize = c;
		}
		else
		if (nMode == JPEG_FFs1)
		{
			long nPos = ftell (m_pFile) - 4;
			m_aStruct.push_back (nPos);
			nMode = JPEG_S0;
			nSize = (nSize << 8) + c;
			long nNextFF = ftell (pFile) + nSize - 2;
			segment (nSegm, nSize);
			c = fseek (pFile, nNextFF, SEEK_SET);
			assert (c == 0);

//			const char * szRemark = jfif_segm_name (nSegm);
//			trace ("JPEG: FF %02X %04X (%9d)\t%s\n", (int)nSegm, (int)nSize, (int)nSize, szRemark);
		}
		else
		{
			if (!m_nJPEG_Start)
				break;
			trace ("jpeg %08X: 0x%02x\n", (int)i, (int)c);
		}
	}
//	trace ("image:  SOI=%08X  SOS=%08X  EOS=%08X\n", m_nJPEG_Start, m_nJPEG_Scan, m_nJPEG_End);
}
//---------------------------------------------------------------------------
void jpeg_test()
{
	//const char * szFile = "D:\\jvater\\Media\\Bilder\\Beispiel.jpg";
	const char * szFile = "D:\\jvater\\Media\\Bilder\\Kanupark\\k10051615044.jpg";
	CJPEG aJPEG (szFile);
}
//---------------------------------------------------------------------------
