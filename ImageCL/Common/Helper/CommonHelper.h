#pragma once


template< typename T >
void SafeDelete( T& t )
{
	if( t )
	{
		delete( t );
		t = nullptr;
	}
}

template< typename T >
void SafeDeleteArray( T& t )
{
	if( t )
	{
		delete[ ] t;
		t = nullptr;
	}
}

inline std::wstring StringToWstring( const std::string& str )
{
	std::wstring_convert< std::codecvt_utf8< wchar_t >, wchar_t > conv;

	return conv.from_bytes( str );
}

inline std::string WstringToString( const std::wstring& str )
{
	std::wstring_convert< std::codecvt_utf8< wchar_t >, wchar_t > conv;

	return conv.to_bytes( str );
}

template< typename T >
inline void SafeRelease( T** ppInterfaceToRelease )
{
	if( *ppInterfaceToRelease )
	{
		( *ppInterfaceToRelease )->Release( );
		( *ppInterfaceToRelease ) = nullptr;
	}
}
