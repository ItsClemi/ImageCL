#pragma	once

#include <Window/Ctrl/ScintillaCtrl.h>


class CCodeDoc;
class CCodeView : public CView
{
	DECLARE_DYNCREATE( CCodeView );

public:
	CCodeView( );
	virtual ~CCodeView( );


protected:
	virtual void OnDraw( CDC* pDC ) override;

public:
	virtual void Serialize( CArchive& ar ) override;


private:
	void SetAStyle( int style, COLORREF fore, COLORREF back = RGB( 30, 30, 30 ), int size = -1, const char *face = '\0' );

	void SetEditColor( int nStyle, COLORREF clrForeground, COLORREF clrBackground = RGB( 30, 30, 30 ) );

private:
	afx_msg	int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );

	afx_msg void OnEditCopy( );
	afx_msg void OnEditCut( );
	afx_msg void OnEditPaste( );
	afx_msg void OnEditClear( );
	afx_msg void OnEditUndo( );
	afx_msg void OnEditRedo( );
	afx_msg void OnEditSelectAll( );
	afx_msg void OnEditFind( );
	afx_msg void OnEditReplace( );
	afx_msg void OnEditRepeat( );

	DECLARE_MESSAGE_MAP( );

public:
	inline CCodeDoc* GetDocument( )
	{
		return reinterpret_cast< CCodeDoc* >( m_pDocument );
	}

	inline CScintillaCtrl& GetEditCtrl( )
	{
		return m_wndEdit;
	}


private:
	CScintillaCtrl			m_wndEdit;

};