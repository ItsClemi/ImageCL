#pragma once

class CImageView : public CScrollView
{
	DECLARE_DYNCREATE( CImageView );

public:
	CImageView( );
	virtual ~CImageView( );

private:
	virtual void OnUpdate( CView* pSender, LPARAM lHint, CObject* pHint ) override;
	virtual void OnDraw( CDC* pDC ) override;

private:
	afx_msg int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );

	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );


	DECLARE_MESSAGE_MAP( );


private:
	CHwndRenderTarget	m_renderTarget;


};