/**********************************************************
 * This file contains special types, constants
 * for use with Sensoray sx11 DLL v.2.10.
 *
 * Copyright (C) Sensoray 2002
 **********************************************************/
/*-------------------------------------------------------
 * Default mode parameters.
 *-------------------------------------------------------*/
#define DEF_SCALE               SCALE8          //default scaling mode, full image;
#define DEF_COLOR               COLOR_RGB       //default color mode, RGB output;
#define DEF_STORE               STORE_DIB       //default store mode, DIB;
#define DEF_MUX                 MUX_0           //default input channel, S-video;

/*-------------------------------------------------------
 * Default advanced mode parameters.
 *-------------------------------------------------------*/
#define DEF_YFACTOR             1.f             //vert scaling factor;
#define DEF_BRIGHT              0               //brightness;
#define DEF_CONTRAST            0x0d8           //contrast;
#define DEF_SAT_U               0x0fe           //U-gain;
#define DEF_SAT_V               0x0b4           //V-gain;
#define DEF_HUE                 0               //hue;
#define DEF_DEC_RAT             0               //decimation rate, no decimation;
#define DEF_ADELAY_NTSC         0x68            //AGC gate delay for back-porch sampling;
#define DEF_BDELAY_NTSC         0x5d            //burst gate delay for sub-carrier sampling;
#define DEF_ADELAY_PAL          0x80            //AGC gate delay for back-porch sampling;
#define DEF_BDELAY_PAL          0x73            //burst gate delay for sub-carrier sampling;

// default mode structures
#define DEF_ADVANCED_NTSC        {IMG_INTERLACED, XTOT_NTSC, XACT_NTSC, XDLY_NTSC, \
                                 DEF_YFACTOR, YACT_NTSC, YDLY_NTSC, FORMAT_NTSC, \
                                 DEF_BRIGHT, DEF_CONTRAST, DEF_SAT_U, DEF_SAT_V, \
                                 DEF_HUE, LNOTCH_ON, LDEC_OFF, DEF_DEC_RAT, \
                                 PEAK_OFF, CAGC_OFF, CKILL_OFF, HFILT_AUTO, \
                                 RANGE_NORM, CORE_OFF, YCOMB_OFF, CCOMB_ON, \
                                 DEF_ADELAY_NTSC, DEF_BDELAY_NTSC, SLEEP_OFF, CRUSH_OFF, \
                                 VFILT_0, COLORBARS_OFF, GAMMA_REMOVE_OFF, PKTP32, \
                                 BIMODAL_OFF, RGB(255, 255, 255), BUF_MEM, GPINT_LEVEL | GPINT_NONINV, 0, 0, 0, 0}

#define DEF_ADVANCED_PAL         {IMG_INTERLACED, XTOT_PAL, XACT_PAL, XDLY_PAL, \
                                 DEF_YFACTOR, YACT_PAL, YDLY_PAL, FORMAT_PAL, \
                                 DEF_BRIGHT, DEF_CONTRAST, DEF_SAT_U, DEF_SAT_V, \
                                 DEF_HUE, LNOTCH_ON, LDEC_OFF, DEF_DEC_RAT, \
                                 PEAK_OFF, CAGC_OFF, CKILL_OFF, HFILT_AUTO, \
                                 RANGE_NORM, CORE_OFF, YCOMB_OFF, CCOMB_ON, \
                                 DEF_ADELAY_PAL, DEF_BDELAY_PAL, SLEEP_OFF, CRUSH_OFF, \
                                 VFILT_0, COLORBARS_OFF, GAMMA_REMOVE_OFF, PKTP32, \
                                 BIMODAL_OFF, RGB(255, 255, 255), BUF_MEM, GPINT_LEVEL | GPINT_NONINV, 0, 0, 0, 0}

#define DEF_MODE_NTSC            DEF_SCALE, DEF_COLOR, DEF_STORE, DEF_MUX, DEF_ADVANCED_NTSC
#define DEF_MODE_PAL             DEF_SCALE, DEF_COLOR, DEF_STORE, DEF_MUX, DEF_ADVANCED_PAL

/*-------------------------------------------------------
 * Control/status constants. Do not modify.
 *-------------------------------------------------------*/
#define STATUS_ERROR            0x000b0000      //error;
#define STATUS_READY            0x00000800      //acquisition ready;
#define STATUS_GPINT			0x00000200		//interrupt on GPIO port;
#define STATUS_VIDEO            0x00000020      //video present status change;
#define STATUS_HLOCK            0x00000010      //hor lock condition change;
#define STATUS_OFLOW            0x00000008      //luma or chroma ADC overflow;
#define STATUS_HSYNC            0x00000004      //new video line start;
#define STATUS_VSYNC            0x00000002      //new video field start;
#define STATUS_FMT              0x00000001      //video format change;
#define STATUS_READY_ALL        0x10000000      //acquisition ready;

