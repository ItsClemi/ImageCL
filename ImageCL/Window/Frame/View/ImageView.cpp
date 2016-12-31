#include "stdafx.h"
#include "ImageView.h"

#include <Resource.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CImageView, CView );

BEGIN_MESSAGE_MAP( CImageView, CView )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_ERASEBKGND( )


	ON_COMMAND( ID_IMAGE_OPEN, &CImageView::OnImageOpen )

END_MESSAGE_MAP( );


CImageView::CImageView( )
{ }

CImageView::~CImageView( )
{ }

void CImageView::OnDraw( CDC* pDC )
{
	Gdiplus::Graphics g( pDC->GetSafeHdc( ) );

	g.Clear( Gdiplus::Color::MakeARGB( 255, 30, 30, 30 ) );

}

int CImageView::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CView::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}

	if( !m_wndToolBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE ) ||
		!m_wndToolBar.LoadToolBar( IDR_IMAGE_VIEW_TOOLBAR, 0, 0, TRUE )
		)
	{
		return -1;
	}

 	

	return 0;
}

void CImageView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	int cyTlb = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;

	m_wndToolBar.SetWindowPos( nullptr, 0, 0, cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER );
}

BOOL CImageView::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CImageView::OnImageOpen( )
{
	CFileDialog	dlg(
		TRUE,
		L"image",
		L"",
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"Image Files|*.bmp; *.gif; *.jpeg; *.jpg; *.png; *.tiff||"
	);

	if( dlg.DoModal( ) == IDOK )
	{
		auto szPath = std::make_shared< std::wstring >( dlg.GetPathName( ) );
		auto pCursor = std::make_shared< CWaitCursor >( );

		Concurrency::create_task( [ szPath, pCursor ] {
			auto pBmp = Gdiplus::Bitmap::FromFile( szPath->c_str( ), FALSE );

			auto status = pBmp->GetLastStatus( );

			if( status == Gdiplus::Status::Ok )
			{
				LogSuccess( L"Successfully loaded image %s", szPath->c_str( ) );
			}
			else
			{
				LogError( L"Failed to load image %s GdiResult: %s(%d)", 
					szPath->c_str( ), 
					Gdiplus::GdiStatusToString( status ),
					status
				);
			}

			pCursor->Restore( );
		} );
	}
}

