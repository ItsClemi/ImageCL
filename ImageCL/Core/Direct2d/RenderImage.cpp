#include "stdafx.h"
#include "RenderImage.h"


CRenderImage::CRenderImage( )
{

}

CRenderImage::~CRenderImage( )
{

}

void CRenderImage::OnCreate( CHwndRenderTarget* pTarget )
{
	m_noBitmap.OnCreate( pTarget );

}

void CRenderImage::OnLoadImage( CD2DBitmapEx* pBitmap )
{
	if( m_pBitmap )
	{
		
	}

}

void CRenderImage::OnSwitchImage( )
{

}
