#pragma once


inline HICON LoadResourceIcon( WORD nId )
{
	return reinterpret_cast< HICON >(
		::LoadImageW(
			AfxGetResourceHandle( ),
			MAKEINTRESOURCEW( nId ),
			IMAGE_ICON,
			GetSystemMetrics( SM_CXSMICON ),
			GetSystemMetrics( SM_CYSMICON ),
			0
		) );
}


inline int RichEditGetVisibleLines( CRichEditCtrl* pCtrl )
{
	CRect rcEdit;
	pCtrl->GetClientRect( rcEdit );

	int nFirstChar = pCtrl->CharFromPos( { 0, 0 } );
	int nLastChar = pCtrl->CharFromPos( { rcEdit.right, rcEdit.bottom } );

	if( nLastChar < 0 )
	{
		nLastChar = pCtrl->GetTextLength( );
	}

	int nFirstLine = pCtrl->LineFromChar( nFirstChar );
	int nLastLine = pCtrl->LineFromChar( nLastChar );

	return ( nLastLine - nFirstLine );
}



#ifndef ON_COMMAND_PTR
#define ON_COMMAND_PTR( id, memberFxn ) \
	{ WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, AfxSigCmd_v_pv, \
		(AFX_PMSG) \
		(static_cast< void (AFX_MSG_CALL CCmdTarget::*)(void*) > \
		(memberFxn)) },

#endif

inline int AfxMessageBox( std::wstring& szMessage )
{
	return AfxMessageBox( szMessage.c_str( ) );
}


inline const std::string LoadTextFromResource( int nId )
{
	HINSTANCE hInstance = AfxGetApp( )->m_hInstance;
	HRSRC hRes = FindResource( hInstance, MAKEINTRESOURCE( nId ), L"TEXT" );

	if( hRes )
	{
		HGLOBAL hLoaded = LoadResource( hInstance, hRes );

		if( hLoaded )
		{
			void* pLocked = LockResource( hLoaded );

			if( pLocked )
			{
				DWORD dwResourceSize = SizeofResource( hInstance, hRes );

				if( dwResourceSize > 0 )
				{
					return std::string(
						static_cast< char* >( pLocked ),
						static_cast< size_t >( dwResourceSize )
					);		
				}
			}
		}
	}

	return "<Error>";
}