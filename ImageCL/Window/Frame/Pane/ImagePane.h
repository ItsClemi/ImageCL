#pragma once

#include "Window/Frame/View/ImageView.h"

class CImagePane : public CDockablePane
{
public:
	CImagePane( );
	virtual ~CImagePane( );

public:


private:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnContextMenu( CWnd* pWnd, CPoint point );

	DECLARE_MESSAGE_MAP( );


private:
	CImageView*		m_pImageView;

};