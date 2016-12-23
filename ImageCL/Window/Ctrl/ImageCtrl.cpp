#include "stdafx.h"
#include "ImageCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP( CImageCtrl, CWnd )
	ON_WM_CREATE( )
	//ON_WM_SIZE( ) 

	ON_REGISTERED_MESSAGE( AFX_WM_DRAW2D, &CImageCtrl::OnDraw2D )
END_MESSAGE_MAP( );

CImageCtrl::CImageCtrl( )
{ }

CImageCtrl::~CImageCtrl( )
{ }

int CImageCtrl::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CWnd::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}

	EnableD2DSupport( TRUE );

	m_pD2DBackground = new CD2DSolidColorBrush(
		GetRenderTarget( ), D2D1::ColorF( D2D1::ColorF::Indigo )
	);

	return 0;
}

LRESULT CImageCtrl::OnDraw2D( WPARAM wParam, LPARAM lParam )
{
	auto pTarget = reinterpret_cast< CHwndRenderTarget* >( lParam );
	ASSERT_VALID( pTarget );


	CRect rcWindow;
	GetClientRect( &rcWindow );


	pTarget->BeginDraw( );
	
	pTarget->Clear( D2D1::ColorF( D2D1::ColorF::Indigo ) );

	//pTarget->DrawBitmap(, )

	//pTarget->GetRenderTarget()->CreateBitmap()

	pTarget->EndDraw( );

	return TRUE;
}
