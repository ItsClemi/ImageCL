#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       



class CApp : public CWinAppEx
{
public:
	CApp( );

private:
	virtual BOOL InitInstance( ) override;
	virtual int ExitInstance( ) override;

	virtual void PreLoadState( ) override;
	virtual void LoadCustomState( ) override;
	virtual void SaveCustomState( ) override;


private:
	afx_msg void OnFileNew( );
	afx_msg void OnFileOpen( );

	afx_msg void OnAppAbout( );

	DECLARE_MESSAGE_MAP( )

private:
	ULONG_PTR	m_gdiToken;

};


extern CApp theApp;
