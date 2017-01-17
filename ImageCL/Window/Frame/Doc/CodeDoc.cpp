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

BOOL CCodeDoc::IsModified( )
{
	return GetView( )->GetEditCtrl( ).GetModify( );
}

const std::shared_ptr< std::vector< char > > CCodeDoc::GetCode( )
{
	const auto pView = GetView( );
	const int nCodeLen = pView->GetEditCtrl( ).GetLength( );

	auto pData = std::make_shared< std::vector< char > >( nCodeLen + 1 );

	for( int i = 0; i < nCodeLen; i += 4095 )
	{
		int nGrabSize = nCodeLen - i;
		if( nGrabSize > 4095 )
		{
			nGrabSize = 4095;
		}

		Sci_TextRange tr;
		{
			tr.chrg.cpMin = i;
			tr.chrg.cpMax = i + nGrabSize;
			tr.lpstrText = &pData->at( i );
		}
		pView->GetEditCtrl( ).GetTextRange( &tr );
	}
	
	return pData;
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
