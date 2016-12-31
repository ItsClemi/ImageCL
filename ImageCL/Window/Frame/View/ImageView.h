#pragma once

class CImageRenderViewToolbar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI( CFrameWnd*, BOOL bDisableIfNoHndler )
	{
		CMFCToolBar::OnUpdateCmdUI( ( CFrameWnd* )GetOwner( ), bDisableIfNoHndler );
	}

	virtual BOOL AllowShowOnList( ) const
	{
		return FALSE;
	}
};


class CImageView : public CView
{
protected:
	DECLARE_DYNCREATE( CImageView );
	CImageView( );

public:
	virtual ~CImageView( );


	virtual void OnDraw( CDC* pDC ) override;

private:
	afx_msg	int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );



	afx_msg void OnImageOpen( );

	DECLARE_MESSAGE_MAP( );



private:	
	CImageRenderViewToolbar		m_wndToolBar;


};