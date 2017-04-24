//GrowableArray.h: 可增长数组模板类

#pragma once

#include <new>
#include <assert.h>

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

