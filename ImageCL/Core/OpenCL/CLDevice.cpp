#include "stdafx.h"
#include "CLDevice.h"

#include "CLManager.h"

using namespace std;
using namespace Concurrency;

CCLDevice::CCLDevice( SCLPlatform* pPlatform, cl::Device clDevice ) : 
	m_pPlatform( pPlatform ), 
	m_clDevice( clDevice ),
	m_clContext( clDevice ),
	m_clCommandQueue( m_clContext, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE )
{

}

CCLDevice::~CCLDevice( )
{
	m_vecExtensions.clear( );
}

void CCLDevice::FetchDeviceInfo( )
{
	m_eType = static_cast< eDeviceType >(
		m_clDevice.getInfo< CL_DEVICE_TYPE >( )
		);

	{
		auto szDeviceName = m_clDevice.getInfo< CL_DEVICE_NAME >( );

		CCLManager::RemoveWhiteSpace( szDeviceName );
		m_szDeviceName = StringToWstring( szDeviceName ).c_str( );
	}

	{
		auto szExtensions = m_clDevice.getInfo< CL_DEVICE_EXTENSIONS >( );
		CCLManager::RemoveWhiteSpace( szExtensions );

		wistringstream s( StringToWstring( szExtensions ) );
		vector< wstring > vecExtensions = {
			istream_iterator< wstring, wchar_t >{ s },
			istream_iterator< wstring, wchar_t >{ }
		};

		for( auto i : vecExtensions )
		{
			m_vecExtensions.push_back( i.c_str( ) );
		}
	}

	m_nComputeUnits = m_clDevice.getInfo< CL_DEVICE_MAX_COMPUTE_UNITS >( );
	m_nMaxWorkGroups = m_clDevice.getInfo< CL_DEVICE_MAX_WORK_GROUP_SIZE >( );
	m_nLocalMemorySize = m_clDevice.getInfo< CL_DEVICE_LOCAL_MEM_SIZE >( );
	m_nGlobalMemorySize = m_clDevice.getInfo< CL_DEVICE_GLOBAL_MEM_SIZE >( );
	m_bDeviceAvilable = m_clDevice.getInfo< CL_DEVICE_AVAILABLE >( );
	m_bCompilerAvailable = m_clDevice.getInfo< CL_DEVICE_COMPILER_AVAILABLE >( );
}

void CCLDevice::BuildCode( const shared_ptr< vector< char > > vecCode )
{
	m_bDeviceInUse = true;

	LogInfo( L"OpenCL: Running kernel on %s(%s)", GetDeviceName(), GetPlatform( )->m_szVendor );

	create_task( [ this, vecCode ] { 
		try
		{
			cl::Program program( GetContext( ), &vecCode->at( 0 ) );

			program.build( );

 			const auto szTest = program.getBuildInfo< CL_PROGRAM_BUILD_LOG >( m_clDevice );


// 			if( szTest.length( ) > 0 )
// 			{
// 				LogInfo( L"%s", StringToWstring( szTest ).c_str( ) );
// 			}

// 			auto p = program.getInfo< CL_PROGRAM_BINARIES >( );

			cl::Kernel kernel( program, "image_kernel" );

			IWICBitmap* pBitmap = nullptr;



		}
		catch( const cl::Error ex )
		{
			LogError( 
				L"OpenCL: %s failed with %s (%d)", 
				StringToWstring( ex.what( ) ).c_str( ), 
				cl::GetErrorString( ex.err( ) ), 
				ex.err( )
			);
		}
	} );
}
