Attribute VB_Name = "Module1"
'**********************************************************
'* This file contains special types, constants and
'* functions' prototypes for use with Sensoray
'* X11 frame grabber dynamic link library.
'*
'* Copyright (C) Sensoray Company 1998-2001
'**********************************************************/
'*-------------------------------------------------------
'* Mode constants. Do not modify.
'*-------------------------------------------------------*/
Public Const SCALE_ADVANCED = 0         'advanced image scaling;
Public Const SCALE8 = 8                 'full size image;
Public Const SCALE6 = 6                 '3/4 size image;
Public Const SCALE4 = 4                 '1/2 size image;
Public Const SCALE2 = 2                 '1/4 size image;

Public Const COLOR_MONO = 6             'monochrome, 1 byte/pixel;
Public Const COLOR_YCRCB = 4            'YCrCb (4:2:2), 2 bytes/pixel;
Public Const COLOR_RGB = 1              'color, RGB, 3 bytes/pixel;
Public Const COLOR_RGB32 = 0            'RGB32 color format;
Public Const COLOR_RGB16 = 2            'RGB16 color format;
Public Const COLOR_RGB15 = 3            'RGB15 color format;

Public Const STORE_DIB = 0              'image stored bottom to top;
Public Const STORE_FLAT = 1             'image stored top to bottom;

Public Const MUX_0 = 2                  'input channel, MUX0;
Public Const MUX_1 = 3                  'input channel, MUX1;
Public Const MUX_2 = 1                  'input channel, MUX2;
Public Const MUX_3 = 0                  'input channel, MUX3;
Public Const MUX_4 = 4                  'input channel, MUX4;

Public Const IMG_INTERLACED = 1         'interlaced image;
Public Const IMG_NONINTERLACED = 0      'non-interlaced image;

Public Const XTOT_NTSC = 780            'total output pixels, square pix NTSC;
Public Const XTOT_PAL = 944             'total output pixels, square pix PAL;
Public Const XACT_NTSC = 640            'active pixels, square pix NTSC;
Public Const XACT_PAL = 768             'active pixels, square pix PAL;
Public Const XDLY_NTSC = &H7C           'hor delay, NTSC;
Public Const XDLY_PAL = &H9A            'hor delay, PAL;
Public Const YTOT_NTSC = 525            'total output lines, NTSC;
Public Const YTOT_PAL = 625             'total output lines, PAL;
Public Const YACT_NTSC = 480            'active lines, NTSC;
Public Const YACT_PAL = 576             'active lines, PAL;
Public Const YDLY_NTSC = &H16           'vert delay, NTSC;
Public Const YDLY_PAL = &H20            'vert delay, PAL;

Public Const FORMAT_NTSC = 1            'NTSC input format;
Public Const FORMAT_NTSCJ = 2           'NTSC/Japan input format;
Public Const FORMAT_PAL = 3             'PAL (B, D, G, H, I) input format;
Public Const FORMAT_PALM = 4            'PAL(M) input format;
Public Const FORMAT_PALN = 5            'PAL(N) input format;
Public Const FORMAT_PALNC = 7           'PAL(N-combination) input format (Bt848A/849A);
Public Const FORMAT_SECAM = 6           'SECAM input format;

