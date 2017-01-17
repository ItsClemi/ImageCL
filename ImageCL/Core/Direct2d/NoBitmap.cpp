#include "stdafx.h"
#include "NoBitmap.h"

CNoBitmap::CNoBitmap( )
{
}

CNoBitmap::~CNoBitmap( )
{
}

void CNoBitmap::OnCreate( CHwndRenderTarget* pTarget )
{
	const auto dpi = pTarget->GetDpi( );
	CreateDevice( dpi );

	ATLENSURE_SUCCEEDED(
		m_pRenderTarget->CreateSolidColorBrush(
			CRenderTarget::COLORREF_TO_D2DCOLOR( RGB( 120, 120, 120 ) ),
			&m_pBrushOutline
		)
	);

	ATLENSURE_SUCCEEDED(
		m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF( D2D1::ColorF::White ),
			&m_pBrushText
		)
	);

	ATLENSURE_SUCCEEDED(
		AfxGetD2DState( )->GetWriteFactory( )->CreateTextFormat(
			L"Verdana",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			D2dMulDivF( 16.0f, dpi.width, 96.0f ),
			L"",
			&m_pTextFormat
		)
	);

	ATLENSURE_SUCCEEDED(
		m_pTextFormat->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_CENTER )
	);
	
	ATLENSURE_SUCCEEDED( 
		m_pTextFormat->SetParagraphAlignment( DWRITE_PARAGRAPH_ALIGNMENT_CENTER ) 
	);

	DrawBitmap( );
	CopyBitmapBuffer( );

	ATLENSURE_SUCCEEDED(
		pTarget->GetHwndRenderTarget( )->CreateBitmapFromWicBitmap( 
			m_pWICBitmap,
			D2D1::BitmapProperties(
				D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE )

			),
			&m_pBitmap
		)
	);
}

void CNoBitmap::CreateDevice( const CD2DSizeF& targetDpi )
{
	UINT nWidth = static_cast< UINT >( D2dMulDivF( SAMPLE_IMAGE_WIDTH, targetDpi.width, 96.0f ) );
	UINT nHeight = static_cast< UINT >( D2dMulDivF( SAMPLE_IMAGE_HEIGHT, targetDpi.width, 96.0f ) );

	const auto pD2DState = AfxGetD2DState( );
	const auto pImageFactory = pD2DState->GetWICFactory( );
	ASSERT( pImageFactory );

	ATLENSURE_SUCCEEDED(
		pImageFactory->CreateBitmap(
			nWidth,
			nHeight,
			GUID_WICPixelFormat32bppBGR,
			WICBitmapCacheOnLoad,
			&m_pWICBitmap
		)
	);

	ATLENSURE_SUCCEEDED(
		pD2DState->GetDirect2dFactory( )->CreateWicBitmapRenderTarget(
			m_pWICBitmap,
			D2D1::RenderTargetProperties( 
				D2D1_RENDER_TARGET_TYPE_DEFAULT, 
				D2D1::PixelFormat( ), 
				targetDpi.width, 
				targetDpi.height 
			),
			&m_pRenderTarget
		)
	);
}

void CNoBitmap::DrawBitmap( )
{
	m_pRenderTarget->BeginDraw( );
	{
		m_pRenderTarget->SetTransform( D2D1::Matrix3x2F::Identity( ) );
		m_pRenderTarget->SetTextAntialiasMode( D2D1_TEXT_ANTIALIAS_MODE_DEFAULT );

		m_pRenderTarget->Clear( CHwndRenderTarget::COLORREF_TO_D2DCOLOR( RGB( 40, 40, 40 ) ) );

		float fDpiWidth, fDpiHeight;
		m_pRenderTarget->GetDpi( &fDpiWidth, &fDpiHeight );

		const auto size = m_pRenderTarget->GetSize( );

		float fWidth = D2dMulDivF( 1.0f, fDpiWidth, 96.0f );
		float fWidthHalf = fWidth / 2.0f;


		const auto rcImage = CD2DRectF(
			0.0f + fWidthHalf,
			0.0f + fWidthHalf,
			size.width - fWidthHalf,
			size.height - fWidthHalf
		);

		m_pRenderTarget->DrawRectangle( rcImage, m_pBrushOutline, fWidth );

		m_pRenderTarget->DrawTextW(
			m_szText,
			m_szText.GetLength( ),
			m_pTextFormat,
			&rcImage,
			m_pBrushText
		);
	}
	ATLENSURE_SUCCEEDED(
		m_pRenderTarget->EndDraw( )
	);
}

void CNoBitmap::CopyBitmapBuffer( )
{
	const auto size = GetWICBitmapSize( );

	WICRect rcLock = { 
		0, 
		0, 
		static_cast< INT >( size.width ),
		static_cast< INT >( size.height )
	};

	CComPtr< IWICBitmapLock > pLock;
	ATLENSURE_SUCCEEDED(
		m_pWICBitmap->Lock( &rcLock, WICBitmapLockWrite, &pLock )
	);


	UINT cbBufferSize = 0;
	uint8_t* pData = nullptr;

	ATLENSURE_SUCCEEDED(
		pLock->GetDataPointer( &cbBufferSize, &pData )
	);

	m_vecBitmapData.resize( cbBufferSize );
	memcpy_s( &m_vecBitmapData[ 0 ], cbBufferSize, pData, cbBufferSize );
}