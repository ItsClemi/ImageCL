#pragma once

enum class eDeviceType : cl_device_type
{
	eDefault = CL_DEVICE_TYPE_DEFAULT,
	eCpu = CL_DEVICE_TYPE_CPU,
	eGpu = CL_DEVICE_TYPE_GPU,
	eAccelerator = CL_DEVICE_TYPE_ACCELERATOR,
	eCustom = CL_DEVICE_TYPE_CUSTOM,
};


struct SCLPlatform;
class CCLDevice
{
public:
	CCLDevice( SCLPlatform* pPlatform, cl::Device clDevice );
	~CCLDevice( );

public:
	void FetchDeviceInfo( );

	void BuildCode( const std::shared_ptr< std::vector< char > > vecCode );


public:
	inline const auto GetPlatform( ) const
	{
		return m_pPlatform;
	}

	inline const auto GetDevice( ) const
	{
		return m_clDevice;
	}

	inline const auto GetContext( ) const
	{
		return m_clContext;
	}

	inline const auto GetCommandQueue( ) const
	{
		return m_clCommandQueue;
	}

	inline const auto GetDeviceName( ) const
	{
		return m_szDeviceName;
	}

	inline const auto GetDeviceType( ) const
	{
		return m_eType;
	}

	inline const auto GetDeviceTypeString( ) const
	{
		switch( GetDeviceType( ) )
		{
			case eDeviceType::eDefault:		return L"<Default>";
			case eDeviceType::eCpu:			return L"CPU";
			case eDeviceType::eGpu:			return L"GPU";
			case eDeviceType::eAccelerator:	return L"Accelerator";
			case eDeviceType::eCustom:		return L"<Custom>";
			default:						return L"<Invalid Device Type>";
		}
	}

	inline const auto GetComputeUnits( ) const
	{
		return m_nComputeUnits;
	}

	inline const auto GetMaxWorkGroups( ) const
	{
		return m_nMaxWorkGroups;
	}

	inline const auto GetLocalMemorySize( ) const
	{
		return m_nLocalMemorySize;
	}

	inline const auto GetGlobalMemorySize( ) const
	{
		return m_nGlobalMemorySize;
	}

public:
	inline const auto IsDeviceAvilable( ) const
	{
		return m_bDeviceAvilable;
	}

	inline const auto IsCompilerAvilable( ) const
	{
		return m_bCompilerAvailable;
	}

	inline const auto IsDeviceInUse( ) const
	{
		return m_bDeviceInUse;
	}

public:
	inline void ForExtensions( std::function< void( const CString& szExtension ) > fn )
	{
		for( const auto& i : m_vecExtensions )
		{
			fn( i );
		}
	}

private:

	SCLPlatform*		m_pPlatform;
	cl::Device			m_clDevice;
	cl::Context			m_clContext;
	cl::CommandQueue	m_clCommandQueue;

	CString				m_szDeviceName;
	eDeviceType			m_eType;

	cl_uint				m_nComputeUnits;
	size_t				m_nMaxWorkGroups;
	cl_ulong			m_nLocalMemorySize;
	cl_ulong			m_nGlobalMemorySize;
	cl_bool				m_bDeviceAvilable;
	cl_bool				m_bCompilerAvailable;

	bool				m_bDeviceInUse = false;



	std::vector< CString >		m_vecExtensions;
};