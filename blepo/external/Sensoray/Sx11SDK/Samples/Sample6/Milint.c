/***********************************************************
 * SX11 Sample Application - MIL Interface.
 * Copyright (C) 1998-2002 Sensoray
 *
 * This sample application illustrates the interface of an
 * SX11 SDK built imaging application to Matrox Imaging Library
 * (MIL) v. 4.00. It is based on Sample C Application 2.
 * By allocating external buffer, SX11 image buffer is
 * mapped to the MIL image buffer, facilitating direct image
 * acquisition into the MIL image buffer (to avoid data copying).
 *************************************************************/

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <mil.h>

#include "milint.h"
#include "sx11.h"
#include "sx11f.h"


LRESULT WINAPI WndProc (HWND, UINT, UINT, LONG);    // Window procedure
ECODE FrameGrabber (DWORD);                 // Frame grabber procedure
void ErrAbort (ECODE);                      // Error handler

/***************************************
 * Globals
 ***************************************/ 
HWND hwnd;                                  //Main window handle
LPBITMAPINFO lpbmi;                         //pointer to DIB info structure
LPVOID lpvbits;                             //pointer to DIB data
/***************************************/

// Main function
int PASCAL WinMain (HINSTANCE hInst, HINSTANCE hInstPrev,
                    LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;
    ECODE ecode;
    char szAppName [] = "SX11-MIL";
    char str[80];

    // Quit if another instance is already running
    if (FindWindow (szAppName, NULL) != NULL) {
        sprintf (str, "Only one instance of this application is allowed to run.");
        MessageBox (NULL, str, szAppName, MB_OK | MB_ICONSTOP);
        return 0;
    }

    // Register window class
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInst;
    wndclass.hIcon = LoadIcon (hInst, "ICONX11");
    wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = GetStockObject (WHITE_BRUSH);
    wndclass.lpszMenuName = "MIL_int";
    wndclass.lpszClassName = szAppName;
    RegisterClass (&wndclass);

    // Create program window
    hwnd = CreateWindow (szAppName, szAppName,
                         WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^WS_SIZEBOX,
                         CW_USEDEFAULT, CW_USEDEFAULT,
                         XSIZE + GetSystemMetrics (SM_CXBORDER) * 2,
                         YSIZE + GetSystemMetrics (SM_CYCAPTION) +
                         GetSystemMetrics (SM_CYMENU) +
                         GetSystemMetrics (SM_CYBORDER) * 2,
                         NULL, NULL, hInst, NULL);
    
    ShowWindow (hwnd, SW_SHOWNORMAL);
    SetWindowPos (hwnd, HWND_TOP, 0, 0,
                  XSIZE + GetSystemMetrics (SM_CXBORDER) * 2,
                  YSIZE + GetSystemMetrics (SM_CYCAPTION) +
                  GetSystemMetrics (SM_CYMENU), SWP_NOMOVE);

    // Initialize frame grabber
    if (ecode = FrameGrabber (FG_INIT)) {
        sprintf (str, "Initialization failed; error %d", ecode);
        MessageBox (hwnd, str, szAppName, MB_OK | MB_ICONSTOP);
        SendMessage (hwnd, WM_DESTROY, 0, 0L);
        return 0;
    }

    UpdateWindow (hwnd);

    // Main message loop
    do {
        //process Windows messages first
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        } else if ((ecode = FrameGrabber (FG_GRAB))) {
                ErrAbort (ecode);
            }
        }
    } while (msg.message != WM_QUIT);

    return msg.wParam;
}


// Window procedure
LRESULT WINAPI WndProc (HWND hwnd, UINT message, UINT wParam, LONG lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    static HMENU hmenu;

    switch (message) {
        case WM_CREATE:
            hmenu = GetMenu (hwnd);
            CheckMenuItem (hmenu, FG_SCALE8, MF_CHECKED);
            CheckMenuItem (hmenu, FG_COLOR, MF_CHECKED);
            CheckMenuItem (hmenu, FG_CHAN0, MF_CHECKED);
            return 0;

        case WM_PAINT:
            BeginPaint (hwnd, &ps);
            //display bitmap 
            hdc = GetDC (hwnd);
            GetClientRect (hwnd, (LPRECT) &rect);
            SetDIBitsToDevice (hdc, rect.left, rect.top,
                               rect.right - rect.left + 1,
                               rect.bottom - rect.top + 1,
                               0, 0, 0,
                               (UINT) lpbmi->bmiHeader.biHeight,
                               lpvbits, lpbmi,
                               DIB_RGB_COLORS);
            ReleaseDC (hwnd, hdc);
            //
            EndPaint (hwnd, &ps);
            return 0;

        case WM_COMMAND:
            //process menu commands
            switch (wParam) {
                case FG_EXIT:
                    FrameGrabber (FG_STOP);
                    FrameGrabber (FG_QUIT);
                    PostMessage (hwnd, WM_QUIT, 0, 0L);
                    break;

                case FG_STOP:
                    FrameGrabber (FG_STOP);
                    break;

                case FG_START:
                    FrameGrabber (FG_START);
                    break;

                case FG_CHAN0:
                case FG_CHAN1:
                case FG_CHAN2:
                case FG_CHAN3:
                case FG_CHAN4:
                    FrameGrabber (wParam);
                    //check selected menu item
                    CheckMenuItem (hmenu, FG_CHAN0, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN1, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN2, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN3, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN4, MF_UNCHECKED);
                    CheckMenuItem (hmenu, wParam, MF_CHECKED);
                    break;

                case FG_SAVE:
                    FrameGrabber (FG_STOP);
                    FrameGrabber (FG_SAVE);
                    FrameGrabber (FG_START);
                    break;

            }
            return 0;

        case WM_DESTROY:
            FrameGrabber (FG_QUIT);
            PostQuitMessage (0);
            return 0;
    }

    return DefWindowProc (hwnd, message, wParam, lParam);
}

