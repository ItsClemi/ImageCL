#include "stdafx.h"
#include "ImageView.h"

#include "Window/Frame/Doc/ImageDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE( CImageView, CView )

BEGIN_MESSAGE_MAP( CImageView, CView )
END_MESSAGE_MAP( )



CImageView::CImageView( )
{ }

CImageView::~CImageView( )
{ }

BOOL CImageView::PreCreateWindow( CREATESTRUCT& cs )
{
	return CView::PreCreateWindow( cs );
}

void CImageView::OnDraw( CDC* pDC )
{
	CImageDoc* pDoc = GetDocument( );
	ASSERT_VALID( pDoc );
}

#ifdef _DEBUG
void CImageView::AssertValid( ) const
{
	CView::AssertValid( );
}

void CImageView::Dump( CDumpContext& dc ) const
{
	CView::Dump( dc );
}


CImageDoc* CImageView::GetDocument( ) const
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CImageDoc ) ) );
	return ( CImageDoc* )m_pDocument;
}
#endif //_DEBUG
