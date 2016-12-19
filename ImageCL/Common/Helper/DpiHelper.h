#pragma once

inline void ScaleDpi( SIZE& size )
{
	const HDC hDC = GetDC( nullptr );
	const int dpiX = GetDeviceCaps( hDC, LOGPIXELSX );
	const int dpiY = GetDeviceCaps( hDC, LOGPIXELSY );
	ReleaseDC( nullptr, hDC );

	size.cx = MulDiv( size.cx, dpiX, 96 );
	size.cy = MulDiv( size.cy, dpiY, 96 );
}
