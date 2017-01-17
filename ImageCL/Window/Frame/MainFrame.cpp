#include "stdafx.h"
#include "MainFrame.h"

#include "Window/Frame/View/CodeView.h"
#include "Window/Style/VisualStyle.h"

#include "Window/Dialog/DeviceInfoDialog.h"

#include "Window/Frame/Doc/CodeDoc.h"

#include "Core/OpenCL/CLDevice.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CMainFrame, CMDIFrameWndEx )

BEGIN_MESSAGE_MAP( CMainFrame, CMDIFrameWndEx )
	ON_WM_CREATE( )
	ON_WM_SETTINGCHANGE( )

	ON_COMMAND( ID_TOOLS_DEVICESTATS, &CMainFrame::OnDeviceStats )
	ON_COMMAND( ID_VIEW_OUTPUTPANE, &CMainFrame::OnShowOutputPane )
	ON_COMMAND( ID_VIEW_HISTOGRAM, &CMainFrame::OnShowHistogramPane )

	ON_COMMAND( ID_CL_RUNCODE, &CMainFrame::OnRunKernel )
	ON_COMMAND( ID_RUN_KERNEL, &CMainFrame::OnRunKernel )

	ON_UPDATE_COMMAND_UI( ID_RUN_KERNEL, &CMainFrame::OnUpdateRunKernel )
END_MESSAGE_MAP( )



CMainFrame::CMainFrame( )
{
	EnableLoadDockState( FALSE );
	GetDockingManager( )->DisableRestoreDockState( );

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
	{
		mdiTabParams.m_bActiveTabCloseButton = TRUE;
		mdiTabParams.m_bTabIcons = FALSE;
		mdiTabParams.m_bAutoColor = FALSE;
		mdiTabParams.m_bDocumentMenu = TRUE;
		mdiTabParams.m_nTabBorderSize = 0;
		mdiTabParams.m_bFlatFrame = TRUE;
	}
	EnableMDITabbedGroups( TRUE, mdiTabParams );

	if( !m_wndMenuBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY ) ||
		!m_wndMenuBar.LoadToolBar( IDR_MAINFRAME )
		)
	{
		TRACE0( "Failed to create menubar\n" );
		return -1;
	}

	CMFCPopupMenu::SetForceMenuFocus( FALSE );

	if( !m_wndToolBar.CreateEx( this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar( IDR_CODE_EDIT )
		)
	{
		TRACE0( "Failed to create toolbar\n" );
		return -1;
	}

	int nIndex = m_wndToolBar.CommandToIndex( ID_SELECT_PROCESSOR );
	m_wndToolBar.SetButtonInfo( nIndex, ID_SELECT_PROCESSOR, TBBS_SEPARATOR, 205 );

	CRect rect;
	m_wndToolBar.GetItemRect( nIndex, &rect );
	rect.top = 1;
	rect.right = 400;
	rect.bottom = rect.top + 350;
	if( !m_wndDevice.Create( CBS_DROPDOWNLIST | CBS_SORT | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL | WS_CHILD, rect, &m_wndToolBar, ID_SELECT_PROCESSOR ) )
	{
		TRACE( "Failed to create combo-box\n" );
		return FALSE;
	}
	m_wndDevice.SetFont( &afxGlobalData.fontRegular );


	if( !m_wndStatusBar.Create( this ) )
	{
		TRACE0( "Failed to create status bar\n" );
		return -1;
	}

	static const UINT indicators[ ] = { ID_SEPARATOR, };
	m_wndStatusBar.SetIndicators( indicators, ARRAYSIZE( indicators ) );


	EnableDocking( CBRS_ALIGN_ANY );

	DockPane( &m_wndMenuBar );
	DockPane( &m_wndToolBar );


	CMFCVisualManager::SetDefaultManager( RUNTIME_CLASS( CVisualStyle ) );
	CVisualStyle::SetStyle( CMFCVisualManagerOffice2007::Office2007_ObsidianBlack );
	CDockingManager::SetDockingMode( DT_SMART );

	EnableAutoHidePanes( CBRS_ALIGN_ANY );


	if( !CreateDockingWindows( ) )
	{
		TRACE0( "Failed to create docking windows\n" );
		return -1;
	}


	ScanOpenCLDevices( );

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
	else if( message == WM_COMMAND )
	{
		//Route all accelerator commands to the current focused CWnd
		void* pExtra = reinterpret_cast< void* >( lParam );

		if( HIWORD( wParam ) == 1 )
		{
			const auto pFocus = GetFocus( );

			if( pFocus )
			{
				const auto pOwner = pFocus->GetOwner( );
				if( pOwner && pOwner->OnCmdMsg( LOWORD( wParam ), CN_COMMAND, pExtra, nullptr ) )
				{
					*pResult = 0;
					return TRUE;
				}
			}
		}
	}

	return CMDIFrameWndEx::OnWndMsg( message, wParam, lParam, pResult );
}

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

	if( !m_wndHistogram.Create(
		L"Histogram",
		this,
		{ 0, 0, rcWindow.Width( ) / 6, 250 },
		TRUE,
		170,
		WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI
	) )
	{
		TRACE0( "failed to create histogram pane" );
		return FALSE;
	}


	HICON hOutputBarIcon = LoadResourceIcon( IDI_OUTPUT_WND_HC );

	m_wndOutput.SetIcon( hOutputBarIcon, FALSE );


	m_wndHistogram.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndHistogram );

	m_wndOutput.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndOutput );


	m_wndImage.EnableDocking( CBRS_ALIGN_ANY );
	DockPane( &m_wndImage );


	ModifyStyle( 0, FWS_PREFIXTITLE );


	return TRUE;
}

