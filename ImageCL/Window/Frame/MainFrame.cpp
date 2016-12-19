#include "stdafx.h"
#include "App.h"

#include "MainFrame.h"

#include <Window/Frame/View/TextEditView.h>
#include <Window/Frame/View/ImageRenderView.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CMainFrame, CFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CFrameWndEx )
	ON_WM_CREATE( )
	ON_WM_SETTINGCHANGE( )
	ON_WM_SIZE( )

	ON_COMMAND( ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize )

	ON_REGISTERED_MESSAGE( AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew )

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

	if( !m_wndMenuBar.Create( this ) )
	{
		TRACE0( "Failed to create menubar\n" );
		return -1;
	}

	if( !m_wndMenuBar.LoadToolBar( IDR_MAINFRAME_256 ) )
	{
		TRACE0( "Failed to LoadToolBar\n" );

		return -1;
	}

	m_wndMenuBar.SetPaneStyle( m_wndMenuBar.GetPaneStyle( ) | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY );

	CMFCPopupMenu::SetForceMenuFocus( FALSE );

	EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndMenuBar );


	CDockingManager::SetDockingMode( DT_SMART );

	EnableAutoHidePanes( CBRS_ALIGN_ANY );

	CMFCToolBar::AddToolBarForImageCollection( IDR_MENU_IMAGES, IDB_MENU_IMAGES_24 );


	if( !CreateDockingWindows( ) )
	{
		TRACE0( "Failed to create docking windows\n" );
		return -1;
	}

	m_wndOutput.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndOutput );

	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CMFCVisualManagerOffice2007 ) );
	CMFCVisualManagerOffice2007::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );


	return 0;
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext )
{
	CRect rcWindow;
	GetWindowRect( &rcWindow );

	if( !m_wndSplitter.CreateStatic( this, 1, 2 ) )
	{
		AfxMessageBox( L"Failed to create splitter!", IDOK | MB_ICONERROR );
		return FALSE;
	}

	if( !m_wndSplitter.CreateView( 0, 0, RUNTIME_CLASS( CTextEditView ), { rcWindow.Width( ) / 2, rcWindow.Height( ) }, pContext ) )
	{
		AfxMessageBox( L"Failed to create TextEditView!", IDOK | MB_ICONERROR );
		return FALSE;
	}

	if( !m_wndSplitter.CreateView( 0, 1, RUNTIME_CLASS( CImageRenderView ), { rcWindow.Width( ) / 2, rcWindow.Height( ) }, pContext ) )
	{
		AfxMessageBox( L"Failed to create ImageRenderView!", IDOK | MB_ICONERROR );
		return FALSE;
	}

	m_bSplitterInitialized = true;

	return TRUE;
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


//> Mfc style message reflector(so that our panes get command messages too)
BOOL CMainFrame::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	//> only reflect CN_COMMAND messages
	if( nCode == CN_COMMAND )
	{
		auto pDockingMng = afxGlobalUtils.GetDockingManager( this );

		CObList list;
		pDockingMng->GetPaneList( list );

		for( POSITION pos = list.GetHeadPosition( ); pos != nullptr; )
		{
			auto pPane = DYNAMIC_DOWNCAST( CBasePane, list.GetNext( pos ) );
			ASSERT_VALID( pPane );

			if( ::IsWindow( *pPane ) && pPane->IsVisible( ) && pPane->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo ) )
			{
				return TRUE;
			}
		}
	}

	return CFrameWndEx::OnCmdMsg( nID, nCode, pExtra, pHandlerInfo );
}

BOOL CMainFrame::CreateDockingWindows( )
{
	CString strOutputWnd;
	BOOL bNameValid = strOutputWnd.LoadString( IDS_OUTPUT_WND );
	ASSERT( bNameValid );

	CSize outputPaneHeight = { 100, 300 };
	ScaleDpi( outputPaneHeight );

	if( !m_wndOutput.Create(
		strOutputWnd,
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

	HICON hOutputBarIcon = reinterpret_cast< HICON >( ::LoadImageW(
		::AfxGetResourceHandle( ),
		MAKEINTRESOURCE( IDI_OUTPUT_WND_HC ),
		IMAGE_ICON,
		::GetSystemMetrics( SM_CXSMICON ),
		::GetSystemMetrics( SM_CYSMICON ),
		0
	) );

	m_wndOutput.SetIcon( hOutputBarIcon, FALSE );

	return TRUE;
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

void CMainFrame::OnViewCustomize( )
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog( this, TRUE );
	pDlgCust->Create( );
}

LRESULT CMainFrame::OnToolbarCreateNew( WPARAM wp, LPARAM lp )
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew( wp, lp );
	if( lres == 0 )
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = ( CMFCToolBar* )lres;
	ASSERT_VALID( pUserToolbar );

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString( IDS_TOOLBAR_CUSTOMIZE );
	ASSERT( bNameValid );

	pUserToolbar->EnableCustomizeButton( TRUE, ID_VIEW_CUSTOMIZE, strCustomize );
	return lres;
}


void CMainFrame::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
	CFrameWndEx::OnSettingChange( uFlags, lpszSection );
	//m_wndOutput.UpdateFonts( );
}

void CMainFrame::OnSize( UINT nType, int cx, int cy )
{
	CFrameWndEx::OnSize( nType, cx, cy );

	CRect rcWindow;
	GetWindowRect( &rcWindow );

	if( m_bSplitterInitialized && nType != SIZE_MINIMIZED )
	{
		m_wndSplitter.SetRowInfo( 0, cy, 0 );
		m_wndSplitter.SetColumnInfo( 0, rcWindow.Width( ) / 2, 50 );
		m_wndSplitter.SetColumnInfo( 1, rcWindow.Width( ) / 2, 50 );

		m_wndSplitter.RecalcLayout( );
	}
}

