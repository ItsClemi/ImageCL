#pragma once

#include "Core/Direct2d/RenderImage.h"

class CD2DBitmapEx;
class CImageView : public CScrollView
{
	DECLARE_DYNCREATE( CImageView );

public:
	CImageView( );
	virtual ~CImageView( );

private:
	virtual void OnDraw( CDC* pDC ) override;

public:
	void UpdateImage( CD2DBitmapEx* pBitmap );
	void SaveImageToFile( const CString& szPath );
	void SwitchImage( );

private:
	afx_msg int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message );

	afx_msg LRESULT OnDrawD2D( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP( );



private:
	CRenderImage		m_renderImage;
	CD2DPointF			m_ptCenter;

};