#pragma once

enum class eDeviceType : cl_device_type
{
	eDefault = CL_DEVICE_TYPE_DEFAULT,
	eCpu = CL_DEVICE_TYPE_CPU,
	eGpu = CL_DEVICE_TYPE_GPU,
	eAccelerator = CL_DEVICE_TYPE_ACCELERATOR,
	eCustom = CL_DEVICE_TYPE_CUSTOM,
};

class CCLPlatform;

class CCLDevice
{
public:
	explicit CCLDevice( cl::Device clDevice, CCLPlatform* pParent, size_t nId )
		: m_clDevice( clDevice ), m_pParent( pParent ), m_nId( nId )
	{ }


public:
	inline const auto GetName( ) const
	{
		return StringToWstring(
			m_clDevice.getInfo< CL_DEVICE_NAME >( )
		);
	}

	inline auto GetType( ) const
	{
		return static_cast< eDeviceType >(
			m_clDevice.getInfo< CL_DEVICE_TYPE >( )
			);
	}

	inline const auto GetTypeString( ) const
	{
		switch( GetType( ) )
		{
			case eDeviceType::eDefault:		return L"<Default>";
			case eDeviceType::eCpu:			return L"CPU";
			case eDeviceType::eGpu:			return L"GPU";
			case eDeviceType::eAccelerator:	return L"Accelerator";
			case eDeviceType::eCustom:		return L"<Custom>";
			default:						return L"<Invalid Device Type>";
		}
	}

	inline auto GetMaxComputeUnits( ) const
	{
		return m_clDevice.getInfo< CL_DEVICE_MAX_COMPUTE_UNITS >( );
	}

	inline auto GetMaxWorkGroupSize( ) const
	{
		return m_clDevice.getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE >( );
	}

	inline auto GetLocalMemorySize( ) const
	{
		return m_clDevice.getInfo< CL_DEVICE_LOCAL_MEM_SIZE >( );
	}
	
	inline auto GetGlobalMemorySize( ) const
	{
		return m_clDevice.getInfo< CL_DEVICE_GLOBAL_MEM_SIZE >( );
	}

	inline auto IsDeviceAvailable( ) const
	{
		return m_clDevice.getInfo< CL_DEVICE_AVAILABLE >( );
	}

	inline auto IsCompilerAvailable( ) const
	{
		return m_clDevice.getInfo< CL_DEVICE_COMPILER_AVAILABLE >( );
	}

	inline const auto GetDeviceExtensions( ) const
	{
		return StringToWstring(
			m_clDevice.getInfo< CL_DEVICE_EXTENSIONS >( )
		);
	}

	inline size_t GetId( ) const
	{
		return m_nId;
	}

	inline CCLPlatform* GetParentPlatform( ) const
	{
		return m_pParent;
	}

public:
	inline void ForDeviceExtension( std::function< void( std::wstring& szExtension ) > f )
	{
		const auto szExtensions = GetDeviceExtensions( );

		std::wistringstream stream( szExtensions );
		std::vector< std::wstring > vecExtensions = {
			std::istream_iterator< std::wstring, wchar_t >{ stream },
			std::istream_iterator< std::wstring, wchar_t >{ }
		};

		for( auto i : vecExtensions )
		{
			f( i );
		}
	}

private:
	cl::Device		m_clDevice;
	CCLPlatform*	m_pParent;

	size_t			m_nId;
};



class CCLPlatform
{
	friend class CCLManager;

public:
	explicit CCLPlatform( cl::Platform clPlatform )
		: m_clPlatform( clPlatform )
	{ }

public:
	inline const auto GetName( ) const
	{
		return StringToWstring(
			m_clPlatform.getInfo< CL_PLATFORM_NAME >( )
		);
	}

	inline const auto GetVendor( ) const
	{
		return StringToWstring(
			m_clPlatform.getInfo< CL_PLATFORM_VENDOR >( )
		);
	}

	inline const auto GetPlatformVersion( ) const
	{
		return StringToWstring(
			m_clPlatform.getInfo< CL_PLATFORM_VERSION >( )
		);
	}

	inline const auto GetPlatformExtensions( ) const
	{
		return StringToWstring(
			m_clPlatform.getInfo< CL_PLATFORM_EXTENSIONS >( )
		);
	}

public:
	inline void ForExtension( std::function< void( std::wstring& szExtension ) > f )
	{
		const auto szExtensions = GetPlatformExtensions( );

		std::wistringstream stream( szExtensions );
		std::vector< std::wstring > vecExtensions = {
			std::istream_iterator< std::wstring, wchar_t >{ stream },
			std::istream_iterator< std::wstring, wchar_t >{ }
		};

		for( auto i : vecExtensions )
		{
			f( i );
		}
	}


private:
	cl::Platform		m_clPlatform;

	std::vector< cl::Device >	m_vecDevices;
};


class CCLManager
{
public:
	CCLManager( );
	~CCLManager( );

public:
	void InitializeAsync( );


public:
	inline void ForPlatform( std::function< void( const CCLPlatform* pPlatform ) > f )
	{
		ASSERT( IsInitialized( ) );

		for( auto i : m_vecPlatforms )
		{
			f( i );
		}
	}

	inline void ForDevices( std::function< void( const CCLDevice* pDevice ) > f )
	{
		ASSERT( IsInitialized( ) );

		for( auto i : m_vecDevices )
		{
			f( i );
		}
	}

	// 	inline cl::Device GetDeviceByName( const std::wstring& szDeviceName )
	// 	{
	// 		ASSERT( IsInitialized( ) );
	// 
	// 
	// 
	// 	}

	inline CCLDevice* GetDeviceById( size_t nId )
	{
		ASSERT( IsInitialized( ) );

		for( auto i : m_vecDevices )
		{
			if( i->GetId( ) == nId )
			{
				return i;
			}
		}

		return nullptr;
	}

	inline CCLDevice* GetDeviceAt( size_t nIndex )
	{
		ASSERT( IsInitialized( ) );

		return m_vecDevices[ nIndex ];
	}

	inline bool IsInitialized( )
	{
		return m_bInitialized.load( std::memory_order::memory_order_relaxed );
	}

private:
	std::atomic_bool				m_bInitialized = false;

	std::vector< CCLPlatform* >		m_vecPlatforms;
	std::vector< CCLDevice* >		m_vecDevices;

};