Public Const LNOTCH_OFF = 1             'luma notch filter disabled;
Public Const LNOTCH_ON = 0              'luma notch filter enabled;
Public Const LDEC_OFF = 1               'luma decimation filter disabled;
Public Const LDEC_ON = 0                'luma decimation filter enabled;
Public Const PEAK_OFF = 0               'normal luma LPF;
Public Const PEAK_ON = 1                'peaking luma LPF;
Public Const CAGC_OFF = 0               'chroma AGC off;
Public Const CAGC_ON = 1                'chroma AGC on;
Public Const CKILL_OFF = 0              'low color removal off;
Public Const CKILL_ON = 1               'low color removal on;
Public Const HFILT_AUTO = 0             'hor filter auto format;
Public Const HFILT_CIF = 1              'CIF hor filter;
Public Const HFILT_QCIF = 2             'QCIF hor filter;
Public Const HFILT_ICON = 3             'ICON hor filter;
Public Const RANGE_NORM = 0             'normal luma range (16-253);
Public Const RANGE_FULL = 1             'full luma range (0-255);
Public Const CORE_OFF = 0               'no coring;
Public Const CORE_8 = 1                 'coring on, limit 8;
Public Const CORE_16 = 2                'coring on, limit 16;
Public Const CORE_32 = 3                'coring on, limit 32;
Public Const YCOMB_OFF = 0              'vert LPF + interpolation;
Public Const YCOMB_ON = 1               'vert LPF only;
Public Const CCOMB_OFF = 0              'chroma comb disabled;
Public Const CCOMB_ON = 1               'chroma comb enabled;
Public Const SLEEP_OFF = 0              'both ADCs active;
Public Const Y_SLEEP = 2                'Y ADC in sleep mode;
Public Const C_SLEEP = 1                'C ADC in sleep mode;
Public Const CRUSH_OFF = 0              'non-adaptive AGC;
Public Const CRUSH_ON = 1               'adaptive AGC;
Public Const VFILT_0 = 0                'vert scaling filter 0;
Public Const VFILT_1 = 1                'vert scaling filter 1;
Public Const VFILT_2 = 2                'vert scaling filter 2;
Public Const VFILT_3 = 3                'vert scaling filter 3;
Public Const COLORBARS_OFF = 0          'color bars off;
Public Const COLORBARS_ON = 1           'color bars on;
Public Const GAMMA_REMOVE_ON = 0        'enable gamma correction removal;
Public Const GAMMA_REMOVE_OFF = 1       'disable gamma correction removal;
Public Const PKTP4 = 0                  'FIFO trigger point, 4 DWORDS;
Public Const PKTP8 = 1                  '8 DWORDS;
Public Const PKTP16 = 2                 '16 DWORDS;
Public Const PKTP32 = 3                 '32 DWORDS;
Public Const BIMODAL_ON = 1             'bi-modal buffer;
Public Const BIMODAL_OFF = 0            'single mode buffer;

Public Const BUF_MEM = 0                'buffer in RAM;
Public Const BUF_EXT = 1                'external buffer;
Public Const BUF_VIDEO = 2              'buffer in video RAM;

'*-------------------------------------------------------
'* Default mode parameters.
'*-------------------------------------------------------*/
Public Const DEF_SCALE = SCALE8         'default scaling mode, full image;
Public Const DEF_COLOR = COLOR_RGB      'default color mode, RGB output;
Public Const DEF_STORE = STORE_DIB      'default store mode, DIB;
Public Const DEF_INPUT = MUX_0          'default input channel, S-video;

'*-------------------------------------------------------
'* Default advanced mode parameters.
'*-------------------------------------------------------*/
Public Const DEF_YFACTOR = 1#           'vert scaling factor;
Public Const DEF_BRIGHT = 0             'brightness;
Public Const DEF_CONTRAST = &HD8        'contrast;
Public Const DEF_SAT_U = &HFE           'U-gain;
Public Const DEF_SAT_V = &HB4           'V-gain;
Public Const DEF_HUE = 0                'hue;
Public Const DEF_DEC_RAT = 0            'decimation rate, no decimation;
Public Const DEF_ADELAY_NTSC = &H68     'AGC gate delay for back-porch sampling;
Public Const DEF_BDELAY_NTSC = &H5D     'burst gate delay for sub-carrier sampling;
Public Const DEF_ADELAY_PAL = &H80      'AGC gate delay for back-porch sampling;
Public Const DEF_BDELAY_PAL = &H73      'burst gate delay for sub-carrier sampling;

'*-------------------------------------------------------
'* Control/status constants. Do not modify.
'*-------------------------------------------------------*/
Public Const STATUS_ERROR = &HB0000     'error;
Public Const STATUS_READY = &H800       'acquisition ready;
Public Const STATUS_VIDEO = &H20        'video present status change;
Public Const STATUS_HLOCK = &H10        'hor lock condition change;
Public Const STATUS_OFLOW = &H8         'luma or chroma ADC overflow;
Public Const STATUS_HSYNC = &H4         'new video line start;
Public Const STATUS_VSYNC = &H2         'new video field start;
Public Const STATUS_FMT = &H1           'video format change;
Public Const STATUS_READY_ALL = &H10000000 'acquisition ready;

Public Const DSTATUS_PRES = &H80        'video present;
Public Const DSTATUS_HLOC = &H40        'device in hor lock;
Public Const DSTATUS_FIELD = &H20       'field: 0 - odd, 1 - even;
Public Const DSTATUS_NUML = &H10        'number of lines: 0 - 525, 1 - 625;
Public Const DSTATUS_CSEL = &H8         'crystal selected;
Public Const DSTATUS_LOF = &H2          'luma overflow;
Public Const DSTATUS_COF = &H1          'chroma overflow;


Public Const AMODE_SINGLE = &H0         'single buffer acquisition;
Public Const AMODE_CONT = &H1           'continuous acquisition;
 
