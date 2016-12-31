#pragma once

class CVisualStyle : public CMFCVisualManagerOffice2007
{
	DECLARE_DYNCREATE( CVisualStyle );
private:
	CVisualStyle( );
public:
	virtual ~CVisualStyle( );




public:
	virtual void OnFillHighlightedArea( CDC* pDC, CRect rect, CBrush* pBrush, CMFCToolBarButton* pButton ) override;

	virtual void OnDrawMenuBorder( CDC* pDC, CMFCPopupMenu* pMenu, CRect rect ) override;

	virtual void OnFillBarBackground( CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE ) override;

	virtual void OnFillSplitterBackground( CDC* pDC, CSplitterWndEx* pSplitterWnd, CRect rect ) override;

	virtual void OnDrawSplitterBorder( CDC* pDC, CSplitterWndEx* pSplitterWnd, CRect rect ) override;

	virtual void OnDrawFloatingToolbarBorder( CDC* pDC, CMFCBaseToolBar* pToolBar, CRect rectBorder, CRect rectBorderSize ) override;

public:
	virtual COLORREF GetMenuItemTextColor( CMFCToolBarMenuButton* pButton, BOOL bHighlighted, BOOL bDisabled ) override;


public:
	inline const COLORREF GetBackground( ) const
	{
		return m_crlBkgn;
	}

public:
	virtual BOOL IsToolbarRoundShape( CMFCToolBar* pToolBar ) override;


private:
	COLORREF	m_clrLightBkgn;
	COLORREF	m_crlBkgn;
	COLORREF	m_crlBorder;
	
	CBrush		m_brLightBkgn;
	CBrush		m_brBkgn;
	CBrush		m_brLightBorder;


};