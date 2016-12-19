#pragma once

#include "Window/Frame/Pane/OutputWnd.h"



class CMainFrame : public CFrameWndEx
{
protected:
	CMainFrame( );
	DECLARE_DYNCREATE( CMainFrame )
public:
	virtual ~CMainFrame( );

private:
	virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs, CCreateContext* pContext );
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );

	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );

public:
#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

private:
	BOOL CreateDockingWindows( );

protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnViewCustomize( );
	afx_msg LRESULT OnToolbarCreateNew( WPARAM wp, LPARAM lp );
	afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	DECLARE_MESSAGE_MAP( )


protected:
	CMFCMenuBar			m_wndMenuBar;
	COutputWnd			m_wndOutput;
		
	CSplitterWnd		m_wndSplitter;
	bool				m_bSplitterInitialized = false;


};


