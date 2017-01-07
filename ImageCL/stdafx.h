#pragma once

#ifdef _M_IX86
#pragma message( "Warning x32 build is untested!" )
#endif

//////////////////////////////////////////////////////////////////////////
//=> Defines

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN          
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS    
#define _AFX_ALL_WARNINGS
#define __CL_ENABLE_EXCEPTIONS

//////////////////////////////////////////////////////////////////////////
//=> Includes

#include "targetver.h"

#include <afxwin.h>       
#include <afxext.h>         
#include <afxdtctl.h>   
#include <afxcmn.h>             
#include <afxcontrolbars.h>   
#include <VersionHelpers.h>
#include <gdiplus.h>

#include <memory>
#include <string>
#include <vector>
#include <array>
#include <codecvt>	
#include <fstream>
#include <atomic>
#include <istream>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <ppl.h>
#include <ppltasks.h>
#include <agents.h>
#include <concurrent_queue.h>

#include <Scintilla.h>
#include <CL/cl.hpp>



#include <Resource.h>
 
#include "Common/Helper/CommonHelper.h"
#include "Common/Helper/DpiHelper.h"
#include "Common/Helper/MFCHelper.h"
#include "Common/Helper/GDIHelper.h"
#include "Core/System/ISystem.h"

#include "Core/Log/Log.h"

#include "Core/System/GlobalEnviroment.h"



//////////////////////////////////////////////////////////////////////////
//=> Lib

#pragma comment( lib, "OpenCL.lib" )
#pragma comment( lib, "Imm32.lib" )


#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif





//////////////////////////////////////////////////////////////////////////
//=> Messages

#define WM_COMMAND_REFLECT		( WM_APP + 1 )


#define WM_ADD_OUTPUT			( WM_USER + 1 )
#define WM_UPDATE_IMAGE			( WM_USER + 2 )
#define WM_SET_CL_PROGRAM		( WM_USER + 3 )
#define WM_STATUS_BAR_UPDATE	( WM_USER + 4 )
#define WM_ADD_CL_DEVICE		( WM_USER + 5 )
