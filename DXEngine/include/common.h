#ifndef __COMMON_H__
#define __COMMON_H__

#include <Windows.h>
#include <tchar.h>
#include <new>
#include <assert.h>

//-------------------------类型定义------------------------------------------
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
//===================================================================


//-------------------------释放宏------------------------------------------
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	{ if (p) { delete (p); (p) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p) = NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p) = NULL; } }
#endif
//====================================================================

//-------------------------字符串类---------------------------------------
class CStdString
{
	enum { MAX_LOCAL_STRING_LEN = 63 };

public:
	CStdString();
	CStdString(const CStdString& rhs);
	CStdString(LPCTSTR psz, int nLength = -1);
	~CStdString();

public:
	void Empty();
	bool IsEmpty() const;

	int GetLength() const {return _tcslen(m_psz);}
	LPCTSTR GetData() const {return m_psz;}

	TCHAR GetAt(int nIndex) const {return m_psz[nIndex];}
	void SetAt(int nIndex, TCHAR ch) {m_psz[nIndex] = ch;}

	void Append(LPCTSTR psz);
	void Assign(LPCTSTR psz, int nLength = -1);
	int Format(LPCTSTR pszFmt, ...);

	void MakeUpper();
	void MakeLower();

	CStdString Left(int nLength) const;
	CStdString Right(int nLength) const;
	CStdString Mid(int nPos, int nLength = -1) const;

	int Find(TCHAR ch, int nPos = 0) const;
	int Find(LPCTSTR psz, int nPos = 0) const;
	int RFind(TCHAR ch) const;

	TCHAR operator[](int nIndex) const;

	const CStdString& operator=(LPCTSTR psz);
	const CStdString& operator+=(LPCTSTR psz);
	const CStdString operator+(LPCTSTR psz);

	const CStdString& operator=(const CStdString& rhs);
	const CStdString& operator+=(const CStdString& rhs);
	const CStdString operator+(const CStdString& rhs);

	bool operator == (LPCTSTR psz) const;
	bool operator != (LPCTSTR psz) const;
	bool operator <= (LPCTSTR psz) const;
	bool operator <  (LPCTSTR psz) const;
	bool operator >= (LPCTSTR psz) const;
	bool operator >  (LPCTSTR psz) const;

	bool operator == (const CStdString& rhs) const;
	bool operator != (const CStdString& rhs) const;
	bool operator <= (const CStdString& rhs) const;
	bool operator <  (const CStdString& rhs) const;
	bool operator >= (const CStdString& rhs) const;
	bool operator >  (const CStdString& rhs) const;

private:
	LPTSTR m_psz;
	TCHAR m_szBuf[MAX_LOCAL_STRING_LEN + 1];
};
//===================================================================================

//--------------------------------------可增长数组模板类------------------------------------------------
template<typename T> class CGrowableArray
{
public:
	CGrowableArray() { m_pData = NULL; m_nSize = m_nMaxSize = 0; }
	CGrowableArray( const CGrowableArray<T>& rhs ) { for(int i = 0; i < rhs.m_nSize; i++) Add(rhs.m_pData[i]); }
	~CGrowableArray() { RemoveAll(); }

public:
	CGrowableArray& operator=( const CGrowableArray<T>& rhs )
	{
		if ( this == &rhs )
			return *this;

		RemoveAll();

		for ( int i = 0; i < rhs.m_nSize; i++ )
			Add( rhs.m_pData[i] );

		return *this;
	}
	T& operator[]( int nIndex ) const { return GetAt(nIndex); }

public:
	HRESULT SetSize( int nSize);
	int GetSize() const { return m_nSize; }
	T* GetData() const { return m_pData; }
	HRESULT Add( const T& value);
	HRESULT Insert( int nIndex, const T& value );
	HRESULT Remove( int nIndex );
	VOID RemoveAll() { SetSize(0); }
	HRESULT SetAt( int nIndex, const T& value )
	{
		if ( nIndex < 0 || nIndex >= m_nSize)
			return E_INVALIDARG;

		m_pData[nIndex] = value;

		return S_OK;
	}
	T& GetAt( int nIndex ) const
	{
		assert( nIndex >= 0 && nIndex < m_nSize );
		return m_pData[nIndex];
	}

private:
	T* m_pData;
	int m_nSize;
	int m_nMaxSize;
};

template<typename T>
HRESULT CGrowableArray<T>::SetSize( int nSize)
{
	if ( nSize < 0 || nSize > INT_MAX/sizeof(T) )
		return E_INVALIDARG;

	if ( nSize == 0)
	{
		//释放内存
		if ( m_pData )
		{
			free(m_pData);
			m_pData = NULL;
		}
	}
	else if ( nSize > m_nMaxSize )
	{
		//重新分配内存
		int nGrowBy = (m_nMaxSize == 0) ? 16 : m_nMaxSize;
		m_nMaxSize += nGrowBy;
		if ( m_nMaxSize > INT_MAX / sizeof(T))
			m_nMaxSize = INT_MAX;
		T* pDateNew = (T*)realloc( m_pData, m_nMaxSize * sizeof(T) );
		if ( pDateNew == NULL )
			return E_OUTOFMEMORY;

		m_pData = pDateNew;
	}

	if ( nSize < m_nSize)
	{
		//析构T
		for ( int i = nSize; i < m_nSize; i++)
			m_pData[i].~T();
	}
	else
	{
		//构造T
		for ( int i = m_nSize; i < nSize; i++)
			::new(&m_pData[i]) T;
	}

	m_nSize = nSize;

	return S_OK;
}

template<typename T>
HRESULT CGrowableArray<T>::Add( const T& value)
{
	HRESULT hr;
	if ( FAILED( hr = SetSize( m_nSize + 1 ) ) )
		return hr;

	assert( m_pData != NULL);

	new( &m_pData[m_nSize-1] ) T;

	m_pData[m_nSize-1] = value;

	return S_OK;
}

template<typename T>
HRESULT CGrowableArray<T>::Insert( int nIndex, const T& value )
{
	HRESULT hr;

	if ( nIndex < 0 || nIndex > m_nSize)
		return E_INVALIDARG;

	if ( FAILED( hr == SetSize( m_nSize + 1) ) )
		return hr;

	MoveMemory( &m_pData[nIndex+1], &m_pData[nIndex], sizeof(T) * (m_nSize - (nIndex+1)) );

	new( &m_pData[nIndex] ) T;

	m_pData[nIndex] = value;

	return S_OK;
}

template<typename T>
HRESULT CGrowableArray<T>::Remove( int nIndex )
{
	if ( nIndex < 0 || nIndex >= m_nSize )
		return E_INVALIDARG;

	m_pData[nIndex].~T();

	MoveMemory( &m_pData[nIndex], &m_pData[nIndex+1], sizeof(T) * (m_nSize - (nIndex +1)) );
	--m_nSize;

	return S_OK;
}
//======================================================================================

#endif // __COMMON_H__