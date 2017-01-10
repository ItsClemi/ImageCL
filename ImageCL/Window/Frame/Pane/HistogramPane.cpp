#include "stdafx.h"
#include "HistogramPane.h"


IMPLEMENT_DYNCREATE( CHistogramPane, CDockablePane )

BEGIN_MESSAGE_MAP( CHistogramPane, CDockablePane )
	ON_WM_CREATE( )
	ON_WM_ERASEBKGND( )

	ON_COMMAND( WM_UPDATE_HISTOGRAM, &CHistogramPane::OnUpdateHistogram )

END_MESSAGE_MAP( )


CHistogramPane::CHistogramPane( )
{

}

CHistogramPane::~CHistogramPane( )
{

}

int CHistogramPane::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if( CDockablePane::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	EnableD2DSupport( );

	return 0;
}

void CHistogramPane::OnUpdateHistogram( )
{

}

BOOL CHistogramPane::OnEraseBkgnd( CDC* pDC )
{
	return FALSE;
}