/**********************************************************
 * Error Handler
 **********************************************************/
void ErrAbort (ECODE ecode)
{
    char str[60];

    sprintf (str, "Error %d. Abort application?", ecode);
    if (MessageBox (hwnd, str, "SX11 Error", MB_YESNO | MB_ICONSTOP) == IDYES) {
        SendMessage (hwnd, WM_DESTROY, 0, 0L);
    }
    return;
}

/**********************************************************
 * FrameGrabber (fgcommand)
 * Performs frame grabber functions depending on fgcommand:
 *      FG_INIT - initializes the frame grabber;
 *      FG_START - starts continuous acquisition;
 *      FG_GRAB - grabs a frame and requests its display;
 *      FG_STOP - stops acquisition;
 *      FG_SCALEn - rescales the image;
 *      FG_CHANn - selects input channel;
 *      FG_COLOR, FG_MONO - selects color mode;
 *      FG_QUIT - cleans up.
 *      FG_SAVE - saves the image into a file.
 *
 * Returns error code in case of an error,
 * or 0 if successful.
 *********************************************************/
ECODE FrameGrabber (DWORD fgcommand)
{
    PCI pci;
    static BUFFER buffer[2];
    static MODE mode = {DEF_MODE_NTSC};
    static HFG hfg;
    DWORD xsize, ysize;
    static DWORD timer0;
    DWORD timer;
    static DWORD frames = 0;
    static BOOL acqflag = FALSE;
    static int current;                 //index of current buffer
    ECODE ecode;
    char str[80];
    DWORD status;
    HANDLE hlib;
    void *pMILbuf;
    /* MIL variables */
    static MIL_ID   MILApp,
                    MILSystem,
                    MILHostSystem,
                    MILDisplay,
                    MILImage[2],
                    MILResult[2]; 
    
    switch (fgcommand) {
        
        case FG_INIT:
            //Dynamicaly link the library
			if (ecode = X11_DLLOpen ()) {
				return ecode;
			}
            //Initialize system
            if ((ecode = X11_InitSystem (&pci))) {
                return ecode;
            }
            //Get frame grabber handle
            if ((ecode = X11_GetHFG (&hfg, pci.PCIslot[0]))) {
                return ecode;
            }
            //Set frame grabber mode
            mode.scale = SCALE4;
            mode.color = COLOR_MONO;
            mode.advanced.buffertype = BUF_EXT;
            if ((ecode = X11_SetMode (hfg, &mode))) {
                return ecode;
            }
            if ((ecode = X11_GetImageSize (&mode, &xsize, &ysize))) {
                return ecode;
            }
            //Allocate MIL objects
            MappAllocDefault(M_SETUP, &MILApp, &MILSystem, &MILDisplay,
                             M_NULL, M_NULL);
            MsysAlloc (M_SYSTEM_HOST, M_DEFAULT, M_COMPLETE, &MILHostSystem);
            //Set up MIL display
            MdispControl (MILDisplay, M_WINDOW_TITLE_NAME,
                          (long) "Edge detection filter");
            MdispControl (MILDisplay, M_WINDOW_SYSBUTTON,
                          M_DISABLE);
            //Allocate MIL image buffers
            //...acquisition buffers
            MbufAlloc2d(MILHostSystem, xsize, ysize, 8 + M_UNSIGNED,
                        M_IMAGE + M_PROC, &MILImage[0]);
            MbufAlloc2d(MILHostSystem, xsize, ysize, 8 + M_UNSIGNED,
                        M_IMAGE + M_PROC, &MILImage[1]);
            //...display buffers
            MbufAlloc2d (MILSystem, xsize, ysize, 8 + M_UNSIGNED,
                         M_IMAGE + M_PROC + M_DISP,
                         &MILResult[0]);
            MbufAlloc2d (MILSystem, xsize, ysize, 8 + M_UNSIGNED,
                         M_IMAGE + M_PROC + M_DISP,
                         &MILResult[1]);

            //Get pointers to MIL buffers and map 2 SX11 buffers
            pMILbuf = (void *) MbufInquire (MILImage[0], M_HOST_ADDRESS, M_NULL);
            if ((ecode = X11_AllocBuffer (&mode, &buffer[0], (DWORD) pMILbuf))) {
                return ecode;
            }
            pMILbuf = (void *) MbufInquire (MILImage[1], M_HOST_ADDRESS, M_NULL);
            if ((ecode = X11_AllocBuffer (&mode, &buffer[1], (DWORD) pMILbuf))) {
                return ecode;
            }

            //Set up globals
            lpbmi = buffer[current].frame[0].lpbmi;
            lpvbits = buffer[current].frame[0].lpvbits;
            //Start acquisition
            current = 0;
            acqflag = TRUE;
            timer0 = clock();
            X11_StartAcquire (hfg, buffer[current].hbuf, AMODE_SINGLE);
            return 0;
            
        case FG_GRAB:
            if (acqflag) {
                //Check if frame ready
                if ((ecode = X11_GetStatus (hfg, &status))) {
                    return ecode;
                }
                if (status & STATUS_READY) {
                    X11_StopAcquire (hfg);
                    //Set up global pointers
                    lpbmi = buffer[current].frame[0].lpbmi;
                    lpvbits = buffer[current].frame[0].lpvbits;

                    //Start acquisition into alternative buffer
                    X11_StartAcquire (hfg, buffer[current ? 0 : 1].hbuf, AMODE_SINGLE);
                    //Display source image
                    SendMessage (hwnd, WM_PAINT, 0, 0L);
                    //Update title bar
                    frames++;
                    timer = clock() - timer0;
                    sprintf (str, "SX11-MIL;   FPS=%4.1f\0", (float) frames * CLOCKS_PER_SEC / timer);
                    SetWindowText (hwnd, (LPCSTR) &str[0]);

                    //Perform a MIL function over a buffer
                    MimEdgeDetect(MILImage[current], MILResult[0], M_NULL, 
                                  M_SOBEL + M_OVERSCAN_DISABLE, M_FAST_EDGE_DETECT, M_NULL);
                    MimFlip (MILResult[0], MILResult[1], M_FLIP_VERTICAL, M_DEFAULT);
                    //display MIL buffer
                    MdispSelect(MILDisplay, MILResult[1]);

                    //Toggle active buffer index
                    current = (current ? 0 : 1);
				} else if (status & STATUS_ERROR) {					//error during acquisition
		            X11_StartAcquire (hfg, buffer[current].hbuf, AMODE_SINGLE);	//restart capture
				}
            }
            return 0;
            
        case FG_START:
            acqflag = TRUE;
            timer0 = clock();
            frames = 0;
            X11_StartAcquire (hfg, buffer[current].hbuf, AMODE_SINGLE);
            return 0;
            
        case FG_STOP:
            acqflag = FALSE;
            X11_StopAcquire (hfg);
            return 0;
    
        case FG_CHAN0:
        case FG_CHAN1:
        case FG_CHAN2:
        case FG_CHAN3:
        case FG_CHAN4:
            //select input channel
            switch (fgcommand - FG_CHAN) {
                case 0:
                    mode.input = MUX_0;
                    mode.advanced.LNOTCH = LNOTCH_OFF;
					mode.advanced.CCOMB = CCOMB_OFF;
                    break;
                case 1:
                    mode.input = MUX_1;
                    mode.advanced.LNOTCH = LNOTCH_ON;
					mode.advanced.CCOMB = CCOMB_ON;
                    break;
                case 2:
                    mode.input = MUX_2;
                    mode.advanced.LNOTCH = LNOTCH_ON;
					mode.advanced.CCOMB = CCOMB_ON;
                    break;
                case 3:
                    mode.input = MUX_3;
                    mode.advanced.LNOTCH = LNOTCH_ON;
					mode.advanced.CCOMB = CCOMB_ON;
                    break;
                case 4:
                    mode.input = MUX_4;
                    mode.advanced.LNOTCH = LNOTCH_ON;
					mode.advanced.CCOMB = CCOMB_ON;
                    break;
            }
            X11_SetMode (hfg, &mode);
            return 0;
           
        case FG_QUIT:
            //clean up
            X11_CloseSystem ();
            //close MIL objects
            MbufFree (MILImage[0]);
            MbufFree (MILImage[1]);
            MbufFree (MILResult[0]);
            MbufFree (MILResult[1]);
            MsysFree (MILHostSystem);
            MappFreeDefault(MILApp, MILSystem, MILDisplay, M_NULL, M_NULL);
			X11_DLLClose();
            return 0;

        default:
            return 0;           
    }
}



