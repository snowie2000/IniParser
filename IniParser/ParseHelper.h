
#pragma once

#include <wtypes.h>
#include <tchar.h>
#include "strtod.h"

#pragma warning(disable:4786)
#include <string>
#include <map>

using namespace std;


#ifdef UNICODE
#define STLSTRING	wstring
#else
#define STLSTRING	string
#endif

//////////////////////////////////////////////////////////////////////////
// CParseHelper

class CParseHelper
{
private:
	CParseHelper();
public:
	virtual ~CParseHelper();

public:
	static BOOL IsStringLeadWith(LPCTSTR lpszString, LPCTSTR lpszLead);
	static BOOL IsUnicodeText(LPVOID pText);
	static BOOL IsUTF8Text(LPVOID pText);

	//Parse (T)
	static COLORREF ParseColor(LPCTSTR lpszColor);		//XXX XXX XXX
	static COLORREF ParseColor(LPCTSTR lpszColor, COLORREF clrDefault);		//XXX XXX XXX
	static int ParseInt(LPCTSTR lpszInt);		//XXX
	static int ParseInt(LPCTSTR lpszInt, int iDefault);		//XXX
	static INT32 ParseInt32(LPCTSTR lpszInt32);		//XXX
	static INT32 ParseInt32(LPCTSTR lpszInt32, INT32 iDefault);		//XXX
	static double ParseDouble(LPCTSTR lpszDouble);		//XXX.xx
	static double ParseDouble(LPCTSTR lpszDouble, double fDefault);		//XXX.xx
	//static BOOL ParseFont(LPCTSTR lpszFont, LOGFONT* lpLogFont);	//FontFaceName 12 Bold
	//static BOOL ParseFont(LPCTSTR lpszFont, LPTSTR lpszFaceNameBuf, DWORD cchBufLen, LPINT lpSize, LPBOOL lpBold, LPBOOL lpItalic);
	//static BOOL ParseMargins(LPCTSTR lpszMargins, LPRECT lpMargins);	//X,X,X,X
	//static BOOL ParseRect(LPCTSTR lpszMargins, LPRECT lpMargins);	//X,X,X,X
	static SIZE ParseSize(LPCTSTR lpszSize);	//X,X
	static POINT ParsePoint(LPCTSTR lpszPoint);	//X,X
	static BOOL ParseBool(LPCTSTR lpszBool);	//true or false
	static BOOL ParseBool(LPCTSTR lpszBool, BOOL bBoolDefault);	//true or false
	static LONG ParseResFileName(LPCTSTR lpszFileName, LPTSTR lpszBuf, DWORD cchBufSize);	//convert AA\BB.bmp to AA_BB_BMP

	static LPCTSTR GetNextPart(LPCTSTR lpszString, TCHAR chSeparator);
	static LPCTSTR GetNextPart(LPCTSTR lpszString);	//Separated by [Space] or ,
	static DWORD CopyFirstPart(LPTSTR lpszBuf, LPCTSTR lpszString, DWORD dwMaxLen);		//Copy the first part of the string, terminated by [Space] or ,
	static DWORD CopyFirstPart(LPTSTR lpszBuf, LPCTSTR lpszString, DWORD dwMaxLen, TCHAR chSeparator);		//Copy the first part of the string, terminated by [Space] or ,

public:
	static void SizeAbs(SIZE *lpSize);	//abs(size.cx), abs(size.cy)
	static void PointAbs(POINT *lpPoint);	//abs(point.x), abs(point.y)
	static BOOL IsEmptyMargins(LPCRECT lprcMargins);

protected:
#ifdef UNICODE
	struct _CNoCaseLess
	{
		bool operator()(const wstring& str1, const wstring& str2) const
		{	return _tcsicmp(str1.c_str(), str2.c_str()) < 0;	}
	};
	typedef map<wstring, int, _CNoCaseLess> TYPEDINTMAP;
	TYPEDINTMAP m_mapTypedInt;
#else
	struct _CNoCaseLess
	{
		bool operator()(const string& str1, const string& str2) const
		{	return _tcsicmp(str1.c_str(), str2.c_str()) < 0;	}
	};
	typedef map<string, int, _CNoCaseLess> TYPEDINTMAP;
	TYPEDINTMAP m_mapTypedInt;
#endif

};