#include "stdafx.h"
#include "ImagePane.h"

#include "Core/TaskWorker.h"


using namespace Gdiplus;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE

static char THIS_FILE[ ] = __FILE__;
#endif


BEGIN_MESSAGE_MAP( CImagePane, CDockablePane )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_CONTEXTMENU( )

	ON_COMMAND( ID_IMAGE_OPEN, &CImagePane::OnImageOpen )

	ON_COMMAND_PTR( WM_UPDATE_IMAGE, &CImagePane::OnUpdateImage )
END_MESSAGE_MAP( );



CImagePane::CImagePane( )
{
}

CImagePane::~CImagePane( )
{
}

int CImagePane::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CDockablePane::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	if( !m_wndToolBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE ) ||
		!m_wndToolBar.LoadToolBar( IDR_IMAGE_VIEW_TOOLBAR, 0, 0, TRUE )
		)
	{
		return -1;
	}

	m_wndToolBar.SetRouteCommandsViaFrame( FALSE );

	m_pImageView = new CImageView( );
	if( !m_pImageView->Create( nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect( ), this, AFX_IDW_PANE_FIRST ) )
	{
		return -1;
	}
	
	return 0;
}

void CImagePane::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize( nType, cx, cy );

	int cyTlb = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;

	m_wndToolBar.SetWindowPos( nullptr, 0, 0, cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER );
	m_pImageView->SetWindowPos( nullptr, 0, cyTlb, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER );
}

void CImagePane::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CMenu menu;
	menu.LoadMenuW( IDR_OUTPUT_POPUP );

	CMenu* pSumMenu = menu.GetSubMenu( 0 );

	CRect rcWindow;
	m_pImageView->GetWindowRect( &rcWindow );

	if( rcWindow.PtInRect( point ) )
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if( !pPopupMenu->Create( this, point.x, point.y, ( HMENU )pSumMenu->m_hMenu, FALSE, TRUE ) )
		{
			return;
		}

		reinterpret_cast< CMDIFrameWndEx* >( AfxGetMainWnd( ) )->OnShowPopupMenu( pPopupMenu );
		UpdateDialogControls( this, FALSE );
	}
	else
	{
		CDockablePane::OnContextMenu( pWnd, point );
	}
}

void CImagePane::OnImageOpen( )
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

				PostCommandMessage( WM_UPDATE_IMAGE, pBmp );
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

void CImagePane::OnUpdateImage( void* ptr )
{
	auto pImage = reinterpret_cast< Bitmap* >( ptr );

	//Invalidate( );
}



/*
m_rcDrawArea = RectF(
0.0f,
static_cast< REAL >( cyTlb ),
static_cast< REAL >( cx ),
static_cast< REAL >( cy - cyTlb )
);

m_ptCenter = PointF(
( m_rcDrawArea.GetLeft( ) + m_rcDrawArea.GetRight( ) ) / 2.0f,
( m_rcDrawArea.GetTop( ) + m_rcDrawArea.GetBottom( ) ) / 2.0f
);

LogInfo( __FUNCTIONW__ );


CPaintDC dc( this );

Graphics g( dc.GetSafeHdc( ) );

g.Clear( Color::MakeARGB( 255, 30, 30, 30 ) );


auto pBrush = new Gdiplus::SolidBrush( Gdiplus::Color::Cyan );

// 	auto pMat = new Gdiplus::Matrix( );
// 	pMat->Scale( 2.0f, 2.0f );
// 	pMat->Translate( -2.0f, -2.0f );
//
// 	g.SetTransform( pMat );
// 	g.TranslateTransform( 2.0f, 2.0f );


if( m_pImage )
{
REAL fSizeX = static_cast< REAL >( m_pImage->GetWidth( ) );
REAL fSizeY = static_cast< REAL >( m_pImage->GetHeight( ) );

fSizeX *= 2.0f;
fSizeY *= 2.0f;


RectF rcImage(
m_ptCenter.X - ( fSizeX / 2.0f ),
m_ptCenter.Y - ( fSizeY / 2.0f ),
fSizeX,
fSizeY
);

g.DrawImage( m_pImage, rcImage );

LogInfo( L"Redraw!" );
}


*/