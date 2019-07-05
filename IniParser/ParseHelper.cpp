
#include "ParseHelper.h"

//////////////////////////////////////////////////////////////////////
// CParseHelper
//////////////////////////////////////////////////////////////////////

CParseHelper::CParseHelper()
{
	
}

CParseHelper::~CParseHelper()
{

}

BOOL CParseHelper::IsStringLeadWith(LPCTSTR lpszString, LPCTSTR lpszLead)
{
	int nLenLead = lstrlen(lpszLead);
	int nLenString = lstrlen(lpszString);

	if(nLenLead > nLenString)
		return FALSE;

	LPTSTR pszLeadUpper = new TCHAR[nLenLead + 1];
	lstrcpyn(pszLeadUpper, lpszLead, nLenLead + 1);

	LPTSTR pszStringUpper = new TCHAR[nLenLead + 1];
	lstrcpyn(pszStringUpper, lpszString, nLenLead + 1);

	BOOL bResult = (lstrcmpi(pszLeadUpper, pszStringUpper) == 0);

	delete []pszStringUpper;
	delete []pszLeadUpper;

	return bResult;
}

BOOL CParseHelper::IsUnicodeText(LPVOID pText)
{
	unsigned char *pData = (unsigned char*)pText;
	if((unsigned char)pData[0] == 0xFF && (unsigned char)pData[1] == 0xFE)
		return TRUE;
	if((unsigned char)pData[1] == 0x00 && (unsigned char)pData[3] == 0x00)
		return TRUE;

	return FALSE;
}

BOOL CParseHelper::IsUTF8Text(LPVOID pText)
{
	unsigned char *pData = (unsigned char*)pText;
	if ((unsigned char)pData[0] == 0xEF && (unsigned char)pData[1] == 0xBB && (unsigned char)pData[2] == 0xBF)
		return TRUE;
	return FALSE;
}

LPCTSTR CParseHelper::GetNextPart(LPCTSTR lpszString)
{
	while(*lpszString != _T(' ') && *lpszString != _T(',') && *lpszString != _T('\0'))
		lpszString++;

	if(*lpszString != _T('\0'))
	{
		lpszString++;
	}
	return lpszString;
}

LPCTSTR CParseHelper::GetNextPart(LPCTSTR lpszString, TCHAR chSeparator)
{
	while(*lpszString != chSeparator && *lpszString != _T('\0'))
		lpszString++;

	if(*lpszString != _T('\0'))
	{
		lpszString++;
	}
	return lpszString;
}

DWORD CParseHelper::CopyFirstPart(LPTSTR lpszBuf, LPCTSTR lpszString, DWORD dwMaxLen)		//Copy the first part of the string, terminated by [Space] or ,
{
	DWORD dwCopyLen = 0;
	while(*(lpszString + dwCopyLen) != _T('\0')
		&& *(lpszString + dwCopyLen) != _T(' ')
		&& *(lpszString + dwCopyLen) != _T(','))
		dwCopyLen++;

	dwCopyLen++;	//for '\0';

	dwCopyLen = min(dwCopyLen, dwMaxLen);
	lstrcpyn(lpszBuf, lpszString, dwCopyLen);
	return lstrlen(lpszBuf);
}

COLORREF CParseHelper::ParseColor(LPCTSTR lpszColor)		//XXX XXX XXX
{
	LPCTSTR pszCurrentPtr = lpszColor;
	int iRed = _ttoi(pszCurrentPtr);
	pszCurrentPtr = GetNextPart(pszCurrentPtr);
	int iGreen = _ttoi(pszCurrentPtr);
	pszCurrentPtr = GetNextPart(pszCurrentPtr);
	int iBlue = _ttoi(pszCurrentPtr);

	return RGB(iRed, iGreen, iBlue);
}

COLORREF CParseHelper::ParseColor(LPCTSTR lpszColor, COLORREF clrDefault)		//XXX XXX XXX
{
	if(lpszColor[0] == _T('\0'))
		return clrDefault;

	return ParseColor(lpszColor);
}

int CParseHelper::ParseInt(LPCTSTR lpszInt)		//XXX
{
	return _ttoi(lpszInt);
}

int CParseHelper::ParseInt(LPCTSTR lpszInt, int iDefault)
{
	if(*lpszInt == _T('\0'))
		return iDefault;
	return _ttoi(lpszInt);
}

INT32 CParseHelper::ParseInt32(LPCTSTR lpszInt32)		//XXX
{
	return _ttoi(lpszInt32);
}

INT32 CParseHelper::ParseInt32(LPCTSTR lpszInt32, INT32 iDefault)
{
	if(*lpszInt32 == _T('\0'))
		return iDefault;
	return _ttoi(lpszInt32);
}

double CParseHelper::ParseDouble(LPCTSTR lpszDouble)
{	
	return wstrtod(lpszDouble, NULL);
}

double CParseHelper::ParseDouble(LPCTSTR lpszDouble, double fDefault)
{
	if (*lpszDouble == _T('\0'))
		return fDefault;

	return wstrtod(lpszDouble, NULL);
}


