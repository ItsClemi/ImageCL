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
	virtual BOOL InitInstance( );
	virtual int ExitInstance( );

	virtual void PreLoadState( );
	virtual void LoadCustomState( );
	virtual void SaveCustomState( );

private:
	afx_msg void OnAppAbout( );

	DECLARE_MESSAGE_MAP( )

private:
	ULONG_PTR	m_gdiToken;

};


extern CApp theApp;
