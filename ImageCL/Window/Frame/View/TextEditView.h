#pragma	once

#include <Window/Ctrl/ScintillaCtrl.h>


class CTextEditToolBar : public CMFCToolBar
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

class CTextEditView : public CView
{
	DECLARE_DYNCREATE( CTextEditView );

public:
	CTextEditView( );
	virtual ~CTextEditView( );

protected:
	virtual void OnDraw( CDC* pDC ) override;


private:
	void SetAStyle( int style, COLORREF fore, COLORREF back = RGB( 30, 30, 30 ), int size = -1, const char *face = '\0' );

private:
	afx_msg	int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );


	afx_msg void OnEditCopy( );
	afx_msg void OnEditCut( );
	afx_msg void OnEditPaste( );
	afx_msg void OnEditClear( );
	afx_msg void OnEditUndo( );
	afx_msg void OnEditRedo( );
	afx_msg void OnEditSelectAll( );
	afx_msg void OnEditFind( );
	afx_msg void OnEditReplace( );
	afx_msg void OnEditRepeat( );

	DECLARE_MESSAGE_MAP( );


private:
	CTextEditToolBar		m_wndToolBar;
	CScintillaCtrl			m_wndEdit;
	CStatusBar				m_wndStatusBar;
	CComboBox				m_wndZoomLevel;

};