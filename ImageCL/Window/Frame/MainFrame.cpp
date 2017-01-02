#include "stdafx.h"
#include "App.h"

#include "MainFrame.h"

#include <Window/Frame/View/CodeView.h>
#include <Window/Style/VisualStyle.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CMainFrame, CMDIFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWndEx )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_SETTINGCHANGE( )

	ON_COMMAND( ID_VIEW_OUTPUTPANE, &CMainFrame::OnShowOutputPane )

	ON_COMMAND_PTR( WM_STATUS_BAR_UPDATE, &CMainFrame::OnStatusBarUpdate )

END_MESSAGE_MAP( )



CMainFrame::CMainFrame( )
{
	EnableLoadDockState( FALSE );
}

CMainFrame::~CMainFrame( )
{ }

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CMDIFrameWndEx::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::Style::STYLE_3D_VS2005;
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      
	mdiTabParams.m_bTabIcons = FALSE;   
	mdiTabParams.m_bAutoColor = FALSE;   
	mdiTabParams.m_bDocumentMenu = TRUE;
	EnableMDITabbedGroups( TRUE, mdiTabParams );

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

	int nIndex = m_wndCodeBar.CommandToIndex( ID_SELECT_PROCESSOR );
	m_wndCodeBar.SetButtonInfo( nIndex, ID_SELECT_PROCESSOR, TBBS_SEPARATOR, 205 );

	CRect rect;
	m_wndCodeBar.GetItemRect( nIndex, &rect );
	rect.top = 1;
	rect.bottom = rect.top + 250;
	if( !m_comboBox.Create( CBS_DROPDOWNLIST | CBS_SORT | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL | WS_CHILD, rect, &m_wndCodeBar, ID_SELECT_PROCESSOR ) )
	{
		TRACE( _T( "Failed to create combo-box\n" ) );
		return FALSE;
	}
	m_comboBox.AddString( L"CPU - Fx 8150" );
	m_comboBox.AddString( L"GPU - GTX 980ti" );


	if( !m_wndStatusBar.Create( this ) )
	{
		TRACE0( "Failed to create status bar\n" );
		return -1;
	}

	static const UINT indicators[ ] = { ID_SEPARATOR, };
	m_wndStatusBar.SetIndicators( indicators, ARRAYSIZE( indicators ) );


	EnableDocking( CBRS_ALIGN_ANY );

	m_wndMenuBar.EnableDocking( CBRS_ALIGN_TOP );
	m_wndCodeBar.EnableDocking( CBRS_ALIGN_ANY );

	DockPane( &m_wndMenuBar );
	DockPane( &m_wndCodeBar  );


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
	if( !CMDIFrameWndEx::PreCreateWindow( cs ) )
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

		*pResult = 0;
		return OnCmdMsg( nCode, CN_COMMAND, pExtra, nullptr );
	}

	return CMDIFrameWndEx::OnWndMsg( message, wParam, lParam, pResult );
}

#ifdef _DEBUG
void CMainFrame::AssertValid( ) const
{
	CMDIFrameWndEx::AssertValid( );
}

void CMainFrame::Dump( CDumpContext& dc ) const
{
	CMDIFrameWndEx::Dump( dc );
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
	CMDIFrameWndEx::OnSettingChange( uFlags, lpszSection );
	//m_wndOutput.UpdateFonts( );
}

void CMainFrame::OnShowOutputPane( )
{
	if( !m_wndOutput.IsWindowVisible( ) )
	{
		m_wndOutput.ShowPane( TRUE, FALSE, FALSE );
	}
}

void CMainFrame::OnStatusBarUpdate( void* ptr )
{
	const auto szText = reinterpret_cast< wchar_t* >( ptr );

	m_wndStatusBar.SetPaneText( 0, szText, TRUE );
}

void CMainFrame::OnSize( UINT nType, int cx, int cy )
{
	CMDIFrameWndEx::OnSize( nType, cx, cy );

	

// 	if( m_bSplitterInitialized && nType != SIZE_MINIMIZED )
// 	{
// 		m_wndSplitter.SetRowInfo( 0, cy, 0 );
// 		m_wndSplitter.SetColumnInfo( 0, cx / 2, 50 );
// 		m_wndSplitter.SetColumnInfo( 1, cx / 2, 50 );
// 
// 		m_wndSplitter.RecalcLayout( );
// 	}
}
