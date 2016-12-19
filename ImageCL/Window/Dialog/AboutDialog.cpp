#include "stdafx.h"
#include "AboutDialog.h"


BEGIN_MESSAGE_MAP( CAboutDialog, CDialogEx )
END_MESSAGE_MAP( )


CAboutDialog::CAboutDialog( ) 
	: CDialogEx( IDD_ABOUTBOX )
{
}

void CAboutDialog::DoDataExchange( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange( pDX );
}