Public Const RMODE_ROW = &H0            'get row of data;
Public Const RMODE_COL = &H1            'get column of data;

Public Const SYS_GRABBERS = 8           'max frame grabbers in the system;
Public Const SYS_BUFFERS = 160          'max image buffers;
Public Const SYS_FRAMES = 64            'max frames in buffer;

'*-------------------------------------------------------
'* Error codes
'*-------------------------------------------------------*/
Public Const ERR_WD_OPEN = 1            'driver error
Public Const ERR_DEVICE_NOT_FOUND = 2   'PCI board not found
Public Const ERR_TOOMANYFGS = 3         'too many frame grabbers
Public Const ERR_IMAGE_FORMAT = 4       'invalid image format
Public Const ERR_INVALID_HANDLE = 5     'invalid handle
Public Const ERR_UNABLE_TO_LOCK = 6     'unable to lock any boards
Public Const ERR_NOHANDLES = 7          'no handles available
Public Const ERR_ALLOC_BUFFER = 8       'error allocating buffer memory
Public Const ERR_LOCK_DBUFFER = 9       'error locking buffer
Public Const ERR_LOCK_RBUFFER = 10      'error locking buffer
Public Const ERR_DIB_MEM_ALLOC = 11     'DIB memory allocation error
Public Const ERR_ACQ_TIMEOUT = 12       'acquisition timeout
Public Const ERR_ILLEGAL_MODE_1 = 13    'illegal entry in MODE.advanced;
Public Const ERR_ILLEGAL_MODE_2 = 14    'illegal entry in MODE;
Public Const ERR_FILE_OPEN = 15         'error opening file;
Public Const ERR_FILE_WRITE = 16        'error writing to file;
Public Const ERR_INTERRUPT = 17         'error installing interrupt;
Public Const ERR_FRAMENO = 18           'invalid number of frames specified;
Public Const ERR_ILLEGAL_MODE_3 = 19    'invalid acquisition mode;
Public Const ERR_ILLEGAL_MODE_4 = 20    'invalid mode in X11_GetRC;
Public Const ERR_INVALID_PAR = 21       'invalid parameter value;
Public Const ERR_LOADING_DLL = 22       'error loading DLL;
Public Const ERR_FUNCADDR = 23          'error getting function address;
Public Const ERR_DDCREATE = 24          'error creating DirectDraw object;
Public Const ERR_DDCOLORFORMAT = 25     'incompatible color formats;
Public Const ERR_DDWINDOWFORMAT = 26    'incompatible window formats;
Public Const ERR_DD1 = 27               'DirectDraw error;
Public Const ERR_DD2 = 28               'DirectDraw error;
Public Const ERR_DD3 = 29               'DirectDraw error;
Public Const ERR_DD4 = 30               'DirectDraw error;
Public Const ERR_DD5 = 31               'DirectDraw error;
Public Const ERR_DD6 = 32               'DirectDraw error;
Public Const ERR_DD7 = 33               'DirectDraw error;
Public Const ERR_DD8 = 34               'DirectDraw error;
Public Const ERR_DD = 35                'DirectDraw error;
Public Const ERR_BUFFORMAT = 36         'invalid buffer format;
Public Const ERR_THREAD = 37            'error starting thread;
Public Const ERR_NOTSUPPORTED = 38      'feature not supported;
Public Const ERR_NOHANDLE = 39          'failed to get a frame grabber handle;
Public Const ERR_INVALID_CHANNEL = 40   'invalid video switch channel;
Public Const ERR_I2CTIMEOUT = 41        'internal I2C timeout;
Public Const ERR_I2C = 42               'internal I2C error;
Public Const ERR_DLL = 43               'DLL not loaded;
Public Const ERR_PCI = 44               'PCI access error
Public Const ERR_DATA_ACCESS_TO = 45    'internal data access timeout

'*-------------------------------------------------------
'* Special types
'*-------------------------------------------------------*/
Type PCI                                'system structure;
    boards As Long                       'number of compatible PCI boards found;
    pcislot(0 To SYS_GRABBERS - 1) As Long 'PCI slot numbers for boards found;
End Type

Type frame                              'frame structure;
    lpbmi As Long                       'pointer to BITMAPINFO structure;
    lpvbits As Long                     'pointer to image data;
End Type

Type BUFFER                             'image buffer structure;
    hbuf As Long                        'handle to buffer;
    dwFrames As Long                    'number of frames in buffer;
    dframe(0 To SYS_FRAMES - 1) As frame 'array of FRAME structures;
