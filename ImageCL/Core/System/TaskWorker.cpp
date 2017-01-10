#include "stdafx.h"
#include "TaskWorker.h"

CTaskWorker::CTaskWorker( )
{

}

CTaskWorker::~CTaskWorker( )
{
}

void CTaskWorker::RegisterIdleProcessor( IProcessor* pProcessor )
{
	std::lock_guard< std::mutex > l( m_cs );

	m_vecProcessors.push_back( pProcessor );
}

void CTaskWorker::ProcessIdle( )
{
	std::lock_guard< std::mutex > l( m_cs );

	for( auto i : m_vecProcessors )
	{
		i->OnProcess( );
	}

	std::function< void( void ) > f;
	while( m_queueLater.try_pop( f ) )
	{
		f( );
	}

}

void RunUIThread( std::function< void( void ) > f )
{
	gEnv->pTaskWorker->AddUIMessage( f );
}
