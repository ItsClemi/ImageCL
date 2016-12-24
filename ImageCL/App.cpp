#include "stdafx.h"
#include "App.h"

#include "Window/Frame/MainFrame.h"
#include "Window/Dialog/AboutDialog.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




BEGIN_MESSAGE_MAP( CApp, CWinAppEx )
	ON_COMMAND( ID_APP_ABOUT, &CApp::OnAppAbout )

	ON_COMMAND( ID_FILE_NEW, &CApp::OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, &CApp::OnFileOpen )
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

#ifdef _DEBUG
	AfxEnableMemoryTracking( TRUE );
#endif

	CWinAppEx::InitInstance( );

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


	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	if( !pFrame->LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL, NULL ) )
	{
		return FALSE;
	}


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
}

void CApp::LoadCustomState( )
{ }

void CApp::SaveCustomState( )
{ }


void CApp::OnFileNew( )
{

}

void CApp::OnFileOpen( )
{

}

void CApp::OnAppAbout( )
{
	CAboutDialog aboutDlg;
	aboutDlg.DoModal( );
}


CApp theApp;



