#include "stdafx.h"
#include "D2DBitmapEx.h"


CD2DBitmapEx::CD2DBitmapEx( const CString& szPath )
{ 
	m_strPath = szPath;
}

CD2DBitmapEx::~CD2DBitmapEx( )
{
}

HRESULT CD2DBitmapEx::Create( CRenderTarget* pRenderTarget )
{
	HRESULT hr = S_OK;

	CComPtr< IWICBitmapDecoder > pDecoder;
	CComPtr< IWICBitmapFrameDecode > pSource;
	CComPtr< IWICFormatConverter > pConverter;

	hr = AfxGetD2DState( )->GetWICFactory( )->CreateDecoderFromFilename(
		m_strPath,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);

	if( FAILED( hr ) )
	{
		return hr;
	}

	if( SUCCEEDED( hr ) )
	{
		// Create the initial frame.
		hr = pDecoder->GetFrame( 0, &pSource );
		m_pWICBitmap = /*( IWICBitmap* )*/pSource;
	}

	if( SUCCEEDED( hr ) )
	{
		hr = AfxGetD2DState( )->GetWICFactory( )->CreateFormatConverter( &pConverter );
	}



	if( SUCCEEDED( hr ) )
	{
		hr = pConverter->Initialize(
			m_pWICBitmap,
			GUID_WICPixelFormat32bppBGR,
			WICBitmapDitherTypeNone,
			NULL,
			0.0,
			WICBitmapPaletteTypeMedianCut
		);
	}


	if( SUCCEEDED( hr ) )
	{
		hr = pRenderTarget->GetRenderTarget( )->CreateBitmapFromWicBitmap( pConverter, &m_pBitmap );
	}


	return hr;
}
