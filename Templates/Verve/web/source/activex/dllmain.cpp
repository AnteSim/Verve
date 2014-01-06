//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "IEWebGamePlugin_i.h"
#include "dllmain.h"
#include "../common/webCommon.h"

CIEWebGamePluginModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	WebCommon::gPluginModule = (HMODULE) hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