End Type

Type MODE_ADVANCED                      'frame grabber advanced mode structure;
    interlace As Long                   'interlacing flag;
    xTotal As Long                      'total hor pixels;
    xActive As Long                     'active hor pixels;
    xDelay As Long                      'hor delay;
    yFactor As Single                   'vert scaling factor;
    yActive As Long                     'active vert pixels (lines);
    yDelay As Long                      'vert delay, lines;
    FORMAT As Long                      'input video format;
    BRIGHT As Long                      'brightness;
    CONTRAST As Long                    'contrast;
    SAT_U As Long                       'chroma (U) gain;
    SAT_V As Long                       'chroma (V) gain;
    HUE As Long                         'hue;
    LNOTCH As Long                      'luma notch filter control;
    LDEC As Long                        'luma decimation control;
    DEC_RAT As Long                     'temporal decimation control;
    PEAK As Long                        'luma LPF peaking control (Bt848A/849A);
    CAGC As Long                        'chroma ADC control;
    CKILL As Long                       'low color removal control;
    HFILT As Long                       'luma low pass filter mode;
    RANGE As Long                       'luma output range;
    CORE As Long                        'luma coring control;
    YCOMB As Long                       'luma comb enable;
    CCOMB As Long                       'chroma comb enable;
    ADELAY As Long                      'AGC gate delay for back-porch sampling;
    BDELAY As Long                      'burst gate delay for sub-carrier sampling;
    SLEEP As Long                       'ADCs' sleep mode;
    CRUSH As Long                       'adaptive AGC;
    VFILT As Long                       'vertical scaling filter control;
    COLOR_BARS As Long                  'color bars test enable;
    GAMMA As Long                       'gamma correction removal;
    PKTP As Long                        'FIFO trigger point;
    bimodal As Long                     'bimodal buffer flag;
    colorkey As Long                    'color key for DirectDraw overlay;
    buffertype As Long                  'type of buffer: memory, external, or video;
    gpintmode As Long                   'GPIO port interrupt mode;
    reserved1 As Long                   'reserved;
    reserved2 As Long                   'reserved;
    reserved3 As Long                   'reserved;
    reserved4 As Long                   'reserved;
End Type

Type MODE                               'frame grabber mode structure;
    fgscale As Long                     'image scale;
    color As Long                       'output color encoding;
    store As Long                       'store as DIB or flat;
    input As Long                       'input channel;
    advanced As MODE_ADVANCED
End Type






'*-------------------------------------------------------
'* SX11.dll exported functions prototypes
'*-------------------------------------------------------*/
Declare Function X11_InitSystem Lib "sx11.dll" (dpci As PCI) As Long
Declare Function X11_GetHFG Lib "sx11.dll" (phfg As Long, ByVal pcislot As Long) As Long
Declare Function X11_AllocBuffer Lib "sx11.dll" (dmode As MODE, pbuffer As BUFFER, ByVal param As Long) As Long
Declare Sub X11_FreeBuffer Lib "sx11.dll" (ByVal hbuf As Long)
Declare Sub X11_CloseSystem Lib "sx11.dll" ()
Declare Function X11_Acquire Lib "sx11.dll" (ByVal hfg As Long, ByVal hbuf As Long, ByVal timeout As Single, status As Long) As Long
Declare Function X11_StartAcquire Lib "sx11.dll" (ByVal hfg As Long, ByVal hbuf As Long, ByVal acqmode As Long) As Long
Declare Function X11_StopAcquire Lib "sx11.dll" (ByVal hfg As Long) As Long
Declare Function X11_GetStatus Lib "sx11.dll" (ByVal hfg As Long, status As Long) As Long
Declare Function X11_ResetStatus Lib "sx11.dll" (ByVal hfg As Long, status As Long) As Long
Declare Function X11_SetMode Lib "sx11.dll" (ByVal hfg As Long, dmode As MODE) As Long
Declare Function X11_GetImageSize Lib "sx11.dll" (dmode As MODE, x As Long, y As Long) As Long
Declare Function X11_GetRC Lib "sx11.dll" (ByVal hbuf As Long, ByVal frame As Long, pmode As MODE, ByVal rowcol As Long, ByVal rcnum As Long, parray As Byte) As Long
Declare Function X11_WritePort Lib "sx11.dll" (ByVal hfg As Long, ByVal data As Long, ByVal mask As Long) As Long
Declare Function X11_ReadPort Lib "sx11.dll" (ByVal hfg As Long, pval As Long) As Long

