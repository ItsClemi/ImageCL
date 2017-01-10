#include "stdafx.h"
#include "App.h"

#include "Window/Frame/MainFrame.h"
#include "Window/Frame/ChildFrame.h"

#include "Window/Frame/Doc/CodeDoc.h"
#include "Window/Frame/View/CodeView.h"

#include "Window/Dialog/AboutDialog.h"

#include "Window/Style/VisualStyle.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP( CApp, CWinAppEx )
	ON_COMMAND( ID_FILE_NEW, &CWinAppEx::OnFileNew )
	ON_COMMAND( ID_FILE_OPEN, &CApp::OnFileOpen )

	ON_COMMAND( ID_APP_ABOUT, &CApp::OnAppAbout )
END_MESSAGE_MAP( )

CApp gApp;

CApp::CApp( )
{
	m_bSaveState = FALSE;

	SetAppID( L"ImageCL" );
}

BOOL CApp::InitInstance( )
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof( InitCtrls );
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx( &InitCtrls );

	CWinAppEx::InitInstance( );

	InitializeSystem( );

	Gdiplus::GdiplusStartupInput input;
	Gdiplus::GdiplusStartupOutput output;
	if( Gdiplus::GdiplusStartup( &m_gdiToken, &input, &output ) != Gdiplus::Status::Ok )
	{
		AfxMessageBox( L"Failed to init GDI+!" );
		return FALSE;
	}

	if( !Scintilla_RegisterClasses( m_hInstance ) )
	{
		AfxMessageBox( L"Failed to register scintilla" );
		return FALSE;
	}

	if( !AfxInitRichEdit2( ) )
	{
		AfxMessageBox( L"Failed to load RichEdit2" );
		return FALSE;
	}

	if( !EnableD2DSupport( D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_MULTI_THREADED ) )
	{
		AfxMessageBox( L"Failed to enable d2d" );
		return FALSE;
	}

	EnableTaskbarInteraction( FALSE );

	SetRegistryKey( L"ImageCL" );

	InitContextMenuManager( );
	InitKeyboardManager( );
	InitTooltipManager( );

	CMFCToolTipInfo ttParams;
	{
		ttParams.m_bVislManagerTheme = TRUE;
	}
	GetTooltipManager( )->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );
	
	auto pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME, 
		RUNTIME_CLASS( CCodeDoc ), 
		RUNTIME_CLASS( CChildFrame ),
		RUNTIME_CLASS( CCodeView ) 
	);

	AddDocTemplate( pDocTemplate );


	auto pMainFrame = new CMainFrame;
	if( !pMainFrame->LoadFrame( IDR_MAINFRAME ) )
	{
		SafeDelete( pMainFrame );
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	CCommandLineInfo cmdInfo;
	ParseCommandLine( cmdInfo );
	if( !ProcessShellCommand( cmdInfo ) )
	{
		return FALSE;
	}

	m_pMainWnd->ShowWindow( SW_SHOWMAXIMIZED );
	m_pMainWnd->UpdateWindow( );
	m_pMainWnd->DragAcceptFiles( );

	if( !IsWindows8OrGreater( ) )
	{
		LogInfo( L"Warning: old windows version detected! things might not work correctly!" );
	}


	RunUIThread( [ ] { 
		printf( "asdasd LOL" );
	
	} );
		
	return TRUE;
}

int CApp::ExitInstance( )
{
	Gdiplus::GdiplusShutdown( m_gdiToken );
	Scintilla_ReleaseResources( );


	return CWinAppEx::ExitInstance( );
}

BOOL CApp::OnIdle( LONG lCount )
{
	if( !CWinAppEx::OnIdle( lCount ) )
	{
		return FALSE;
	}

	gEnv->pTaskWorker->ProcessIdle( );

	return TRUE;
}


void CApp::OnFileOpen( )
{
 	CFileDialog	dlg(
 		TRUE,
 		L"OpenCL Files",
 		L"",
 		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
 		L"OpenCL Kernel|*.cl||"
 	);
 
 	if( dlg.DoModal( ) == IDOK )
 	{ 
		OpenDocumentFile( dlg.GetPathName( ) );
  	}
}

void CApp::OnAppAbout( )
{
	CAboutDialog aboutDlg;
	aboutDlg.DoModal( );
}