/*
BOOL CParseHelper::ParseFont(LPCTSTR lpszFont, LOGFONT* lpLogFont)	//FontFaceName 12 Bold
{
	memset(lpLogFont, 0, sizeof(LOGFONT));
	TCHAR szFaceName[32];
	BOOL bBold = FALSE;
	INT iSize = 0;
	BOOL bItalic = FALSE;
	BOOL bResult = ParseFont(lpszFont, szFaceName, 32, &iSize, &bBold, &bItalic);

	if(bResult)
	{
		lpLogFont->lfHeight = iSize;
		lpLogFont->lfWeight = bBold ? FW_BOLD : 0;
		lpLogFont->lfItalic = bItalic;
		lpLogFont->lfCharSet = DEFAULT_CHARSET;
		lstrcpyn(lpLogFont->lfFaceName, szFaceName, sizeof(lpLogFont->lfFaceName));
	}

	return bResult;
}

BOOL CParseHelper::ParseFont(LPCTSTR lpszFont, LPTSTR lpszFaceNameBuf, DWORD cchBufLen, LPINT lpSize, LPBOOL lpBold, LPBOOL lpItalic)
{
	LPCTSTR pszCurrentPtr = lpszFont;

	//Font FaceName
	CopyFirstPart(lpszFaceNameBuf, lpszFont, cchBufLen, _T(','));

	//FontSize
	pszCurrentPtr = GetNextPart(pszCurrentPtr, _T(','));
	*lpSize = _ttoi(pszCurrentPtr);
	*lpSize = -MulDiv(*lpSize, GetDeviceCaps(NULL, LOGPIXELSY), 72);

	//Bold or Italic
	pszCurrentPtr = GetNextPart(pszCurrentPtr, _T(','));
	while(*pszCurrentPtr && *pszCurrentPtr == _T(' '))
		pszCurrentPtr++;

	if(IsStringLeadWith(pszCurrentPtr, _T("Bold")))
		*lpBold = TRUE;
	else if(IsStringLeadWith(pszCurrentPtr, _T("Italic")))
		*lpItalic = TRUE;

	return TRUE;

}*/

/*
BOOL CParseHelper::ParseMargins(LPCTSTR lpszMargins, LPRECT lpMargins)	//X,X,X,X
{
	LPCTSTR pszCurrentPtr = lpszMargins;
	int iLeft = _ttoi(pszCurrentPtr);
	pszCurrentPtr = GetNextPart(pszCurrentPtr, _T(','));
	int iRight = _ttoi(pszCurrentPtr);
	pszCurrentPtr = GetNextPart(pszCurrentPtr, _T(','));
	int iTop = _ttoi(pszCurrentPtr);
	pszCurrentPtr = GetNextPart(pszCurrentPtr, _T(','));
	int iBottom = _ttoi(pszCurrentPtr);

	return ::SetRect(lpMargins, iLeft, iTop, iRight, iBottom);
}

BOOL CParseHelper::ParseRect(LPCTSTR lpszMargins, LPRECT lpMargins)	//X,X,X,X
{
	return ParseMargins(lpszMargins, lpMargins);
}*/

SIZE CParseHelper::ParseSize(LPCTSTR lpszSize)	//X,X
{
	LPCTSTR pszCurrentPtr = lpszSize;
	SIZE size;
	size.cx = _ttoi(pszCurrentPtr);
	pszCurrentPtr = GetNextPart(pszCurrentPtr, _T(','));
	size.cy = _ttoi(pszCurrentPtr);

	return size;
}

BOOL CParseHelper::ParseBool(LPCTSTR lpszBool)	//true or false
{
	while(*lpszBool && *lpszBool == _T(' '))
		lpszBool++;

	if(IsStringLeadWith(lpszBool, _T("True")))
		return TRUE;
	
	//else
	return FALSE;
}

BOOL CParseHelper::ParseBool(LPCTSTR lpszBool, BOOL bBoolDefault)	//true or false
{
	while(*lpszBool && *lpszBool == _T(' '))
		lpszBool++;

	if(IsStringLeadWith(lpszBool, _T("True")))
		return TRUE;
	else if(IsStringLeadWith(lpszBool, _T("False")))
		return FALSE;
	
	//else
	return bBoolDefault;
}

LONG CParseHelper::ParseResFileName(LPCTSTR lpszFileName, LPTSTR lpszBuf, DWORD cchBufSize)
{
	LPCTSTR pszOriginal = lpszBuf;
	lstrcpyn(lpszBuf, lpszFileName, cchBufSize);
	while(*lpszBuf)
	{
		if(*lpszBuf == _T('\\') || *lpszBuf == _T('.'))
			*lpszBuf = _T('_');
		
		lpszBuf++;
	}
	return lstrlen(pszOriginal);
}


DWORD CParseHelper::CopyFirstPart(LPTSTR lpszBuf, LPCTSTR lpszString, DWORD dwMaxLen, TCHAR chSeparator)
{
	DWORD dwCopyLen = 0;
	while(*(lpszString + dwCopyLen) != _T('\0')
		&& *(lpszString + dwCopyLen) != chSeparator)
		dwCopyLen++;

	dwCopyLen++;	//for '\0';

	dwCopyLen = min(dwCopyLen, dwMaxLen);
	lstrcpyn(lpszBuf, lpszString, dwCopyLen);
	return lstrlen(lpszBuf);
}

POINT CParseHelper::ParsePoint(LPCTSTR lpszPoint)	//X,X
{
	SIZE size = ParseSize(lpszPoint);
	POINT point = {size.cx, size.cy};
	return point;
}

void CParseHelper::SizeAbs(SIZE *lpSize)
{
	lpSize->cx = abs(lpSize->cx);
	lpSize->cy = abs(lpSize->cy);
}

void CParseHelper::PointAbs(POINT *lpPoint)
{
	lpPoint->x = abs(lpPoint->x);
	lpPoint->y = abs(lpPoint->y);
}

BOOL CParseHelper::IsEmptyMargins(LPCRECT lprcMargins)
{
	if(lprcMargins == NULL)
		return TRUE;
	return (lprcMargins->left == 0 && lprcMargins->right == 0 && lprcMargins->top == 0 && lprcMargins->bottom == 0);
}
