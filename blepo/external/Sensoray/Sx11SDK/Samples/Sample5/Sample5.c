/***********************************************************
 * SX11 Sample Application 5.
 * Copyright (C) 1998-2002 Sensoray
 *
 * This sample application illustrates the use of bimodal
 * image capture: one field of each frame is captured in
 * color, the other field - in monochrome. The sample shows
 * how to handle the image buffer as two separate buffers,
 * and two separate bitmaps (in case the image display is
 * required).
 ************************************************************/

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "sample5.h"
#include "sx11.h"
#include "sx11f.h"


LRESULT WINAPI WndProc (HWND, UINT, UINT, LONG);    // Window procedure
ECODE FrameGrabber (DWORD);                 // Frame grabber procedure
void ErrAbort (ECODE);                      // Error handler
ECODE ConvertDIBHeader (LPBITMAPINFO *);    //

/***************************************
 * Globals
 ***************************************/ 
HWND hwnd;                                  //Main window handle;
LPBITMAPINFO lpbmi1, lpbmi2;                //pointer to DIB info structure;
LPVOID lpvbits1, lpvbits2;                  //pointer to DIB data;
/***************************************/

// Main function
int PASCAL WinMain (HINSTANCE hInst, HINSTANCE hInstPrev,
                    LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;
    ECODE ecode;
    char szAppName [] = "X11 Sample5";
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
    wndclass.lpszMenuName = "Sample5";
    wndclass.lpszClassName = szAppName;
    RegisterClass (&wndclass);

    // Create program window
    hwnd = CreateWindow (szAppName, szAppName,
                         WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^WS_SIZEBOX,
                         CW_USEDEFAULT, CW_USEDEFAULT,
                         XACT_NTSC + GetSystemMetrics (SM_CXBORDER) * 2,
                         YACT_NTSC + GetSystemMetrics (SM_CYCAPTION) +
                         GetSystemMetrics (SM_CYMENU) +
                         GetSystemMetrics (SM_CYBORDER) * 2,
                         NULL, NULL, hInst, NULL);
    
    ShowWindow (hwnd, SW_SHOWNORMAL);
    SetWindowPos (hwnd, HWND_TOP, 0, 0,
                  XACT_NTSC + GetSystemMetrics (SM_CXBORDER) * 2,
                  YACT_NTSC + GetSystemMetrics (SM_CYCAPTION) +
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
        } else if (ecode = FrameGrabber (FG_GRAB)) {
            ErrAbort (ecode);
        }
    } while (msg.message != WM_QUIT);

	DestroyIcon (wndclass.hIcon);
	DestroyCursor (wndclass.hCursor);

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
            CheckMenuItem (hmenu, FG_CHAN0, MF_CHECKED);
            return 0;

        case WM_PAINT:
            BeginPaint (hwnd, &ps);
            //display bitmaps
            hdc = GetDC (hwnd);
            GetClientRect (hwnd, (LPRECT) &rect);
 
            SetDIBitsToDevice (hdc, rect.left, rect.top,
                               rect.right - rect.left + 1,
                               (rect.bottom - rect.top + 1) / 2,
                               0, 0, 0, (UINT) lpbmi1->bmiHeader.biHeight,
                               lpvbits1, lpbmi1, DIB_RGB_COLORS);
 
            SetDIBitsToDevice (hdc, rect.left, rect.top + YACT_NTSC / 2,
                               rect.right - rect.left + 1,
                               (rect.bottom - rect.top + 1) / 2,
                               0, 0, 0, (UINT) lpbmi2->bmiHeader.biHeight,
                               lpvbits2, lpbmi2, DIB_RGB_COLORS);
  
            ReleaseDC (hwnd, hdc);
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

            }
            return 0;

        case WM_DESTROY:
            FrameGrabber (FG_QUIT);
			DestroyMenu (hmenu);
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
 *      FG_CHANn - selects input channel;
 *      FG_QUIT - cleans up.
 *      FG_SAVE - saves the image into a file.
 *
 * Returns error code in case of an error,
 * or 0 if successful.
 *********************************************************/
