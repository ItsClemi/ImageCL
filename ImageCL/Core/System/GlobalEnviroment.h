#pragma once

#include "Core/OpenCL/CLManager.h"
#include "Core/Log/LogQueue.h"
#include "Core/System/TaskWorker.h"

struct SGlobalEnvironment
{
	std::unique_ptr< CTaskWorker >	pTaskWorker = std::make_unique< CTaskWorker >( );

	std::unique_ptr< CCLManager >	pClManger;
	std::unique_ptr< CLogQueue >	pLogQueue;

};

void InitializeSystem( );


extern std::unique_ptr< SGlobalEnvironment > gEnv;