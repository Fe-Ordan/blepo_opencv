/***********************************************************
 * DirectDraw Sample Application.
 * Copyright (C) 1998-2002 Sensoray
 *
 * This sample application illustrates the use of SX11.dll
 * functions that take advantage of DirectDraw.
 * It allows image capture directly into the video memory,
 * and display with optional overlay.
 * All pixels of key color (white in this sample) are being
 * replaced with the image, the rest show on top of the image.
 * For the sake of simplicity the application window is
 * assumed to be fully within the desktop area.
 *
 * The video card has to be capable of supporting DirectDraw
 * overlay surface in YUV format.
 ************************************************************/

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "sx11.h"
#include "sx11f.h"
#include "sample4.h"

LRESULT WINAPI WndProc (HWND, UINT, UINT, LONG);    // Window procedure
ECODE FrameGrabber (DWORD);                 // Frame grabber procedure
void ErrAbort (ECODE);                      // Error handler
void RepaintPS (HWND);

/***************************************
 * Globals
 ***************************************/ 
HWND hwnd;                                  //Main window handle
HBITMAP hbmp640, hbmp320;                   //handles to bitmaps
DWORD xsize = XSIZE, ysize = YSIZE;         //window dimensions
/***************************************/

// Main function
int PASCAL WinMain (HINSTANCE hInstance, HINSTANCE hInstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;
    ECODE ecode;
    char szAppName [] = "X11 Sample4";
    char str[80];
    RECT rect;

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
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon (hInstance, "ICONX11");
    wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
    wndclass.hbrBackground = GetStockObject (WHITE_BRUSH);
    wndclass.lpszMenuName = "Sample4";
    wndclass.lpszClassName = szAppName;
    RegisterClass (&wndclass);

    // Create program window
    SetRect (&rect, X0, Y0, X0 + xsize - 1, Y0 + ysize - 1);
    AdjustWindowRectEx (&rect, WS_EX_TOPMOST | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, TRUE, WS_EX_LEFT);

    //create a window
    hwnd = CreateWindow (szAppName, szAppName,
                         WS_EX_TOPMOST | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
                         rect.left, rect.top,
                         rect.right - rect.left + 1, rect.bottom - rect.top + 1,
                         NULL, NULL, hInstance, NULL);
    ShowWindow (hwnd, SW_SHOWNORMAL);
    UpdateWindow (hwnd);

    //Load bitmaps
    hbmp640 = LoadBitmap (hInstance, "SRAYLOGO640");
    hbmp320 = LoadBitmap (hInstance, "SRAYLOGO320");

    // Initialize frame grabber
    if (ecode = FrameGrabber (FG_INIT)) {
        sprintf (str, "Initialization failed; error %d", ecode);
        MessageBox (hwnd, str, szAppName, MB_OK | MB_ICONSTOP);
        PostMessage (hwnd, WM_QUIT, 0, 0L);
    } else {
	    RepaintPS (hwnd);
	}

    // Main message loop
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	DestroyIcon (wndclass.hIcon);
	DestroyCursor (wndclass.hCursor);
	DeleteObject ((HGDIOBJ) hbmp640);
	DeleteObject ((HGDIOBJ) hbmp320);

    return msg.wParam;
}


