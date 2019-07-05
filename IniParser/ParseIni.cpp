
#include "ParseIni.h"
#include "utf8conv.h"

//////////////////////////////////////////////////////////////////////////
// CIniPartValue

CIniPartValue::CIniPartValue()
{
	m_pPart = NULL;
}

CIniPartValue::CIniPartValue(LPCTSTR lpszValue)
{
	m_pPart = NULL;
	m_strValue = lpszValue;
}

CIniPartValue::CIniPartValue(const CIniPartValue& val)
{
	m_strValue = val.m_strValue;
	m_pPart = val.m_pPart;
}

CIniPartValue::~CIniPartValue()
{

}

CIniPartValue::operator LPCTSTR()
{
	return m_strValue.c_str();
}

CIniPartValue& CIniPartValue::operator =(const CIniPartValue& val)
{
	m_pPart = val.m_pPart;
	m_strValue = val.m_strValue;
	return *this;
}

void CIniPartValue::SetValue(LPCTSTR lpszValue)
{
	m_strValue = lpszValue;
}

LPCTSTR CIniPartValue::GetValueRef()
{
	return m_strValue.c_str();
}

int CIniPartValue::ToInt()
{
	return CParseHelper::ParseInt(m_strValue.c_str());
}

int CIniPartValue::ToInt(int iDefault)
{
	return CParseHelper::ParseInt(m_strValue.c_str(), iDefault);
}

INT32 CIniPartValue::ToInt32()
{
	return CParseHelper::ParseInt32(m_strValue.c_str());
}

INT32 CIniPartValue::ToInt32(INT32 iDefault)
{
	return CParseHelper::ParseInt32(m_strValue.c_str(), iDefault);
}

double CIniPartValue::ToDouble()
{
	return CParseHelper::ParseDouble(m_strValue.c_str());
}

double CIniPartValue::ToDouble(double fDefault)
{
	return CParseHelper::ParseDouble(m_strValue.c_str(), fDefault);
}

SIZE CIniPartValue::ToSize()
{
	return CParseHelper::ParseSize(m_strValue.c_str());
}

POINT CIniPartValue::ToPoint()
{
	return CParseHelper::ParsePoint(m_strValue.c_str());
}

LPCTSTR CIniPartValue::ToString()
{
	return m_strValue.c_str();
}

COLORREF CIniPartValue::ToColor()
{
	return CParseHelper::ParseColor(m_strValue.c_str());
}

COLORREF CIniPartValue::ToColor(COLORREF clrDefault)
{
	return CParseHelper::ParseColor(m_strValue.c_str(), clrDefault);
}

BOOL CIniPartValue::ToBool()
{
	return CParseHelper::ParseBool(m_strValue.c_str());
}

BOOL CIniPartValue::ToBool(BOOL bBoolDefault)
{
	return CParseHelper::ParseBool(m_strValue.c_str(), bBoolDefault);
}

LPCTSTR CIniPartValue::ToResFileNameRef()
{
	m_strResFileNameTemp.resize(MAX_PATH);
	CParseHelper::ParseResFileName(m_strValue.c_str(), (LPTSTR)m_strResFileNameTemp.c_str(), MAX_PATH);
	return m_strResFileNameTemp.c_str();
}

/*LPLOGFONT CIniPartValue::ToFontRef()
{
	ToFont(&m_LogFontTemp);
	return &m_LogFontTemp;
}

LPCRECT CIniPartValue::ToRectRef()
{
	ToRect(&m_rcRectTemp);
	return &m_rcRectTemp;
}

LPCRECT CIniPartValue::ToMarginsRef()
{
	ToMargins(&m_rcMarginsTemp);
	return &m_rcMarginsTemp;
}


BOOL CIniPartValue::ToRect(LPRECT lprcRect)
{
	return CParseHelper::ParseRect(m_strValue.c_str(), lprcRect);
}


BOOL CIniPartValue::ToMargins(LPRECT lprcMargins)
{
	return CParseHelper::ParseMargins(m_strValue.c_str(), lprcMargins);

}

BOOL CIniPartValue::ToFont(LPLOGFONT lpLogFont)
{
	return CParseHelper::ParseFont(m_strValue.c_str(), lpLogFont);
}*/

