#pragma once

#include "Window/Frame/Pane/OutputPane.h"
#include "Window/Frame/Pane/ImagePane.h"




class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNCREATE( CMainFrame )

public:
	CMainFrame( );
	virtual ~CMainFrame( );

private:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs ) override;

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult ) override;

public:
#ifdef _DEBUG
	virtual void AssertValid( ) const override;
	virtual void Dump( CDumpContext& dc ) const override;
#endif

private:
	BOOL CreateDockingWindows( );

protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );

	afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );

	afx_msg void OnShowOutputPane( );
	afx_msg void OnStatusBarUpdate( void* ptr );

	DECLARE_MESSAGE_MAP( )


protected:
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar			m_wndCodeBar;
	CComboBox			m_comboBox;
	

	CMFCStatusBar		m_wndStatusBar;


	

	COutputPane			m_wndOutput;
	CImagePane			m_wndImage;


};


