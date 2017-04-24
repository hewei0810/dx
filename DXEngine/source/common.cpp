#include "stdafx.h"
#include "common.h"

CStdString::CStdString()
: m_psz(m_szBuf)
{
	m_szBuf[0] = '\0';
}

CStdString::CStdString(LPCTSTR psz, int nLength)
: m_psz(m_szBuf)
{
	Assign(psz, nLength);
}

CStdString::CStdString(const CStdString& rhs)
: m_psz(m_szBuf)
{
	Assign(rhs.GetData());
}

CStdString::~CStdString()
{
	Empty();
}

void CStdString::Empty()
{
	if (m_psz != m_szBuf)
		free(m_psz);
	m_psz = m_szBuf;
	m_szBuf[0] = '\0';
}

bool CStdString::IsEmpty() const
{
	return m_psz[0] == '\0';
}

void CStdString::Append(LPCTSTR psz)
{
	int nNewLength = GetLength() + _tcslen(psz);
	if (nNewLength >= MAX_LOCAL_STRING_LEN)
	{
		if (m_psz == m_szBuf)
		{
			m_psz = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
			_tcscpy(m_psz, m_szBuf);	
		}
		else
		{
			m_psz = static_cast<LPTSTR>(realloc(m_psz, (nNewLength + 1) * sizeof(TCHAR)));
		}
	}

	_tcscat(m_psz, psz);
}

void CStdString::Assign(LPCTSTR psz, int nLength)
{
	int nNewLength = nLength < 0 ? _tcslen(psz) : nLength;
	if (nNewLength >= MAX_LOCAL_STRING_LEN)
	{
		if (m_psz == m_szBuf)
		{
			m_psz = static_cast<LPTSTR>(malloc((nNewLength + 1) * sizeof(TCHAR)));
		}
		else if(nNewLength > GetLength())
		{
			m_psz = static_cast<LPTSTR>(realloc(m_psz, (nNewLength + 1) * sizeof(TCHAR))); 
		}
	}
	memcpy(m_psz, psz, nNewLength);
	m_psz[nNewLength] = '\0';
}

int CStdString::Format(LPCTSTR pszFmt, ...)
{
	TCHAR szBuf[1024] = {0};
	va_list ap;
	va_start(ap, pszFmt);
	int nRet = wvsprintf(szBuf, pszFmt, ap);
	va_end(ap);
	Assign(szBuf);
	return nRet;
}

void CStdString::MakeUpper()
{
	_tcsupr(m_psz);
}

void CStdString::MakeLower()
{
	_tcslwr(m_psz);
}

CStdString CStdString::Left(int nLength) const
{
	return CStdString(m_psz, nLength); 
}

CStdString CStdString::Right(int nLength) const
{
	return CStdString(m_psz + GetLength() - nLength, nLength);
}

CStdString CStdString::Mid(int nPos, int nLength) const
{
	return CStdString(m_psz + nPos, nLength);
}

int CStdString::Find(TCHAR ch, int nPos /* = 0 */) const
{
	LPCTSTR p = _tcschr(m_psz + nPos, ch);
	if (p == NULL)
		return -1;
	return p-m_psz;
}

int CStdString::Find(LPCTSTR psz, int nPos /* = 0 */) const
{
	LPCTSTR p = _tcsstr(m_psz + nPos, psz);
	if (p == NULL)
		return -1;
	return p-m_psz;
}

int CStdString::RFind(TCHAR ch) const
{
	LPCTSTR p = _tcsrchr(m_psz, ch);
	if (p == NULL)
		return -1;
	return p-m_psz;
}

TCHAR CStdString::operator [](int nIndex) const
{
	return m_psz[nIndex];
}

const CStdString& CStdString::operator =(LPCTSTR psz)
{
	Assign(psz);
	return *this;
}

const CStdString& CStdString::operator =(const CStdString& rhs)
{
	Assign(rhs.GetData());
	return *this;
}

const CStdString& CStdString::operator +=(LPCTSTR psz)
{
	Append(psz);
	return *this;
}

const CStdString& CStdString::operator +=(const CStdString& rhs)
{
	Append(rhs.GetData());
	return *this;
}

const CStdString CStdString::operator +(LPCTSTR psz)
{
	CStdString strTmp(*this);
	strTmp.Append(psz);
	return strTmp;
}

const CStdString CStdString::operator +(const CStdString& rhs)
{
	CStdString strTmp(*this);
	strTmp.Append(rhs.GetData());
	return strTmp;
}

bool CStdString::operator == (LPCTSTR psz) const { return (_tcscmp(m_psz, psz) == 0); }
bool CStdString::operator != (LPCTSTR psz) const { return (_tcscmp(m_psz, psz) != 0); }
bool CStdString::operator <= (LPCTSTR psz) const { return (_tcscmp(m_psz, psz) <= 0); }
bool CStdString::operator <  (LPCTSTR psz) const { return (_tcscmp(m_psz, psz) <  0); }
bool CStdString::operator >= (LPCTSTR psz) const { return (_tcscmp(m_psz, psz) >= 0); }
bool CStdString::operator >  (LPCTSTR psz) const { return (_tcscmp(m_psz, psz) >  0); }

bool CStdString::operator == (const CStdString& rhs) const {return (_tcscmp(m_psz, rhs.GetData()) == 0);}
bool CStdString::operator != (const CStdString& rhs) const {return (_tcscmp(m_psz, rhs.GetData()) != 0);}
bool CStdString::operator <= (const CStdString& rhs) const {return (_tcscmp(m_psz, rhs.GetData()) <= 0);}
bool CStdString::operator <  (const CStdString& rhs) const {return (_tcscmp(m_psz, rhs.GetData()) <  0);}
bool CStdString::operator >= (const CStdString& rhs) const {return (_tcscmp(m_psz, rhs.GetData()) >= 0);}
bool CStdString::operator >  (const CStdString& rhs) const {return (_tcscmp(m_psz, rhs.GetData()) >  0);}