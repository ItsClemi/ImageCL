#include "stdafx.h"
#include "AboutDialog.h"


BEGIN_MESSAGE_MAP( CAboutDialog, CDialogEx )

END_MESSAGE_MAP( )


CAboutDialog::CAboutDialog( ) 
	: CDialogEx( IDD_ABOUTBOX )
{
}

BOOL CAboutDialog::OnInitDialog( )
{
	if( !CDialogEx::OnInitDialog( ) )
	{
		return FALSE;
	}

	SetIcon( AfxGetApp( )->LoadIconW( IDR_MAINFRAME ), FALSE );

	return TRUE;
}

void CAboutDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );

	CString szBuildInfo = L"Build: " _CRT_WIDE( __TIMESTAMP__ );
	CString szThirdParty = 
		L"Using Scintilla\r\n"
		L"Copyright 1998 - 2003 by Neil Hodgson <neilh@scintilla.org>\r\n"
		L"\r\n"
		L"ScintillaCtrl.CPP\r\n"
		L"Copyright (c) 2004 - 2016 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)\r\n"
		L"\r\n"
		L"Help:\r\n"
		L"lua-studio (https://github.com/luaforge/lua-studio)"
		;

	DDX_Text( pDX, IDC_BUILD_INFO, szBuildInfo );
	DDX_Text( pDX, IDC_ABOUT_TPS, szThirdParty );
}

