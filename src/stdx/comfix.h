/***
* comutil.h - Native C++ compiler COM support - BSTR, VARIANT wrappers header
*
*	Copyright (C) 1996-1997 Microsoft Corporation
*	All rights reserved.
*
****/

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_COMUTIL_FIX_
#define _INC_COMUTIL_FIX_
//-----------------------------------------------------------------------------

#include <comdef.h>
#include <ole2.h>
#include <string>

#pragma warning(push)
#pragma warning(disable:4290)
#pragma warning(disable:4310)

using namespace std;
//-----------------------------------------------------------------------------
// Convert char * to BSTR
//-----------------------------------------------------------------------------
inline BSTR ConvertStringToBSTR(const char* pSrc)
{
	if(!pSrc) return NULL;

	DWORD cwch;

	BSTR wsOut(NULL);

	if(cwch = ::MultiByteToWideChar(CP_ACP, 0, pSrc, -1, NULL, 0))//get size minus NULL terminator
	{
		cwch--;
		wsOut = ::SysAllocStringLen(NULL, cwch);

		if(wsOut)
		{
			if(!::MultiByteToWideChar(CP_ACP, 0, pSrc, -1, wsOut, cwch))
			{
				if(ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
					return wsOut;
				::SysFreeString(wsOut);//must clean up
				wsOut = NULL;
			}
		}

	};

	return wsOut;
};
//-----------------------------------------------------------------------------
// Convert BSTR to char *
//-----------------------------------------------------------------------------
inline char* ConvertBSTRToString(BSTR pSrc)
{
	if(!pSrc) return NULL;

	DWORD cb,cwch = ::SysStringLen(pSrc);//convert even embeded NULL

	char *szOut = NULL;

	if(cb = ::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, NULL, 0, 0, 0))
	{
		szOut = new char[cb];
		if(szOut)
		{
			szOut[cb - 1]  = '\0';

			if(!::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, szOut, cb, 0, 0))
			{
				delete []szOut;//clean up if failed;
				szOut = NULL;
			}
		}
	}

	return szOut;
};
//-----------------------------------------------------------------------------
inline char* ConvertBSTRToString(const BSTR pSrc, char * szOut, DWORD cb)
{
	DWORD cwch = ::SysStringLen(pSrc);//convert even embeded NULL
	::WideCharToMultiByte(CP_ACP, 0, pSrc, cwch + 1, szOut, cb, 0, 0);
	return szOut;
}
//-----------------------------------------------------------------------------
#if 0
inline CString & strcpy (CString & strDst, const _variant_t & vSrc)
{
	assert (vSrc.vt == VT_BSTR);
	BSTR pSrc = (_bstr_t)vSrc;
	DWORD nSrc = ::SysStringLen(pSrc);//convert even embeded NULL
	DWORD nDst = ::WideCharToMultiByte(CP_ACP, 0, pSrc, nSrc + 1, NULL, 0, 0, 0);
	LPTSTR szDst = strDst.GetBufferSetLength (nDst + 1);
	::WideCharToMultiByte(CP_ACP, 0, pSrc, nSrc + 1, szDst, nDst, 0, 0);
	return strDst;
}
#endif
//-----------------------------------------------------------------------------
inline string String (const _variant_t & vSrc, short nCase=0)
{
//	assert (vSrc.vt == VT_BSTR);
	if (vSrc.vt != VT_BSTR)
		return string ("");
	char * szRes = ConvertBSTRToString ((_bstr_t)vSrc);
	if (nCase < 0)
		_strlwr (szRes);
	if (nCase > 0)
		_strupr (szRes);
	string strRes (szRes);
	delete [] szRes;
	return strRes;
}
//-----------------------------------------------------------------------------
inline string LString (const _variant_t & vSrc)
{
	return String (vSrc, -1);
}
//-----------------------------------------------------------------------------
inline string UString (const _variant_t & vSrc)
{
	return String (vSrc, +1);
}
//-----------------------------------------------------------------------------
inline string StripWhiteSpace (const _variant_t & vSrc)
{
//	assert (vSrc.vt == VT_BSTR);
	if (vSrc.vt != VT_BSTR)
		return string ("");
	char * szRes = ConvertBSTRToString ((_bstr_t)vSrc);
	char * e = szRes + strlen(szRes);
	char * r = szRes;
	char * w = szRes;
	for (; r < e; r++)
	{
		const bool bSpc = isspace(*r) != 0;
		char c = bSpc ? ' ' : *r;
		if (bSpc)
		if (w == szRes || isspace(w[-1]))
			continue;
		*(w++) = c;
	}
	*w = 0;
	while ((w--) > szRes && isspace(*w))
		*w = 0;
	string strRes (szRes);
	delete [] szRes;
	return strRes;
}
//-----------------------------------------------------------------------------
#pragma warning(pop)
#endif	// _INC_COMUTIL
