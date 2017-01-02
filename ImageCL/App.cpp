#include "stdafx.h"
#include "App.h"

#include "Window/Frame/MainFrame.h"
#include "Window/Frame/ChildFrame.h"

#include "Window/Frame/Doc/CodeDoc.h"
#include "Window/Frame/View/CodeView.h"

#include "Window/Dialog/AboutDialog.h"

#include "Core/TaskWorker.h"

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
	ttParams.m_bVislManagerTheme = TRUE;
	gApp.GetTooltipManager( )->SetTooltipParams( AFX_TOOLTIP_TYPE_ALL, RUNTIME_CLASS( CMFCToolTipCtrl ), &ttParams );
	
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

	m_pClManager->InitializeOpenCLAsync( );	

	return TRUE;
}

int CApp::ExitInstance( )
{
	Gdiplus::GdiplusShutdown( m_gdiToken );
	Scintilla_ReleaseResources( );


	return CWinAppEx::ExitInstance( );
}

void CApp::PreLoadState( )
{ }

void CApp::LoadCustomState( )
{ }

void CApp::SaveCustomState( )
{ }


void CApp::OnFileNew( )
{
	//auto pFrame = DYNAMIC_DOWNCAST( CMainFrame, m_pMainWnd );
	
	CWinAppEx::OnFileNew( );

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
		auto szPath = std::make_shared< std::wstring >( dlg.GetPathName( ) );



		//AfxGetMainWnd( )->PostMessage( );



		//AfxGetMainWnd()->SendMessage()
	}
}

void CApp::OnAppAbout( )
{
	CAboutDialog aboutDlg;
	aboutDlg.DoModal( );
}


CApp gApp;



