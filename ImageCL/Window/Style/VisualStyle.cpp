#include "stdafx.h"
#include "VisualStyle.h"

using namespace Gdiplus;


IMPLEMENT_DYNCREATE( CVisualStyle, CMFCVisualManagerOffice2007 );



CVisualStyle::CVisualStyle( )
{
	m_clrLightBkgn = RGB( 45, 45, 48 );
	m_crlBkgn = RGB( 30, 30, 30 );
	m_crlBorder = RGB( 63, 63, 70 );


	m_brLightBkgn.CreateSolidBrush( m_clrLightBkgn );
	m_brLightBorder.CreateSolidBrush( m_crlBorder );
	m_brBkgn.CreateSolidBrush( m_crlBkgn );

}

CVisualStyle::~CVisualStyle( )
{ }

void CVisualStyle::OnDrawMiniFrameBorder( CDC* pDC, CPaneFrameWnd* pFrameWnd, CRect rectBorder, CRect rectBorderSize )
{
	pDC->FillSolidRect( rectBorder, RGB( 200, 200, 200 ) );
}



void CVisualStyle::OnFillHighlightedArea( CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton )
{
	CMFCVisualManagerOffice2007::OnFillHighlightedArea( pDC, rect, pBrush, pButton );
}

void CVisualStyle::OnDrawMenuBorder( CDC* pDC, CMFCPopupMenu* pMenu, CRect rect )
{
	ASSERT_VALID( pDC );
	ASSERT_VALID( pMenu );

	const BOOL bRTL = pMenu->GetExStyle( ) & WS_EX_LAYOUTRTL;


	pDC->Draw3dRect( rect, m_crlBorder, m_crlBorder );
	rect.DeflateRect( 1, 1 );
	pDC->Draw3dRect( rect, m_crlBorder, m_crlBorder );
}

void CVisualStyle::OnFillBarBackground( CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea )
{
	if( pBar->IsKindOf( RUNTIME_CLASS( CMFCMenuBar ) ) ||
		pBar->IsKindOf( RUNTIME_CLASS( CMFCToolBar ) )
		)
	{
		pDC->FillRect( rectClient, &m_brLightBkgn );
		return;
	}

	auto cls = pBar->GetRuntimeClass( );

	CMFCVisualManagerOffice2007::OnFillBarBackground( pDC, pBar, rectClient, rectClip, bNCArea );
}

void CVisualStyle::OnDrawSplitterBorder( CDC* pDC, CSplitterWndEx* pSplitterWnd, CRect rect )
{
	ASSERT_VALID( pDC );

	pDC->Draw3dRect( rect, m_crlBorder, m_crlBorder );
	rect.InflateRect( -AFX_CX_BORDER, -AFX_CY_BORDER );
	pDC->Draw3dRect( rect, m_crlBorder, m_crlBorder );
}

void CVisualStyle::OnFillSplitterBackground( CDC* pDC, CSplitterWndEx* pSplitterWnd, CRect rect )
{
	ASSERT_VALID( pDC );

	pDC->FillRect( rect, &m_brLightBkgn );
}

void CVisualStyle::OnDrawFloatingToolbarBorder( CDC* pDC, CMFCBaseToolBar* pToolBar, CRect rectBorder, CRect rectBorderSize )
{
	CBrush br( RGB( 255, 0, 0 ) );

	pDC->FillRect( rectBorder, &br );
}



void CVisualStyle::OnDrawTab( CDC* pDC, CRect rectTab, int iTab, BOOL bIsActive, const CMFCBaseTabCtrl* pTabWnd )
{
	ASSERT_VALID( pTabWnd );
	ASSERT_VALID( pDC );


	if( pTabWnd->IsActiveTabCloseButton( ) && bIsActive )
	{
		CRect rectClose = pTabWnd->GetTabCloseButton( );
		rectTab.right = rectClose.left;

		OnDrawTabCloseButton( pDC, rectClose, pTabWnd, pTabWnd->IsTabCloseButtonHighlighted( ), pTabWnd->IsTabCloseButtonPressed( ), FALSE );
	}


	if( bIsActive )
	{
		pDC->FillSolidRect( rectTab, RGB( 0, 122, 204 ) );

	}
	else
	{
		pDC->FillSolidRect( rectTab, RGB( 45, 45, 48 ) );

	}

	CString strText;
	pTabWnd->GetTabLabel( iTab, strText );



	pDC->SetTextColor( RGB( 255, 255, 255 ) );

	pDC->DrawTextW( strText, rectTab, DT_VCENTER );

}


void CVisualStyle::OnDrawTabCloseButton( CDC* pDC, CRect rect, const CMFCBaseTabCtrl* pTabWnd, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled )
{
	CMFCVisualManagerOffice2007::OnDrawTabCloseButton( pDC, rect, pTabWnd, bIsHighlighted, bIsPressed, bIsDisabled );
}

COLORREF CVisualStyle::GetMenuItemTextColor( CMFCToolBarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled )
{
	if( bDisabled )
	{
		return RGB( 101, 101, 101 );
	}
	
	return RGB( 241, 241, 241 );
}
