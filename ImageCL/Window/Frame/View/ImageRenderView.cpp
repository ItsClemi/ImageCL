#include "stdafx.h"
#include "ImageRenderView.h"


IMPLEMENT_DYNCREATE( CImageRenderView, CView );

BEGIN_MESSAGE_MAP( CImageRenderView, CView )
	ON_WM_CREATE( )
	ON_WM_ERASEBKGND( )
	ON_WM_SIZE( )

END_MESSAGE_MAP( );


CImageRenderView::CImageRenderView( )
{ }

CImageRenderView::~CImageRenderView( )
{ }

void CImageRenderView::OnDraw( CDC* pDC )
{
	Gdiplus::Graphics g( pDC->GetSafeHdc( ) );

	g.Clear( Gdiplus::Color::MakeARGB( 255, 30, 30, 30 ) );



}

int CImageRenderView::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CView::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}

	if( !m_wndToolBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE ) )
	{
		return -1;
	}

	return 0;
}

void CImageRenderView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	CRect rectClient;
	GetClientRect( rectClient );
	int cyTlb = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;

	m_wndToolBar.SetWindowPos( NULL, 0, 0, cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER );
}

BOOL CImageRenderView::OnEraseBkgnd( CDC* pDC )
{
	return FALSE;
}
