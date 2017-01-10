#include "stdafx.h"
#include "GlobalEnviroment.h"

using namespace std;

unique_ptr< SGlobalEnvironment > gEnv = make_unique< SGlobalEnvironment >( );


void InitializeSystem( )
{
	gEnv->pClManger = make_unique< CCLManager >( );
	gEnv->pLogQueue = make_unique< CLogQueue >( );
}

