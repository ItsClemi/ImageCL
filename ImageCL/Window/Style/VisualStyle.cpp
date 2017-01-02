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
	//pDC->FillSolidRect( rectBorder, RGB( 128, 128, 128 ) );
	CMFCVisualManagerOffice2007::OnDrawMiniFrameBorder( pDC, pFrameWnd, rectBorder, rectBorderSize );
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
	if( pTabWnd->IsActiveTabCloseButton( ) && bIsActive )
	{
		CRect rectClose = pTabWnd->GetTabCloseButton( );
		rectTab.right = rectClose.left;

		OnDrawTabCloseButton( pDC, rectClose, pTabWnd, pTabWnd->IsTabCloseButtonHighlighted( ), pTabWnd->IsTabCloseButtonPressed( ), FALSE /* Disabled */ );
	}


	if( pTabWnd->IsFlatTab( ) )
	{
		pDC->FillSolidRect( rectTab, RGB( 0, 0, 255 ) );

	}
	else
	{
		pDC->FillSolidRect( rectTab, RGB( 255, 0, 0 ) );

	}

	CString strText;
	pTabWnd->GetTabLabel( iTab, strText );


	CRect rcText = rectTab;
	pDC->SetTextColor( GetTabTextColor( pTabWnd, iTab, bIsActive ) );

	pDC->DrawText( strText, rcText, DT_CENTER );

}





COLORREF CVisualStyle::GetMenuItemTextColor( CMFCToolBarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled )
{
	if( bDisabled )
	{
		return RGB( 101, 101, 101 );
	}
	
	return RGB( 241, 241, 241 );
}


BOOL CVisualStyle::IsToolbarRoundShape( CMFCToolBar* pToolBar )
{
	return FALSE;
}

