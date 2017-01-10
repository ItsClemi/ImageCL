#pragma once

enum class eDeviceType : cl_device_type
{
	eDefault = CL_DEVICE_TYPE_DEFAULT,
	eCpu = CL_DEVICE_TYPE_CPU,
	eGpu = CL_DEVICE_TYPE_GPU,
	eAccelerator = CL_DEVICE_TYPE_ACCELERATOR,
	eCustom = CL_DEVICE_TYPE_CUSTOM,
};

struct SCLPlatform
{
	SCLPlatform( cl::Platform clPlatform )
		: m_clPlatform( clPlatform )
	{

	}


	CString			m_szPlatformName;
	CString			m_szVendor;
	CString			m_szVersion;

	cl::Platform	m_clPlatform;
};

struct SCLDevice
{
	SCLDevice( SCLPlatform* pPlatform, cl::Device clDevice )
		: m_pPlatform( pPlatform ), m_clDevice( clDevice )
	{

	}

	inline const auto GetTypeName( ) const
	{
		switch( m_eType )
		{
			case eDeviceType::eDefault:		return L"<Default>";
			case eDeviceType::eCpu:			return L"CPU";
			case eDeviceType::eGpu:			return L"GPU";
			case eDeviceType::eAccelerator:	return L"Accelerator";
			case eDeviceType::eCustom:		return L"<Custom>";
			default:						return L"<Invalid Device Type>";
		}
	}


	SCLPlatform*	m_pPlatform;
	cl::Device		m_clDevice;

	CString			m_szDeviceName;
	eDeviceType		m_eType;

	cl_uint			m_nComputeUnits;
	size_t			m_nMaxWorkGroups;
	cl_ulong		m_nLocalMemorySize;
	cl_ulong		m_nGlobalMemorySize;
	cl_bool			m_bDeviceAvilable;
	cl_bool			m_bCompilerAvailable;

	std::vector< CString >	m_vecExtensions;
};



class CCLManager
{
public:
	CCLManager( );
	~CCLManager( );

public:
	Concurrency::task< void > InitializeAsync( );


private:
	void FetchPlatformInfo( SCLPlatform* pPlatform );
	void FetchDeviceInfo( SCLDevice* pDevice );


public:
	inline void EnumDevices( std::function< void( const SCLDevice* pDevice ) > f )
	{
 		for( auto i : m_vecDevices )
 		{
 			f( i );
 		}
	}

	inline const auto GetDevices( ) const
	{
		return m_vecDevices;
	}

private:
	inline void RemoveWhiteSpace( std::string& szStr )
	{
		if( szStr.length( ) <= 2 )
		{
			return;
		}

		for( size_t i = szStr.length( ) - 2; i >= 0; i-- )
		{
			if( szStr.at( i ) == ' ' )
			{
				szStr.erase( szStr.begin( ) + i );
			}
			else
			{
				break;
			}
		}
	}



private:

	std::vector< SCLPlatform* >		m_vecPlatforms;
	std::vector< SCLDevice* >		m_vecDevices;


};
