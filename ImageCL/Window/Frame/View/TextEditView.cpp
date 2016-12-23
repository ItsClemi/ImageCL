#include "stdafx.h"
#include "TextEditView.h"

#include <SciLexer.h>
#include <resource.h>

IMPLEMENT_DYNCREATE( CTextEditView, CView );

BEGIN_MESSAGE_MAP( CTextEditView, CView )
	ON_WM_CREATE( )
	ON_WM_SIZE( )

	ON_COMMAND( ID_EDIT_COPY, &CTextEditView::OnEditCopy )
	ON_COMMAND( ID_EDIT_CUT, &CTextEditView::OnEditCut )
	ON_COMMAND( ID_EDIT_PASTE, &CTextEditView::OnEditPaste )
	ON_COMMAND( ID_EDIT_CLEAR, &CTextEditView::OnEditClear )
	ON_COMMAND( ID_EDIT_UNDO, &CTextEditView::OnEditUndo )
	ON_COMMAND( ID_EDIT_REDO, &CTextEditView::OnEditRedo )
	ON_COMMAND( ID_EDIT_SELECT_ALL, &CTextEditView::OnEditSelectAll )
	ON_COMMAND( ID_EDIT_FIND, &CTextEditView::OnEditFind )
	ON_COMMAND( ID_EDIT_REPLACE, &CTextEditView::OnEditReplace )
	ON_COMMAND( ID_EDIT_REPEAT, &CTextEditView::OnEditRepeat )
END_MESSAGE_MAP( )

static const wchar_t cppKeyWords[ ] =
L"and and_eq asm auto bitand bitor bool break "
L"case catch char class compl const const_cast continue "
L"default delete do double dynamic_cast else enum explicit export extern false float for "
L"friend goto if inline int long mutable namespace new not not_eq "
L"operator or or_eq private protected public "
L"register reinterpret_cast return short signed sizeof static static_cast struct switch "
L"template this throw true try typedef typeid typename union unsigned using "
L"virtual void volatile wchar_t while xor xor_eq "

//Extended
L"__asm __asume __based __box __cdecl __declspec "
L"__delegate delegate depreciated dllexport dllimport "
L"event __event __except __fastcall __finally __forceinline "
L"__int8 __int16 __int32 __int64 __int128 __interface "
L"interface __leave naked noinline __noop noreturn "
L"nothrow novtable nullptr safecast __stdcall "
L"__try __except __finally __unaligned uuid __uuidof "
L"__virtual_inheritance "

//OpenCL
L"__kernel __local __global "
L"get_global_id get_local_id get_group_id barrier "
L"CLK_LOCAL_MEM_FENCE CLK_GLOBAL_MEM_FENCE "
L"double2 double4 double8 double16 half2 half4 half8 half16 "
;


CTextEditView::CTextEditView( )
{ }

CTextEditView::~CTextEditView( )
{ }

void CTextEditView::SetAStyle( int style, COLORREF fore, COLORREF back, int size, const char* face )
{
	m_wndEdit.StyleSetFore( style, fore );
	m_wndEdit.StyleSetBack( style, back );

	if( size >= 1 )
		m_wndEdit.StyleSetSize( style, size );

	if( face )
		m_wndEdit.StyleSetFont( style, face );
}

int CTextEditView::OnCreate( LPCREATESTRUCT lpcs )
{
	if( CView::OnCreate( lpcs ) == -1 )
	{
		return -1;
	}

	if( !m_wndToolBar.Create( this, AFX_DEFAULT_TOOLBAR_STYLE ) )
	{
		return -1;
	}

	if( !m_wndToolBar.LoadToolBar( IDR_TEXT_EDIT_TOOLBAR, 0, 0, TRUE ) )
	{
		return -1;
	}

	if( !m_wndEdit.Create( WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect( 0, 0, 300, 400 ), this, 0 ) )
	{
		return -1;
	}

	m_wndEdit.SetLexer( SCLEX_CPP );

	m_wndEdit.SetKeyWords( 0, cppKeyWords );

	SetAStyle( STYLE_DEFAULT, RGB( 189, 183, 107 ), RGB( 30, 30, 30 ), 10, "Consolas" );

	SetAStyle( SCE_C_DEFAULT, RGB( 160, 0, 0 ) );
	SetAStyle( SCE_C_COMMENT, RGB( 87, 166, 74 ) );
	SetAStyle( SCE_C_COMMENTLINE, RGB( 87, 166, 74 ) );
	SetAStyle( SCE_C_COMMENTDOC, RGB( 87, 166, 74 ) );
	SetAStyle( SCE_C_COMMENTLINEDOC, RGB( 87, 166, 74 ) );
	SetAStyle( SCE_C_COMMENTDOCKEYWORD, RGB( 87, 166, 74 ) );
	SetAStyle( SCE_C_COMMENTDOCKEYWORDERROR, RGB( 87, 166, 74 ) );

	SetAStyle( SCE_C_NUMBER, RGB( 181, 206, 168 ) );

	SetAStyle( SCE_C_WORD, RGB( 86, 156, 214 ) );

	SetAStyle( SCE_C_STRING, RGB( 214, 157, 133 ) );
	SetAStyle( SCE_C_CHARACTER, RGB( 214, 157, 133 ) );

	SetAStyle( SCE_C_IDENTIFIER, RGB( 181, 206, 168 ) );

	SetAStyle( SCE_C_OPERATOR, RGB( 180, 180, 180 ) );


	m_wndEdit.SetSelBack( TRUE, RGB( 38, 79, 120 ) );

	m_wndEdit.SetCaretFore( RGB( 240, 240, 240 ) );

	m_wndEdit.StyleSetBold( SCE_C_OPERATOR, TRUE );
	m_wndEdit.StyleSetBold( SCE_C_WORD, TRUE );
	m_wndEdit.StyleSetItalic( SCE_C_IDENTIFIER, TRUE );


	return 0;
}

void CTextEditView::OnSize( UINT nType, int cx, int cy )
{
	CView::OnSize( nType, cx, cy );

	CRect rectClient;
	GetClientRect( rectClient );
	int cyTlb = m_wndToolBar.CalcFixedLayout( FALSE, TRUE ).cy;

	m_wndToolBar.SetWindowPos( NULL, 0, 0, cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER );
	m_wndEdit.SetWindowPos( NULL, 0, cyTlb, cx, cy - ( cyTlb ), SWP_NOACTIVATE | SWP_NOZORDER );

}

void CTextEditView::OnEditCopy( )
{
	m_wndEdit.Copy( );
}

void CTextEditView::OnEditCut( )
{
	m_wndEdit.Cut( );
}

void CTextEditView::OnEditPaste( )
{
	m_wndEdit.Paste( );
}

void CTextEditView::OnEditClear( )
{
	m_wndEdit.Clear( );
}

void CTextEditView::OnEditUndo( )
{
	m_wndEdit.Undo( );
}

void CTextEditView::OnEditRedo( )
{
	m_wndEdit.Redo( );
}

void CTextEditView::OnEditSelectAll( )
{
	m_wndEdit.SelectAll( );
}

void CTextEditView::OnEditFind( )
{ }

void CTextEditView::OnEditReplace( )
{ }

void CTextEditView::OnEditRepeat( )
{ }

void CTextEditView::OnDraw( CDC* pDC )
{ }

