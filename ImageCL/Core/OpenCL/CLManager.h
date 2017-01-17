#pragma once




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


class CCLDevice;
class CCLManager
{
public:
	CCLManager( );
	~CCLManager( );

public:
	Concurrency::task< void > InitializeAsync( );


private:
	void FetchPlatformInfo( SCLPlatform* pPlatform );



public:
	inline void EnumDevices( std::function< void( const CCLDevice* pDevice ) > f )
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

public:
	static void RemoveWhiteSpace( std::string& szStr )
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
	std::vector< CCLDevice* >		m_vecDevices;


};


namespace cl
{
	static const wchar_t* GetErrorString( cl_int error )
	{
		switch( error )
		{
			case CL_SUCCESS:									return L"CL_SUCCESS";
			case CL_DEVICE_NOT_FOUND:							return L"CL_DEVICE_NOT_FOUND";
			case CL_DEVICE_NOT_AVAILABLE:						return L"CL_DEVICE_NOT_AVAILABLE";
			case CL_COMPILER_NOT_AVAILABLE:						return L"CL_COMPILER_NOT_AVAILABLE";
			case CL_MEM_OBJECT_ALLOCATION_FAILURE:				return L"CL_MEM_OBJECT_ALLOCATION_FAILURE";
			case CL_OUT_OF_RESOURCES:							return L"CL_OUT_OF_RESOURCES";
			case CL_OUT_OF_HOST_MEMORY:							return L"CL_OUT_OF_HOST_MEMORY";
			case CL_PROFILING_INFO_NOT_AVAILABLE:				return L"CL_PROFILING_INFO_NOT_AVAILABLE";
			case CL_MEM_COPY_OVERLAP:							return L"CL_MEM_COPY_OVERLAP";
			case CL_IMAGE_FORMAT_MISMATCH:						return L"CL_IMAGE_FORMAT_MISMATCH";
			case CL_IMAGE_FORMAT_NOT_SUPPORTED:					return L"CL_IMAGE_FORMAT_NOT_SUPPORTED";
			case CL_BUILD_PROGRAM_FAILURE:						return L"CL_BUILD_PROGRAM_FAILURE";
			case CL_MAP_FAILURE:								return L"CL_MAP_FAILURE";
			case CL_MISALIGNED_SUB_BUFFER_OFFSET:				return L"CL_MISALIGNED_SUB_BUFFER_OFFSET";
			case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:	return L"CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
			case CL_COMPILE_PROGRAM_FAILURE:					return L"CL_COMPILE_PROGRAM_FAILURE";
			case CL_LINKER_NOT_AVAILABLE:						return L"CL_LINKER_NOT_AVAILABLE";
			case CL_LINK_PROGRAM_FAILURE:						return L"CL_LINK_PROGRAM_FAILURE";
			case CL_DEVICE_PARTITION_FAILED:					return L"CL_DEVICE_PARTITION_FAILED";
			case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:				return L"CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

				// compile-time errors
			case CL_INVALID_VALUE:						return L"CL_INVALID_VALUE";
			case CL_INVALID_DEVICE_TYPE:				return L"CL_INVALID_DEVICE_TYPE";
			case CL_INVALID_PLATFORM:					return L"CL_INVALID_PLATFORM";
			case CL_INVALID_DEVICE:						return L"CL_INVALID_DEVICE";
			case CL_INVALID_CONTEXT:					return L"CL_INVALID_CONTEXT";
			case CL_INVALID_QUEUE_PROPERTIES:			return L"CL_INVALID_QUEUE_PROPERTIES";
			case CL_INVALID_COMMAND_QUEUE:				return L"CL_INVALID_COMMAND_QUEUE";
			case CL_INVALID_HOST_PTR:					return L"CL_INVALID_HOST_PTR";
			case CL_INVALID_MEM_OBJECT:					return L"CL_INVALID_MEM_OBJECT";
			case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:	return L"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
			case CL_INVALID_IMAGE_SIZE:					return L"CL_INVALID_IMAGE_SIZE";
			case CL_INVALID_SAMPLER:					return L"CL_INVALID_SAMPLER";
			case CL_INVALID_BINARY:						return L"CL_INVALID_BINARY";
			case CL_INVALID_BUILD_OPTIONS:				return L"CL_INVALID_BUILD_OPTIONS";
			case CL_INVALID_PROGRAM:					return L"CL_INVALID_PROGRAM";
			case CL_INVALID_PROGRAM_EXECUTABLE:			return L"CL_INVALID_PROGRAM_EXECUTABLE";
			case CL_INVALID_KERNEL_NAME:				return L"CL_INVALID_KERNEL_NAME";
			case CL_INVALID_KERNEL_DEFINITION:			return L"CL_INVALID_KERNEL_DEFINITION";
			case CL_INVALID_KERNEL:						return L"CL_INVALID_KERNEL";
			case CL_INVALID_ARG_INDEX:					return L"CL_INVALID_ARG_INDEX";
			case CL_INVALID_ARG_VALUE:					return L"CL_INVALID_ARG_VALUE";
			case CL_INVALID_ARG_SIZE:					return L"CL_INVALID_ARG_SIZE";
			case CL_INVALID_KERNEL_ARGS:				return L"CL_INVALID_KERNEL_ARGS";
			case CL_INVALID_WORK_DIMENSION:				return L"CL_INVALID_WORK_DIMENSION";
			case CL_INVALID_WORK_GROUP_SIZE:			return L"CL_INVALID_WORK_GROUP_SIZE";
			case CL_INVALID_WORK_ITEM_SIZE:				return L"CL_INVALID_WORK_ITEM_SIZE";
			case CL_INVALID_GLOBAL_OFFSET:				return L"CL_INVALID_GLOBAL_OFFSET";
			case CL_INVALID_EVENT_WAIT_LIST:			return L"CL_INVALID_EVENT_WAIT_LIST";
			case CL_INVALID_EVENT:						return L"CL_INVALID_EVENT";
			case CL_INVALID_OPERATION:					return L"CL_INVALID_OPERATION";
			case CL_INVALID_GL_OBJECT:					return L"CL_INVALID_GL_OBJECT";
			case CL_INVALID_BUFFER_SIZE:				return L"CL_INVALID_BUFFER_SIZE";
			case CL_INVALID_MIP_LEVEL:					return L"CL_INVALID_MIP_LEVEL";
			case CL_INVALID_GLOBAL_WORK_SIZE:			return L"CL_INVALID_GLOBAL_WORK_SIZE";
			case CL_INVALID_PROPERTY:					return L"CL_INVALID_PROPERTY";
			case CL_INVALID_IMAGE_DESCRIPTOR:			return L"CL_INVALID_IMAGE_DESCRIPTOR";
			case CL_INVALID_COMPILER_OPTIONS:			return L"CL_INVALID_COMPILER_OPTIONS";
			case CL_INVALID_LINKER_OPTIONS:				return L"CL_INVALID_LINKER_OPTIONS";
			case CL_INVALID_DEVICE_PARTITION_COUNT:		return L"CL_INVALID_DEVICE_PARTITION_COUNT";

				// extension errors
			case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR:	return L"CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
			case CL_PLATFORM_NOT_FOUND_KHR:					return L"CL_PLATFORM_NOT_FOUND_KHR";

			default:	return L"Unknown OpenCL error";
		}
	}
}