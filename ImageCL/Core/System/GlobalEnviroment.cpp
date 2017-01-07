#include "stdafx.h"
#include "GlobalEnviroment.h"

using namespace std;


std::unique_ptr< SGlobalEnvironment > gEnv = std::make_unique< SGlobalEnvironment >( );