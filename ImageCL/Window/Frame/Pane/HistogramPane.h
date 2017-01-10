#pragma once

class CHistogramPane : public CDockablePane
{
	DECLARE_DYNCREATE( CHistogramPane )
	 
public:
	CHistogramPane( );
	virtual ~CHistogramPane( );

public:



private:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnUpdateHistogram( );
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );


	DECLARE_MESSAGE_MAP( );

};