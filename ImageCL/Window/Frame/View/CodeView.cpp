#include "stdafx.h"
#include "CodeView.h"

#include <SciLexer.h>
#include <resource.h>

IMPLEMENT_DYNCREATE( CCodeView, CView );

BEGIN_MESSAGE_MAP( CCodeView, CView )
	ON_WM_CREATE( )
	ON_WM_SIZE( )
	ON_WM_ERASEBKGND( )

	ON_COMMAND( ID_EDIT_COPY, &CCodeView::OnEditCopy )
	ON_COMMAND( ID_EDIT_CUT, &CCodeView::OnEditCut )
	ON_COMMAND( ID_EDIT_PASTE, &CCodeView::OnEditPaste )
	ON_COMMAND( ID_EDIT_CLEAR, &CCodeView::OnEditClear )
	ON_COMMAND( ID_EDIT_UNDO, &CCodeView::OnEditUndo )
	ON_COMMAND( ID_EDIT_REDO, &CCodeView::OnEditRedo )
	ON_COMMAND( ID_EDIT_SELECT_ALL, &CCodeView::OnEditSelectAll )
	ON_COMMAND( ID_EDIT_FIND, &CCodeView::OnEditFind )
	ON_COMMAND( ID_EDIT_REPLACE, &CCodeView::OnEditReplace )
	ON_COMMAND( ID_EDIT_REPEAT, &CCodeView::OnEditRepeat )
END_MESSAGE_MAP( )

static const wchar_t cppKeyWords[ ] =
L"and and_eq asm auto bitand bitor bool break "
L"case catch char class compl const const_cast continue "
L"default delete do double else enum explicit export extern false float for "
L"friend goto if inline int long mutable namespace new not not_eq "
L"operator or or_eq private protected public "
L"register reinterpret_cast return short signed sizeof static static_cast struct switch "
L"template this throw true try typedef typeid typename union unsigned using "
L"virtual void volatile wchar_t while xor xor_eq "

//Extended
L"__cdecl "
L"__int8 __int16 __int32 __int64 __int128 "
L"__stdcall "

//OpenCL
L"__kernel __local __global "
L"get_global_id get_local_id get_group_id barrier "
L"CLK_LOCAL_MEM_FENCE CLK_GLOBAL_MEM_FENCE "

L"complex imaginary "

L"char2 char4 char8 char16 "
L"uchar2 uchar4 uchar8 uchar16 "
L"short2 short4 short8 short16 "
L"ushort2 ushort4 ushort8 ushort16"
L"int2 int4 int8 int16 "
L"uint2 uint4 uint8 uint16 "
L"long2 long4 long8 long16 "
L"ulong2 ulong4 ulong8 ulong16 "
L"float2 float4 float8 float16 "
L"double2 double4 double8 double16 "

L"bool2 bool4 bool8 bool16 "
L"half2 half4 half8 half16 "
L"quad2 quad4 quad8 quad16 "

L"float2x2 float2x4 float2x8 float2x16"
L"float4x2 float4x4 float4x8 float4x16"
L"float8x2 float8x4 float8x8 float8x16"
L"float16x2 float16x4 float16x8 float16x16"

L"double2x2 double2x4 double2x8 double2x16"
L"double4x2 double4x4 double4x8 double4x16"
L"double8x2 double8x4 double8x8 double8x16"
L"double16x2 double16x4 double16x8 double16x16"

;


CCodeView::CCodeView( )
{ }

CCodeView::~CCodeView( )
{ }

void CCodeView::OnDraw( CDC* pDC )
{ }

void CCodeView::Serialize( CArchive& ar )
{
	if( ar.IsLoading( ) )
	{
		m_wndEdit.Cancel( );
		m_wndEdit.SetUndoCollection( FALSE );

		CFile* pFile = ar.GetFile( );
		char Buffer[ 4096 ];
		int nBytesRead = 0;
		do
		{
			nBytesRead = pFile->Read( Buffer, 4096 );

			if( nBytesRead )
			{
				m_wndEdit.AddText( nBytesRead, Buffer );
			}

		} while( nBytesRead );

		if( ( ( GetFileAttributes( pFile->GetFilePath( ) ) & FILE_ATTRIBUTE_READONLY ) == FILE_ATTRIBUTE_READONLY ) )
		{
			m_wndEdit.SetReadOnly( TRUE );
		}
		else
		{
			m_wndEdit.SetReadOnly( FALSE );
		}

		m_wndEdit.SetUndoCollection( TRUE );
		m_wndEdit.EmptyUndoBuffer( );
		m_wndEdit.SetSavePoint( );
		m_wndEdit.GotoPos( 0 );
	}
	else
	{
		char Buffer[ 4096 ];
		int nDocLength = m_wndEdit.GetLength( );

		CFile* pFile = ar.GetFile( );
		for( int i = 0; i < nDocLength; i += 4095 ) 
		{
			int nGrabSize = nDocLength - i;
			if( nGrabSize > 4095 )
			{
				nGrabSize = 4095;
			}

			Sci_TextRange tr;
			{
				tr.chrg.cpMin = i;
				tr.chrg.cpMax = i + nGrabSize;
				tr.lpstrText = Buffer;
			}
			m_wndEdit.GetTextRange( &tr );

			pFile->Write( Buffer, nGrabSize );
		}
	}
}

