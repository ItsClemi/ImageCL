#pragma once


class CTaskWorker
{
public:
	CTaskWorker( );
	~CTaskWorker( );

public:
	void RegisterIdleProcessor( IProcessor* pProcessor );

public:
	void ProcessIdle( );

	void AddUIMessage( std::function< void( void ) > f )
	{
		m_queueLater.push( f );
	}

private:
	std::mutex	m_cs;

	std::vector< IProcessor* >	m_vecProcessors;


	Concurrency::concurrent_queue< std::function< void( void ) > > m_queueLater;
};


template< typename T >
inline void PostCommandMessage( UINT nId, T lParam )
{
	auto pApp = AfxGetApp( );

	if( pApp )
	{
		auto pWnd = pApp->GetMainWnd( );

		if( pWnd && IsWindow( pWnd->GetSafeHwnd( ) ) )
		{
			BOOL bResult = pWnd->PostMessageW(
				WM_COMMAND_REFLECT,
				static_cast< WPARAM >( nId ),
				reinterpret_cast< LPARAM >( lParam )
			);

			ASSERT( bResult == TRUE );
		}
	}
}




void RunUIThread( std::function< void( void ) > f );