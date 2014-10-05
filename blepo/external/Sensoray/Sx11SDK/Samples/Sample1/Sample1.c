/***********************************************************
 * SX11 Sample Application 1.
 * Copyright (C) 1998-2002 Sensoray
 *
 * This sample application illustrates the use of sx11.dll.
 * It allows image capture and display, saving the image to
 * the file, and changing some operation modes.
 *
 * The method implemented herein is the slowest (of all
 * sample applications): the image is acquired into the buffer,
 * then displayed, acquired, displayed, etc.  In this manner,
 * at least every other frame is usually lost, unless the image
 * is scaled down not to exceed a quarter size, so the frames
 * per second (FPS) rate does not exceed 15 for scales 8 and 6.
 *
 * All calls to the DLL functions are localized inside the
 * FrameGrabber function, which is not imperative.
 ************************************************************/

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "sample1.h"
#include "sx11.h"
#include "sx11f.h"


LRESULT WINAPI WndProc (HWND, UINT, UINT, LONG);    // Window procedure
ECODE FrameGrabber (DWORD);                 // Frame grabber procedure
void ErrAbort (ECODE);                      // Error handler
void SaveBMP (BOOL);                        // File save procedure

/***************************************
 * Globals
 ***************************************/ 
HWND hwnd;                                  //Main window handle;
LPBITMAPINFO lpbmi;                         //pointer to DIB info structure;
LPVOID lpvbits;                             //pointer to DIB data;
/***************************************/

