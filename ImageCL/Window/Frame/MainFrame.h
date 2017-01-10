#pragma once

#include "Window/Frame/Pane/OutputPane.h"
#include "Window/Frame/Pane/ImagePane.h"
#include "Window/Frame/Pane/HistogramPane.h"




class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNCREATE( CMainFrame )

public:
	CMainFrame( );
	virtual ~CMainFrame( );

private:
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs ) override;

	virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult ) override;


private:
	BOOL CreateDockingWindows( );

protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );

	afx_msg void OnSettingChange( UINT uFlags, LPCTSTR lpszSection );

	afx_msg void OnDeviceStats( );
	afx_msg void OnShowOutputPane( );
	afx_msg void OnShowHistogramPane( );

	afx_msg void OnRunKernel( );


	DECLARE_MESSAGE_MAP( )


protected:
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar			m_wndToolBar;
	CComboBox			m_wndDevice;
	

	CMFCStatusBar		m_wndStatusBar;


	

	COutputPane			m_wndOutput;
	CImagePane			m_wndImage;
	CHistogramPane		m_wndHistogram;


};


