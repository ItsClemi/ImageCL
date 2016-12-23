#include "stdafx.h"
#include <afxwinappex.h>
#include <afxdialogex.h>
#include "App.h"
#include "Window/Frame/MainFrame.h"

#include "Window/Frame/Doc/ImageDoc.h"
#include "Window/Frame/View/ImageView.h"
#include "Window/Dialog/AboutDialog.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




BEGIN_MESSAGE_MAP( CApp, CWinAppEx )
	ON_COMMAND( ID_APP_ABOUT, &CApp::OnAppAbout )

	ON_COMMAND( ID_FILE_NEW, &CWinAppEx::OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, &CWinAppEx::OnFileOpen )
END_MESSAGE_MAP( )


CApp::CApp( )
{
	SetAppID( L"ImageCL.AppID.NoVersion" );
}


BOOL CApp::InitInstance( )
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance( );


	if( !EnableD2DSupport( D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED ) )
	{
		AfxMessageBox( L"Failed to enable d2d!" );
		return FALSE;
	}

	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	if( Gdiplus::GdiplusStartup( &m_gdiToken, &input, &output ) != Gdiplus::Status::Ok )
	{
		AfxMessageBox( L"Failed to init GDIP!" );
		return FALSE;
	}

	if( !Scintilla_RegisterClasses( AfxGetApp( )->m_hInstance ) )
	{
		AfxMessageBox( L"Failed to register scintilla" );
		return FALSE;
	}

	EnableTaskbarInteraction( FALSE );

	if( !AfxInitRichEdit2( ) )
	{
		AfxMessageBox( L"Failed to initialize richedit!" );
		return FALSE;
	}


	SetRegistryKey( L"Local AppWizard-Generated Applications" );
	LoadStdProfileSettings( 0 );

	InitContextMenuManager( );
	InitKeyboardManager( );
	InitTooltipManager( );

	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager( )->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );

	CSingleDocTemplate* pDocTemplate = new CSingleDocTemplate( IDR_MAINFRAME, RUNTIME_CLASS( CImageDoc ), RUNTIME_CLASS( CMainFrame ), RUNTIME_CLASS( CImageView ) );
	if( !pDocTemplate )
	{
		return FALSE;
	}

	AddDocTemplate( pDocTemplate );


	CCommandLineInfo cmdInfo;
	ParseCommandLine( cmdInfo );
	if( !ProcessShellCommand( cmdInfo ) )
	{
		return FALSE;
	}

	m_pMainWnd->ShowWindow( SW_SHOWMAXIMIZED );
	m_pMainWnd->UpdateWindow( );

	return TRUE;
}


int CApp::ExitInstance( )
{
	Gdiplus::GdiplusShutdown( m_gdiToken );
	Scintilla_ReleaseResources( );


	return CWinAppEx::ExitInstance( );
}





void CApp::PreLoadState( )
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString( IDS_EDIT_MENU );
	ASSERT( bNameValid );
	GetContextMenuManager( )->AddMenu( strName, IDR_POPUP_EDIT );
	bNameValid = strName.LoadString( IDS_EXPLORER );
	ASSERT( bNameValid );
	GetContextMenuManager( )->AddMenu( strName, IDR_POPUP_EXPLORER );
}

void CApp::LoadCustomState( )
{ }

void CApp::SaveCustomState( )
{ }


void CApp::OnAppAbout( )
{
	CAboutDialog aboutDlg;
	aboutDlg.DoModal( );
}


CApp theApp;



