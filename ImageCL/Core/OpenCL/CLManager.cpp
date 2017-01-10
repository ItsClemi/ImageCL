#include "stdafx.h"
#include "CLManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE

static char THIS_FILE[ ] = __FILE__;
#endif



using namespace std;
using namespace Concurrency;




CCLManager::CCLManager( )
{

}

CCLManager::~CCLManager( )
{
	for( auto i : m_vecDevices )
	{
		SafeDelete( i );
	}

	for( auto i : m_vecPlatforms )
	{
		SafeDelete( i );
	}

	m_vecDevices.clear( );
	m_vecPlatforms.clear( );
}

task< void > CCLManager::InitializeAsync( )
{
	return create_task( [ & ] {
		LogInfo( L"Scanning OpenCL Devices..." );

		try
		{
			vector< cl::Platform > vecPlatforms;
			cl::Platform::get( &vecPlatforms );

			for( auto i : vecPlatforms )
			{
				SCLPlatform* pPlatform = new SCLPlatform( i );

				FetchPlatformInfo( pPlatform );

				std::vector< cl::Device > vecDevices;
				i.getDevices( CL_DEVICE_TYPE_ALL, &vecDevices );

				for( auto i : vecDevices )
				{
					SCLDevice* pDevice = new SCLDevice( pPlatform, i );

					FetchDeviceInfo( pDevice );

					LogInfo( L"Adding device %s", pDevice->m_szDeviceName );

					m_vecDevices.push_back( pDevice );
				}

				m_vecPlatforms.push_back( pPlatform );
			}
		}
		catch( const cl::Error& err )
		{
			LogError( L"OpenCL startup failed! %s", StringToWstring( err.what( ) ) );
			return;
		}

		LogSuccess( L"Found %I64d devices and %I64d Platforms!", m_vecDevices.size( ), m_vecPlatforms.size( ) );
	} );
}

void CCLManager::FetchPlatformInfo( SCLPlatform* pPlatform )
{
	auto szPlatformName = pPlatform->m_clPlatform.getInfo< CL_PLATFORM_NAME >( );
	pPlatform->m_szPlatformName = StringToWstring( szPlatformName ).c_str( );

	auto szVendor = pPlatform->m_clPlatform.getInfo< CL_PLATFORM_VENDOR >( );
	pPlatform->m_szVendor = StringToWstring( szVendor ).c_str( );

	auto szVersion = pPlatform->m_clPlatform.getInfo< CL_PLATFORM_VERSION >( );
	RemoveWhiteSpace( szVersion );

	pPlatform->m_szVersion = StringToWstring( szVersion ).c_str( );
}

void CCLManager::FetchDeviceInfo( SCLDevice* pDevice )
{
	pDevice->m_eType = static_cast< eDeviceType >(
		pDevice->m_clDevice.getInfo< CL_DEVICE_TYPE >( )
		);

	{
		auto szDeviceName = pDevice->m_clDevice.getInfo< CL_DEVICE_NAME >( );

		RemoveWhiteSpace( szDeviceName );
		pDevice->m_szDeviceName = StringToWstring( szDeviceName ).c_str( );
	}

	{
		auto szExtensions = pDevice->m_clDevice.getInfo< CL_DEVICE_EXTENSIONS >( );
		RemoveWhiteSpace( szExtensions );

		wistringstream s( StringToWstring( szExtensions ) );
		vector< wstring > vecExtensions = {
			istream_iterator< wstring, wchar_t >{ s },
			istream_iterator< wstring, wchar_t >{ }
		};

		for( auto i : vecExtensions )
		{
			pDevice->m_vecExtensions.push_back( i.c_str( ) );
		}
	}

	{
		pDevice->m_nComputeUnits = pDevice->m_clDevice.getInfo< CL_DEVICE_MAX_COMPUTE_UNITS >( );
		pDevice->m_nMaxWorkGroups = pDevice->m_clDevice.getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE >( );
		pDevice->m_nLocalMemorySize = pDevice->m_clDevice.getInfo< CL_DEVICE_LOCAL_MEM_SIZE >( );
		pDevice->m_nGlobalMemorySize = pDevice->m_clDevice.getInfo< CL_DEVICE_GLOBAL_MEM_SIZE >( );
		pDevice->m_bDeviceAvilable = pDevice->m_clDevice.getInfo< CL_DEVICE_AVAILABLE >( );
		pDevice->m_bCompilerAvailable = pDevice->m_clDevice.getInfo< CL_DEVICE_COMPILER_AVAILABLE >( );
	}
}