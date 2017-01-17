#include "stdafx.h"
#include "CLManager.h"

#include "CLDevice.h"


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
					CCLDevice* pDevice = new CCLDevice( pPlatform, i );
					{
						pDevice->FetchDeviceInfo( );
					}


					LogInfo( L"Adding %s device %s", pDevice->GetDeviceTypeString(), pDevice->GetDeviceName() );

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
