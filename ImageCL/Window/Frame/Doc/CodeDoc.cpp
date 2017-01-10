#include "stdafx.h"
#include "CodeDoc.h"

#include "Window/Frame/View/CodeView.h"



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
	{
		return FALSE;
	}



	GetView( )->GetEditCtrl( ).SetText( 
		StringToWstring( 
			LoadTextFromResource( IDR_CODE_TEMPLATE_EMPTY ) 
		).c_str()
	);

	return TRUE;
}

void CCodeDoc::Serialize( CArchive& ar )
{
	GetView( )->Serialize( ar );
}

CCodeView* CCodeDoc::GetView( )
{
	POSITION pos = GetFirstViewPosition( );

	while( pos )
	{
		CView* pView = GetNextView( pos );

		if( pView->IsKindOf( RUNTIME_CLASS( CCodeView ) ) )
		{
			return reinterpret_cast< CCodeView* >( pView );
		}
	}

	return nullptr;
}
