#pragma once

#include "Core/OpenCL/CLManager.h"
#include "Core/Log/LogQueue.h"

struct SGlobalEnvironment
{
	std::unique_ptr< CCLManager >	pClManger = std::make_unique< CCLManager >( );
	std::unique_ptr< CLogQueue >	pLogQueue = std::make_unique< CLogQueue >( );

};


extern std::unique_ptr< SGlobalEnvironment > gEnv;