Attribute VB_Name = "Module2"
'Sensoray X11 VB sample application
'Copyright (C) Sensoray 2001

'*-------------------------------------------------------
'* Some Windows API data structures
'*-------------------------------------------------------
Type BITMAPINFO
    biSize As Long
    biWidth As Long
    biHeight As Long
    biPlanes As Integer
    biBitCount As Integer
    biCompression As Long
    biSizeImage As Long
    biXPelsPerMeter As Long
    biYPelsPerMeter As Long
    biClrUsed As Long
    biClrImportant As Long
    bmiColors(0 To 255) As Long
End Type

Type RECT
    left As Long
    top As Long
    right As Long
    bottom As Long
End Type

'*-------------------------------------------------------
'* Windows API functions prototypes
'*-------------------------------------------------------
Declare Sub GetClientRect Lib "user32" (ByVal hWnd As Long, clrect As RECT)
Declare Function SetDIBitsToDevice Lib "gdi32" (ByVal hDC As Long, ByVal XDest As Integer, ByVal YDest As Integer, ByVal cx As Long, ByVal cy As Long, ByVal XSrc As Integer, ByVal YSrc As Integer, ByVal uStartScan As Integer, ByVal cScanLines As Integer, ByVal lpvbits As Long, ByVal lpbmi As Long, ByVal fuColorUse As Integer) As Integer

'*-------------------------------------------------------
'* Globals
'*-------------------------------------------------------
Public hfg As Long         'frame grabber handle
Public dpci As PCI         'PCI structure
Public dbuffer As BUFFER   'buffer
Public DEF_MODE As MODE    'default mode
Public DEF_ADVANCED As MODE_ADVANCED
Public fgmode As MODE      'frame grabber mode
Public lpbmi As Long        'pointer to bitmap info
Public lpvbits As Long      'pointer to bitmap data
Public crect As RECT        'client rectangle
Public stopflag As Boolean  'stop flag

' --- nkanher ---
Public capture_on_flag As Boolean
Public capture_directory As String
Public n_frame As Integer
Public max_frames As Integer
Public seq_title As String
' --- nkanher ---

Public ecode As Long
Public Const XSIZE = 640
Public Const YSIZE = 480

Sub Main()
'initialize defaults
DEF_ADVANCED.interlace = IMG_INTERLACED
DEF_ADVANCED.xTotal = XTOT_NTSC
DEF_ADVANCED.xActive = XACT_NTSC
DEF_ADVANCED.xDelay = XDLY_NTSC
DEF_ADVANCED.yFactor = DEF_YFACTOR
DEF_ADVANCED.yActive = YACT_NTSC
DEF_ADVANCED.yDelay = YDLY_NTSC
DEF_ADVANCED.FORMAT = FORMAT_NTSC
DEF_ADVANCED.BRIGHT = DEF_BRIGHT
DEF_ADVANCED.CONTRAST = DEF_CONTRAST
DEF_ADVANCED.SAT_U = DEF_SAT_U
DEF_ADVANCED.SAT_V = DEF_SAT_V
DEF_ADVANCED.HUE = DEF_HUE
DEF_ADVANCED.LNOTCH = LNOTCH_OFF
DEF_ADVANCED.LDEC = LDEC_OFF
DEF_ADVANCED.DEC_RAT = DEF_DEC_RAT
DEF_ADVANCED.PEAK = PEAK_OFF
DEF_ADVANCED.CAGC = CAGC_OFF
DEF_ADVANCED.CKILL = CKILL_OFF
DEF_ADVANCED.HFILT = HFILT_AUTO
DEF_ADVANCED.RANGE = RANGE_NORM
DEF_ADVANCED.CORE = CORE_OFF
DEF_ADVANCED.YCOMB = YCOMB_OFF
DEF_ADVANCED.CCOMB = CCOMB_OFF
DEF_ADVANCED.ADELAY = DEF_ADELAY_NTSC
DEF_ADVANCED.BDELAY = DEF_BDELAY_NTSC
DEF_ADVANCED.SLEEP = SLEEP_OFF
DEF_ADVANCED.CRUSH = CRUSH_OFF
DEF_ADVANCED.VFILT = VFILT_0
DEF_ADVANCED.COLOR_BARS = COLORBARS_OFF
DEF_ADVANCED.GAMMA = GAMMA_REMOVE_OFF
DEF_ADVANCED.PKTP = PKTP32
DEF_ADVANCED.bimodal = BIMODAL_OFF
DEF_ADVANCED.colorkey = RGB(255, 255, 255)
DEF_ADVANCED.buffertype = BUF_MEM
DEF_ADVANCED.reserved1 = 0
DEF_ADVANCED.reserved2 = 0
DEF_ADVANCED.reserved3 = 0
DEF_ADVANCED.reserved4 = 0

DEF_MODE.fgscale = DEF_SCALE
DEF_MODE.color = DEF_COLOR
DEF_MODE.store = DEF_STORE
DEF_MODE.input = DEF_INPUT
DEF_MODE.advanced = DEF_ADVANCED

Call Init

End Sub
Sub ErrorHandler(ecode As Long)
    'format output
    out$ = FORMAT$(ecode, "00")
    If ecode <> 12 Then
        'post message
        MsgBox "Error " + out$, MB_OK + MB_ICONSTOP, "X11 VB demo"
    Else
        If MsgBox("Acquisition timeout. Retry?", vbRetryCancel, "X11 VB demo") = vbCancel Then
            'exit
            X11_CloseSystem
            End
        End If
    End If
End Sub

Sub Init()
    Dim status As Long, dummy As Long
    
    'set up default mode
    fgmode = DEF_MODE
    
    'initialize system
    ecode = X11_InitSystem(dpci)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    
    'get frame grabber handle
    ecode = X11_GetHFG(hfg, dpci.pcislot(0))
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    
    'allocate image buffer
    ecode = X11_AllocBuffer(fgmode, dbuffer, 1)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    
    'set up globals
    lpbmi = dbuffer.dframe(0).lpbmi
    lpvbits = dbuffer.dframe(0).lpvbits
    'set frame grabber mode
    ecode = X11_SetMode(hfg, fgmode)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    
    Form1.ScaleMode = vbPixels
    Form1.Scale (20, 20)-(20 + XSIZE, 20 + YSIZE)
    Form1.ScaleMode = vbPixels
    Form1.Show
    Form1.Picture1.Visible = True

    Call GetClientRect(Form1.Picture1.hWnd, crect)
    
    Do
        ecode = X11_Acquire(hfg, dbuffer.hbuf, 0.5, status)
        If ecode <> 0 Then
            Call ErrorHandler(ecode)
        End If
        dummy = SetDIBitsToDevice(Form1.Picture1.hDC, crect.left, crect.top, crect.right - crect.left + 1, crect.bottom - crect.top + 1, 0, 0, 0, YSIZE, lpvbits, lpbmi, 0)
        
        If capture_on_flag <> True Then
        Form1.Picture1.Refresh
        End If
        
        DoEvents
    Loop While stopflag = False

    X11_CloseSystem
    End
End Sub
