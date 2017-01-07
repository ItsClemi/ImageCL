#pragma once

class CImageView : public CScrollView
{
	DECLARE_DYNCREATE( CImageView );

public:
	CImageView( );
	virtual ~CImageView( );

private:
	virtual void OnDraw( CDC* pDC ) override;

public:
	void UpdateImage( CD2DBitmap* pBitmap );
	void SafeImageToFile( const CString& szPath );

private:
	afx_msg int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );

	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );

	DECLARE_MESSAGE_MAP( );


public:
	inline CHwndRenderTarget* GetRenderTarget( )
	{
		return &m_renderTarget;
	}


private:
	CHwndRenderTarget	m_renderTarget;
	CD2DBitmap*			m_pBitmap = nullptr;

};