void CCodeView::SetAStyle( int style, COLORREF fore, COLORREF back, int size, const char* face )
{
	m_wndEdit.StyleSetFore( style, fore );
	m_wndEdit.StyleSetBack( style, back );

	if( size >= 1 )
	{
		m_wndEdit.StyleSetSize( style, size );
	}

	if( face )
	{
		m_wndEdit.StyleSetFont( style, face );
	}
}

void CCodeView::SetEditColor( int nStyle, COLORREF clrForeground, COLORREF clrBackground )
{
	m_wndEdit.StyleSetFore( nStyle, clrForeground );
	m_wndEdit.StyleSetBack( nStyle, clrBackground );
}

int CCodeView::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CView::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}

	if( !m_wndEdit.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( ), this, 0 ) )
	{
		return -1;
	}


	m_wndEdit.SetLexer( SCLEX_CPP );

	m_wndEdit.SetKeyWords( 0, cppKeyWords );

	SetAStyle( STYLE_DEFAULT, RGB( 189, 183, 107 ), RGB( 30, 30, 30 ), 10, "Consolas" );

	SetEditColor( SCE_C_DEFAULT, RGB( 160, 0, 0 ) );
	SetEditColor( SCE_C_COMMENT, RGB( 87, 166, 74 ) );
	SetEditColor( SCE_C_COMMENTLINE, RGB( 87, 166, 74 ) );
	SetEditColor( SCE_C_COMMENTDOC, RGB( 87, 166, 74 ) );
	SetEditColor( SCE_C_COMMENTLINEDOC, RGB( 87, 166, 74 ) );
	SetEditColor( SCE_C_COMMENTDOCKEYWORD, RGB( 87, 166, 74 ) );
	SetEditColor( SCE_C_COMMENTDOCKEYWORDERROR, RGB( 87, 166, 74 ) );

	SetEditColor( SCE_C_NUMBER, RGB( 181, 206, 168 ) );

	SetEditColor( SCE_C_WORD, RGB( 86, 156, 214 ) );

	SetEditColor( SCE_C_STRING, RGB( 214, 157, 133 ) );
	SetEditColor( SCE_C_CHARACTER, RGB( 214, 157, 133 ) );

	SetEditColor( SCE_C_IDENTIFIER, RGB( 181, 206, 168 ) );

	SetEditColor( SCE_C_OPERATOR, RGB( 180, 180, 180 ) );



	m_wndEdit.SetSelBack( TRUE, RGB( 38, 79, 120 ) );

	m_wndEdit.SetCaretFore( RGB( 240, 240, 240 ) );

	m_wndEdit.StyleSetBold( SCE_C_OPERATOR, TRUE );
	m_wndEdit.StyleSetBold( SCE_C_WORD, TRUE );
	m_wndEdit.StyleSetItalic( SCE_C_IDENTIFIER, TRUE );

	m_wndEdit.SetTabWidth( 4 );

	m_wndEdit.SetProperty( "fold", "1" );
	m_wndEdit.SetProperty( "fold.compact", "0" );
	m_wndEdit.SetProperty( "fold.comment", "1" );
	m_wndEdit.SetProperty( "fold.preprocessor", "1" );
	m_wndEdit.SetProperty( "lexer.cpp.track.preprocessor", "0" );


	return 0;
}

void CCodeView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	m_wndEdit.SetWindowPos( nullptr, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER );
}

BOOL CCodeView::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}

void CCodeView::OnEditCopy( )
{
	m_wndEdit.Copy( );
}

void CCodeView::OnEditCut( )
{
	m_wndEdit.Cut( );
}

void CCodeView::OnEditPaste( )
{
	m_wndEdit.Paste( );
}

void CCodeView::OnEditClear( )
{
	m_wndEdit.Clear( );
}

void CCodeView::OnEditUndo( )
{
	m_wndEdit.Undo( );
}

void CCodeView::OnEditRedo( )
{
	m_wndEdit.Redo( );
}

void CCodeView::OnEditSelectAll( )
{
	m_wndEdit.SelectAll( );
}

void CCodeView::OnEditFind( )
{ }

void CCodeView::OnEditReplace( )
{ }

void CCodeView::OnEditRepeat( )
{ }


// 	CStatusBar				m_wndStatusBar;
// 	CComboBox				m_wndZoomLevel;
// 	CSize csZoomLevel( 50 );
// 	ScaleDpi( csZoomLevel );
// 
// 	if( !m_wndStatusBar.Create( this ) ||
// 		!m_wndZoomLevel.Create( WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SORT, { 0, 0, csZoomLevel.cx, 0 }, &m_wndStatusBar, 1 ) )
// 	{
// 		return -1;
// 	}
// 
// 	m_wndZoomLevel.AddString( L"100%" );
// 	m_wndZoomLevel.AddString( L"75%" );
// 	m_wndZoomLevel.AddString( L"50%" );
// 	
//int cyStatus = m_wndStatusBar.CalcFixedLayout( FALSE, TRUE ).cy;

//m_wndStatusBar.SetWindowPos( nullptr, 0, cy - ( cyStatus ), cx, cyStatus, SWP_NOACTIVATE | SWP_NOZORDER );

// 	CRect rcZoomLevel;
// 	m_wndZoomLevel.GetClientRect( rcZoomLevel );
// 
// 	m_wndZoomLevel.SetWindowPos( nullptr, 0, 1, rcZoomLevel.Width( ), cyStatus, SWP_NOACTIVATE | SWP_NOZORDER );