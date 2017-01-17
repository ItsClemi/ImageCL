#pragma once

#include "D2DBitmapEx.h"
#include "NoBitmap.h"


class CRenderImage
{
public:
	CRenderImage( );
	~CRenderImage( );



public:
	void OnCreate( CHwndRenderTarget* pTarget );

	void OnLoadImage( CD2DBitmapEx* pBitmap );

	void OnSwitchImage( );

public:
	inline CD2DSizeF GetSize( ) const
	{
		auto size = GetBitmap( )->GetSize( );

		if( IsBitmapLoaded( ) )
		{			
			ScaleDpi( size );
		}
		
		return size;
	}

	inline ID2D1Bitmap* GetBitmap( ) const
	{
		if( IsBitmapLoaded( ) )
		{
			return m_pBitmap->Get( );
		}

		return m_noBitmap.GetBitmap( );
	}

	inline CSize GetScrollSize( )
	{
		auto size = GetSize( );
		ScaleDpi( size );

		return CSize( 
			static_cast< int >( size.width ),
			static_cast< int >( size.height )
		);
	}

private:
	inline const bool IsBitmapLoaded( ) const
	{
		return m_pBitmap != nullptr;
	}


private:
	CD2DBitmapEx*		m_pBitmap = nullptr;

	CNoBitmap			m_noBitmap;




};