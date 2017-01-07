#pragma once

enum class eLogType : size_t
{
	eError, eInfo, eSuccess
};

enum class eOutputType : size_t
{
	eOutputWnd, eStatusBar
};

struct SLogEntry
{
	wchar_t			m_szMessage[ 1024 ];
	eLogType		m_eType;
	time_t			m_tm;
	eOutputType		m_eOutputType;
};


void AddLog( eLogType eType, eOutputType eOutput, const wchar_t* szFormat, va_list args );

inline void LogError( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eError, eOutputType::eOutputWnd, szFormat, args );
	}
	va_end( args );
}

inline void LogInfo( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eInfo, eOutputType::eOutputWnd, szFormat, args );
	}
	va_end( args );
}

inline void LogSuccess( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eSuccess, eOutputType::eOutputWnd, szFormat, args );
	}
	va_end( args );
}


inline void LogStatusBar( const wchar_t* szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );
	{
		AddLog( eLogType::eSuccess, eOutputType::eStatusBar, szFormat, args );
	}
	va_end( args );
}
