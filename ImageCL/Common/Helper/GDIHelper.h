#pragma once

namespace Gdiplus
{
	inline const wchar_t* GdiStatusToString( Gdiplus::Status status )
	{
		switch( status )
		{
			case Gdiplus::Ok:							return L"Ok";
			case Gdiplus::GenericError:					return L"GenericError";
			case Gdiplus::InvalidParameter:				return L"InvalidParameter";
			case Gdiplus::OutOfMemory:					return L"OutOfMemory";
			case Gdiplus::ObjectBusy:					return L"ObjectBusy";
			case Gdiplus::InsufficientBuffer:			return L"InsufficientBuffer";
			case Gdiplus::NotImplemented:				return L"NotImplemented";
			case Gdiplus::Win32Error:					return L"Win32Error";
			case Gdiplus::WrongState:					return L"WrongState";
			case Gdiplus::Aborted:						return L"Aborted";
			case Gdiplus::FileNotFound:					return L"FileNotFound";
			case Gdiplus::ValueOverflow:				return L"ValueOverflow";
			case Gdiplus::AccessDenied:					return L"AccessDenied";
			case Gdiplus::UnknownImageFormat:			return L"UnknownImageFormat";
			case Gdiplus::FontFamilyNotFound:			return L"FontFamilyNotFound";
			case Gdiplus::FontStyleNotFound:			return L"FontStyleNotFound";
			case Gdiplus::NotTrueTypeFont:				return L"NotTrueTypeFont";
			case Gdiplus::UnsupportedGdiplusVersion:	return L"UnsupportedGdiplusVersion";
			case Gdiplus::GdiplusNotInitialized:		return L"GdiplusNotInitialized";
			case Gdiplus::PropertyNotFound:				return L"PropertyNotFound";
			case Gdiplus::PropertyNotSupported:			return L"PropertyNotSupported";
		}

		return L"<Invalid Status>";
	}

	inline auto RectToRectF( CRect& rc )
	{
		return RectF( 
			static_cast< REAL >( rc.left ), 
			static_cast< REAL >( rc.top ), 
			static_cast< REAL >( rc.Width( ) ), 
			static_cast< REAL >( rc.Height( ) )
 		);
	}

}