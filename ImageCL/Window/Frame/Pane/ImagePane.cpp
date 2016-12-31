#include "stdafx.h"
#include "ImagePane.h"

#include "Resource.h"



BEGIN_MESSAGE_MAP( CImagePane, CDockablePane )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_CONTEXTMENU( )

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

	m_pImageView = reinterpret_cast< CImageView* >( 
		RUNTIME_CLASS( CImageView )->CreateObject( ) 
		);

	if( !m_pImageView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect( 0, 0, 500, 500 ), this, AFX_IDW_PANE_FIRST ) )
	{
		return -1;
	}


	return 0;
}

void CImagePane::OnSize( UINT nType, int cx, int cy )
{
	CDockablePane::OnSize( nType, cx, cy );

	m_pImageView->SetWindowPos( nullptr, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER );

}

void CImagePane::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CMenu menu;
	menu.LoadMenu( IDR_OUTPUT_POPUP );

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
