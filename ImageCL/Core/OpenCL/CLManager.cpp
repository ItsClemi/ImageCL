#include "stdafx.h"
#include "CLManager.h"

CCLManager::CCLManager( )
{

}

CCLManager::~CCLManager( )
{

}

bool CCLManager::Initialize( )
{
	cl_int status;

	LogInfo( L"Scanning OpenCL devices!" );

	std::vector< cl::Platform > vecPlatforms;
	status = cl::Platform::get( &vecPlatforms );

	cl_context_properties properties[ ] = { CL_CONTEXT_PLATFORM, ( cl_context_properties )( vecPlatforms[ 0 ] )( ), 0 };

	cl::Context context( CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU, properties );

	auto deviecs = context.getInfo< CL_CONTEXT_DEVICES >( );

	Concurrency::create_task( [ ] {
		Sleep( 2500 );

		AfxGetApp( )->GetMainWnd( )->PostMessageW( WM_COMMAND_REFLECT, WM_STATUS_BAR_UPDATE, reinterpret_cast< LPARAM >( L"asdasd TEESSASD" ) );

	} );



	return true;
}

void CCLManager::InitializeOpenCLAsync( )
{
	//Concurrency::create_task( [ & ] {

	//} ).then( [ ]( ) {

	//} );

}

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