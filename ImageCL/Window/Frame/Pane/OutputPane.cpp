#include "stdafx.h"
#include "OutputPane.h"

#include <Resource.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE

static char THIS_FILE[ ] = __FILE__;
#endif


BEGIN_MESSAGE_MAP( COutputPane, CDockablePane )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_CONTEXTMENU( )

	ON_COMMAND( WM_COPY, &COutputPane::OnCopyOutput )

	ON_COMMAND( ID_CLEAR_OUTPUT, &COutputPane::OnClearOutput )
	ON_COMMAND( ID_EDIT_COPY, &COutputPane::OnCopyOutput )
	ON_COMMAND( ID_EDIT_CLEAR, &COutputPane::OnClearOutput )

	ON_COMMAND_PTR( WM_ADD_OUTPUT, &COutputPane::OnAddOutput )

END_MESSAGE_MAP( )


COutputPane::COutputPane( )
{ }

COutputPane::~COutputPane( )
{ }

int COutputPane::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CDockablePane::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	if( !m_wndToolBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_OUTPUT_TOOLBAR )  ||
		!m_wndToolBar.LoadToolBar( IDR_OUTPUT_TOOLBAR, 0, 0, TRUE ) 
		)
	{
		return -1;
	}

	m_wndToolBar.SetOwner( this );
	m_wndToolBar.SetRouteCommandsViaFrame( FALSE );

	int cyToolbar = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;
	if( !m_wndOutput.Create(
		WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL | ES_NOOLEDRAGDROP | ES_READONLY,
		CRect( ),
		this,
		AFX_IDW_PANE_FIRST
	) )
	{
		return -1;
	}

	m_wndOutput.SetOwner( this );
	m_wndOutput.SetBackgroundColor( FALSE, RGB( 37, 37, 37 ) );

	return 0;
}

void COutputPane::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize( nType, cx, cy );

 	int cyToolbar = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;
 
 	m_wndToolBar.SetWindowPos( nullptr, 0, 0, cx, cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER );
 	m_wndOutput.SetWindowPos( nullptr, 0, cyToolbar, cx, cy - cyToolbar, SWP_NOACTIVATE | SWP_NOZORDER );
}

void COutputPane::OnContextMenu( CWnd* pWnd, CPoint point )
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

void COutputPane::OnClearOutput( )
{
	m_wndOutput.SetReadOnly( FALSE );
	{
		m_wndOutput.SetSel( 0, -1 );
		m_wndOutput.Clear( );
	}
	m_wndOutput.SetReadOnly( TRUE );
}

void COutputPane::OnCopyOutput( )
{
	m_wndOutput.Copy( );
}

void COutputPane::OnAddOutput( void* ptr )
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