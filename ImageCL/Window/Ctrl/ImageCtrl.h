#pragma once

class CImageCtrl : public CWnd
{
public:
	CImageCtrl( );
	~CImageCtrl( );


private:
	afx_msg int OnCreate( LPCREATESTRUCT lpcs );
	afx_msg LRESULT OnDraw2D( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP( );


private:
	CD2DSolidColorBrush*		m_pD2DBackground;

	CScrollBar					m_wndVertical;
	CScrollBar					m_wndHorizontal;
};