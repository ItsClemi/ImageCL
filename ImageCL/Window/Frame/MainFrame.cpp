#include "stdafx.h"
#include "App.h"

#include "MainFrame.h"

#include <Window/Frame/View/CodeView.h>
#include <Window/Frame/View/ImageView.h>
#include <Window/Style/VisualStyle.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWndEx )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_SETTINGCHANGE( )

	ON_COMMAND( ID_VIEW_OUTPUTPANE, &CMainFrame::OnShowOutputPane )


END_MESSAGE_MAP( )



CMainFrame::CMainFrame( )
{
	EnableLoadDockState( FALSE );
}

CMainFrame::~CMainFrame( )
{ }

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CFrameWndEx::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	if( !m_wndMenuBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY ) ||
		!m_wndMenuBar.LoadToolBar( IDR_MAINFRAME )
		)
	{
		TRACE0( "Failed to create menubar\n" );
		return -1;
	}

	CMFCPopupMenu::SetForceMenuFocus( FALSE );

	if( !m_wndCodeBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndCodeBar.LoadToolBar( IDR_CODE_VIEW )
		)
	{
		TRACE0( "Failed to create toolbar\n" );
		return -1;
	}



	EnableDocking( CBRS_ALIGN_ANY );

	m_wndMenuBar.EnableDocking( CBRS_ALIGN_TOP );
	m_wndCodeBar.EnableDocking( CBRS_ALIGN_ANY );

	DockPane( &m_wndMenuBar );
	DockPane( &m_wndCodeBar );


	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CVisualStyle ) );
	CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );
	CDockingManager::SetDockingMode( DT_SMART );

	EnableAutoHidePanes( CBRS_ALIGN_ANY );


	if( !CreateDockingWindows( ) )
	{
		TRACE0( "Failed to create docking windows\n" );
		return -1;
	}



	return 0;
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs )
{
	if( !CFrameWndEx::PreCreateWindow( cs ) )
	{
		return FALSE;
	}

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
	//> Forwards messages to panes
	if( message == WM_COMMAND_REFLECT )
	{
		UINT nCode = static_cast< UINT >( wParam );
		void* pExtra = reinterpret_cast< void* >( lParam );

		auto pDockingMng = afxGlobalUtils.GetDockingManager( this );

		CObList list;
		pDockingMng->GetPaneList( list );

		for( auto pos = list.GetHeadPosition( ); pos != nullptr; )
		{
			auto pPane = DYNAMIC_DOWNCAST( CBasePane, list.GetNext( pos ) );
			ASSERT_VALID( pPane );

			if( ::IsWindow( *pPane ) &&
				pPane->IsVisible( ) &&
				pPane->OnCmdMsg( nCode, CN_COMMAND, pExtra, nullptr )
				)
			{
				*pResult = 0;
				return TRUE;
			}
		}

		TRACE( "Message not handled! %d\n", message );

		*pResult = 0;
		return FALSE;
	}

	return CFrameWndEx::OnWndMsg( message, wParam, lParam, pResult );
}

#ifdef _DEBUG
void CMainFrame::AssertValid( ) const
{
	CFrameWndEx::AssertValid( );
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
	CFrameWndEx::Dump( dc );
}
#endif //_DEBUG

BOOL CMainFrame::CreateDockingWindows( )
{
	CSize outputPaneHeight = { 100, 150 };
	ScaleDpi( outputPaneHeight );

	if( !m_wndOutput.Create(
		L"Output",
		this,
		{ 0, 0, outputPaneHeight.cx, outputPaneHeight.cy },
		TRUE,
		ID_VIEW_OUTPUTWND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI
	) )
	{
		TRACE0( "Failed to create Output window\n" );
		return FALSE;
	}

	CRect rcWindow;
	GetClientRect( rcWindow );

	if( !m_wndImage.Create(
		L"Image",
		this,
		{ 0, 0, rcWindow.Width( ) / 2, rcWindow.Height( ) - outputPaneHeight.cy },
		TRUE,
		160,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI
	) )
	{
		TRACE0( "Failed to create image window\n" );
		
	
		return FALSE;
	}

	HICON hOutputBarIcon = LoadResourceIcon( IDI_OUTPUT_WND_HC );

	m_wndOutput.SetIcon( hOutputBarIcon, FALSE );


	m_wndOutput.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndOutput );

	m_wndImage.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndImage );

	return TRUE;
}

void CMainFrame::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
	CFrameWndEx::OnSettingChange( uFlags, lpszSection );
	//m_wndOutput.UpdateFonts( );
}

void CMainFrame::OnShowOutputPane( )
{
	if( !m_wndOutput.IsWindowVisible( ) )
	{
		m_wndOutput.ShowPane( TRUE, FALSE, FALSE );
	}
}

void CMainFrame::OnSize( UINT nType, int cx, int cy )
{
	CFrameWndEx::OnSize( nType, cx, cy );

	

// 	if( m_bSplitterInitialized && nType != SIZE_MINIMIZED )
// 	{
// 		m_wndSplitter.SetRowInfo( 0, cy, 0 );
// 		m_wndSplitter.SetColumnInfo( 0, cx / 2, 50 );
// 		m_wndSplitter.SetColumnInfo( 1, cx / 2, 50 );
// 
// 		m_wndSplitter.RecalcLayout( );
// 	}
}