/*
BOOL CIniPartValue::ToFont(LPTSTR lpszFaceNameBuf, DWORD cchBufLen, LPINT lpSize, LPBOOL lpBold, LPBOOL lpItalic)
{
	return CParseHelper::ParseFont(m_strValue.c_str(), lpszFaceNameBuf, cchBufLen, lpSize, lpBold, lpItalic);
}*/

BOOL CIniPartValue::ToResFileName(LPTSTR lpszBuf, LONG cchBufLen)
{
	return CParseHelper::ParseResFileName(m_strValue.c_str(), lpszBuf, cchBufLen);
}

//////////////////////////////////////////////////////////////////////////
// CIniPart

CIniPart::CIniPart(LPCTSTR lpszPartName, CParseIni *pProfile, LONG iPartIndex)
{
	m_strPartName = lpszPartName;
	m_pProfile = pProfile;
	m_iPartIndex = iPartIndex;
}

CIniPart::~CIniPart()
{
	Clear();
}

void CIniPart::Clear()
{
	PARTVALUEMAP::iterator it = m_mapPartValues.begin();
	for(; it!=m_mapPartValues.end(); it++)
		delete it->second;
	m_mapPartValues.clear();
}

LONG CIniPart::GetPartIndex()
{
	return m_iPartIndex;
}

LPCTSTR CIniPart::GetPartName()
{
	return m_strPartName.c_str();
}

BOOL CIniPart::WriteFile(HANDLE hFile)
{
	TCHAR szCRLF[2] = {0x0D, 0x0A};
	TCHAR szEqual[3] = {_T(' '), _T('='), _T(' ')};

	POSITION pos = GetFirstKeyValuePos();

	LPCTSTR lpszKey = NULL;
	LPCTSTR lpszValue = NULL;
	DWORD dwWritten = 0;
	while(GetNextKeyValue(pos, lpszKey, lpszValue))
	{
		::WriteFile(hFile, lpszKey, _tcslen(lpszKey) * sizeof(TCHAR), &dwWritten, NULL);
		::WriteFile(hFile, szEqual, sizeof(szEqual), &dwWritten, NULL);
		::WriteFile(hFile, lpszValue, _tcslen(lpszValue) * sizeof(TCHAR), &dwWritten, NULL);
		::WriteFile(hFile, szCRLF, sizeof(szCRLF), &dwWritten, NULL);
	}
	return ::WriteFile(hFile, szCRLF, sizeof(szCRLF), &dwWritten, NULL);
}

void CIniPart::SetValue(LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	PARTVALUEMAP::iterator it = m_mapPartValues.find(lpszKey);
	if(it != m_mapPartValues.end())
		it->second->SetValue(lpszValue);
	else
	{
		CIniPartValue *pValue = new CIniPartValue(lpszValue);
		pValue->m_pPart = this;
		m_mapPartValues[lpszKey] = pValue;
	}
}

LONG CIniPart::GetValueString(LPCTSTR lpszKeyName, LPTSTR lpszBuf, LONG cchBufLen, LPCTSTR lpszDefault /* = _T("") */)
{
	PARTVALUEMAP::iterator it = m_mapPartValues.find(lpszKeyName);
	if(it == m_mapPartValues.end())
	{
		lstrcpyn(lpszBuf, lpszDefault, cchBufLen);
		return lstrlen(lpszBuf);
	}

	lstrcpyn(lpszBuf, it->second->GetValueRef(), cchBufLen);
	return lstrlen(lpszBuf);
}

LPCTSTR CIniPart::GetValueString(LPCTSTR lpszKeyName)
{
	PARTVALUEMAP::iterator it = m_mapPartValues.find(lpszKeyName);
	if(it == m_mapPartValues.end())
		return _T("");

	return it->second->GetValueRef();
}