ECODE FrameGrabber (DWORD fgcommand)
{
    PCI pci;
    static BUFFER buffer;
    static MODE mode = {DEF_MODE_NTSC};
    static HFG hfg;
    DWORD xsize = XACT_NTSC, ysize = YACT_NTSC;
    DWORD colorbits = xsize * ysize * 3 / 2;        //total bits in "color" field
    DWORD monobits = xsize * ysize / 2;             //total bits in "mono" field
    static DWORD timer0;
    DWORD timer;
    static DWORD frames = 0;
    static BOOL acqflag = FALSE;
    ECODE ecode;
    char str[80];
    float timeout = .5f;
    DWORD status;
    
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

            mode.advanced.bimodal = BIMODAL_ON;
            //Allocate image buffer (1 frame)
            if ((ecode = X11_AllocBuffer (&mode, &buffer, 1))) {
                return ecode;
            }
            //Set up globals
            //pointers to data and BITMAPINFO for color bitmap
            lpbmi1 = buffer.frame[0].lpbmi;
            lpbmi1->bmiHeader.biHeight /= 2;          //adjust vert size
            lpvbits1 = (void *) ((BYTE *) buffer.frame[0].lpvbits + colorbits);

            //pointers to data and BITMAPINFO for monochrome bitmap
            lpbmi2 = lpbmi1;                            //create second BMIheader;
            if ((ecode = ConvertDIBHeader (&lpbmi2))) { //convert to monochrome;
                return ecode;
            }
            lpvbits2 = (void *) ((BYTE *) buffer.frame[0].lpvbits +
                         colorbits - monobits - xsize);

            //Set frame grabber mode
            if ((ecode = X11_SetMode (hfg, &mode))) {
                return ecode;
            }
            //Start acquisition
            acqflag = TRUE;
            timer0 = clock();
            return 0;
            
        case FG_GRAB:
            if (acqflag) {
                //Acquire one frame
                if ((ecode = X11_Acquire (hfg, buffer.hbuf, timeout, &status))) {
                    return ecode;
                }
                //Display the image
                SendMessage (hwnd, WM_PAINT, 0, 0L);
                //Update title bar
                frames++;
                timer = clock() - timer0;
                sprintf (str, "X11 Sample5;   FPS=%4.1f\0",
                         (float) frames * CLOCKS_PER_SEC / timer);
                SetWindowText (hwnd, (LPCSTR) &str[0]);
            }
            return 0;
            
        case FG_START:
            acqflag = TRUE;
            timer0 = clock();
            frames = 0;
            return 0;
            
        case FG_STOP:
            acqflag = FALSE;
            return 0;
    
        case FG_CHAN0:
        case FG_CHAN1:
        case FG_CHAN2:
        case FG_CHAN3:
        case FG_CHAN4:
            //select input channel
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
			X11_DLLClose();
            free (lpbmi2);
            return 0;

        default:
            return 0;
            
    }
}



ECODE ConvertDIBHeader (LPBITMAPINFO *plpbmi)
{
    LPBITMAPINFO ptrHeader;
    LPBITMAPINFO ptrOldHeader = *plpbmi;
    DWORD headersize;
    int i;

    headersize = sizeof (BITMAPINFOHEADER) + 256 * sizeof (RGBQUAD);

    /* Allocate memory for DIB info */
    ptrHeader = (LPBITMAPINFO) malloc (headersize);
    if (ptrHeader != NULL) {                    //if memory allocated;
        ptrHeader->bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
        ptrHeader->bmiHeader.biWidth = ptrOldHeader->bmiHeader.biWidth;
        ptrHeader->bmiHeader.biHeight = ptrOldHeader->bmiHeader.biHeight;
        ptrHeader->bmiHeader.biPlanes = 1;
        ptrHeader->bmiHeader.biCompression = 0;
        ptrHeader->bmiHeader.biSizeImage = 0;         //no need if no compression;
        ptrHeader->bmiHeader.biXPelsPerMeter = 0;
        ptrHeader->bmiHeader.biYPelsPerMeter = 0;

        ptrHeader->bmiHeader.biBitCount = 24;
        ptrHeader->bmiHeader.biClrUsed = 0;
        ptrHeader->bmiHeader.biClrImportant = 0;

        /*create grey scale color table */
        for (i = 0; i < 256; i++) {
            ptrHeader->bmiColors[i].rgbBlue = i;
            ptrHeader->bmiColors[i].rgbGreen = i;
            ptrHeader->bmiColors[i].rgbRed = i;
        }
        ptrHeader->bmiHeader.biBitCount = 8;
        ptrHeader->bmiHeader.biClrUsed = 256;
        ptrHeader->bmiHeader.biClrImportant = 0;
        
        *plpbmi = ptrHeader;
        
        return 0;
    } else {
        return ERR_DIB_MEM_ALLOC;
    }
}

