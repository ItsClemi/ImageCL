#pragma once

//> Note: Doesn't support negative values. (Used for DPI scaling)
inline constexpr auto D2dMulDivF( const float fNumber, const float fNumerator, const float fDenominator = 96.0f )
{
	return ( ( fNumber * fNumerator ) + ( fDenominator / 2.0f ) ) / fDenominator;
}


inline void ScaleDpi( SIZE& size )
{
	const HDC hDC = GetDC( nullptr );
	const int dpiX = GetDeviceCaps( hDC, LOGPIXELSX );
	const int dpiY = GetDeviceCaps( hDC, LOGPIXELSY );
	ReleaseDC( nullptr, hDC );

	size.cx = MulDiv( size.cx, dpiX, 96 );
	size.cy = MulDiv( size.cy, dpiY, 96 );
}

inline void ScaleDpi( D2D1_SIZE_F& size, const D2D1_SIZE_F dpi )
{
	size.width = D2dMulDivF( size.width, dpi.width );
	size.height = D2dMulDivF( size.height, dpi.height );
}

inline void ScaleDpi( D2D1_SIZE_F& size )
{
	FLOAT x, y;
	AfxGetD2DState( )->GetDirect2dFactory( )->GetDesktopDpi( &x, &y );

	ScaleDpi( size, { x, y } );
}