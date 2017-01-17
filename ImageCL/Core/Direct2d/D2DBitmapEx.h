#pragma once

class CD2DBitmapEx
{
public:
	explicit CD2DBitmapEx( const CString& szPath );
	virtual ~CD2DBitmapEx( );

public:
	virtual HRESULT Create( CRenderTarget* pRenderTarget );

public:
	inline const auto GetWICBitmap( ) const
	{
		return m_pWICBitmap;
	}

	inline ID2D1Bitmap* Get( ) const
	{
		return m_pBitmap;
	}

private:
	CString			m_strPath;

	CComPtr< IWICBitmap >		m_pWICBitmap;
	CComPtr< ID2D1Bitmap >		m_pBitmap;
};