#define DSTATUS_PRES            0x00000080      //video present;
#define DSTATUS_HLOC            0x00000040      //device in hor lock;
#define DSTATUS_FIELD           0x00000020      //field: 0 - odd, 1 - even;
#define DSTATUS_NUML            0x00000010      //number of lines: 0 - 525, 1 - 625;
#define DSTATUS_PLOCK			0x00000004		//PLL out of lock;
#define DSTATUS_LOF             0x00000002      //luma overflow;
#define DSTATUS_COF             0x00000001      //chroma overflow;

#define AMODE_SINGLE            0x0L            //single buffer acquisition;
#define AMODE_CONT              0x1L            //continuous acquisition;

#define RMODE_ROW               0x0L            //get row;
#define RMODE_COL               0x1L            //get column;

/*-------------------------------------------------------
 * Mode constants. Do not modify.
 *-------------------------------------------------------*/
#define SCALE_ADVANCED          0               //advanced image scaling;
#define SCALE8                  8               //full size image;
#define SCALE6                  6               //3/4 size image;
#define SCALE4                  4               //1/2 size image;
#define SCALE2                  2               //1/4 size image;

#define COLOR_MONO              6               //monochrome, 1 byte/pixel;
#define COLOR_YCRCB             4               //color, YCrCb (4:2:2), 2 bytes/pixel;
#define COLOR_RGB               1               //color, RGB, 3 bytes/pixel;
#define COLOR_RGB32             0               //RGB32 color format;
#define COLOR_RGB16             2               //RGB16 color format;
#define COLOR_RGB15             3               //RGB15 color format;

#define STORE_DIB               0               //image stored bottom to top;
#define STORE_FLAT              1               //image stored top to bottom;

#define MUX_0                   2               //input channel, S-video;
#define MUX_1                   3               //input channel, Video1;
#define MUX_2                   1               //input channel, Video2;
#define MUX_3                   0               //input channel, Video3;
#define MUX_4                   4               //input channel, Video4;

#define IMG_INTERLACED          1               //interlaced image;
#define IMG_NONINTERLACED       0               //non-interlaced image;

#define XTOT_NTSC               780             //total output pixels, square pix NTSC;
#define XTOT_PAL                944             //total output pixels, square pix PAL;
#define XACT_NTSC               640             //active pixels, square pix NTSC;
#define XACT_PAL                768             //active pixels, square pix PAL;
#define XDLY_NTSC               0x7c            //hor delay, NTSC;
#define XDLY_PAL                0x9a            //hor delay, PAL;

#define YTOT_NTSC               525             //total output lines, NTSC;
#define YTOT_PAL                625             //total output lines, PAL;
#define YACT_NTSC               480             //active lines, NTSC;
#define YACT_PAL                576             //active lines, PAL;
#define YDLY_NTSC               0x16            //vert delay, NTSC;
#define YDLY_PAL                0x20            //vert delay, PAL;
 
#define FORMAT_NTSC             1               //NTSC input format;
#define FORMAT_NTSCJ            2               //NTSC/Japan input format;
#define FORMAT_PAL              3               //PAL (B, D, G, H, I) input format;
#define FORMAT_PALM             4               //PAL(M) input format;
#define FORMAT_PALN             5               //PAL(N) input format;
#define FORMAT_PALNC            7               //PAL(N-combination) input format;
#define FORMAT_SECAM            6               //SECAM input format;

#define LNOTCH_OFF              1               //luma notch filter disabled;
#define LNOTCH_ON               0               //luma notch filter enabled;

#define LDEC_OFF                1               //luma decimation filter disabled;
#define LDEC_ON                 0               //luma decimation filter enabled;

#define PEAK_OFF                0               //normal luma LPF;
#define PEAK_ON                 1               //peaking luma LPF;

#define CAGC_OFF                0               //chroma AGC off;
#define CAGC_ON                 1               //chroma AGC on;

#define CKILL_OFF               0               //low color removal off;
#define CKILL_ON                1               //low color removal on;

#define HFILT_AUTO              0               //hor filter auto format;
#define HFILT_CIF               1               //CIF hor filter;
#define HFILT_QCIF              2               //QCIF hor filter;
#define HFILT_ICON              3               //ICON hor filter;

#define RANGE_NORM              0               //normal luma range (16-253);
#define RANGE_FULL              1               //full luma range (0-255);

#define CORE_OFF                0               //no coring;
#define CORE_8                  1               //coring on, limit 8;
#define CORE_16                 2               //coring on, limit 16;
#define CORE_32                 3               //coring on, limit 32;

#define YCOMB_OFF               0               //vert LPF + interpolation;
#define YCOMB_ON                1               //vert LPF only;

