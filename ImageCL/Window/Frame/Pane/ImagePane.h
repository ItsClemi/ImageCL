#pragma once

#include "Window/Frame/View/ImageView.h"


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



class CImagePane : public CDockablePane
{
public:
	CImagePane( );
	virtual ~CImagePane( );

public:


private:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );

	afx_msg void OnImageOpen( );
	afx_msg void OnImageSave( );
	afx_msg void OnImageSwitch( );

	afx_msg void OnUpdateImage( void* ptr );

	DECLARE_MESSAGE_MAP( );


private:
	CImageRenderViewToolbar		m_wndToolBar;
	CImageView*					m_pImageView;

};