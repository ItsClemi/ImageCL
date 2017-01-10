#include "stdafx.h"
#include "LogQueue.h"

CLogQueue::CLogQueue( )
{
	gEnv->pTaskWorker->RegisterIdleProcessor( this );
}

CLogQueue::~CLogQueue( )
{
	m_bActive = false;

	SLogEntry* pEntry;
	while( m_queueLogs.try_pop( pEntry ) )
	{
		SafeDelete( pEntry );
	}
}

void CLogQueue::OnProcess( )
{
	SLogEntry* pEntry;
	while( m_queueLogs.try_pop( pEntry ) )
	{
		AfxGetMainWnd( )->PostMessageW( 
			WM_COMMAND_REFLECT,
			WM_ADD_OUTPUT, 
			reinterpret_cast< LPARAM >( pEntry ) 
		);
	}
}
