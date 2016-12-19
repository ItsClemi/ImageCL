#include "stdafx.h"
#include "ImageDoc.h"

#include "App.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE( CImageDoc, CDocument )

BEGIN_MESSAGE_MAP( CImageDoc, CDocument )
END_MESSAGE_MAP( )


CImageDoc::CImageDoc( )
{ }

CImageDoc::~CImageDoc( )
{ }

BOOL CImageDoc::OnNewDocument( )
{
	return CDocument::OnNewDocument( );
}

void CImageDoc::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) )
	{
	}
	else
	{
	}
}

#ifdef _DEBUG

void CImageDoc::AssertValid( ) const
{
	CDocument::AssertValid( );
}

void CImageDoc::Dump( CDumpContext& dc ) const
{
	CDocument::Dump( dc );
}

#endif //_DEBUG

