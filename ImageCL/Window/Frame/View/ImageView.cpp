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

void CImageView::OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint )
{

// 	if( nullptr != m_spBitmap )
// 	{
// 		m_spBitmap->Destroy( );
// 	}
	CSize sizeImage( 100, 100 );

// 	const CString& strFile = pDoc->GetPathName( );
// 	if( !strFile.IsEmpty( ) )
// 	{
// 		m_spBitmap = std::make_shared<CD2DBitmap>( &m_renderTarget, strFile );
// 		HRESULT hr = m_spBitmap->Create( &m_renderTarget );
// 		if( m_spBitmap->IsValid( ) )
// 		{
// 			CD2DSizeF size = m_spBitmap->GetSize( );
// 			sizeImage.SetSize( static_cast< int >( size.width ), static_cast< int >( size.height ) );
// 		}
// 	}

	SetScrollSizes( MM_TEXT, sizeImage );
	ScrollToPosition( CPoint( 0, 0 ) );
}

void CImageView::OnDraw( CDC* pDC )
{ 
	HRESULT hr;

	if( m_renderTarget.IsValid( ) )
	{
		m_renderTarget.BeginDraw( );

		D2D1_COLOR_F color = { 1.f, 0.5f, 1.f, 0.2f }; 
		m_renderTarget.Clear( color );

// 		if( ( nullptr != m_spBitmap ) && m_spBitmap->IsValid( ) )
// 		{
// 			// apply translation transform according to view's scroll position
// 			CPoint point = GetScrollPosition( );
// 			D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation( ( float )-point.x, ( float )-point.y );
// 			m_renderTarget.SetTransform( matrix );
// 			// draw the bitmap
// 			CD2DSizeF size = m_spBitmap->GetSize( );
// 			m_renderTarget.DrawBitmap( m_spBitmap.get( ), CD2DRectF( 0, 0, size.width, size.height ) );
// 		}

		hr = m_renderTarget.EndDraw( );

		if( hr == D2DERR_RECREATE_TARGET )
		{
			m_renderTarget.ReCreate( m_hWnd );
		}
	}
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
	if( m_renderTarget.IsValid( ) /*&& m_spBitmap->IsValid( )*/ )
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