LONG CIniPart::GetKeyCount()
{
	return m_mapPartValues.size();
}

CIniPart::POSITION CIniPart::GetFirstKeyValuePos()
{
	return (POSITION)m_mapPartValues.begin();
}

BOOL CIniPart::GetNextKeyValue(POSITION &pos, LPCTSTR &lpszrKey, LPCTSTR &lpszrValue)
{
	PARTVALUEMAP::iterator it = pos;
	if(it == m_mapPartValues.end())
		return FALSE;

	lpszrKey = it->first.c_str();
	lpszrValue = it->second->GetValueRef();

	pos++;
	return TRUE;
}

BOOL CIniPart::IsValueExists(LPCTSTR lpszKey)
{
	PARTVALUEMAP::iterator it = m_mapPartValues.find(lpszKey);
	if(it == m_mapPartValues.end())
		return FALSE;

	return TRUE;
}

BOOL CIniPart::DeleteKey(LPCTSTR lpszKeyName)
{

	PARTVALUEMAP::iterator it = m_mapPartValues.find(lpszKeyName);
	if(it == m_mapPartValues.end())
		return FALSE;

	delete (CIniPartValue*)it->second;
	m_mapPartValues.erase(it);

	return TRUE;
}

CIniPartValue& CIniPart::operator[](LPCTSTR lpszKey)
{
	PARTVALUEMAP::iterator it = m_mapPartValues.find(lpszKey);
	if(it == m_mapPartValues.end())
		return m_valEmpty;

	return *((CIniPartValue*)it->second);
}

CIniPart::operator LPCTSTR() {
	if (m_strFullText.length() > 0)
		return m_strFullText.c_str();

	PARTVALUEMAP::iterator it = m_mapPartValues.begin();
	while (it != m_mapPartValues.end()) {
		m_strFullText.append(it->first);
		if (*(LPCTSTR)*(it->second) != L'\0') {
			m_strFullText.append(L"=");
			m_strFullText.append((LPCTSTR)*(it->second));
		}
		m_strFullText.append(L"\1");
		++it;
	}
	m_strFullText.append(L"\1");
	for (int i = 0; i < m_strFullText.length(); ++i) {
		if (m_strFullText[i] == L'\1')
			m_strFullText[i] = L'\0';
	}
	return m_strFullText.c_str();
}

LONG CIniPart::GetValueInt(LPCTSTR lpszKeyName)
{
	return CParseHelper::ParseInt(GetValueString(lpszKeyName));
}

SIZE CIniPart::GetValueSize(LPCTSTR lpszKeyName)
{
	return CParseHelper::ParseSize(GetValueString(lpszKeyName));
}

POINT CIniPart::GetValuePoint(LPCTSTR lpszKeyName)
{
	return CParseHelper::ParsePoint(GetValueString(lpszKeyName));
}

/*
BOOL CIniPart::GetValueRect(LPCTSTR lpszKeyName, LPRECT lprcRect)
{
	return CParseHelper::ParseRect(GetValueString(lpszKeyName), lprcRect);
}

BOOL CIniPart::GetValueMargins(LPCTSTR lpszKeyName, LPRECT lprcRect)
{
	return CParseHelper::ParseMargins(GetValueString(lpszKeyName), lprcRect);
}


BOOL CIniPart::GetValueFont(LPCTSTR lpszKeyName, LPLOGFONT lpLogFont)
{
	return CParseHelper::ParseFont(GetValueString(lpszKeyName), lpLogFont);
}*/

COLORREF CIniPart::GetValueColor(LPCTSTR lpszKeyName)
{
	return CParseHelper::ParseColor(GetValueString(lpszKeyName));
}

LONG CIniPart::GetValueResFileName(LPCTSTR lpszKeyName, LPTSTR lpszBuf, LONG cchBufLen)
{
	return CParseHelper::ParseResFileName(GetValueString(lpszKeyName), lpszBuf, cchBufLen);
}

