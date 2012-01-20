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

#include "CImage.h"
#include <stdarg.h>

#define JPEG_FFD8 1
void jpeg_test()
{
	long nJPEG_Start = 0;
	int nMode = 0;
	char c;
	const char * szFile = "D:\\jvater\\Media\\Bilder\\Beispiel.jpg";
	FILE * pFile = fopen (szFile, "rb");
	assert (pFile);
	while (true)
	{
		c = fgetc (pFile);
		if (c == EOF)
			break;
		if (c == 0xff)
			nMode = c;
		else
		if (nMode == 0xff && c == 0xd8)
		{
			nMode = JPEG_FFD8;
			nJPEG_Start = ftell (pFile);
		}


	}
	fclose (pFile);
}
//---------------------------------------------------------------------------
