#include "stdafx.h"
#include "ImagePane.h"

#include "Core/Direct2d/D2DBitmapEx.h"


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
	ON_COMMAND( ID_IMAGE_SAVE, &CImagePane::OnImageSave )
	ON_COMMAND( ID_IMAGE_SWITCH, &CImagePane::OnImageSwitch )

END_MESSAGE_MAP( );



CImagePane::CImagePane( )
{ }

CImagePane::~CImagePane( )
{ }

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
	m_pImageView->SetWindowPos( nullptr, 0, cyTlb, cx, cy - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER );
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
 		L"Image|*.bmp; *.jpeg; *.jpg; *.png; *.tiff||"
 	);
 
 	if( dlg.DoModal( ) == IDOK )
 	{
 		auto szPath = std::make_shared< CString >( dlg.GetPathName( ) );
 
 		auto pTarget = m_pImageView->GetRenderTarget( );
 
 		Concurrency::create_task( [ this, szPath, pTarget ] {
 			auto pBitmap = new CD2DBitmapEx( *szPath );
 
 			HRESULT hr = pBitmap->Create( pTarget );
 			if( SUCCEEDED( hr ) )
 			{
				RunUIThread( [ this, pBitmap ]
				{ 
					m_pImageView->UpdateImage( pBitmap );
				} );

				const auto csPixel = pBitmap->GetSize( );
				LogSuccess( 
					L"Image: %s loaded (%I64dx%I64d)", 
					szPath->GetString( ), 
					static_cast< size_t >( csPixel.width ), 
					static_cast< size_t >(csPixel.height ) 
				);
 			}
 			else
 			{
 				LogError( L"Failed to load image %s HRESULT: 0x%08x",
 					szPath->GetString( ),
 					hr
 				);
 			}
 		} );
 	}	
}

void CImagePane::OnImageSave( )
{
	CFileDialog	dlg(
		FALSE,
		L"image",
		L"",
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		L"Image|*.bmp; *.jpeg; *.jpg; *.png; *.tiff||"
	);

	if( dlg.DoModal( ) == IDOK )
	{
		m_pImageView->SaveImageToFile( dlg.GetPathName( ) );
	}
}

void CImagePane::OnImageSwitch( )
{
	m_pImageView->SwitchImage( );
}
