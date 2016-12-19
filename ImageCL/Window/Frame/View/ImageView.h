#pragma once



class CImageDoc;
class CImageView : public CView
{
protected:
	CImageView( );
	DECLARE_DYNCREATE( CImageView )
public:
	virtual ~CImageView( );

public:
	CImageDoc* GetDocument( ) const;

public:
	virtual void OnDraw( CDC* pDC ) override;
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs ) override;

#ifdef _DEBUG
	virtual void AssertValid( ) const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

protected:
	DECLARE_MESSAGE_MAP( )

};

#ifndef _DEBUG  
inline CImageDoc* CImageView::GetDocument( ) const
{
	return reinterpret_cast< CImageDoc* >( m_pDocument );
}
#endif

