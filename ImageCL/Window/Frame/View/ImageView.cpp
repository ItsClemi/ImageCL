#include "stdafx.h"
#include "ImageView.h"


IMPLEMENT_DYNCREATE( CImageView, CScrollView )

BEGIN_MESSAGE_MAP( CImageView, CScrollView )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_ERASEBKGND( )
	ON_WM_MOUSEACTIVATE( )


END_MESSAGE_MAP( )


CImageView::CImageView( )
{
}

CImageView::~CImageView( )
{
}


void CImageView::OnDraw( CDC* pDC )
{ 
	HRESULT hr;

	if( m_renderTarget.IsValid( ) )
	{
		m_renderTarget.BeginDraw( );

		m_renderTarget.Clear( CHwndRenderTarget::COLORREF_TO_D2DCOLOR( RGB( 30, 30, 30 ) ) );

		CPoint point = GetScrollPosition( );
		D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation( ( float )-point.x, ( float )-point.y );

		m_renderTarget.SetTransform( matrix );




 		if( m_pBitmap && m_pBitmap->IsValid( ) )
 		{
			CD2DSizeF size = m_pBitmap->GetSize( );
 			m_renderTarget.DrawBitmap( m_pBitmap, CD2DRectF( 0, 0, size.width, size.height ) );
 		}
		else
		{
			m_renderTarget.FillRectangle( 
				CD2DRectF( 0.0f, 0.0f, 250.0f, 250.0f ), 
				new CD2DSolidColorBrush( &m_renderTarget, D2D1::ColorF::Azure ) 
			);
		}

		hr = m_renderTarget.EndDraw( );

		if( hr == D2DERR_RECREATE_TARGET )
		{
			m_renderTarget.ReCreate( m_hWnd );
		}
	}
}


void CImageView::UpdateImage( CD2DBitmap* pBitmap )
{
	m_pBitmap = pBitmap;

	
	CD2DSizeF size = m_pBitmap->GetSize( );
	CSize sizeImage( static_cast< int >( size.width ), static_cast< int >( size.height ) );

	SetScrollSizes( MM_TEXT, sizeImage );
	ScrollToPosition( CPoint( 0, 0 ) );


	PostMessage( WM_PAINT );
}

void CImageView::SafeImageToFile( const CString& szPath )
{

}

int CImageView::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CScrollView::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}	
	
	SetScrollSizes( MM_TEXT, CSize( 600, 400 ) );


	HRESULT hr = m_renderTarget.Create( m_hWnd );
	return SUCCEEDED( hr ) ? 0 : -1;
}

void CImageView::OnSize( UINT nType, int cx, int cy )
{
	CScrollView::OnSize( nType, cx, cy );

	if( m_renderTarget.IsValid( ) )
	{
		m_renderTarget.Resize( CD2DSizeU( cx, cy ) );
	}
}

BOOL CImageView::OnEraseBkgnd( CDC* pDC )
{
	if( m_renderTarget.IsValid( ) && m_pBitmap && m_pBitmap->IsValid( ) )
	{
		return TRUE;
	}

	return CScrollView::OnEraseBkgnd( pDC );
}

int CImageView::OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message )
{
	CFrameWnd* pParentFrame = GetParentFrame( );

	if( pParentFrame == pDesktopWnd || pDesktopWnd->IsChild( pParentFrame ) )
	{
		return CScrollView::OnMouseActivate( pDesktopWnd, nHitTest, message );
	}

	return MA_NOACTIVATE;
}