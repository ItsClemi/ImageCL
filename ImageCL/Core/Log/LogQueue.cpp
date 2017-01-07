#include "stdafx.h"
#include "LogQueue.h"

#include "Core/TaskWorker.h"

CLogQueue::CLogQueue( )
{
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

void CLogQueue::Process( )
{
	SLogEntry* pEntry;
	while( m_queueLogs.try_pop( pEntry ) )
	{
		switch( pEntry->m_eOutputType )
		{
			case eOutputType::eOutputWnd:
				PostCommandMessageSync( WM_ADD_OUTPUT, pEntry );
				break;
		
			case eOutputType::eStatusBar:
				PostCommandMessageSync( WM_STATUS_BAR_UPDATE, pEntry );
				break;

			default: throw std::runtime_error( "invalid output type" );
		}
	}
}

