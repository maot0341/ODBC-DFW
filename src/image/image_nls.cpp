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
#include "image.h"
//---------------------------------------------------------------------------
const char * iptc_field (iptc_t i)
{
	if (i == IPTC_HEADLINE)
		return "Headline";
	if (i == IPTC_CAPTION)
		return "Caption/ Abstract";
	if (i == IPTC_KEYWORDS)
		return "Keywords";
	if (i == IPTC_BY_LINE)
		return "By-line";
	if (i == IPTC_BY_LINE_TITLE)
		return "By-line Title";
	if (i == IPTC_COPYRIGHT)
		return "Copyright";
	if (i == IPTC_CREDIT)
		return "Credit";
//	if (i == CONTACT)
//		return "Contact";
	if (i == IPTC_OBJECT_NAME)
		return "Object Name";
	if (i == IPTC_DATE_CREATED)
		return "Date Created";
//	if (i == TIME_CREATED)
//		return "Time Created";
//	if (i == DIGITAL_CREATION_DATE)
//		return "Digital Creation Date";
//	if (i == DIGITAL_CREATION_TIME)
//		return "Digital Creation Time";
	if (i == IPTC_CITY)
		return "City";
//	if (i == SUBLOCATION)
//		return "Sublocation";
	if (i == IPTC_PROVINCE_STATE)
		return "Province/ State";
//	if (i == COUNTRY_CODE)
//		return "Country/ Primary Location Code";
	if (i == IPTC_COUNTRY_NAME)
		return "Country/ Primary Location Name";
	if (i == IPTC_SPECIAL_INSTRUCTIONS)
		return "Special Instructions";
	if (i == IPTC_SOURCE)
		return "Source";
//	if (i == IPTC_PROGRAM)
//		return "Originating Program";
//	if (i == IPTC_EDIT_STATUS)
//		return "Edit Status";
//	if (i == IPTC_ENVELOPE_PRIORITY)
//		return "Envelope Priority";
//	if (i == IPTC_OBJECT_CYCLE)
//		return "Object Cycle";
	if (i == IPTC_ORIGINAL_TRANSMISSION_REFERENCE)
		return "Original Transmission Reference";
//	if (i == IPTC_FIXTURE_IDENTIFIER)
//		return "Fixture Identifier";
	if (i == IPTC_WRITER_EDITOR)
		return "Writer/ Editor";
	if (i == IPTC_CATEGORY)
		return "Category";
	if (i == IPTC_SUPPLEMENTAL_CATEGORY)
		return "Supplemental Category";
	return "iptc?!";
}
//---------------------------------------------------------------------------
const char * jfif_segm_name (short nSegm)
{
	if (nSegm == 0xD8)
		return "SOI Start Of Image";
	if (nSegm == 0xD9) 
		return "EOI End of Image";
	if (nSegm == 0xDA) 
		return "SOS Start of Scan";

	if (nSegm == 0xC0)
		return "SOF0 Baseline DCT";
	if (nSegm == 0xC1)
		return "SOF1 Extended sequential DCT";
	if (nSegm == 0xC2)
		return "SOF2 Progressive DCT";
	if (nSegm == 0xC3)
		return "SOF3 Lossless (sequential)";
	if (nSegm == 0xC5)
		return "SOF5 Differential sequential DCT";
	if (nSegm == 0xC6)
		return "SOF6 Differential progressive DCT";
	if (nSegm == 0xC7)
		return "SOF7 Differential lossless (sequential)";
	if (nSegm == 0xC8)
		return "JPEG (reserved for extensions)";
	if (nSegm == 0xC9)
		return "SOF9 Extended sequential DCT";
	if (nSegm == 0xCA)
		return "SOF10 Progressive DCT";
	if (nSegm == 0xCB)
		return "SOF11 Lossless (sequential)";
	if (nSegm == 0xCC)
		return "SOF13 Differential sequential DCT";

	if (nSegm == 0xC4)
		return "DHT Definition der Huffman-Tabellen";
	if (nSegm == 0xCC)
		return "DAC Definition der arithmetischen Codierung";
	if (nSegm == 0xDB)
		return "DQT Definition der Quantisierungstabellen";

	//// APPn 
	if (nSegm == 0xE0)
		return "APP0 JFIF tag";
	if (nSegm == 0xE1)
		return "APP1 (Exif)";
	if (nSegm == 0xED)
		return "APP13 (IPTC NAA)";
	if (nSegm == 0xEE)
		return "APP14 (Copyright)";
	if (0xE0 <= nSegm && nSegm <= 0xEF)
		return "APPn ";

	if (nSegm == 0xFE) 
		return "COM Comment";

	return "?!";
}
//---------------------------------------------------------------------------
