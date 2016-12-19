#include "stdafx.h"

#include "OutputWnd.h"
#include <Resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE

static char THIS_FILE[ ] = __FILE__;
#endif


BEGIN_MESSAGE_MAP( COutputWnd, CDockablePane )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_CONTEXTMENU( )
	ON_WM_ERASEBKGND( )

	ON_COMMAND( ID_CLEAR_OUTPUT, &COutputWnd::OnClearOutput )

	ON_COMMAND_PTR( WM_ADD_OUTPUT, &COutputWnd::OnAddOutput )
END_MESSAGE_MAP( )


COutputWnd::COutputWnd( )
{ }

COutputWnd::~COutputWnd( )
{ }

int COutputWnd::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CDockablePane::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}


	CRect rcWindow;
	GetClientRect( &rcWindow );

	if( !m_wndToolBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_OUTPUT_TOOLBAR ) )
	{
		return -1;
	}

	if( !m_wndToolBar.LoadToolBar( IDR_OUTPUT_TOOLBAR, 0, 0, TRUE ) )
	{
		return -1;
	}

	m_wndToolBar.SetRouteCommandsViaFrame( FALSE );

	int cyToolbar = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;
	if( !m_wndOutput.Create(
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL | ES_NOOLEDRAGDROP | ES_READONLY,
		{ cyToolbar + 5, cyToolbar + 5, 100, 200 },
		this,
		AFX_IDW_PANE_FIRST
	) )
	{
		return -1;
	}

	m_wndOutput.SetBackgroundColor( FALSE, RGB( 37, 37, 37 ) );
	

	return 0;
}

void COutputWnd::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize( nType, cx, cy );

	CRect rcWindow;
	GetClientRect( &rcWindow );

	int cyToolbar = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;

	m_wndToolBar.SetWindowPos( nullptr, 0, 0, cx, cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER );
	m_wndOutput.SetWindowPos( nullptr, 0, cyToolbar + 5, cx, cy - ( cyToolbar + 5 ), SWP_NOACTIVATE | SWP_NOZORDER );
}

BOOL COutputWnd::OnEraseBkgnd( CDC* pDC )
{
	CRect rcWindow;
	GetClientRect( &rcWindow );

	auto pStyle = DYNAMIC_DOWNCAST( CMFCVisualManagerOffice2007, CMFCVisualManager::GetInstance( ) );
	{
		pStyle->OnFillBarBackground( pDC, this, rcWindow, { 0, 0, 0, 0 } );
	}

	return FALSE;
}

void COutputWnd::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CMenu menu;
	menu.LoadMenu( IDR_OUTPUT_POPUP );

	CMenu* pSumMenu = menu.GetSubMenu( 0 );

	CRect rcWindow;
	m_wndOutput.GetWindowRect( &rcWindow );

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

void COutputWnd::OnClearOutput( )
{
	m_wndOutput.SetReadOnly( FALSE );
	{
		m_wndOutput.SetSel( 0, -1 );
		m_wndOutput.Clear( );
	}
	m_wndOutput.SetReadOnly( TRUE );
}

void COutputWnd::OnAddOutput( void* ptr )
{
	auto pInfo = reinterpret_cast< SLogEntry* >( ptr );
	{
		CHARFORMAT cf;
		cf.cbSize = sizeof( CHARFORMAT );
		cf.dwMask = CFM_COLOR;
		cf.dwEffects = 0;

		switch( pInfo->m_eType )
		{
			case eLogType::eError:			cf.crTextColor = RGB( 255, 0, 0 );				break;
			case eLogType::eSuccess:		cf.crTextColor = RGB( 70, 255, 0 );				break;
			default:						cf.crTextColor = RGB( 160, 160, 160 );			break;
		}
	
		CString szMessage;
		{
			CTime time = pInfo->m_tm;
		
			szMessage += time.Format( L"<%H:%M:%S>" );
			szMessage += " ";
			szMessage += pInfo->m_szMessage;
			szMessage += L"\r\n";
		}

		int nInsertionPoint = m_wndOutput.GetWindowTextLength( );

		m_wndOutput.SetSel( nInsertionPoint, -1 );
		m_wndOutput.SetSelectionCharFormat( cf );
		m_wndOutput.ReplaceSel( szMessage );

		int nVisible = RichEditGetVisibleLines( &m_wndOutput );

		if( m_wndOutput.GetFocus( ) != &m_wndOutput )
		{
			m_wndOutput.LineScroll( INT_MAX );
			m_wndOutput.LineScroll( 1 - nVisible );
		}
	}

	SafeDelete( pInfo );
}