void CMainFrame::ScanOpenCLDevices( )
{
	gEnv->pClManger->InitializeAsync( ).then( [ & ] {

		RunUIThread( [ & ]
		{
			for( const auto i : gEnv->pClManger->GetDevices( ) )
			{
				CString szDeviceName;
				szDeviceName.Format( L"%s (%s)", i->GetDeviceName( ), i->GetPlatform( )->m_szVendor );
				int nPos = m_wndDevice.AddString( szDeviceName );

				m_wndDevice.SetItemDataPtr( nPos, i );
			}

			RunUIThread( [ & ]
			{
				//=> Update Width
				int nNumEntries = m_wndDevice.GetCount( );
				int nWidth = 0;
				CString str;


				CDC* dc = m_wndDevice.GetDC( );
				dc->SelectObject( m_wndDevice.GetFont( ) );

				int nScrollWidth = ::GetSystemMetrics( SM_CXVSCROLL );
				for( int i = 0; i < nNumEntries; i++ )
				{
					m_wndDevice.GetLBText( i, str );
					int nLength = dc->GetTextExtent( str ).cx + nScrollWidth;
					nWidth = max( nWidth, nLength );
				}

				nWidth += dc->GetTextExtent( L"0" ).cx;
				m_wndDevice.SetDroppedWidth( nWidth );

				int nIndex = m_wndToolBar.CommandToIndex( ID_SELECT_PROCESSOR );
				m_wndToolBar.SetButtonInfo( nIndex, ID_SELECT_PROCESSOR, TBBS_SEPARATOR, 205 );

				CRect rect;
				m_wndToolBar.GetItemRect( nIndex, &rect );

				m_wndDevice.SetWindowPos( nullptr, rect.left, rect.top, nWidth, rect.bottom, SWP_NOACTIVATE | SWP_NOZORDER );

				m_wndDevice.SetCurSel( 0 );
			} );
		} );
	} );
}



void CMainFrame::OnSettingChange( UINT uFlags, LPCTSTR lpszSection )
{
	CMDIFrameWndEx::OnSettingChange( uFlags, lpszSection );
}

void CMainFrame::OnDeviceStats( )
{
	CDeviceInfoDialog dlg;

	dlg.DoModal( );
}

void CMainFrame::OnShowOutputPane( )
{
	if( !m_wndOutput.IsWindowVisible( ) )
	{
		m_wndOutput.ShowPane( TRUE, FALSE, FALSE );
	}
}

void CMainFrame::OnShowHistogramPane( )
{
	if( !m_wndHistogram.IsWindowVisible( ) )
	{
		m_wndHistogram.ShowPane( TRUE, FALSE, FALSE );
	}
}

void CMainFrame::OnRunKernel( )
{
	const auto pChild = MDIGetActive( );
	const auto pDoc = pChild->GetActiveDocument( );

	if( pDoc && pDoc->IsKindOf( RUNTIME_CLASS( CCodeDoc ) ) )
	{
		const auto pCodeDoc = DYNAMIC_DOWNCAST( CCodeDoc, pDoc );
		auto pDevice = GetSelectedDevice( );

		pDevice->BuildCode( pCodeDoc->GetCode( ) );
	}
}

void CMainFrame::OnUpdateRunKernel( CCmdUI* pCmdUI )
{
	const auto pDevice = GetSelectedDevice( );

	if( pDevice )
	{
		pCmdUI->Enable( !pDevice->IsDeviceInUse( ) );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}
