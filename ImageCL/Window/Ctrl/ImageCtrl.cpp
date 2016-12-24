#include "stdafx.h"
#include "ImageCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP( CImageCtrl, CWnd )
	ON_WM_CREATE( )
	//ON_WM_SIZE( ) 

END_MESSAGE_MAP( );

CImageCtrl::CImageCtrl( )
{ }

CImageCtrl::~CImageCtrl( )
{ }

int CImageCtrl::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CWnd::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}


	return 0;
}