// Main function
int PASCAL WinMain (HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASS wndclass;
    ECODE ecode;
    char szAppName [] = "X11 Sample1";
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
    wndclass.lpszMenuName = "Sample1";
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
        } else if (ecode = FrameGrabber (FG_GRAB)) {			//grab a frame if no messages in queue
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
    ECODE ecode;
    static HMENU hmenu;

    switch (message) {
        case WM_CREATE:
            hmenu = GetMenu (hwnd);
            CheckMenuItem (hmenu, FG_SCALE8, MF_CHECKED);
            CheckMenuItem (hmenu, FG_COLOR, MF_CHECKED);
            CheckMenuItem (hmenu, FG_CHAN0, MF_CHECKED);
            CheckMenuItem (hmenu, FG_NTSC, MF_CHECKED);
            return 0;

        case WM_PAINT:
            BeginPaint (hwnd, &ps);
            //display bitmap 
            hdc = GetDC (hwnd);
            GetClientRect (hwnd, (LPRECT) &rect);
            SetDIBitsToDevice (hdc, rect.left, rect.top,
                               rect.right - rect.left + 1,
                               rect.bottom - rect.top + 1,
                               0, 0, 0, (UINT) lpbmi -> bmiHeader.biHeight,
                               lpvbits, lpbmi, DIB_RGB_COLORS);
  
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

                case FG_SCALE + SCALE2:
                case FG_SCALE + SCALE4:
                case FG_SCALE + SCALE6:
                case FG_SCALE + SCALE8:
                    if ((ecode = FrameGrabber (wParam))) {
                        ErrAbort (ecode);
                    } else {
                        //check selected menu item
                        CheckMenuItem (hmenu, FG_SCALE2, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_SCALE4, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_SCALE6, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_SCALE8, MF_UNCHECKED);
                        CheckMenuItem (hmenu, wParam, MF_CHECKED);
                    }
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

                case FG_COLOR:
                case FG_MONO:
                    FrameGrabber (FG_STOP);
                    if ((ecode = FrameGrabber (wParam))) {
                        ErrAbort (ecode);
                    } else {
                        //check selected menu item
                        CheckMenuItem (hmenu, FG_MONO, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_COLOR, MF_UNCHECKED);
                        CheckMenuItem (hmenu, wParam, MF_CHECKED);
                    }
                    FrameGrabber (FG_START);
                    break;

				case FG_NTSC:
				case FG_PALSECAM:
					FrameGrabber (FG_STOP);
                    if ((ecode = FrameGrabber (wParam))) {
                        ErrAbort (ecode);
                    } else {
                        //check selected menu item
                        CheckMenuItem (hmenu, FG_NTSC, MF_UNCHECKED);
                        CheckMenuItem (hmenu, FG_PALSECAM, MF_UNCHECKED);
                        CheckMenuItem (hmenu, wParam, MF_CHECKED);
                    }
                    FrameGrabber (FG_START);
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
 * File save procedure
 **********************************************************/
void SaveBMP (BOOL color)
{
    OPENFILENAME ofn;
    BITMAPFILEHEADER bfh;
    HANDLE hfile;
    DWORD byteswritten;
    BOOL ecode = FALSE;
    long bmpsize = lpbmi -> bmiHeader.biWidth *
                   lpbmi -> bmiHeader.biHeight *
                   lpbmi -> bmiHeader.biBitCount / 8;
    const long SIZE_BFH = sizeof (BITMAPFILEHEADER);
    const long SIZE_BIH = sizeof (BITMAPINFOHEADER);
    const long SIZE_CLR = 256 * sizeof (RGBQUAD);
    static char szFileName [_MAX_PATH] = "image.bmp";
    static char szTitleName [_MAX_FNAME + _MAX_EXT];
    static char * szFilter[] = {"Bitmap Files (*.BMP)\0*.bmp\0"};

    // Fill in the structure for File Save dialog
    ofn.lStructSize = sizeof (OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = szFilter[0];
    ofn.lpstrCustomFilter = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = _MAX_PATH;
    ofn.lpstrFileTitle = szTitleName;
    ofn.nMaxFileTitle = _MAX_FNAME + _MAX_EXT;
    ofn.lpstrDefExt = "bmp";
    ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

    // Fill in the BITMAPFILEHEADER structure
    bfh.bfType = * (WORD *) "BM";
    bfh.bfOffBits = SIZE_BFH + SIZE_BIH + (color ? 0 : SIZE_CLR);
    bfh.bfSize =  bfh.bfOffBits + bmpsize;

    // Get file name
    if (!GetSaveFileName (&ofn)) {
        if (CommDlgExtendedError ()) {
            MessageBox (hwnd, "Error saving file.",
                        "SX11 error", MB_ICONSTOP | MB_OK);
        }
        return;
    }

    // Create a file
    if ((hfile = CreateFile (ofn.lpstrFile, GENERIC_WRITE, 0,
                             NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE,
                             NULL)) == INVALID_HANDLE_VALUE) {
        ecode = TRUE;
    } else {
        // Write BITMAPFILEHEADER to file
        if (!WriteFile (hfile, (LPCVOID) &bfh, SIZE_BFH,
                        &byteswritten, NULL)) {
            ecode = TRUE;
        } else {
            // Write BITMAPINFOHEADER to file
            if (!WriteFile (hfile, (LPCVOID) lpbmi, SIZE_BIH,
                            &byteswritten, NULL)) {
                ecode = TRUE;
            } else {
                // If monochrome image - write color table to file
                if (!color) {
                    if (!WriteFile (hfile, (LPCVOID) ((BYTE *) lpbmi + SIZE_BIH),
                                    SIZE_CLR, &byteswritten, NULL)) {
                        ecode = TRUE;
                    }
                }
                // Write bitmap data to file
                if (!ecode) {
                    if (!WriteFile (hfile, (LPCVOID) lpvbits, bmpsize,
                                    &byteswritten, NULL)) {
                        ecode = TRUE;
                    }
                }
            }
        }
    }
    CloseHandle (hfile);
    if (ecode) {
        MessageBox (hwnd, "Error saving file.", "SX11 error", MB_ICONSTOP | MB_OK);
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
    static BUFFER buffer;
	static MODE mode_NTSC = {DEF_MODE_NTSC};
	static MODE mode_PAL = {DEF_MODE_PAL};
    static MODE mode = {DEF_MODE_NTSC};
    static HFG hfg;
    DWORD xsize, ysize;
    static DWORD timer0;
    DWORD timer;
    static DWORD frames = 0;
    static BOOL acqflag = FALSE;
    ECODE ecode;
    char str[80];
    float timeout = .5f;
    DWORD status, dw1, dw2;
    
    switch (fgcommand) {
        
        case FG_INIT:
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
            //Allocate image buffer (1 frame)
            if ((ecode = X11_AllocBuffer (&mode, &buffer, 1))) {
                return ecode;
            }
            //Set up globals
            lpbmi = buffer.frame[0].lpbmi;
            lpvbits = buffer.frame[0].lpvbits;
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
                sprintf (str, "X11 Sample1;   FPS=%4.1f\0", (float) frames * CLOCKS_PER_SEC / timer);
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
    
        case FG_SCALE8:
        case FG_SCALE6:
        case FG_SCALE4:
        case FG_SCALE2:
            //set mode variable
            mode.scale = fgcommand - FG_SCALE;
            //get new image size
            if ((ecode = X11_GetImageSize(&mode, &xsize, &ysize))) {
                return ecode;
            }
            //set frame grabber mode
            if ((ecode = X11_SetMode (hfg, &mode))) {
                return ecode;
            }
            //allocate new buffer
            X11_FreeBuffer (buffer.hbuf);
            if (!(ecode = X11_AllocBuffer (&mode, &buffer, 1))) {
                lpbmi = buffer.frame[0].lpbmi;
                lpvbits = buffer.frame[0].lpvbits;
            }
            else {
                return ecode;
            }
            //resize display window
            SetWindowPos (hwnd, HWND_TOP, 0, 0,
                      xsize + GetSystemMetrics (SM_CXBORDER) * 2,
                      ysize + GetSystemMetrics (SM_CYCAPTION) +
                      GetSystemMetrics (SM_CYMENU) *
                      ((mode.scale == SCALE2) ? 2 : 1), SWP_NOMOVE);
            //reset variables
            timer0 = clock();
            frames = 0;
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

        case FG_MONO:
            //set monochrome mode
            mode.color = COLOR_MONO;
            if ((ecode = X11_SetMode (hfg, &mode))) {
                return ecode;
            }
            //allocate new buffer
            X11_FreeBuffer (buffer.hbuf);
            if ((ecode = X11_AllocBuffer (&mode, &buffer, 1))) {
                return ecode;
            }
            //set globals
            lpbmi = buffer.frame[0].lpbmi;
            lpvbits = buffer.frame[0].lpvbits;
            return 0;
                    
        case FG_COLOR:
            //set color mode
            mode.color = COLOR_RGB;
            if ((ecode = X11_SetMode (hfg, &mode))) {
                return ecode;
            }
            //allocate new buffer
            X11_FreeBuffer (buffer.hbuf);
            if ((ecode = X11_AllocBuffer (&mode, &buffer, 1))) {
                return ecode;
            }
            //set globals
            lpbmi = buffer.frame[0].lpbmi;
            lpvbits = buffer.frame[0].lpvbits;
            return 0;
           
		case FG_NTSC:
		case FG_PALSECAM:
			//keep old input and scale
			dw1 = mode.input;
			dw2 = mode.scale;
			mode = ((fgcommand == FG_NTSC) ? mode_NTSC : mode_PAL);
			mode.input = dw1;
			mode.scale = dw2;

            //get new image size
            if ((ecode = X11_GetImageSize(&mode, &xsize, &ysize))) {
                return ecode;
            }
            //set frame grabber mode
            if ((ecode = X11_SetMode (hfg, &mode))) {
                return ecode;
            }
            //allocate new buffers
            X11_FreeBuffer (buffer.hbuf);
            if ((ecode = X11_AllocBuffer (&mode, &buffer, 1))) {
                return ecode;
            }
            //set up global pointers
            lpbmi = buffer.frame[0].lpbmi;
            lpvbits = buffer.frame[0].lpvbits;
            //resize display window
            SetWindowPos (hwnd, HWND_TOP, 0, 0,
                      xsize + GetSystemMetrics (SM_CXBORDER) * 2,
                      ysize + GetSystemMetrics (SM_CYCAPTION) +
                      GetSystemMetrics (SM_CYMENU) *
                      ((mode.scale == SCALE2) ? 2 : 1), SWP_NOMOVE);
            //reset variables
            timer0 = clock();
            frames = 0;
            return 0;

        case FG_QUIT:
            //clean up
            X11_CloseSystem ();
			X11_DLLClose ();
            return 0;

        case FG_SAVE:
            SaveBMP (mode.color == COLOR_RGB);
            return 0;

        default:
            return 0;
            
    }
}



