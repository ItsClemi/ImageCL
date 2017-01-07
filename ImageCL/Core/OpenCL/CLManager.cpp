#include "stdafx.h"
#include "CLManager.h"

#include "Core/TaskWorker.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE

static char THIS_FILE[ ] = __FILE__;
#endif



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

void CCLManager::InitializeAsync( )
{
	Concurrency::create_task( [ & ] {
		LogStatusBar( L"Scanning OpenCL Devices..." );

		try
		{
			vector< cl::Platform > vecPlatforms;
			cl::Platform::get( &vecPlatforms );

			size_t nId = 0;
			for( auto i : vecPlatforms )
			{
				auto pPlatform = new CCLPlatform( i );

				i.getDevices( CL_DEVICE_TYPE_ALL, &pPlatform->m_vecDevices );

				for( auto it : pPlatform->m_vecDevices )
				{
					m_vecDevices.push_back( new CCLDevice( it, pPlatform, nId++ ) );
				}

				m_vecPlatforms.push_back( pPlatform );
			}

			m_bInitialized = true;
		}
		catch( const cl::Error& err )
		{
			LogError( L"OpenCL startup failed! %s", StringToWstring( err.what( ) ) );
		}
	} ).then( [ ] {

		PostCommandMessage( WM_ADD_CL_DEVICE, nullptr );

		LogStatusBar( L"Done!" );
	} );
}


// 	cl_context_properties properties[ ] = { CL_CONTEXT_PLATFORM, ( cl_context_properties )( vecPlatforms[ 0 ] )( ), 0 };
// 
// 	cl::Context context( CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU, properties );
// 
// 	auto vecDevices = context.getInfo< CL_CONTEXT_DEVICES >( );
// 
// 	
// 	for( auto i : vecDevices )
// 	{
// 		
// 
// 	}



/*
const char* LoadTemplateCode( )
{
	HINSTANCE hInstance = AfxGetApp( )->m_hInstance;
	HRSRC hRes = FindResource( hInstance, MAKEINTRESOURCE( IDR_CODE_TEMPLATE_EMPTY ), L"TEXT" );

	if( hRes )
	{
		HGLOBAL hLoaded = LoadResource( hInstance, hRes );

		if( hLoaded )
		{
			void* pLocked = LockResource( hLoaded );

			if( pLocked )
			{
				DWORD dwResourceSize = SizeofResource( hInstance, hRes );

				if( dwResourceSize > 0 )
				{
					std::string sz(
						static_cast< char* >( pLocked ),
						static_cast< size_t >( dwResourceSize )
					);

					return (char*)pLocked;
				}
			}
		}
	}

	return "";
}

*/