#include "stdafx.h"
#include "Log.h"

#include "Core/TaskWorker.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE

static char THIS_FILE[ ] = __FILE__;
#endif


void AddLog( eLogType eType, const wchar_t* szFormat, va_list args )
{
	SLogEntry* pEntry = new SLogEntry;

	if( _vsnwprintf_s( pEntry->m_szMessage, ARRAYSIZE( pEntry->m_szMessage ), szFormat, args ) == _TRUNCATE )
	{
		wcscat_s( pEntry->m_szMessage, L"<failed to print message>" );
	}

	pEntry->m_eType = eType;
	pEntry->m_tm = _time64( nullptr );

	PostCommandMessage( WM_ADD_OUTPUT, pEntry );
}
