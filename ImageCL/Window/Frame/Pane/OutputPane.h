#pragma once


class COutputToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI( CFrameWnd*, BOOL bDisableIfNoHndler )
	{
		CMFCToolBar::OnUpdateCmdUI( 
			static_cast< CFrameWnd* >( GetOwner( ) ), 
			bDisableIfNoHndler 
		);
	}

	virtual BOOL AllowShowOnList( ) const
	{
		return FALSE;
	}
};



class COutputPane : public CDockablePane
{
	enum { IDD_OUTPUT_EDIT = 105 };

public:
	COutputPane( );
	virtual ~COutputPane( );

private:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );
	
	afx_msg void OnCopyOutput( );
	afx_msg void OnClearOutput( );

	afx_msg void OnAddOutput( void* ptr );

	DECLARE_MESSAGE_MAP( )

private:
	COutputToolBar		m_wndToolBar;
	CRichEditCtrl		m_wndOutput;

};