#define CCOMB_OFF               0               //chroma comb disabled;
#define CCOMB_ON                1               //chroma comb enabled;

#define SLEEP_OFF               0               //both ADCs active;
#define Y_SLEEP                 2               //Y ADC in sleep mode;
#define C_SLEEP                 1               //C ADC in sleep mode;

#define CRUSH_OFF               0               //non-adaptive AGC;
#define CRUSH_ON                1               //adaptive AGC;

#define VFILT_0                 0               //vert scaling filter 0;
#define VFILT_1                 1               //vert scaling filter 1;
#define VFILT_2                 2               //vert scaling filter 2;
#define VFILT_3                 3               //vert scaling filter 3;

#define COLORBARS_OFF           0               //color bars off;
#define COLORBARS_ON            1               //color bars on;

#define GAMMA_REMOVE_ON         0               //enable gamma correction removal;
#define GAMMA_REMOVE_OFF        1               //disable gamma correction removal;

#define PKTP4                   0               //FIFO trigger point, 4 DWORDS;
#define PKTP8                   1               //8 DWORDS;
#define PKTP16                  2               //16 DWORDS;
#define PKTP32                  3               //32 DWORDS;

#define BIMODAL_ON              1               //bi-modal buffer;
#define BIMODAL_OFF             0               //single mode buffer;

#define BUF_MEM                 0               //buffer in RAM;
#define BUF_EXT                 1               //external buffer;
#define BUF_VIDEO               2               //buffer in video RAM;

#define GPINT_LEVEL				0x0000			//GPINT level sensitive;
#define GPINT_EDGE				0x8000			//GPINT (rising) edge sensitive;

#define GPINT_NONINV			0x0000			//GPINT input not inverted
#define GPINT_INV				0x4000			//GPINT input inverted

/*-------------------------------------------------------
 * Limited support
 *-------------------------------------------------------*/

#define PCIMODE_NORMAL          0               //normal PCI mode;
#define PCIMODE_TRIDENT         0x1L            //Trident chipset comatible PCI mode;
/*-------------------------------------------------------*/


/*-------------------------------------------------------
 * Special types
 *-------------------------------------------------------*/
typedef DWORD HFG;                      //frame grabber handle;
typedef DWORD HBUF;                     //image buffer handle;
typedef DWORD ECODE;                    //error code;

#define SYS_GRABBERS            8       //max frame grabbers in the system;
#define SYS_FRAMES              64		//max frames in buffer;

typedef struct {                        //system structure;
    DWORD boards;                       //number of compatible PCI boards found;
    DWORD PCIslot[SYS_GRABBERS];        //PCI slot numbers for boards found;
} PCI;
    
typedef struct {                        //frame structure;
    LPBITMAPINFO lpbmi;                 //pointer to BITMAPINFO structure;
    void *lpvbits;						//pointer to image data;
} FRAME;
   
typedef struct {                        //image buffer structure;
    HBUF hbuf;                          //handle to buffer;
    DWORD dwFrames;                     //number of frames in buffer;
    FRAME frame[SYS_FRAMES];            //array of FRAME structures;
} BUFFER;

typedef struct {                        //frame grabber advanced mode structure;
    DWORD interlace;                    //interlacing flag;
    DWORD xTotal;                       //total hor pixels;
    DWORD xActive;                      //active hor pixels;
    DWORD xDelay;                       //hor delay;
    float yFactor;                      //vert scaling factor;
    DWORD yActive;                      //active vert pixels (lines);
    DWORD yDelay;                       //vert delay, lines;
    DWORD FORMAT;                       //input video format;
    DWORD BRIGHT;                       //brightness;
    DWORD CONTRAST;                     //contrast;
    DWORD SAT_U;                        //chroma (U) gain;
    DWORD SAT_V;                        //chroma (V) gain;
    DWORD HUE;                          //hue;
    DWORD LNOTCH;                       //luma notch filter control;
    DWORD LDEC;                         //luma decimation control;
    DWORD DEC_RAT;                      //temporal decimation control;
    DWORD PEAK;                         //luma LPF peaking control (Bt848A/849A);
    DWORD CAGC;                         //chroma ADC control;
    DWORD CKILL;                        //low color removal control;
    DWORD HFILT;                        //luma low pass filter mode;
    DWORD RANGE;                        //luma output range;
    DWORD CORE;                         //luma coring control;
    DWORD YCOMB;                        //luma comb enable;
    DWORD CCOMB;                        //chroma comb enable;
    DWORD ADELAY;                       //AGC gate delay for back-porch sampling;
    DWORD BDELAY;                       //burst gate delay for sub-carrier sampling;
    DWORD SLEEP;                        //ADCs' sleep mode;
    DWORD CRUSH;                        //adaptive AGC;
    DWORD VFILT;                        //vertical scaling filter control;
    DWORD COLOR_BARS;                   //color bars test enable;
    DWORD GAMMA;                        //gamma correction removal;
    DWORD PKTP;                         //FIFO trigger point;
    DWORD bimodal;                      //bimodal buffer flag;
    DWORD colorkey;                     //color key for DirectDraw overlay;
    DWORD buffertype;                   //type of buffer: memory, external, or video;
    DWORD gpintmode;					//GP interrupt mode;
    DWORD reserved1;                    //reserved;
    DWORD reserved2;                    //reserved;
    DWORD reserved3;                    //reserved;
    DWORD reserved4;                    //reserved;
} MODE_ADVANCED;

