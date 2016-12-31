#include "stdafx.h"
#include "CodeDoc.h"

IMPLEMENT_DYNCREATE( CCodeDoc, CDocument )

BEGIN_MESSAGE_MAP( CCodeDoc, CDocument )
END_MESSAGE_MAP( )



CCodeDoc::CCodeDoc( )
{ }

CCodeDoc::~CCodeDoc( )
{ }

BOOL CCodeDoc::OnNewDocument( )
{
	if( !CDocument::OnNewDocument( ) )
		return FALSE;


	return TRUE;
}

void CCodeDoc::Serialize( CArchive& ar )
{
	if( ar.IsStoring( ) )
	{

	}
	else
	{

	}
}
