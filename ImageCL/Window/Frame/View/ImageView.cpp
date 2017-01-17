#include "stdafx.h"
#include "ImageView.h"


IMPLEMENT_DYNCREATE( CImageView, CScrollView )

BEGIN_MESSAGE_MAP( CImageView, CScrollView )
	ON_WM_CREATE( )
	ON_WM_MOUSEACTIVATE( )
	ON_WM_SIZE( )

	ON_REGISTERED_MESSAGE( AFX_WM_DRAW2D, &CImageView::OnDrawD2D )
END_MESSAGE_MAP( )


CImageView::CImageView( )
{ }

CImageView::~CImageView( )
{ }


void CImageView::OnDraw( CDC* pDC )
{ }

void CImageView::UpdateImage( CD2DBitmapEx* pBitmap )
{
	m_renderImage.OnLoadImage( pBitmap );

	SetScrollSizes( MM_TEXT, m_renderImage.GetScrollSize( ) );
	ScrollToPosition( CPoint( 0, 0 ) );

	RedrawWindow( );
}

void CImageView::SaveImageToFile( const CString& szPath )
{

}

void CImageView::SwitchImage( )
{
	m_renderImage.OnSwitchImage( );

	RedrawWindow( );
}

int CImageView::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CScrollView::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}

	EnableD2DSupport( );

	try
	{
		m_renderImage.OnCreate( GetRenderTarget( ) );
	}
	catch( CException* e )
	{
		e->ReportError( );
		AfxThrowUserException( );
	}

	SetScrollSizes( MM_TEXT, m_renderImage.GetScrollSize( ) );

	return 0;
}

void CImageView::OnSize( UINT nType, int cx, int cy )
{
	CScrollView::OnSize( nType, cx, cy );

	m_ptCenter = CD2DPointF(
		static_cast< float >( cx ) / 2.0f,
		static_cast< float >( cy ) / 2.0f
	);

	RedrawWindow( );
}

int CImageView::OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message )
{
	return CWnd::OnMouseActivate( pDesktopWnd, nHitTest, message );
}

LRESULT CImageView::OnDrawD2D( WPARAM wParam, LPARAM lParam )
{
	const auto pTarget = reinterpret_cast< CHwndRenderTarget* >( lParam );

	const auto dpi = pTarget->GetDpi( );

	//> Disable DPI scaling so we get real coord values
	pTarget->SetDpi( { 96.0f, 96.0f } );

	pTarget->Clear( CHwndRenderTarget::COLORREF_TO_D2DCOLOR( RGB( 30, 30, 30 ) ) );


	const auto ptScroll = GetScrollPosition( );
	const auto matrix = D2D1::Matrix3x2F::Translation( ( float )-ptScroll.x, ( float )-ptScroll.y );

	pTarget->SetTransform( matrix );


	ID2D1Bitmap* pBitmap = m_renderImage.GetBitmap( );

	auto size = m_renderImage.GetSize( );

	CD2DRectF rcDraw(
		m_ptCenter.x - ( size.width / 2.0f ),
		m_ptCenter.y - ( size.height / 2.0f ),
		m_ptCenter.x + ( size.width / 2.0f ),
		m_ptCenter.y + ( size.height / 2.0f )
	);

	pTarget->GetHwndRenderTarget( )->DrawBitmap( pBitmap, rcDraw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR );


	pTarget->SetDpi( dpi );

	return TRUE;
}