/*
BOOL CIniPart::GetValueFont(LPCTSTR lpszKeyName, LPTSTR lpszFaceNameBuf, DWORD cchBufLen, LPINT lpSize, LPBOOL lpBold, LPBOOL lpItalic)
{
	return CParseHelper::ParseFont(GetValueString(lpszKeyName), lpszFaceNameBuf, cchBufLen, lpSize, lpBold, lpItalic);
}*/

BOOL CIniPart::GetValueBool(LPCTSTR lpszKeyName)
{
	return CParseHelper::ParseBool(GetValueString(lpszKeyName));
}

//////////////////////////////////////////////////////////////////////////
// CParseIni

CParseIni::CParseIni()
{
}

CParseIni::~CParseIni()
{
	Clear();
}

void CParseIni::Clear()
{
	INIPARTMAP::iterator it = m_mapIniParts.begin();
	for(; it!=m_mapIniParts.end(); it++)
		delete (CIniPart*)it->second;

	m_mapIniParts.clear();
}

BOOL CParseIni::LoadFromFile(LPCTSTR lpszFilePathName)
{
	char* pData = NULL;
	HANDLE hFile = NULL;

	hFile = ::CreateFile(lpszFilePathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD dwFileSize, dwFileSizeHigh;
	dwFileSize = ::GetFileSize(hFile, &dwFileSizeHigh);

	DWORD dwRead = 0;
	pData = new char[dwFileSize + 2];
	::ReadFile(hFile, pData, dwFileSize, &dwRead, NULL);
	pData[dwFileSize] = pData[dwFileSize + 1] = 0;

	BOOL bResult = FALSE;
#ifdef UNICODE
	if(dwFileSize > 1 && CParseHelper::IsUnicodeText(pData))
	{
		bResult = LoadFromString(((LPCTSTR)pData) + 1);
	}
	else if (dwFileSize > 2 && CParseHelper::IsUTF8Text(pData))
	{
		string utf8txt = (LPCSTR)pData + 3;
		wstring unicodetxt = win32::Utf8ToUtf16(utf8txt);
		bResult = LoadFromString(unicodetxt.c_str());
	}
	else
	{
		bResult = LoadFromStringA((LPCSTR)pData);
	}
#else
	if(dwFileSize > 1 && CParseHelper::IsUnicodeText(pData))
	{
		bResult = LoadFromStringW(((LPCWSTR)pData) + 1);
	}
	else
	{
		bResult = LoadFromString((LPCTSTR)pData);
	}
#endif

	delete []pData;
	
	::CloseHandle(hFile);

	return bResult;
}

BOOL CParseIni::WriteToFile(LPCTSTR lpszFilePathName)
{
	HANDLE hFile = NULL;

	hFile = ::CreateFile(lpszFilePathName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	TCHAR szCRLF[2] = {0x0D, 0x0A};
	TCHAR szSurround[2] = {_T('['), _T(']')};

	DWORD dwWritten = 0;
#ifdef UNICODE
	TCHAR szUNICODE[2] = {0xFF, 0xFE};
	if(!::WriteFile(hFile, szCRLF, sizeof(szCRLF), &dwWritten, NULL))
	{
		::CloseHandle(hFile);
		return FALSE;
	}
#endif

	CIniPart *pRootPart = FindPart(INI_ROOTSECTION_NAME);
	if(pRootPart)
		pRootPart->WriteFile(hFile);

	POSITION pos = GetFirstPartPos();
	CIniPart *pPart = NULL;
	while(pPart = GetNextPart(pos))
	{
		if(pPart == pRootPart)
			continue;

		//Write Section Name
		::WriteFile(hFile, szSurround, sizeof(TCHAR), &dwWritten, NULL);
		::WriteFile(hFile, pPart->GetPartName(), _tcslen(pPart->GetPartName()) * sizeof(TCHAR), &dwWritten, NULL);
		::WriteFile(hFile, szSurround + 1, sizeof(TCHAR), &dwWritten, NULL);
		::WriteFile(hFile, szCRLF, sizeof(szCRLF), &dwWritten, NULL);
		pPart->WriteFile(hFile);
	}

	::CloseHandle(hFile);
	return TRUE;
}

CParseIni::POSITION CParseIni::GetFirstPartPos()
{
	return m_mapIniParts.begin();
}

CIniPart* CParseIni::GetNextPart(POSITION &pos)
{
	CIniPart *pPart = NULL;
	INIPARTMAP::iterator it = pos;
	if (it != m_mapIniParts.end())
	{
		pPart = it->second;
		pos++;
	}
	
	return pPart;
}


BOOL CParseIni::LoadFromStringA(LPCSTR lpszString)
{
#ifdef UNICODE
	int nLen = strlen(lpszString);
	WCHAR *pwBuf = new WCHAR[nLen * 2 + 2];

	MultiByteToWideChar(CP_ACP, 0, lpszString, -1, pwBuf, nLen * 2 + 2);
	BOOL bResult = LoadFromStringW(pwBuf);
	delete []pwBuf;

	return bResult;
#else
	return LoadFromString(lpszString);
#endif
}

BOOL CParseIni::LoadFromStringW(LPCWSTR lpszString)
{
#ifdef UNICODE
	return LoadFromString(lpszString);
#else
	int nLen = wcslen(lpszString);
	CHAR *pwBuf = new CHAR[nLen * 2 + 2];

	WideCharToMultiByte(CP_ACP, 0, lpszString, -1, pwBuf, nLen * 2 + 2, NULL, NULL);
	BOOL bResult = LoadFromStringA(pwBuf);
	delete []pwBuf;

	return bResult;
#endif
}

LONG CParseIni::LoadFromString(LPCTSTR lpszString, UINT cchKeyBufferLen /* = 256 */, UINT cchValueBufferLen /* = 1024 */)
{
	Clear();

	LONG nCurrentPartIndex = 0;

	// Parse the whole string, using DFA method

	LPTSTR szPart1 = new TCHAR[cchKeyBufferLen];
	LPTSTR szPart2 = new TCHAR[cchValueBufferLen];
	int iPos1 = 0;
	int iPos2 = 0;
	int iStatus = INI_STATUS_START;

#ifndef UNICODE
	int iPreStatus = INI_STATUS_START;
#endif

	CIniPart *pCurrentPart = new CIniPart(INI_ROOTSECTION_NAME, this, nCurrentPartIndex++);
	m_mapIniParts[INI_ROOTSECTION_NAME] = pCurrentPart;

	while(*lpszString)
	{
		switch(iStatus)
		{
		case INI_STATUS_START:	//Start status
#ifndef UNICODE
			if(*lpszString & 0x80)
			{
				iPreStatus = INI_STATUS_KEY;
				iStatus = INI_STATUS_DBCS_FIRSTPART;
				iPos1 = 0;
				szPart1[iPos1++] = *lpszString;
			}
			else
#endif
			if(*lpszString == _T(';'))
			{
				iStatus = INI_STATUS_COMMENT;
			}
			else if(*lpszString == _T('['))
			{
				iStatus = INI_STATUS_SECTION;
				iPos1 = 0;
			}
			else if(*lpszString > 0x20 && *lpszString != _T('='))
			{
				iStatus = INI_STATUS_KEY;
				iPos1 = 0;
				szPart1[iPos1++] = *lpszString;
			}
			break;
		case INI_STATUS_SECTION:	//In section name
#ifndef UNICODE
			if(*lpszString & 0x80)
			{
				iPreStatus = iStatus;
				iStatus = INI_STATUS_DBCS_FIRSTPART;
				szPart1[iPos1++] = *lpszString;
			}
			else
#endif
			if(*lpszString == _T(']'))
			{
				szPart1[iPos1] = _T('\0');
				INIPARTMAP::iterator it = m_mapIniParts.find(szPart1);
				if(it == m_mapIniParts.end())
				{
					pCurrentPart = new CIniPart(szPart1, this, nCurrentPartIndex++);
					m_mapIniParts[szPart1] = pCurrentPart;
				}
				else
					pCurrentPart = it->second;
				
				iStatus = INI_STATUS_START;
			}
			else
			{
				szPart1[iPos1++] = *lpszString;
			}
			break;
		case INI_STATUS_KEY:	//In Key name
#ifndef UNICODE
			if(*lpszString & 0x80)
			{
				iPreStatus = iStatus;
				iStatus = INI_STATUS_DBCS_FIRSTPART;
				szPart1[iPos1++] = *lpszString;
			}
			else
#endif
			if(*lpszString == _T('\r') || *lpszString == _T('\n'))
			{
				//allow keys without value
				szPart1[iPos1--] = _T('\0');
				while (iPos1 > 0 && szPart1[iPos1] <= 0x20)
					szPart1[iPos1--] = _T('\0');
				pCurrentPart->SetValue(szPart1, L"");
				//just store it
				iStatus = INI_STATUS_START;
			}
			else if(*lpszString != _T('='))
			{
				szPart1[iPos1++] = *lpszString;
			}
			else if(*lpszString == _T('='))
			{
				szPart1[iPos1] = _T('\0');

				//TrimRight
#ifndef UNICODE
				iPos1--;
				while(szPart1[iPos1] <= 0x20 && (iPos1 - 1 > 0) && (szPart1[iPos1 - 1] & 0x80) == 0)
					szPart1[iPos1--] = _T('\0');
#else
				iPos1--;
				while(szPart1[iPos1] <= 0x20)
					szPart1[iPos1--] = _T('\0');
#endif

				iStatus = INI_STATUS_PREVALUE;
			}
			break;
		case INI_STATUS_PREVALUE:		//after '=', before Value
#ifndef UNICODE
			if(*lpszString & 0x80)
			{
				iPreStatus = INI_STATUS_VALUE;
				iStatus = INI_STATUS_DBCS_FIRSTPART;
				iPos2 = 0;
				szPart2[iPos2++] = *lpszString;
			}
			else
#endif
			if(*lpszString == _T('\r') || *lpszString == _T('\n'))
			{
				iStatus = INI_STATUS_START;
			}
			if(*lpszString > 0x20)
			{
				iStatus = INI_STATUS_VALUE;
				iPos2 = 0;
				szPart2[iPos2++] = *lpszString;
			}
			break;
		case INI_STATUS_VALUE:	//In value
#ifndef UNICODE
			if(*lpszString & 0x80)
			{
				iPreStatus = iStatus;
				iStatus = INI_STATUS_DBCS_FIRSTPART;
				szPart2[iPos2++] = *lpszString;
			}
			else
#endif
			if(*lpszString == _T('\r') || *lpszString == _T('\n'))
			{
				szPart2[iPos2] = _T('\0');

				//TrimRight
#ifndef UNICODE
				iPos2--;
				while(szPart2[iPos2] <= 0x20 && (iPos2 - 1 > 0) && (szPart2[iPos2 - 1] & 0x80) == 0)
					szPart2[iPos2--] = _T('\0');
#else
				iPos2--;
				while(szPart2[iPos2] <= 0x20)
					szPart2[iPos2--] = _T('\0');
#endif

				pCurrentPart->SetValue(szPart1, szPart2);
				iStatus = INI_STATUS_START;
			}
			else if(*lpszString == _T(';'))
			{
				szPart2[iPos2] = _T('\0');
				pCurrentPart->SetValue(szPart1, szPart2);
				iStatus = INI_STATUS_COMMENT;
			}
			else if(*lpszString >= 0x20)
			{
				szPart2[iPos2++] = *lpszString;
			}
			break;
		case INI_STATUS_COMMENT:	//In Comment
#ifndef UNICODE
			if(*lpszString & 0x80)
			{
				iPreStatus = iStatus;
				iStatus = INI_STATUS_DBCS_FIRSTPART;
				szPart2[iPos2++] = *lpszString;
			}
			else
#endif
			if(*lpszString == _T('\r') || *lpszString == _T('\n'))
			{
				iStatus = INI_STATUS_START;
			}
			break;
#ifndef UNICODE
		case INI_STATUS_DBCS_FIRSTPART:	//In First part of DBCS
			iStatus = iPreStatus;
			if(iPreStatus < INI_STATUS_PREVALUE)
				szPart1[iPos1++] = *lpszString;
			else
				szPart2[iPos2++] = *lpszString;
			break;
#endif
		}
		lpszString++;
	}

	if (INI_STATUS_KEY == iStatus && iPos1 > 0) {
		szPart1[iPos1--] = _T('\0');
		while (iPos1 > 0 && szPart1[iPos1] <= 0x20)
			szPart1[iPos1--] = _T('\0');
		if (iPos1 > 0)
			pCurrentPart->SetValue(szPart1, L"");
	}

	if (INI_STATUS_VALUE == iStatus && iPos2 > 0) {
		szPart2[iPos2] = _T('\0');
#ifndef UNICODE
		iPos2--;
		while (szPart2[iPos2] <= 0x20 && (iPos2 - 1 > 0) && (szPart2[iPos2 - 1] & 0x80) == 0)
			szPart2[iPos2--] = _T('\0');
#else
		iPos2--;
		while (szPart2[iPos2] <= 0x20)
			szPart2[iPos2--] = _T('\0');
#endif

		pCurrentPart->SetValue(szPart1, szPart2);
	}

	delete []szPart1;
	delete []szPart2;

	return m_mapIniParts.size();
}

LONG CParseIni::GetPartNamesLeadWith(LPCTSTR lpszLead, STRINGVECTOR *pStringVector, BOOL bClear)
{
	if(pStringVector == NULL || lpszLead == NULL)
		return 0;

	if(bClear)
		pStringVector->clear();

	map <LONG, STLSTRING> mapParts;

	POSITION pos = GetFirstPartPos();
	CIniPart *pPart = NULL;
	
	LONG nAdded = 0;
	while((pPart = GetNextPart(pos)) != NULL)
	{
		if(CParseHelper::IsStringLeadWith(pPart->GetPartName(), lpszLead))
		{
			mapParts[pPart->GetPartIndex()] = pPart->GetPartName();
		}
	}

	map <LONG, STLSTRING>::iterator it = mapParts.begin();
	for(;it!=mapParts.end();it++)
	{
		pStringVector->push_back(it->second);
		nAdded++;
	}
	return nAdded;
}

LONG CParseIni::GetPartCount()
{
	return m_mapIniParts.size();
}

CIniPart* CParseIni::FindPart(LPCTSTR lpszPartName)
{
	int nLen = 0;
	LPCTSTR pszPartName = NULL;
	if(lpszPartName != NULL)
		pszPartName = lpszPartName;
	else	//Root Part
		pszPartName = INI_ROOTSECTION_NAME;

	INIPARTMAP::iterator it = m_mapIniParts.find(pszPartName);

	CIniPart *pPart = NULL;
	if(it == m_mapIniParts.end())
		pPart = CreatePart(pszPartName);
	else
		pPart = it->second;

	return pPart;
}

CIniPart* CParseIni::CreatePart(LPCTSTR lpszPartName)
{
	INIPARTMAP::iterator it = m_mapIniParts.find(lpszPartName);
	if(it != m_mapIniParts.end())
		return it->second;

	CIniPart *pPart = new CIniPart(lpszPartName, this, GetPartCount());
	m_mapIniParts[lpszPartName] = pPart;

	return pPart;
}

CIniPart& CParseIni::operator[](LPCTSTR lpszPartName)
{
	CIniPart *pPart = FindPart(lpszPartName);
	return *pPart;
}

BOOL CParseIni::IsPartExists(LPCTSTR lpszPartName)
{
	int nLen = 0;
	LPCTSTR pszPartName = NULL;
	if(lpszPartName != NULL)
		pszPartName = lpszPartName;
	else	//Root Part
		pszPartName = INI_ROOTSECTION_NAME;

	INIPARTMAP::iterator it = m_mapIniParts.find(pszPartName);

	if(it == m_mapIniParts.end())
		return FALSE;

	return TRUE;
}
