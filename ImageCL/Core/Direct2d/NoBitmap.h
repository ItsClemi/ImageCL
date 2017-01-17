#pragma once


class CNoBitmap
{
	const float SAMPLE_IMAGE_WIDTH = 300.0f;
	const float SAMPLE_IMAGE_HEIGHT = 300.0f;

public:
	CNoBitmap( );
	~CNoBitmap( );

public:
	void OnCreate( CHwndRenderTarget* pTarget );
	
private:
	void CreateDevice( const CD2DSizeF& targetDpi );
	void DrawBitmap( );

	void CopyBitmapBuffer( );

public:
// 	const auto GetSize( ) const
// 	{
// 		UINT nWidth = 0, nHeight = 0;
// 
// 		HRESULT hr = m_pWICBitmap->GetSize( &nWidth, &nHeight ) ;
// 
// 		ASSERT( SUCCEEDED( hr ) );
// 
// 		return CD2DSizeF( 
// 			static_cast< float >( nWidth ), 
// 			static_cast< float >( nHeight ) 
// 		);
// 	}
 
	inline ID2D1Bitmap* GetBitmap( ) const
	{
		return m_pBitmap;
	}

private:
	inline const auto GetWICBitmapSize( )
	{
		UINT nWidth = 0, nHeight = 0;
		ATLENSURE_SUCCEEDED(
			m_pWICBitmap->GetSize( &nWidth, &nHeight )
		);

		return CD2DSizeU( nWidth, nHeight );
	}


private:
	CString			m_szText = L"No Image";


	CComPtr< IWICBitmap >				m_pWICBitmap;
	CComPtr< ID2D1RenderTarget >		m_pRenderTarget;
	CComPtr< ID2D1Bitmap >				m_pBitmap;
	CComPtr< ID2D1SolidColorBrush >		m_pBrushOutline;
	CComPtr< ID2D1SolidColorBrush >		m_pBrushText;
	CComPtr< IDWriteTextFormat >		m_pTextFormat;

	
	std::vector< uint8_t >	m_vecBitmapData;

};