typedef struct {                        //frame grabber mode structure;
    DWORD scale;                        //image scale;
    DWORD color;                        //output color encoding;
    DWORD store;                        //store as DIB or flat;
    DWORD input;                        //input channel;
    MODE_ADVANCED advanced;
} MODE;

typedef DWORD (*FPTR)(void);				//pointer to interrupt service function;

typedef struct {
    HFG hfg;                            //frame grabber handle;
    DWORD mask;                         //interrupt mask;
    DWORD status;                       //interrupt status;
    FPTR func;                          //pointer to user function;
    int priority;                       //interrupt thread priority;
    DWORD total;                        //number of interrupts occured;
    DWORD lost;                         //number of interrupts lost;
} INT_DATA;

typedef struct {						//status structure
	DWORD statusA;						//status A
	DWORD statusB;						//status B
	DWORD reserved;						//reserved
} STATUS;

typedef struct {						//output image size structure
	DWORD xsize;						//horizontal size, pixels
	DWORD ysize;						//vertical size, pixels
} IMAGESIZE;

/*-------------------------------------------------------
 * Error codes
 *-------------------------------------------------------*/
#define ERR_SX_OPEN             1               //driver error
#define ERR_DEVICE_NOT_FOUND    2               //PCI board not found
#define ERR_TOOMANYFGS          3               //too many frame grabbers
#define ERR_IMAGE_FORMAT        4               //invalid image format
#define ERR_INVALID_HANDLE      5               //invalid handle
#define ERR_UNABLE_TO_LOCK      6               //unable to lock any boards
#define ERR_NOHANDLES           7               //no handles available
#define ERR_ALLOC_BUFFER        8               //error allocating buffer memory
#define ERR_LOCK_DBUFFER        9               //error locking buffer
#define ERR_LOCK_RBUFFER        10              //error locking buffer
#define ERR_DIB_MEM_ALLOC       11              //DIB memory allocation error
#define ERR_ACQ_TIMEOUT         12              //acquisition timeout
#define ERR_ILLEGAL_MODE_1      13              //illegal entry in MODE.advanced;
#define ERR_ILLEGAL_MODE_2      14              //illegal entry in MODE;
#define ERR_FILE_OPEN           15              //error opening file;
#define ERR_FILE_WRITE          16              //error writing to file;
#define ERR_INTERRUPT           17              //error installing interrupt;
#define ERR_FRAMENO             18              //invalid number of frames specified;
#define ERR_ILLEGAL_MODE_3      19              //invalid acquisition mode;
#define ERR_ILLEGAL_MODE_4      20              //invalid mode in X11_GetRC;
#define ERR_INVALID_PAR         21              //invalid parameter value;
#define ERR_LOADING_DLL         22              //error loading DLL;
#define ERR_FUNCADDR            23              //error getting function address;
#define ERR_DDCREATE            24              //error creating DirectDraw object;
#define ERR_DDCOLORFORMAT       25              //incompatible color formats;
#define ERR_DDWINDOWFORMAT      26              //incompatible window formats;
#define ERR_DD1                 27              //DirectDraw error;
#define ERR_DD2                 28              //DirectDraw error;
#define ERR_DD3                 29              //DirectDraw error;
#define ERR_DD4                 30              //DirectDraw error;
#define ERR_DD5                 31              //DirectDraw error;
#define ERR_DD6                 32              //DirectDraw error;
#define ERR_DD7                 33              //DirectDraw error;
#define ERR_DD8                 34              //DirectDraw error;
#define ERR_DD                  35              //DirectDraw error;
#define ERR_BUFFORMAT           36              //invalid buffer format;
#define ERR_THREAD              37              //error starting thread;
#define ERR_NOTSUPPORTED        38              //feature not supported;
#define ERR_NOHANDLE			39				//failed to get a frame grabber handle;
#define ERR_INVALID_CHANNEL		40				//invalid video switch channel;
#define ERR_I2CTIMEOUT			41				//internal I2C timeout;
#define ERR_I2C					42				//internal I2C error;
#define ERR_DLL			        43              //DLL not loaded;
#define ERR_PCI					44				//PCI access error
#define ERR_DATA_ACCESS_TO		45				//internal data access timeout