// Window procedure
LRESULT WINAPI WndProc (HWND hwnd, UINT message, UINT wParam, LONG lParam)
{
    ECODE ecode;
    static HMENU hmenu;
	static BOOL moving = FALSE;
    HDC hdc;
    RECT rect;
    struct tm *tod;
    time_t ltime;
	char *string;
    PAINTSTRUCT ps;
    LOGFONT lf;
    TEXTMETRIC tm;
    HFONT hfont, hfold;
    int factor = (xsize == XSIZE) ? 2 : 1;      //font size scale factor

    switch (message) {
        case WM_CREATE:
            hmenu = GetMenu (hwnd);
            CheckMenuItem (hmenu, FG_SCALE8, MF_CHECKED);
            CheckMenuItem (hmenu, FG_CHAN0, MF_CHECKED);
            CheckMenuItem (hmenu, FG_NTSC, MF_CHECKED);
            SetTimer (hwnd, TIMER_ID, TIMER_RATE, NULL);
            return 0;

        case WM_SYSCOMMAND:
            //check if window is being moved
            if ((wParam & 0xfff0) == SC_MOVE) {                 //started moving;
                FrameGrabber (FG_STOP);
                moving = TRUE;
            }
            break;

        case WM_SETCURSOR:
            if (moving) {
                moving = FALSE;
                if (ecode = FrameGrabber (FG_RESTART)) {
                    ErrAbort (ecode);
                }
                RepaintPS (hwnd);
            }
            break;

        case WM_TIMER:
            //redraw text on timer
            hdc = GetDC (hwnd);
            //client rectangle of image window
            GetClientRect (hwnd, &rect);

            //set up font size and attributes
            factor = (xsize >= XSIZE) ? 2 : 1;
            ZeroMemory (&lf, sizeof(lf));
            GetTextMetrics (hdc, &tm);
            lf.lfHeight = factor * tm.tmHeight;
            lf.lfWidth = factor * tm.tmAveCharWidth;
            lf.lfCharSet = tm.tmCharSet;
            lf.lfWeight = 750;
            hfont = CreateFontIndirect (&lf);
            hfold = SelectObject (hdc, hfont);
            SetTextColor (hdc, CLR_FORE);
            SetBkColor (hdc, CLR_BACK);

            //get time in a string
            time (&ltime);
            tod = localtime (&ltime);
			string = asctime (tod);
            GetTextMetrics (hdc, &tm);
            
            //display string
            TextOut (hdc, rect.left + 4, rect.bottom - tm.tmHeight - 1, string, 24);

            //clean up
            ReleaseDC (hwnd, hdc);
            DeleteObject (hfont);
            break;


        case WM_PAINT:
            BeginPaint (hwnd, &ps);
            RepaintPS (hwnd);  
            EndPaint (hwnd, &ps);
            break;

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

                case FG_SCALE + SCALE4:
                case FG_SCALE + SCALE8:
                    FrameGrabber (FG_STOP);
                    if (ecode = FrameGrabber (wParam)) {
                        ErrAbort (ecode);
                    } else {
                        //check selected menu item
                        CheckMenuItem (hmenu, FG_SCALE4, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_SCALE8, MF_UNCHECKED);
                        CheckMenuItem (hmenu, wParam, MF_CHECKED);
	                    RepaintPS (hwnd);  
		                FrameGrabber (FG_START);
                    }
                    break;

                case FG_CHAN0:
                case FG_CHAN1:
                case FG_CHAN2:
                case FG_CHAN3:
                case FG_CHAN4:
                    FrameGrabber (FG_STOP);
                    FrameGrabber (wParam);
                    //check selected menu item
                    CheckMenuItem (hmenu, FG_CHAN0, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN1, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN2, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN3, MF_UNCHECKED);
                    CheckMenuItem (hmenu, FG_CHAN4, MF_UNCHECKED);
                    CheckMenuItem (hmenu, wParam, MF_CHECKED);
                    FrameGrabber (FG_START);
                    break;

				case FG_NTSC:
				case FG_PALSECAM:
					FrameGrabber (FG_STOP);
                    if (ecode = FrameGrabber (wParam)) {
                        ErrAbort (ecode);
                    } else {
                        //check selected menu item
                        CheckMenuItem (hmenu, FG_NTSC, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_PALSECAM, MF_UNCHECKED);
                        CheckMenuItem (hmenu, wParam, MF_CHECKED);
	                    FrameGrabber (FG_START);
                    }


                    break;

            }
            return 0;

        case WM_DESTROY:
            FrameGrabber (FG_QUIT);
            KillTimer (hwnd, TIMER_ID);
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
 *      FG_RESTART - re-allocates image buffer, starts acquisition;
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
    static BUFFER buffer;
	static MODE mode_NTSC = {DEF_MODE_NTSC};
	static MODE mode_PAL = {DEF_MODE_PAL};
    static MODE mode = {DEF_MODE_NTSC};
    static HFG hfg;
    ECODE ecode;
	RECT rect;
	DWORD dw1, dw2;
	static void *pBuf;
    
    switch (fgcommand) {
        
        case FG_INIT:
            //Dynamicaly link the library
			if (ecode = X11_DLLOpen ()) {
				return ecode;
			}
            //Initialize system
            if (ecode = X11_InitSystem (&pci)) {
                return ecode;
            }
            //Get frame grabber handle
            if ((ecode = X11_GetHFG (&hfg, pci.PCIslot[0]))) {
                return ecode;
            }

            mode.color = COLOR_YCRCB;
            mode.advanced.buffertype = BUF_VIDEO;
            //Allocate image buffer
            if (ecode = X11_AllocBuffer (&mode, &buffer, (DWORD) hwnd)) {
                return ecode;
            }

            //Set frame grabber mode
            if (ecode = X11_SetMode (hfg, &mode)) {
                return ecode;
            }
            
            //Start continuous acquisition
            X11_StartAcquire (hfg, buffer.hbuf, AMODE_CONT);
            return 0;
            
        case FG_START:
            X11_StartAcquire (hfg, buffer.hbuf, AMODE_CONT);
            return 0;

        case FG_RESTART:
            X11_FreeBuffer (buffer.hbuf);
            if (ecode = X11_AllocBuffer (&mode, &buffer, (DWORD) hwnd)) {
                return ecode;
            }
            X11_StartAcquire (hfg, buffer.hbuf, AMODE_CONT);
            return 0;
            
        case FG_STOP:
            X11_StopAcquire (hfg);
            return 0;
    
        case FG_SCALE8:
        case FG_SCALE4:
            //set mode variable
            mode.scale = fgcommand - FG_SCALE;
            //get new image size
            if (ecode = X11_GetImageSize (&mode, &xsize, &ysize)) {
                return ecode;
            }
            //set frame grabber mode
            if (ecode = X11_SetMode (hfg, &mode)) {
                return ecode;
            }
            //resize display window
			SetRect (&rect, X0, Y0, X0 + xsize - 1, Y0 + ysize - 1);
			AdjustWindowRectEx (&rect, WS_EX_TOPMOST | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, TRUE, WS_EX_LEFT);
			SetWindowPos (hwnd, HWND_TOP, 0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1, SWP_NOMOVE);
            //allocate new buffer
            X11_FreeBuffer (buffer.hbuf);
            if (ecode = X11_AllocBuffer (&mode, &buffer, (DWORD) hwnd)) {
                return ecode;
            }
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
            if (ecode = X11_SetMode (hfg, &mode)) {
                return ecode;
            }
            return 0;

		case FG_NTSC:
		case FG_PALSECAM:
			//keep old input and scale
			dw1 = mode.input;
			dw2 = mode.scale;
			mode = ((fgcommand == FG_NTSC) ? mode_NTSC : mode_PAL);
			mode.input = dw1;
			mode.scale = dw2;
            mode.color = COLOR_YCRCB;
            mode.advanced.buffertype = BUF_VIDEO;

            //get new image size
            if (ecode = X11_GetImageSize(&mode, &xsize, &ysize)) {
                return ecode;
            }
            //set frame grabber mode
            if (ecode = X11_SetMode (hfg, &mode)) {
                return ecode;
            }
            //release old buffer
            X11_FreeBuffer (buffer.hbuf);
            //resize display window
			SetRect (&rect, X0, Y0, X0 + xsize - 1, Y0 + ysize - 1);
			AdjustWindowRectEx (&rect, WS_EX_TOPMOST | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, TRUE, WS_EX_LEFT);
			SetWindowPos (hwnd, HWND_TOP, 0, 0, rect.right - rect.left + 1, rect.bottom - rect.top + 1, SWP_NOMOVE);
            //allocate new buffer
            if (ecode = X11_AllocBuffer (&mode, &buffer, (DWORD) hwnd)) {
                return ecode;
            }
			return 0;

        case FG_QUIT:
            //clean up
            X11_StopAcquire (hfg);
            X11_CloseSystem ();
			X11_DLLClose ();
            return 0;

        default:
            return 0;
            
    }
}


void RepaintPS (HWND hwnd)
{
    //redraws bitmap in application window
    
    HDC hdc = GetDC (hwnd);
    HDC hdcmem = CreateCompatibleDC (hdc);
    POINT ptsize, ptorg;
    BITMAP bm;
    HBITMAP hbitmap = (xsize >= XSIZE) ? hbmp640 : hbmp320;
    
    SelectObject (hdcmem, hbitmap);
    SetMapMode (hdcmem, GetMapMode(hdc));
    GetObject (hbitmap, sizeof(BITMAP), &bm);
    ptsize.x = bm.bmWidth;
    ptsize.y = bm.bmHeight;
    DPtoLP (hdc, &ptsize, 1);
    
    ptorg.x = 0;
    ptorg.y = 0;
    DPtoLP (hdc, &ptorg, 1);
    BitBlt (hdc, 0, 0, ptsize.x, ptsize.y, hdcmem, ptorg.x, ptorg.y, SRCCOPY);
    DeleteDC (hdcmem);
    ReleaseDC (hwnd, hdc);

    return;
}


