/**********************************************************
 * This file contains sx11.dll functions declarations
 *
 * Copyright (C) Sensoray 2001
 **********************************************************/

#ifndef XFUNCX11
//Pointers to DLL functions
#define XFUNCX11(RTNTYPE,FUNCNAME)		extern RTNTYPE (__stdcall *FUNCNAME)
#endif


#ifdef __cplusplus
extern "C" {
#endif

//DLL load/unload functions
extern ECODE X11_DLLOpen (void);
extern void X11_DLLClose (void);

//Pointers to sx11.dll exported functions

XFUNCX11(ECODE, X11_InitSystem)(PCI *);
XFUNCX11(ECODE,	X11_GetHFG)(HFG *, DWORD);
XFUNCX11(ECODE,	X11_AllocBuffer)(MODE *, BUFFER *, DWORD);
XFUNCX11(void,	X11_FreeBuffer)(HBUF);
XFUNCX11(void,	X11_CloseSystem)(void);
XFUNCX11(ECODE,	X11_Acquire)(HFG, HBUF, float, DWORD *);
XFUNCX11(ECODE,	X11_StartAcquire)(HFG, HBUF, DWORD);
XFUNCX11(ECODE,	X11_StopAcquire)(HFG);
XFUNCX11(ECODE,	X11_GetStatus)(HFG, DWORD *);
XFUNCX11(ECODE,	X11_ResetStatus)(HFG, DWORD);
XFUNCX11(ECODE,	X11_GetStatusEx)(HFG, STATUS *);
XFUNCX11(ECODE,	X11_ResetStatusEx)(HFG, STATUS *);
XFUNCX11(ECODE,	X11_SetMode)(HFG, MODE *);
XFUNCX11(ECODE,	X11_GetImageSize)(MODE *, DWORD *, DWORD *);
XFUNCX11(ECODE,	X11_WritePort)(HFG, DWORD, DWORD);
XFUNCX11(ECODE,	X11_ReadPort)(HFG, DWORD *);
XFUNCX11(ECODE,	X11_InterruptOn)(INT_DATA *);
XFUNCX11(ECODE,	X11_InterruptOff)(INT_DATA *);
XFUNCX11(ECODE,	X11_InterruptMask)(INT_DATA *);
XFUNCX11(ECODE,	X11_InterruptUnmask)(INT_DATA *);
XFUNCX11(ECODE, X11_GetRC)(HBUF, DWORD, MODE *,DWORD, DWORD, void *);

#ifdef __cplusplus
}
#endif
