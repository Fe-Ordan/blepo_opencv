/***********************************************************
 * This file MUST be included into any application project
 * that uses sx11.dll. It provides DLL initialization.
 * Copyright (C) Sensoray 2001
 **********************************************************/

/********************* IMPORTANT *********************
An option "Not using precompiled headers" must be set in project
properties for this file if it is an MFC project.
******************************************************/

//Function pointers are created here. All modules that call
//functions from sx11.dll must include sx11f.h

#define XFUNCX11(RTNTYPE,FUNCNAME)		RTNTYPE (__stdcall *FUNCNAME)

#include <windows.h>
#include "sx11.h"
#include "sx11f.h"

HINSTANCE hx11lib;

//Initialize pointers to exported functions.
//Return 0 if success

#define GETFPTR(FPTR,FNAM)	if (!((FARPROC)FPTR = GetProcAddress(hx11lib, FNAM))) return ERR_FUNCADDR;

ECODE GetFuncPtrs (void)
{
	GETFPTR (	X11_InitSystem,			"X11_InitSystem");
	GETFPTR (	X11_GetHFG,				"X11_GetHFG");
	GETFPTR (	X11_AllocBuffer,		"X11_AllocBuffer");
	GETFPTR (	X11_FreeBuffer,			"X11_FreeBuffer");
	GETFPTR (	X11_CloseSystem,		"X11_CloseSystem");
	GETFPTR (	X11_Acquire,			"X11_Acquire");
	GETFPTR (	X11_StartAcquire,		"X11_StartAcquire");
	GETFPTR (	X11_StopAcquire,		"X11_StopAcquire");
	GETFPTR (	X11_GetStatus,			"X11_GetStatus");
	GETFPTR (	X11_ResetStatus,		"X11_ResetStatus");
	GETFPTR (	X11_GetStatusEx,		"X11_GetStatusEx");
	GETFPTR (	X11_ResetStatusEx,		"X11_ResetStatusEx");
	GETFPTR (	X11_SetMode,			"X11_SetMode");
	GETFPTR (	X11_GetImageSize,		"X11_GetImageSize");
	GETFPTR (	X11_WritePort,			"X11_WritePort");
	GETFPTR (	X11_ReadPort,			"X11_ReadPort");
	GETFPTR (	X11_InterruptOn,		"X11_InterruptOn");
	GETFPTR (	X11_InterruptOff,		"X11_InterruptOff");
	GETFPTR (	X11_InterruptMask,		"X11_InterruptMask");
	GETFPTR (	X11_InterruptUnmask,	"X11_InterruptUnmask");
	GETFPTR (	X11_GetRC,				"X11_GetRC");

	return 0;
}

// DLL open/close functions
ECODE X11_DLLOpen (void)
{
	ECODE ecode;

	hx11lib = LoadLibrary ("sx11.dll");
	if (hx11lib == NULL) {
		return ERR_LOADING_DLL;
	}

	// Create pointers to exported functions
	if (ecode = GetFuncPtrs()) {
		FreeLibrary (hx11lib);
		return ecode;
	}                                                                      

	return 0;
}


void X11_DLLClose (void)
{
	if (hx11lib) {
		FreeLibrary (hx11lib);
	}
	return;
}

