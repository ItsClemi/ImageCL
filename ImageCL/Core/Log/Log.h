#pragma once

enum class eLogType : size_t
{
	eError, eInfo, eSuccess
};

struct SLogEntry
{
	wchar_t		m_szMessage[ 1024 ];
	eLogType	m_eType;
	time_t		m_tm;
};


void AddLog( eLogType eType, const wchar_t* szFormat, va_list args );


inline void LogError( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eError, szFormat, args );
	}
	va_end( args );
}

inline void LogInfo( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eInfo, szFormat, args );
	}
	va_end( args );
}

inline void LogSuccess( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eSuccess, szFormat, args );
	}
	va_end( args );
}