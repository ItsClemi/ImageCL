#pragma once

class CTaskWorker : public CWinThread
{
	DECLARE_DYNCREATE( CTaskWorker );
public:
	CTaskWorker( );
	virtual ~CTaskWorker( );

private:


	DECLARE_MESSAGE_MAP( )


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