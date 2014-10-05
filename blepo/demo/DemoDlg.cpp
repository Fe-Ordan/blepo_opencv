/* 
 * Copyright (c) 2004-2009 Stan Birchfield.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma warning(disable: 4786)  // identifier was truncated to '255' characters

#include "stdafx.h"
#include "Demo.h"
#include "DemoDlg.h"
#include "../src/blepo.h"
#include <direct.h>  // _getcwd
#include "../src/Quick/Quick.h"  // debugging

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace blepo;

// uncomment exactly one of these
#define G_DIR "..\\..\\"   // look for images using Blepo directory structure (default, recommended)
//#define G_DIR ""      // to compile Demo for distribution

// ================> begin local functions (available only to this translation unit)
namespace
{

CString iGetExecutableDirectory()
{
  const char* help_path = AfxGetApp()->m_pszHelpFilePath;
  char* s = const_cast<char*>( strrchr(help_path, '\\') );
  *(s+1) = '\0';
  return CString(help_path);
}

CString iGetImagesDir()
{
  return iGetExecutableDirectory() + G_DIR + "images\\";
//  return "C:/Users/stb/cvs-local/research/code/vc6/alpha1/images/";
}

CString iLetUserSelectImageFile(const char* default_filename, 
                                const char* title = "Load image",
                                const char* default_directory = NULL)
{
  CString dir = default_directory ? default_directory : iGetImagesDir();
  CString filter = "All image and video files|*.pgm;*.ppm;*.bmp;*.jpg;*.jpeg;*.avi|"
                   "All image files|*.pgm;*.ppm;*.bmp;*.jpg;*.jpeg|"
                   "PGM/PPM files (*.pgm,*.ppm)|*.pgm;*.ppm|"
                   "BMP files (*.bmp)|*.bmp|"
                   "JPEG files (*.jpg,*.jpeg)|*.jpg;*.jpeg|"
                   "AVI files (*.avi)|*.avi|"
                   "All files (*.*)|*.*||";
  CFileDialog dlg(TRUE,              // open file dialog
                  NULL,              // default extension
                  default_filename,  // default filename
                  OFN_HIDEREADONLY,  // flags
                  filter,            // filter
                  NULL);
	dlg.m_ofn.lpstrInitialDir = (const char*) dir;
	dlg.m_ofn.lpstrTitle = (const char*) title;
  if (dlg.DoModal() == IDOK)
  {
    return dlg.GetPathName();
  }
  else
  {
    return "";
  }
}

};
// ================< end local functions


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

CDemoDlg::CDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoDlg)
	DDX_Control(pDX, IDC_CHECK_FLIP_IMAGE, m_check_flip_image);
	DDX_Control(pDX, IDC_EDIT_SAVE_FNAME_FMT, m_edit_save_fname_fmt);
	DDX_Control(pDX, IDC_CHECK_SAVE_TO_FILE, m_check_save_to_file);
	DDX_Control(pDX, IDC_CHECK_RUN_FACE_DETECTOR, m_check_run_face_detector);
	DDX_Control(pDX, IDC_BUTTON_DSHOW_CAPTURE, m_button_dshow_capture);
	DDX_Control(pDX, IDC_LIST_DSHOW_VIDEO_DEVICES, m_dshow_video_devices);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_static_status);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_AND_DISPLAY, OnButtonLoadAndDisplay)
	ON_BN_CLICKED(IDC_BUTTON_FACE_DETECT, OnButtonFaceDetect)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_DETECT, OnButtonEdgeDetect)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTED_COMPONENTS, OnButtonConnectedComponents)
	ON_BN_CLICKED(IDC_BUTTON_KLT_FEATURE_TRACKER, OnButtonKltFeatureTracker)
	ON_BN_CLICKED(IDC_BUTTON_LOG, OnButtonLog)
	ON_BN_CLICKED(IDC_BUTTON_LINEAR_ALGEBRA, OnButtonLinearAlgebra)
	ON_BN_CLICKED(IDC_BUTTON_SPLITANDMERGE, OnButtonSplitandmerge)
	ON_BN_CLICKED(IDC_BUTTON_SNAKES, OnButtonSnakes)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_MORPHOLOGY, OnButtonMorphology)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_MOVIE, OnButtonShowMovie)
	ON_BN_CLICKED(IDC_BUTTON_IEEE1394, OnButtonIeee1394)
	ON_BN_CLICKED(IDC_BUTTON_FLOOD_FILL, OnButtonFloodFill)
	ON_BN_CLICKED(IDC_BUTTON_HISTOGRAM, OnButtonHistogram)
	ON_BN_CLICKED(IDC_BUTTON_CROSSCORR_STEREO, OnButtonCrosscorrStereo)
	ON_BN_CLICKED(IDC_BUTTON_CROSSCORR_TEMPLATE, OnButtonCrosscorrTemplate)
	ON_BN_CLICKED(IDC_BUTTON_EFROSLEUNG, OnButtonEfrosleung)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, OnButtonTest2)
	ON_BN_CLICKED(IDC_BUTTON_MEAN_SHIFT_SEGMENTATION, OnButtonMeanShiftSegmentation)
	ON_BN_CLICKED(IDC_BUTTON_CANNY, OnButtonCanny)
	ON_BN_CLICKED(IDC_BUTTON_REGIONPROPS, OnButtonRegionprops)
	ON_BN_CLICKED(IDC_BUTTON_LKAFFINE, OnButtonLucasKanadeAffine)
	ON_BN_CLICKED(IDC_BUTTON_REALTIME_STEREO, OnButtonRealtimeStereo)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT_RECT, OnButtonExtractRect)
	ON_BN_CLICKED(IDC_BUTTON_COPY_RECT, OnButtonCopyRect)
	ON_BN_CLICKED(IDC_BUTTON_DSHOW_UPDATE_DEVICES, OnButtonDshowUpdateDevices)
	ON_BN_CLICKED(IDC_BUTTON_DSHOW_CAPTURE, OnButtonDshowCapture)
	ON_LBN_SELCHANGE(IDC_LIST_DSHOW_VIDEO_DEVICES, OnSelchangeListDshowVideoDevices)
	ON_BN_CLICKED(IDC_BUTTON_FFT, OnButtonFft)
	ON_BN_CLICKED(IDC_BUTTON_ELLIPTICAL_HEAD_TRACKER, OnButtonEllipticalHeadTracker)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, OnButtonCalibrate)
	ON_BN_CLICKED(IDC_BUTTON_FAST_FEATURE_TRACKER, OnButtonFastFeatureTracker)
	ON_BN_CLICKED(IDC_BUTTON_WATERSHED, OnButtonWatershed)
	ON_BN_CLICKED(IDC_BUTTON_WARP, OnButtonWarp)
	ON_BN_CLICKED(IDC_BUTTON_CHAMFER, OnButtonChamfer)
	ON_BN_CLICKED(IDC_BUTTON_DELAUNAY, OnButtonDelaunay)
	ON_BN_CLICKED(IDC_CHECK_SAVE_TO_FILE, OnCheckSaveToFile)
	ON_BN_CLICKED(IDC_BUTTON_HORN_SCHUNCK, OnButtonHornSchunck)
	ON_BN_CLICKED(IDC_BUTTON_LINEFIT, OnButtonLinefit)
	ON_BN_CLICKED(IDC_BUTTON_GRAPH_BASED_SEGMENTATION, OnButtonGraphBasedSegmentation)
	ON_BN_CLICKED(IDC_BUTTON_HOUGH, OnButtonHough)
	ON_BN_CLICKED(IDC_BUTTON_CAMSHIFT, OnButtonCamshift)
	ON_BN_CLICKED(IDC_BUTTON_BLOCK_MATCH_OF, OnButtonBlockMatchOpticalflow)
	ON_BN_CLICKED(IDC_BUTTON_FUNDAMENTAL_MATRIX, OnButtonFundamentalMatrix)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_AVI, OnButtonLoadAvi)
	ON_BN_CLICKED(IDC_BUTTON_CHAN_VESE, OnButtonChanVese)
	ON_BN_CLICKED(IDC_BUTTON_JPEG_MEMORY_TEST, OnButtonJpegMemoryTest)
	ON_BN_CLICKED(IDC_BUTTON_OPENGL, OnButtonOpengl)
	ON_BN_CLICKED(IDC_BUTTON_PLOT, OnButtonPlot)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_THRESHOLD, OnButtonThreshold)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg message handlers

BOOL CDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
  EnableDisable();
//  char buff[100];
//  _getcwd(buff, 100);
  m_edit_save_fname_fmt.SetWindowText("img%04d.bmp");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDemoDlg::EnableDisable()
{
  bool dshow_device_selected = ( m_dshow_video_devices.GetCurSel() >= 0 );
  m_button_dshow_capture.EnableWindow(dshow_device_selected);
  m_check_run_face_detector.EnableWindow(dshow_device_selected);
  m_check_save_to_file.EnableWindow(dshow_device_selected);

  bool save_to_file = m_check_save_to_file.GetCheck() != 0;
  m_edit_save_fname_fmt.EnableWindow(save_to_file);
}

void CDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDemoDlg::OnButtonLoadAndDisplay() 
{
  try 
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgBgr img;
      Load(fname, &img);
      Figure fig("Image loaded");
      fig.Draw(img);
    }

  } 
  catch (const Exception& e)
  {
    // image failed to load, so notify user
    e.Display();
  }
}

void CDemoDlg::OnButtonExtractRect() 
{
  try 
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgBgr img, img_extracted;
      Load(fname, &img);
      Figure fig("Image:  Click and drag mouse");
      fig.Draw(img);
      Rect rect = fig.GrabRect();
      fig.SetTitle("Image");
      Extract(img, rect, &img_extracted);
      Figure fig2("Extracted rect");
      fig2.Draw(img_extracted);
    }

  } 
  catch (const Exception& e)
  {
    // image failed to load, so notify user
    e.Display();
  }	
}


void CDemoDlg::OnButtonCopyRect() 
{
  try 
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      // load image and create flipped copy
      ImgBgr img1, img2;
      Load(fname, &img1);
      FlipVertical(img1, &img2);

      // display images side-by-side
      Figure fig1("Image1:  Click and drag mouse"), fig2("Image2");
      fig1.Draw(img1);
      fig2.Draw(img2);
      fig2.PlaceToTheRightOf(fig1);

      // allow user to select rect and copy pixels
      // from 'img1' to 'img2'
      Rect rect = fig1.GrabRect();
      fig1.SetTitle("Image1");
      Set(&img2, rect.TopLeft(), img1, rect);
      fig2.Draw(img2);
    }
  } 
  catch (const Exception& e)
  {
    // image failed to load, so notify user
    e.Display();
  }	
}

void CDemoDlg::OnButtonFaceDetect() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("freethrow.bmp");
    if (fname != "")
    {
      FaceDetector detector;
      ImgBgr img;
      Load(fname, &img);
      std::vector<Rect> front, left, right;
      detector.DetectAllFaces(img, &front, &left, &right);
      int i;
      for (i=0 ; i<(int)left.size() ; i++) {
        DrawRect(left[i], &img, Bgr(255,0,0));
      }
      for (i=0 ; i<(int)right.size() ; i++) {
        DrawRect(right[i], &img, Bgr(0,0,255));
      }
      for (i=0 ; i<(int)front.size() ; i++) {
        DrawRect(front[i], &img, Bgr(0,255,0));
      }
      Figure fig("facedetect");
      fig.Draw(img);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}


void CDemoDlg::OnButtonEdgeDetect() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      // load image
      ImgBgr img;
      Load(fname, &img);

      // convert to grayscale
      ImgGray gray, grad_prewitt_x, grad_prewitt_y, gradmag;
      ImgFloat gradx, grady;
      Convert(img, &gray);
      GradPrewitt(gray, &gradx, &grady);
      GradMagPrewitt(gray, &gradmag);

  #if 0
      // convolve with Prewitt edge detector (horizontal and vertical)
      // see external/doc/intel/ipllib/iplman.pdf Chapter 6
      grad_prewitt_x.Reset(gray.Width(), gray.Height());
      grad_prewitt_y.Reset(gray.Width(), gray.Height());
      ImgIplImage ipl1(gray), ipl2(grad_prewitt_x), ipl3(grad_prewitt_y);
    //  iplFixedFilter(gray, grad_prewitt_x, IPL_PREWITT_3x3_H);
    //  iplFixedFilter(gray, grad_prewitt_y, IPL_PREWITT_3x3_V);
      iplFixedFilter(ipl1, ipl2, IPL_PREWITT_3x3_H);
      iplFixedFilter(ipl1, ipl3, IPL_PREWITT_3x3_V);
      ipl1.CastToGray(&gray);
      ipl2.CastToGray(&grad_prewitt_x);
      ipl3.CastToGray(&grad_prewitt_y);

      // convolve with Laplacian edge detector
      grad_log.Reset(gray.Width(), gray.Height());
      ImgIplImage ipl4(grad_log);
    //  iplFixedFilter(gray, grad_log, IPL_LAPLACIAN_3x3);
      iplFixedFilter(ipl1, ipl4, IPL_LAPLACIAN_3x3);
      ipl4.CastToGray(&grad_log);
  #endif

      // display results
      Figure fig1("img"), fig2("Prewitt horizontal"), fig3("Prewitt vertical"), fig4("Prewitt magnitude"); //, fig4("Laplacian of Gaussian");
      fig1.Draw(img);
      fig2.Draw(gradx); //grad_prewitt_x);
      fig3.Draw(grady); //grad_prewitt_y);
      fig4.Draw(gradmag);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonConnectedComponents() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      // load image
      ImgBgr img;
	    Load(fname, &img);

      // convert to grayscale and quantize
      ImgGray gray;
      Convert(img, &gray);
      ImgGray::Iterator p;
      for (p = gray.Begin() ; p != gray.End() ; p++)
      {
        *p = (*p & 0xC0);  // quantize pixel
      }

      // compute connected components
      ImgInt labels;
      std::vector<ConnectedComponentProperties<ImgGray::Pixel> > reg;
      ConnectedComponents4(gray, &labels, &reg);

      // convert to pseudo-random colors
      ImgBgr display;
      PseudoColor(labels, &display);

      Figure fig1("quantized image");
      Figure fig2("labels");
      Figure fig3("colored labels");
      fig1.Draw(gray);
      fig2.Draw(labels);
      fig3.Draw(display);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonKltFeatureTracker() 
{
//#define USE_OPENCV_FAST_FEATURE_TRACKER_INSTEAD
  try
  {
    ImgBgr img1, img2;  // for display
    ImgGray gray_img1, gray_img2;
    Figure fig1("features 1");
    Figure fig2("features 2");
    int nfeatures = 100;
    CString fmt = iGetImagesDir() + "img%0d.pgm";
    const int start_frame = 1;
    const int end_frame = 3;
//    CString fmt = "G:/data/images/ross_david/track_david/track_david%05d.pgm";
//    const int start_frame = 1;
//    const int end_frame = 100;
//    CString fmt = "c:/stb/tmp/desk/img%04d.jpg";
//    const int start_frame = 1;
//    const int end_frame = 785;

    //Create feature list and tracking context
#ifdef USE_OPENCV_FAST_FEATURE_TRACKER_INSTEAD
    FastFeatureTracker track;
    FastFeatureTracker::FeatureArr points;
    FastFeatureTracker::Params params = track.GetParams();
//    params.window_hw = 10;
//    params.window_hh = 10;
//    track.SetParams(params);  // results go haywire when window size is changed; don't know why
#else
    KltFeatureList fl(nfeatures);
    KltTrackingContext tc;
//    ((KLT_TrackingContext) tc)->window_width = 20;
//    ((KLT_TrackingContext) tc)->window_height = 20;
#endif

    for (int i = start_frame ; i < end_frame ; i++)
    {
      CString fname;
      fname.Format(fmt, i);
      Load(fname, &gray_img2);
//      Extract(gray_img2, Rect(0,0,320,240),&gray_img2);
      Convert(gray_img2, &img2);
  
      if (i == start_frame)
      {
#ifdef USE_OPENCV_FAST_FEATURE_TRACKER_INSTEAD
        track.SelectFeatures(gray_img2, nfeatures, &points);
        track.DrawFeatures(&img2, points, Bgr(0, 0, 255));
#else
        tc.SelectGoodFeatures(&fl, gray_img2);
//        fl.Reset(1);
//        fl[0]->x = 158;
//        fl[0]->y = 88;
//        fl[0]->val = 0;
        tc.OverlayFeatures(&img2, fl);
#endif
        fig1.Draw(img2);
        fig2.Draw(img2);
        fig2.PlaceToTheRightOf(fig1);
      }
      else
      {
#ifdef USE_OPENCV_FAST_FEATURE_TRACKER_INSTEAD
        track.TrackFeatures(gray_img2, &points, false);
        track.DrawFeatures(&img2, points, Bgr(0, 0, 255));
#else
        tc.TrackFeatures(&fl, gray_img1, gray_img2);
        tc.ReplaceLostFeatures(&fl, gray_img2);
        tc.OverlayFeatures(&img2, fl);
#endif
        fig2.Draw(img2);
      }
      gray_img1 = gray_img2;
      if (i < end_frame-1)  fig2.GrabMouseClick();
    }
  } catch (Exception& e)
  {
    e.Display();
  }
}


//  CWnd goo;
void CDemoDlg::OnButtonLog() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgGray img;
      Load(fname, &img);
#if 1
      ImgInt log;
      ImgBinary slog, zc;
      SmoothGauss3x3(img, &img);
//      SmoothGauss5x5(img, &img);
      LaplacianOfGaussian(img, &log);
      Slog(img, &slog);
      ZeroCrossings(log, &zc);

      Figure fig1("image"), fig2("LoG"), fig3("Slog"), fig4("zero crossings");
      fig1.Draw(img);
      fig2.Draw(log);
      fig3.Draw(slog);
      fig4.Draw(zc);

#else
      // arbitrary kernel  
      ImgInt int_image,int_out,int_kernel(3,3);
      ImgFloat float_image, float_out, float_kernel(3,3);
  
      Convert(img,&int_image);
      Convert(img,&float_image);
  
      int_kernel(0,0) = 1;
      int_kernel(1,0) = 1;
      int_kernel(2,0) = 1;
      int_kernel(0,1) = 1;
      int_kernel(1,1) = -8;
      int_kernel(2,1) = 1;
      int_kernel(0,2) = 1;
      int_kernel(1,2) = 1;
      int_kernel(2,2) = 1;

      float_kernel(0,0) = 1;
      float_kernel(1,0) = 1;
      float_kernel(2,0) = 1;
      float_kernel(0,1) = 1;
      float_kernel(1,1) = -8;
      float_kernel(2,1) = 1;
      float_kernel(0,2) = 1;
      float_kernel(1,2) = 1;
      float_kernel(2,2) = 1;

      Convolve(int_image,int_kernel,&int_out);
      Convolve(float_image,float_kernel,&float_out);

      Figure fig1("image loaded");
      Figure fig2("LoG: Image as Integer");
      Figure fig3("LoG: Image as Float");
      fig1.Draw(img);
      fig2.Draw(int_out);
      fig3.Draw(float_out);
#endif
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonLinearAlgebra() 
{
  try
  {
    MatDbl foo(2,3), u, s, v;
    foo(0,0) = 0.5;
    foo(1,0) = 0.8;
    foo(0,1) = 1.4;
    foo(1,1) = 17.2;
    foo(0,2) = -0.5;
    foo(1,2) = 18.1;
    Svd(foo, &u, &s, &v);	
    Transpose(&u);
    Display(u);
    Display(v);
    AfxMessageBox("SVD computed");

    MatDbl a(3,3), out;
    a(0,0) = 8;
    a(1,0) = 2;
    a(2,0) = 3;
    a(0,1) = 2;
    a(1,1) = 7;
    a(2,1) = 6;
    a(0,2) = 3;
    a(1,2) = 6;
    a(2,2) = 9;
    //CholeskyDecomp(a, &out);
    Display(a);
    Display(out);
    Display((out) * Transpose(out));
    AfxMessageBox("Cholesky computed");


    /*
     Matlab test:
      a = [0.5 0.8 ; 1.4 17.2 ; -0.5 18.1];
      [u,s,v] = svd(a)
      */
  } catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonSplitandmerge() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      Figure fig1("image"), fig2("labels"), fig3("colored labels");
      ImgGray img, seg, seg2;
      ImgInt random_labels, labels;
      Load(fname, &img);
      SplitAndMergeSegmentation(img, &labels, 20);
      ImgBgr bgr;

      // convert to pseudo-random colors
      ImgBgr display;
      PseudoColor(labels, &display);

      // display
      fig1.Draw(img);
      fig2.Draw(labels);
      fig3.Draw(display);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	

//  Save(display, "split_and_merge_output.ppm");

//  ImgBinary mask;
//  while (1)
//  {
//    Point p = fig3.GrabMouseClick();
//    int a = labels(p.x, p.y);
//    Equal(labels, a, &mask);
////    fig3.Draw(mask);
//    float std = StandardDeviation(img, mask);
//    CString msg;
//    msg.Format("%5.1f", std);
//    AfxMessageBox(msg);
//  }
}

void CDemoDlg::OnButtonSnakes() 
{
  try
  {
    if (1)
    {
      ImgGray img;
      float alpha = 1.0f;
      float beta = 1.0f;
      bool snake_moved = true;
      Snake snake;
      Figure fig;

      // initialize image
      img.Reset(256, 256);
      Set(&img, 0);
      Set(&img, Rect(64, 64, 192, 192), 255);
      // initialize snake
      snake.push_back(Point(206, 54));
      snake.push_back(Point(217, 74));
      snake.push_back(Point(206, 95));
      snake.push_back(Point(220, 112));
      snake.push_back(Point(206, 133));
      snake.push_back(Point(222, 150));
      snake.push_back(Point(208, 173));
      snake.push_back(Point(221, 194));
      snake.push_back(Point(199, 204));
      snake.push_back(Point(180, 222));
      snake.push_back(Point(166, 206));
      snake.push_back(Point(137, 218));
      snake.push_back(Point(118, 204));
      snake.push_back(Point(96, 224));
      snake.push_back(Point(84, 206));
      snake.push_back(Point(60, 216));
      snake.push_back(Point(50, 202));
      snake.push_back(Point(48, 184));
      snake.push_back(Point(34, 168));
      snake.push_back(Point(47, 146));
      snake.push_back(Point(32, 126));
      snake.push_back(Point(46, 110));
      snake.push_back(Point(32, 86));
      snake.push_back(Point(46, 77));
      snake.push_back(Point(42, 56));
      snake.push_back(Point(57, 50));
      snake.push_back(Point(73, 40));
      snake.push_back(Point(94, 46));
      snake.push_back(Point(108, 36));
      snake.push_back(Point(126, 42));

      const int maxiter = 5000;
      int iter = 0;
      while (snake_moved && iter < maxiter)
      {
        {
          ImgBgr foo;
          Convert(img, &foo);
          for (int i=0 ; i<(int)snake.size() ; i++)
          {
            const Point& pt = snake[i];
            Bgr color = (i==0) ? Bgr::GREEN : (i==1) ? Bgr::YELLOW : Bgr::RED;
            DrawCircle(pt, 2, &foo, color, -1);
  //          foo(pt.x, pt.y) = i==0 ? Bgr(0, 255, 0) : (i==1) ? Bgr(0, 255, 255) : Bgr(0, 0, 255);
          }
          fig.Draw(foo);
        }
    //    snake_moved = SnakeIteration(img, alpha, &snake);
        snake_moved = SnakeIteration(img, alpha, beta, &snake);
        iter++;
        //Sleep(100);
      }

      CString str;
      str.Format("Finished after %d iterations", iter);
      AfxMessageBox(str);
    }
  
    else
    {
      CString fname = iLetUserSelectImageFile("fruitfly.pgm");
      if (fname != "")
      {
        ImgGray img;
//      CString fname = "c:/stb/doc/www/ece877/fruitfly.pgm";
//      CString fname = "c:/stb/doc/www/ece877/mri_head.bmp";
        Load(fname, &img);
        ImgGray gradmag, tmp;
        float alpha = 1.0f;
        float beta = 1.0f;
        bool snake_moved = true;
        Array<Figure::MouseClick> pts;
        Snake snake;
        Figure fig, fig2;
        SmoothGauss5x5(img, &tmp);
        SmoothGauss5x5(tmp, &img);
        GradMagPrewitt(img, &gradmag);
        fig2.Draw(gradmag);
        fig.Draw(img);
  //      Figure::WhichButton bfoo = Figure::MC_NONE;
  //      while (bfoo != Figure::MC_RIGHT)
  //        bfoo = fig.TestMouseClick();
  //      fig.GrabMouseClicks(15, &pts);
        fig.GrabLeftMouseClicks(&pts, RGB(255,0,0), RGB(0, 0, 255));
        fig.SetTitle("Left click:  set points,  Right click:  run algorithm");
        for (int i=0 ; i<pts.Len() ; i++)  snake.push_back(pts[i].pt);
        const int maxiter = 5000;
        int iter = 0;
        while (snake_moved && iter < maxiter)
        {
          {
            ImgBgr foo;
            Convert(img, &foo);
            for (int i=0 ; i<(int)snake.size() ; i++)
            {
              const Point& pt = snake[i];
            Bgr color = i==0 ? Bgr(0, 255, 0) : (i==1) ? Bgr(0, 255, 255) : Bgr(0, 0, 255);
            DrawCircle(pt, 2, &foo, color, -1);
    //          foo(pt.x, pt.y) = i==0 ? Bgr(0, 255, 0) : (i==1) ? Bgr(0, 255, 255) : Bgr(0, 0, 255);
            }
            fig.Draw(foo);
          }
      //    snake_moved = SnakeIteration(img, alpha, &snake);
          snake_moved = SnakeIteration(img, alpha, beta, &snake);
          iter++;
          //Sleep(100);
        }

        CString str;
        str.Format("Finished after %d iterations", iter);
        AfxMessageBox(str);
      }
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CDemoDlg::OnButtonMorphology() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgBgr img;
      ImgGray imgg, out;
    //  ImgBinary bin;
      Load(fname, &img);
      Convert(img, &imgg);
      for (unsigned char* p=imgg.Begin() ; p!=imgg.End() ; p++)  *p = (*p<128) ? 0 : 255;
    //  Threshold(imgg, 128, &bin);
    //  Convert(bin, &imgg);
      Figure fig("image"), fig2("erode"), fig3("dilate");
      fig.Draw(imgg);
      Erode3x3(imgg, &out);
      fig2.Draw(out);
      Dilate3x3(imgg, &out);
      fig3.Draw(out);

      {
        ImgBinary a, b;
        Convert(imgg, &a);
        b = a;
        while (1)
        {
          Thin3x3(&a);
          if (IsIdentical(a, b))  break;
          b = a;
        }
        Figure fig4("thin3x3");
        fig4.Draw(a);
      }
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonShowMovie() 
{
  ImgGray img;
  CString str;
  Figure fig;
  for (int i=550 ; i<650 ; i++) 
  {
    str.Format(iGetImagesDir() + "movie/img%04d.jpg", i);
    Load(str, &img);
    fig.Draw(img);
    Sleep(20);
  }	
}

void iQuantizeImage(ImgBgr* out)
{
  // quantize
  ImgBgr::Iterator p;
  for (p = out->Begin() ; p != out->End() ; p++)
  {
    p->b &= 0xC0;  // quantize pixel
    p->g &= 0xC0;  // quantize pixel
    p->r &= 0xC0;  // quantize pixel
  }
}

void CDemoDlg::OnButtonFloodFill() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("img0.pgm");
    if (fname != "")
    {
      if (0)
      { // floodfill intensity edges of grayscale image
        ImgBinary bin;
        ImgGray img, edges;
        Load(fname, &img);
        GradMagPrewitt(img, &edges);
        Threshold(edges, 50, &bin);
        Figure fig;
        fig.Draw(bin);
        CPoint pt = fig.GrabMouseClick();
        FloodFill4(bin, pt.x, pt.y, true, &bin);
        fig.Draw(bin);
      }
      else
      { // floodfill quantized Bgr image
        ImgBgr bgr;
        Load(fname, &bgr);
        iQuantizeImage(&bgr);
        Figure fig("Left click:  Floodfill, Right click:  Done");
        fig.Draw(bgr);
        while (1)
        {
          Figure::WhichButton b;
          CPoint pt = fig.GrabMouseClick(&b);
          if (b == Figure::MC_RIGHT)  break;
          FloodFill4(bgr, pt.x, pt.y, Bgr(0,0,255), &bgr);
          fig.Draw(bgr);
        }
      }
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonHistogram() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      int prev_energy_vaule=100;
      int energy_value=0;
      std::vector<int> out;
      ImgGray img;
      int wh,bl;
		
		  //IMGGRAY
		  Load(fname, &img);
		  Figure AB("Original Image for Conservative Smoothing");
		  AB.Draw(img);
		  
		  ImgGray tmp;
		  ConservativeSmoothing(img,3,5,&tmp);			
		  Figure AC("Conservative Smoothing using ImgGray");
		  AC.Draw(tmp);


		  HistogramGray(img,15,&out);	//Histogram with BIN size of 15
  //        cout<<"Histogram Of the Image\n";
  //		for(int i=0;i<out.size();i++)
  //		{
  //			cout<<out.at(i)<<"\n";
  //		}
		  

		  HistogramBinary(img,&wh,&bl);
  //		cout<<"Number of White Pixels : "<<wh<<"\nNumber of Black Pixels : "<<bl;
		  
		  //IMGINT
		  
		  ImgInt img1;
		  ImgGray temp1;
		  Load(fname,&temp1);
		  Convert(temp1,&img1);
		  
		  ImgInt tmp1;
		  ConservativeSmoothing(img1,3,5,&tmp1);			
		  Convert(tmp1,&temp1);
		  Figure AD("Conservative Smoothing using ImgInt");
		  AD.Draw(temp1);
		  
		  
		  //IMGFLOAT
		  
		  ImgFloat img2;
		  ImgGray temp2;
		  Load(fname,&temp2);
		  Convert(temp2,&img2);
		  
		  ImgFloat tmp2;
		  ConservativeSmoothing(img2,3,5,&tmp2);			
		  Convert(tmp2,&temp2);
		  Figure AE("Conservative Smoothing using ImgFloat");
		  AE.Draw(temp2);	
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonCrosscorrStereo() 
{
  try
  {
    CWaitCursor wait;
    CString method="basic";

    ImgGray im_left, im_right, output;

    //	  Load("C:/stb/data/images/stereo/middlebury/data_ppm/tsukuba/tsukuba_left.pgm", &im_left);
    //	  Load("C:/stb/data/images/stereo/middlebury/data_ppm/tsukuba/tsukuba_right.pgm", &im_right);

    CString fname1 = iLetUserSelectImageFile("img0.pgm", "Select left image");
    CString fname2 = iLetUserSelectImageFile("img1.pgm", "Select right image");
    if (fname1 != "" && fname2 != "")
    {
      Load(fname1, &im_left);
      Load(fname2, &im_right);

      int window_size=3;
      int max_disp=6;

      StereoCrossCorr(im_left, im_right, window_size, max_disp,  method, &output);


      int max=0;
      int x, y;
      for (y=0; y<output.Height(); y++)
      {
        for (x=0; x<output.Width(); x++)
        {
          if ( output(x,y)> max)
            max=output(x,y);

        }
      }

      for (y=0; y<output.Height(); y++)
      {
        for (int x=0; x<output.Width(); x++)
        {
          output(x,y)=(int)output(x,y)*255/max;
        }
      }

      Figure fig1, fig2, fig;
      fig1.Draw(im_left);
      fig2.Draw(im_right);
      fig.Draw(output);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }
}

void CDemoDlg::OnButtonCrosscorrTemplate() 
{
  try
  {
    CString fname1 = iLetUserSelectImageFile("img0.pgm", "Select left image");
    CString fname2 = iLetUserSelectImageFile("img1.pgm", "Select right image");
    if (fname1 != "" && fname2 != "")
    {
      CString method="basic";
      ImgGray img_gray,temp_gray, img_gray1;
      Load(fname1, &img_gray);
      CRect head(92, 109, 92+27, 109+33);
      Extract(img_gray, head, &temp_gray);
      CRect search_range=CRect(90, 97, 70+63, 97+63);
      CPoint out;

      Load(fname2, &img_gray1);

      TemplateCrossCorr(temp_gray, img_gray1, search_range, method, &out);

      Rect output(out.x, out.y, out.x+temp_gray.Width(), out.y+temp_gray.Height());

      ImgBgr img_bgr;
      Convert(img_gray1, &img_bgr);
      DrawRect(output, &img_bgr, Bgr(0,0,255));
      DrawRect(search_range, &img_bgr, Bgr(0,255,0));

      Figure fig2, fig, fig1;
      ImgBgr img_foo;
      Convert(img_gray, &img_foo);
      DrawRect(head, &img_foo, Bgr(0,0,255));
      fig.Draw(img_bgr);
      fig1.Draw(temp_gray);
      fig2.Draw(img_foo);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}


void CDemoDlg::OnButtonEfrosleung() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("texture_brick_small.pgm");
    if (fname != "")
    {
      CWaitCursor wait;
		  int img_width;
		  int img_height;
		  int window_width;
		  int window_height;
		  ImgGray img_out;
		  ImgBinary img_bin;
		  img_width=50;
		  img_height=50;
		  window_width=13;
		  window_height=13;
		  int i,j;
		  //---------------------------------------

		  //Initializations
		  ImgGray img_sample;
		  //ImgGray templates;
		  //templates.Reset(window_width,window_height);
		  img_out.Reset(img_width,img_height);
		  img_bin.Reset(img_width,img_height);
		  Figure fig1,fig2;
		  Load(fname, &img_sample);
		  int window_width_f = (int) floor(window_width/2.0);
		  int window_width_c = (int) ceil(window_width/2.0);
		  int window_height_f = (int) floor(window_height/2.0);
		  int window_height_c = (int) ceil(window_height/2.0);
		  //---------------------------------------
		  
		  //Selecting a random seed from the sample
		  int rand_x,rand_y,i_w,i_h;
		  i_w=img_sample.Width();
		  i_h=img_sample.Height();
		  /*if((window_width_c>=(i_w-window_width_c-1))||(window_height_c>=(i_h-window_height_c-1)))
		  {
			  cout<<"decrease window size"<<endl;
			  exit(1);
		  }*/
		  rand_x=blepo_ex::GetRand(window_width_c,i_w-window_height_c-1);
		  rand_y=blepo_ex::GetRand(window_width_c,i_h-window_height_c-1);
		  int img_center_x = (int) ceil(img_width/2.0);
		  int img_center_y = (int) ceil(img_height/2.0);
		  unsigned char *ptr_g;
		  for(ptr_g=img_out.Begin();ptr_g!=img_out.End();ptr_g++)
			  *ptr_g=0;
		  //Creating a binary image to check if a pixel is already painted
		  ImgBinary::Iterator binptr;
		  for(binptr=img_bin.Begin();binptr!=img_bin.End();binptr++)
			  *binptr=false;
		  for (i=-window_width_f;i<window_width_f;i++)
		  {
			  for (j=-window_height_f;j<window_height_f;j++)
			  {
				  img_out(img_center_x+i,img_center_y+j)=img_sample(rand_x+i,rand_y+j);
				  img_bin(img_center_x+i,img_center_y+j)=true;
			  }
		  };
		  //-------------------------------------------------------------------
		  // Call function to synthesize texture
		  SynthesizeTextureEfrosLeung(img_sample, img_bin, window_width, window_height, img_width, img_height, &img_out);
		  fig1.Draw(img_sample);
		  fig2.Draw(img_out);	
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

//CanonVcc4* can = NULL;

// loads a float image from an ASCII file, such as that produced by Matlab's 'save -ascii' command
void LoadFloatFromAscii(const char* fname, ImgFloat* out)
{
  FILE* fp = fopen(fname, "rt");
  if (fp)
  {
    Array<float> vals;
    const int n = 100000;
    char buf[n];
    int nrows = 0, ncols = 0;
    while (1)
    {
      // get row
      char* p = fgets(buf, n, fp);
      if (p == NULL)  break;

      // grab elements from row
      int nval = 0;
      while (1)
      {
        float a;
        int ret = sscanf(p, "%f", &a);
        if (ret != 1)  break;
        vals.Push(a);
        nval++;
        while (*p == ' ' || *p == '\t')  p++;
        while (*p != ' ' && *p != '\t' && *p != '\0')  p++;
      }
      if (ncols != 0 && ncols != nval)  BLEPO_ERROR("Each line of file must have the same number of elements");
      ncols = nval;

      nrows++;
    }
    fclose(fp);

    // copy data to image
    out->Reset(ncols, nrows);
    memcpy(out->Begin(), vals.Begin(), ncols*nrows*sizeof(float));
  }
}

//#include "../src/Image/ImgIplImage.h"
//
//class FastFeatureTracker
//{
//public:
//  double m_quality, m_min_distance, m_k;
//  int m_block_size;
//  bool m_use_harris;
//  bool m_refine_corners;
//  int m_win_size;
//  int m_flags;
//
//public:
//  FastFeatureTracker() 
//    : m_quality ( 0.01 ), 
//      m_min_distance ( 10 ), 
//      m_k ( 0.04 ), 
//      m_block_size ( 3 ), 
//      m_use_harris ( false ), 
//      m_refine_corners ( true ),
//      m_win_size ( 10 ),
//      m_flags ( 0 )
//  {}
//
//  void SelectFeatures(const ImgGray& img, int max_npoints, Cvptarr* points)
//  {
//    points->Reset(max_npoints);
//    m_tmp1.Reset( img.Width(), img.Height() );
//    m_tmp2.Reset( img.Width(), img.Height() );
//    ImgIplImage grey(img);
//    ImgIplImage eig(m_tmp1);
//    ImgIplImage temp(m_tmp2);
//    int count = max_npoints;
//    cvGoodFeaturesToTrack( grey, eig, temp, points->Begin(), &count,
//                           m_quality, m_min_distance, 0, m_block_size, m_use_harris, m_k );
//    if (m_refine_corners)
//    {
//      cvFindCornerSubPix( grey, points->Begin(), count,
//                          cvSize( m_win_size, m_win_size ), cvSize(-1,-1),
//                          cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03) );
//    }
//    points->Reset(count);
//    m_prev_img = img;
//  }
//
//  void TrackFeatures(const ImgGray& img, Cvptarr* points)
//  {
//    ImgIplImage prev_grey(m_prev_img);
//    ImgIplImage grey(img);
//    ImgGray pyr1( img.Width(), img.Height() );
//    ImgGray pyr2( img.Width(), img.Height() );
//    ImgIplImage prev_pyramid(pyr1);
//    ImgIplImage pyramid(pyr2);
//    int npoints = points->Len();
//    Cvptarr pts_out( npoints );
//    Array<char> status ( points->Len() );
//    cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
//                            points->Begin(), pts_out.Begin(), npoints, 
//                            cvSize(m_win_size, m_win_size), 3, status.Begin(), 0,
//                            cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,
//                            20, 0.03), m_flags );
////    m_flags |= CV_LKFLOW_PYR_A_READY;
//    points->Reset();
//    for (int i=0 ; i<npoints ; i++)
//    {
//      if (status[i])
//      {
//        points->Push( pts_out[i] );
//      }
//    }
//    m_prev_img = img;
//  }
//
//  void DrawFeatures(ImgBgr* img, const Cvptarr& points, const Bgr& color)
//  {
//    for (int i=0 ; i<points.Len() ; i++)
//    {
//      int x = blepo_ex::Round( points[i].x );
//      int y = blepo_ex::Round( points[i].y );
//      Point pt( x, y );
//      DrawCircle(pt, 3, img, color, 1);
//    }
//  }
//
//private:
//  ImgFloat m_tmp1, m_tmp2;
//  ImgGray m_prev_img;
//};


//struct Vec
//{
//  Vec(int a, int b, int c) { dx=a; dy=b; index=c; }
//  int dx, dy, index;
//};
//
//void ClusterFeatures(const KltFeatureList& fl_prev, const KltFeatureList& fl, std::vector<int>* ids)
//{
//  // try translation model 
//  assert(fl_prev.GetNFeatures() == fl.GetNFeatures());
//  std::vector<Vec> vec;
//  for (int i=0 ; i<fl_prev.GetNFeatures() ; i++)
//  {
//    const KLT_Feature f1 = fl_prev[i];
//    const KLT_Feature f2 = fl[i];
//    if (f1->val>=0 && f2->val>=0)
//    {
//      vec.push_back( Vec(f2->x - f1->x, f2->y - f1->y, i) );
//    }
//  }
//
//  // compute mean
//  double mx=0, my=0;
//  for (i=0 ; i<vec.size() ; i++)
//  {
//    mx += vec[i].dx;
//    my += vec[i].dy;
//  }
//  mx /= vec.size();
//  my /= vec.size();
//
//  ids->resize( fl_prev.GetNFeatures(), -1 );
//  for (i=0 ; i<vec.size() ; i++)
//  {
//    if (fabs(vec[i].dx - mx) <= 0.5)
//      (*ids)[ vec[i].index ] = 0;
//    else
//      (*ids)[ vec[i].index ] = 1;
//  }  
//}

void TrackFeatures(const char* filename_fmt, int first_frame, int last_frame, int nfeatures, KltFeatureTable* ft)
{
  int nframes = last_frame - first_frame;
  CString fname;
  ImgBgr img1, img2;  // for display
  ImgGray gray_img1, gray_img2;

  // Create feature list and tracking context
  KltFeatureList fl(nfeatures);
  ft->Reset(nframes, nfeatures);
//  KltFeatureTable ft(nframes, nfeatures);
  KltTrackingContext tc;
  tc.SetSequentialMode(true);
  tc.SetAffineConsistencyCheck(true);

  // Select features in first frame
  fname.Format(filename_fmt, first_frame);
  Load(fname, &gray_img1);
//  Convert(gray_img1, &img1);
  tc.SelectGoodFeatures(&fl, gray_img1);
  ft->SetFeatureList(fl, 0);

  // Track features
  Figure fig2;
  for (int i = first_frame + 1 ; i < last_frame ; i++)
  {
//    tc.OverlayFeatures(&img1, fl);
    fname.Format(filename_fmt, i);
    Load(fname, &gray_img2);
    tc.TrackFeatures(&fl, gray_img1, gray_img2);
    tc.ReplaceLostFeatures(&fl, gray_img2);
    {
      Convert(gray_img2, &img2);
      tc.OverlayFeatures(&img2, fl);
      fig2.Draw(img2);
    }
//    tc.OverlayFeatures(&img2, fl);
    ft->SetFeatureList(fl, i - first_frame);
//    gray_img1 = gray_img2;
  }

  //display the results
//  Figure fig1("features 1");
//  fig1.Draw(img1);
//  Figure fig2("features 2");
//  fig2.Draw(img2);
}

// obsolete:
// Shuffles bytes in 8-byte blocks.  This is used to convert the 
// ImgBinary object from big endian to little endian and vice versa,
// because ImgBinary is big endian but MMX/SSE2 instructions load 
// the bytes assuming little endian.
// Example:  
//   00-01-02-03-04-05-06-07-08-09-10-11-12-13-14-15-16-17 ...
//       transforms to 
//   07-06-05-04-03-02-01-00-15-14-13-12-11-10-09-08-23-22 ...
//       where the bytes are written from left to right in
//       increasing memory locations
//void ShuffleBytes8(const ImgBinary& img, ImgBinary* out)
//{
//  out->Reset(img.Width(), img.Height());
//  int nbytes = img.NBytes();
//  assert( nbytes % 8 == 0 );  // will have to handle trailing bits otherwise
//  const unsigned char* p = img.BytePtr();
//  unsigned char* q = out->BytePtr();
//  nbytes /= 8;
//  for (int i=0 ; i<nbytes ; i++)
//  {
//    for (int j=7 ; j>=0 ; j--)
//    {
//      *q++ = p[j];
//    }
//    p += 8;
//  }
//}

//void PrintBitReversalArray()
//{
//  FILE* fp = fopen("foo.txt", "wt");
//  unsigned char reverse[16] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };
//  fprintf(fp, "static unsigned char bit_reversal_array[256] = \n{\n");
//  for (int i=0 ; i<16 ; i++)
//  {
//    fprintf(fp, "  ");
//    int low_nibble = reverse[i];
//    for (int j=0 ; j<16 ; j++)
//    {
//      int high_nibble = reverse[j];
//      unsigned char byte = (high_nibble << 4) | low_nibble;
//      fprintf(fp, "0x%02x, ", byte);
//    }
//    fprintf(fp, "\n");
//  }
//  fprintf(fp, "};\n");
//}

//void ComputeTransformation(std::vector<Point2d>& dvec,
//                           double* dx, double* dy, double* zoom)
//{
//  const int n = dvec.size();
//  for (int i=0 ; i<n ; i++)
//  {
//
//  }
//}

//void TransformImage(double zoom, double dx, double dy, 
//                    int width, int height, 
//                    const ImgBgr& img_orig,
//                    ImgBgr* img_out)
//{
//  const double zoomfact = 1.0;  // real challenge:  camera does not provide absolute scale!!!
//  ImgFloat fx(width, height), fy(width, height);
//  img_out->Reset(width, height);
//  float* px = fx.Begin();
//  float* py = fy.Begin();
//  assert(zoom != 0);
//  zoom = ( zoom - 1.0 ) * zoomfact + 1.0;
//  double invzoom = 1.0 / zoom;
//  for (int y=0 ; y<height ; y++)
//  {
//    for (int x=0; x<width ; x++)
//    {
////      *px++ = static_cast<float>( zoom * x + dx );
////      *py++ = static_cast<float>( zoom * y + dy );
//      *px++ = static_cast<float>( (x - dx) * invzoom );
//      *py++ = static_cast<float>( (y - dy) * invzoom );
//    }
//  }
//  Warp(img_orig, fx, fy, img_out);
//}
//
//// return true for success
//bool ComputeTransformation(const KltFeatureList& fl1, 
//                           const KltFeatureList& fl2, 
//                           double* zoom, double* dx, double* dy)
//{
//  const int n = fl1.GetNFeatures();
//  assert( n == fl2.GetNFeatures() );
//  std::vector<Point2d> v1, v2;
//  for (int i=0 ; i<n ; i++)
//  {
//    const KLT_Feature& feat1 = fl1[i];
//    const KLT_Feature& feat2 = fl2[i];
//    if (feat1->val >= 0 && feat2->val >= 0)
//    {
//      v1.push_back( Point2d( feat1->x, feat1->y ) );
//      v2.push_back( Point2d( feat2->x, feat2->y ) );
//    }
//  }
//  if (v1.size() < 2)  return false;
//  TransScaleFit(v1, v2, zoom, dx, dy);
//  return true;
//}
//
//void DrawRadialLine(const Point& pt1, double angle, double length1, double length2, ImgBgr  * out, const Bgr&    color, int thickness)
//{
//  angle *= blepo_ex::Pi / 180.0;
//  double dx2 = blepo_ex::Round(cos(angle)*length1);
//  double dy2 = blepo_ex::Round(sin(angle)*length1);
//  Point pt2(pt1.x + dx2, pt1.y + dy2);
//  double dx3 = blepo_ex::Round(cos(angle)*length2);
//  double dy3 = blepo_ex::Round(sin(angle)*length2);
//  Point pt3(pt1.x + dx3, pt1.y + dy3);
//  DrawLine(pt2, pt3, out, color, thickness);
//}
//
//// Project for Gillam, PD Singh, Feb 2009
//void CameraImageControl()
//{
//  try
//  {
//    ImgBgr img1, img2, img_ref;  // for display
//    ImgGray gray_img1, gray_img2, gray_img_ref;
//    ImgBgr big_img_to_disp, zoomee;  // x-ray being navigated
//    Figure fig, fig2;
//    int nfeatures = 100;
//    CaptureDirectShow cap;
//    KltFeatureList fl_ref(nfeatures), fl2(nfeatures);
//    KltTrackingContext tc;
//    bool first_frame = true;
//    double dx, dy, zoom;
//    int big_width, big_height;
//
//    // current view into image
//    int xc, yc;
//    double scale;
////    double aspect_ratio;
//
////    Load("C:/stb/personal/pictures/2006_sep_scotland/DSCF0305.JPG", &big_img_to_disp);
//    Load("C:/stb/research/projects/gillam-medical/pd-singh-augreal/xray.jpg", &big_img_to_disp);
//    {
//      ImgBgr foo;
////      Extract(big_img_to_disp, Rect(100, 100, 600, 600), &foo);
////      Downsample(big_img_to_disp, 4, 4, &foo);
//      Resample(big_img_to_disp, 542, 327, &foo);
//      fig.Draw(foo); //big_img_to_disp);
//      return;
//    }
//    big_width = big_img_to_disp.Width();
//    big_height = big_img_to_disp.Height();
//    { // initialize current view
////      aspect_ratio = (double) big_img_to_disp.Width() / (double) big_img_to_disp.Height();
//      xc = big_width/2;
//      yc = big_height/2;
//      scale = 1.0;
//    }
//
//    cap.BuildGraph();
//    cap.Start();
//    while (1)
//    {
//      bool success = cap.GetLatestImage(&img2);
//      if (success)
//      {
//        FlipVertical(img2, &img2);
//        Convert(img2, &gray_img2);
//        if (first_frame)
//        {
//          img_ref = img1 = img2;
//          gray_img_ref = gray_img1 = gray_img2;
//          tc.SelectGoodFeatures(&fl_ref, gray_img_ref);
//          fl2 = fl_ref;
//          tc.OverlayFeatures(&img2, fl2);
//        }
//        else
//        {
//          tc.TrackFeatures(&fl2, gray_img1, gray_img2);
////          tc.ReplaceLostFeatures(&fl2, gray_img2);
//          tc.OverlayFeatures(&img2, fl2);
//        }
//        bool success = ComputeTransformation(fl_ref, fl2, &zoom, &dx, &dy);
//        if (!success)  break;
//        { // velocity control
//          const int new_width = 640;
//          const int new_height = 480;
//          if (fabs(dx) > 2)  xc -= dx;
//          if (fabs(dy) > 2)  yc -= dy;
//          if (fabs(zoom-1.0) > 0.001)  scale /= zoom;
////          int hw = blepo_ex::Round( big_width/2 * scale );
////          int hh = blepo_ex::Round( big_height/2 * scale );
////          { // bounds checking
//            double max_scale = blepo_ex::Min( (big_width - 2.0) / new_width, (big_height - 2.0) / new_height );
//            if (scale < 0.25)  scale = 0.25;
//            if (scale > max_scale)  scale = max_scale;
//            int min_xc = blepo_ex::Round( (new_width/2)*scale );
//            int min_yc = blepo_ex::Round( (new_height/2)*scale );
////            int min_xc = new_width/2;
////            int min_yc = new_height/2;
//            int max_xc = big_width  - min_xc - 1;
//            int max_yc = big_height - min_yc - 1;
//            assert(max_xc >= min_xc && max_yc >= min_yc);  // scale too big!!!
//            if (xc < min_xc)  xc = min_xc;
//            if (yc < min_yc)  yc = min_yc;
//            if (xc > max_xc)  xc = max_xc;
//            if (yc > max_yc)  yc = max_yc;
////          }
//          int hwl = new_width/2;
//          int hht = new_height/2;
//          int hwr = new_width - hwl - 1;
//          int hhb = new_height - hht - 1;
////          Extract(big_img_to_disp, xc, yc, hwl-1, hht-1, &zoomee);
//          Extract(big_img_to_disp, xc, yc, hwl, hwr, hht, hhb, scale, &zoomee);
////          Extract(big_img_to_disp, xc, yc, hwl, hwr, hht, hhb, 2.0, &zoomee);
////          WarpTransScaleCenter(big_img_to_disp, scale, xc, yc, new_width, new_height, &zoomee);
////        }
//          TRACE("%5.3f (%5.1f %5.1f) : %5.3f (%4d %4d) \n", zoom, dx, dy, scale, xc, yc);
//    //    { // overlay circles
//          int foo = 4;  // scale factor for display
//          int radiusee = 20;
//          Point p1(new_width/2, new_height/2);
//          int r1 = radiusee;
//          Point p2(new_width/2 + dx*foo, new_height/2 + dy*foo);
//          int r2 = radiusee; //blepo_ex::Round(20.0);
//          DrawLine(p1, p2, &zoomee, Bgr(0,0,255), 1);
//          DrawCircle(p1, r1, &zoomee, Bgr(0,255,0), 1);
//          DrawCircle(p2, r2, &zoomee, Bgr(0,0,255), 1);
//          { // draw radial lines
//            double length1 = radiusee;
//            double length2 = radiusee * pow(zoom,5);
//            for (double theta=0 ; theta < 359 ; theta += 45)
//            {
//              DrawRadialLine(p1, theta, length1, length2, &zoomee, Bgr(0,255,0), 1);
//            }
//          }
//        }
//        fig.Draw(img2);
//        fig2.Draw(zoomee);
//        fig2.PlaceToTheRightOf(fig);
//        gray_img1 = gray_img2;
//        first_frame = false;
//        if (fig.TestMouseClick())  break;
//      }
//    }
//    cap.Stop();
//  } 
//  catch (Exception& e)
//  {
//    e.Display();
//  }
//}

//#include <vector>
//#include <deque>


typedef std::vector<int> MyHist;

void UseMyVector(const std::vector<int>& histogram)
{
}

void ComputeHistogram(const ImgGray& img, int* hist)
{
  for (int i=0 ; i<256 ; i++)
  {
    hist[i] = 0;
  }

  int x, y;
  for (y=0 ; y < img.Height() ; y++)
  {
    for (x=0 ; x < img.Width() ; x++)
    {
      unsigned char val = img(x, y);
      hist[val] = hist[val] + 1;

      // or this:  hist[ img(x,y) ] = hist[ img(x,y) ] + 1;
    }
  }
}

Bgr GetRandomColor()
{
  int r = blepo_ex::GetRand();
  return ImgBgr::Pixel((r * 12342223) % 0xFFFFFF, Bgr::BLEPO_BGR_XBGR);
}

#include <vector>

void DrawLines(const std::vector<LineFitting::Line>& lines, ImgBgr* img)
{
  for (int i=0 ; i<(int)lines.size() ; i++)
  {
    DrawLine(lines[i].p1, lines[i].p2, img, GetRandomColor());
  }
}

void LedXForm(const ImgBgr& img, ImgBinary* out)
{
  out->Reset(img.Width(), img.Height());
  Set(out, false);
  ImgBgr::ConstIterator p = img.Begin();
  ImgBinary::Iterator po = out->Begin();
  while (p != img.End())
  {
    if (p->g == 255)  *po = true;
    p++;  po++;
  }
}



void CDemoDlg::OnButtonTest() 
{
}

//#include <float.h>

void CDemoDlg::OnButtonTest2() 
{
}

void CDemoDlg::OnButtonMeanShiftSegmentation() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("monalisa.jpg");
    if (fname != "")
    {
      CWaitCursor wait;
      ImgBgr img, segments;
      ImgInt labels;
      Figure fig("image"), fig2("labels"), fig3("segments");
      Load(fname, &img);
      fig.Draw(img);
      MeanShiftSegmentation(img, &labels, &segments);
      fig2.Draw(labels);
      fig3.Draw(segments);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonGraphBasedSegmentation() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("monalisa.jpg");
    if (fname != "")
    {
      CWaitCursor wait;
      ImgBgr img, segments;
      ImgInt labels;
      Figure fig("image"), fig2("labels"), fig3("segments");
      Load(fname, &img);
      fig.Draw(img);
//      char goo[100];
//      getcwd(goo, 100);
//      FILE* fp = fopen("out.txt", "wt");
//      for (int k=10 ; k<1000 ; k+=10)
//      {
//        int nsegments = FHGraphSegmentation(img, 0.5, k, 0, &labels, &segments);
//        fprintf(fp, "%d %d\n", k, nsegments);
//        CString foo;
//        foo.Format("monalisa_segments_%04d.ppm", k);
//        Save(segments, foo);
//      }
//      fclose(fp);
      int nsegments = FHGraphSegmentation(img, 1.5, 500, 0, &labels, &segments);
      fig2.Draw(labels);
      fig3.Draw(segments);

      // display boundaries overlaid on image
      {
        Figure fig5;
        ImgGray gimg;
        Convert(img, &gimg);

        ImgGray glab, diff;
        ImgBinary bdiff;
        RemoveGapsFromLabelImage(labels, &labels);
        assert(nsegments < 256);
        if (nsegments > 256)  AfxMessageBox("Display will be wrong because more than 256 labels.  Problem is using ImgGray to store result to avoid having to write new function.");
        Convert(labels, &glab);
        GradMagPrewitt(glab, &diff);
        Threshold(diff, 1, &bdiff);
        ImgBgr bgr;
        Convert(gimg, &bgr);
        Set(&img, bdiff, Bgr(0,0,255));
        fig5.Draw(img);
      }
    
      CString str;
      str.Format("%d segments found", nsegments);
      AfxMessageBox(str);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonCanny() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
//      CString fname = "C:/stb/doc/www/ece847/cat.pgm";
      ImgGray img;
      ImgBinary edges;
      float sigma = 1.0;
      Load(fname, &img);

      Canny(img, &edges, sigma);
      Figure fig1("Image"), fig2("Canny edges");
      fig1.Draw(img);
      fig2.Draw(edges);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonRegionprops() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("banana.pgm");
    if (fname != "")
    {
      RegionProperties props;
      ImgGray img;
      ImgBinary bin;
      ImgBgr bgr;
      Figure fig;

      // compute region properties
      Load(fname, &img);
      Threshold(img, 128, &bin);
//      bin.Reset(8,5);
//      Set(&bin, 0);
//      bin(1,0) = 1;
//      bin(6,0) = 1;
//      bin(7,0) = 1;
//      bin(2,1) = 1;
//      bin(4,1) = 1;
//      bin(0,2) = 1;
//      bin(3,2) = 1;
//      bin(5,2) = 1;
//      bin(6,2) = 1;
//      bin(1,3) = 1;
//      bin(2,3) = 1;
//      bin(5,3) = 1;
//      bin(6,3) = 1;
//      bin(0,4) = 1;
//      bin(1,4) = 1;
//      bin(4,4) = 1;
//      bin(6,4) = 1;
//      bin(7,4) = 1;
      Figure foo;
      foo.Draw(bin);

      RegionProps(bin, &props);
      Convert(bin, &bgr);

      // display results
      Point pt0((int) props.xc, (int) props.yc);
      Point pt1((int) props.major_axis_x, (int) props.major_axis_y);
      Point pt2((int) props.minor_axis_x, (int) props.minor_axis_y);
      DrawCircle(pt0, 2, &bgr, Bgr(0,0,255));
      DrawLine(pt0, pt0+pt1, &bgr, Bgr(255,0,0), 1);
      DrawLine(pt0, pt0+pt2, &bgr, Bgr(255,0,0), 1);
      DrawCircle(pt0+pt1, 2, &bgr, Bgr(255,0,0));
    //  bgr((pt0+pt1).x, (pt0+pt1).y) = Bgr(255,0,0), 1);
      DrawRect(props.bounding_rect, &bgr, Bgr(0,255,255));
      fig.Draw(bgr);

      // print results
      const CRect& r = props.bounding_rect;
      CString str;
      str.Format("Properties of region:\r\n"
        "  area (number of pixels):  %d\r\n"
        "  bounding rect: (%3d, %3d, %3d, %3d)\r\n"
        "  centroid:  (%5.1f, %5.1f)\r\n"
        "  major axis:  [%5.1f %5.1f]\r\n"
        "  minor axis:  [%5.1f %5.1f]\r\n"
        "  direction (clockwise from horizontal):  %5.1f radians\r\n"
        "  eccentricity:  %5.1f\r\n"
        "  moments:\r\n"
        "    m00:  %11.1f\r\n"
        "    m10:  %11.1f\r\n"
        "    m01:  %11.1f\r\n"
        "    m11:  %11.1f\r\n"
        "    m20:  %11.1f\r\n"
        "    m02:  %11.1f\r\n"
        "  centralized moments:\r\n"
        "    mu00:  %11.1f\r\n"
        "    mu10:  %11.1f\r\n"
        "    mu01:  %11.1f\r\n"
        "    mu11:  %11.1f\r\n"
        "    mu20:  %11.1f\r\n"
        "    mu02:  %11.1f\r\n",
          (int) props.area, r.left, r.top, r.right, r.bottom, 
          props.xc, props.yc,
          props.major_axis_x, props.major_axis_y,
          props.minor_axis_x, props.minor_axis_y,
          props.direction, props.eccentricity, 
          props.m00, props.m10, props.m01, props.m11, props.m20, props.m02, 
          props.mu00, props.mu10, props.mu01, props.mu11, props.mu20, props.mu02);
      AfxMessageBox(str, MB_ICONINFORMATION);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonLucasKanadeAffine() 
{
  try
  {
    CString fname1 = iLetUserSelectImageFile("taxi01.pgm", "Select first image");
    CString fname2 = iLetUserSelectImageFile("taxi02.pgm", "Select second image");
    if (fname1 != "" && fname2 != "")
    {
      ImgGray img1_gray, img2_gray;
      Load(fname1, &img1_gray);
      Load(fname2, &img2_gray);

      ImgFloat img1, img2;
      Convert(img1_gray, &img1);
      Convert(img2_gray, &img2);

      LucasKanadeAffineParameters params;
      MatFlt A_init, d_init(1,2);
      Eye(2, &A_init); // A initialized to identity matrix
      Set(&d_init, 0.0f); // d initialized with zeros

      Rect rect(89,98,120,115);  // rectangular region to be tracked
      LucasKanadeAffineResult result;
      LucasKanadeAffine(img1, img2, rect, params, A_init, d_init, &result);

      // displaying images and result  
      ImgBgr img1_bgr;
      Convert(img1_gray, &img1_bgr);
      DrawRect(Rect(89,98,120,115), &img1_bgr, Bgr(0,0,255));
      Figure fig1("region in reference image"), fig2("reference image"), fig3("warped image");
      fig1.Draw(img1_bgr);
      fig2.Draw(img1);
      fig3.Draw(img2);

      char sts_ch[100], A_ch1[100], A_ch2[100], d_ch1[100], d_ch2[100], ct_ch[100], res_ch[100];
      sprintf(sts_ch,"status is %d\n",result.status);
      sprintf(A_ch1,"A is  %f  %f\n",result.A(0,0), result.A(0,1)); 
      sprintf(A_ch2,"       %f  %f\n",result.A(1,0), result.A(1,1));
      sprintf(d_ch1,"d is  %f\n",result.d(0,0));
      sprintf(d_ch2,"       %f\n",result.d(0,1));
      sprintf(ct_ch,"centroid is (%f, %f)\n",result.centroid_x, result.centroid_y);
      sprintf(res_ch,"residue per pixel is %f\n",result.residue);

      CString result_msg = CString(sts_ch)+CString(A_ch1)+CString(A_ch2)+CString(d_ch1)+
                           CString(d_ch2)+CString(ct_ch)+CString(res_ch);

      AfxMessageBox(result_msg);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
	
}

void CDemoDlg::OnButtonRealtimeStereo() 
{
  try 
  {
#if 1
    CString fname1 = iLetUserSelectImageFile("img0.pgm", "Select left image");
    CString fname2 = iLetUserSelectImageFile("img1.pgm", "Select right image");
    CString fname_ground_truth = "";
#elif 0
    CString fname1 = "C:/stb/data/images/stereo/middlebury/data_ppm/tsukuba/tsukuba_left.pgm";
    CString fname2 = "C:/stb/data/images/stereo/middlebury/data_ppm/tsukuba/tsukuba_right.pgm";
    CString fname_ground_truth = ""; //C:/stb/data/images/middlebury_stereo/truedisp_pgm/tsukuba/truedisp.pgm";
#elif 0
    CString fname1 = "C:/stb/data/images/stereo/img_meter1.pgm";
    CString fname2 = "C:/stb/data/images/stereo/img_meter2.pgm";
    CString fname_ground_truth = "";
#else
    CString fname1 = "C:/stb/data/images/middlebury_stereo/data_ppm/sawtooth/left.ppm";
    CString fname2 = "C:/stb/data/images/middlebury_stereo/data_ppm/sawtooth/right.ppm";
    CString fname_ground_truth = "C:/stb/data/images/middlebury_stereo/truedisp_pgm/sawtooth/truedisp.pgm";
#endif

    if (fname1 != "" && fname2 != "")
    {
      ImgGray img_left, img_right, output;
      ImgGray truth;
	    
	    Load(fname1, &img_left);
	    Load(fname2, &img_right);
      if (strlen(fname_ground_truth)>0)  Load(fname_ground_truth, &truth);
  
      Figure fig1("left"), fig2("right"), fig3("ground truth");
      fig1.Draw(img_left);
      fig2.Draw(img_right);
      fig3.Draw(truth);

      Stopwatch  s;
      int niter = 30;
    //  for (int i=0; i<niter ; i++)
      RealTimeStereo(img_left, img_right, &output, 100, 11);  // used to be 16

    //  long ms = s.GetElapsedMilliseconds(false) / niter;
    //  AfxMessageBox(StringEx("Done %d ms", ms));

      Figure figg("final output");
      LinearlyScale(output, 0, 255, &output);
      figg.Draw(output);

      if (truth.Width() > 0)
      {
        ImgGray diff;
        AbsDiff(output, truth, &diff);
        ImgBinary out;
        Threshold(diff, 2, &out);
        float f = ((float) Sum(out)) / (out.Width() * out.Height());
        AfxMessageBox(StringEx("Error %f ", f));	
      }
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonDshowUpdateDevices() 
{
  std::vector<CString> audio_names, video_names;

  // get video and audio device names
  CaptureDirectShow::GetVideoInputDevices(&video_names);
  CaptureDirectShow::GetAudioInputDevices(&audio_names);

  // display names in list boxes
  m_dshow_video_devices.ResetContent();
  for (int i=0 ; i<(int)video_names.size() ; i++)
  {
    m_dshow_video_devices.AddString(video_names[i]);
  }

  if (video_names.size() > 0)
  {
    m_dshow_video_devices.SetCurSel(0);
  }
  // refresh whether dshow capture button is enabled
  EnableDisable();
}

void CDemoDlg::OnButtonDshowCapture() 
{
  CaptureDirectShow cap;
  try
  {
    // Build the graph
    int camera_index = m_dshow_video_devices.GetCurSel();
//    cap.BuildGraph(320, 240, camera_index);
//    cap.BuildGraph(640, 480, camera_index);
    cap.BuildGraph(1920, 1080, camera_index);
//    CaptureDirectShow::BuildParams params;
//    params.add_to_rot = true;
//    cap.BuildGraph(320, 240, camera_index, params);

    // Tell the user how to stop this thing
    AfxMessageBox("Click on the image to stop the capture", MB_OK, MB_ICONINFORMATION);

    cap.Start();
    ImgBgr img;
    Figure fig;
    CClientDC dc(this);
    FaceDetector detector;

    // variable for saving to file
    bool save_to_file = m_check_save_to_file.GetCheck() != 0;
    int index = 0;
    CString save_fmt;
    if (save_to_file)  
    {
      m_edit_save_fname_fmt.GetWindowText(save_fmt);
      m_edit_save_fname_fmt.EnableWindow(false);
    }

    // Start capturing frames and displaying them
    while (1)
    {
      bool newimage = cap.GetLatestImage(&img, m_check_flip_image.GetCheck() != 0);
      if (newimage)
      {
        assert(img.Width()>0 && img.Height()>0);

        // Run face detector
        if (m_check_run_face_detector.GetCheck())
        {
          std::vector<Rect> front, left, right;
#if 1
          detector.DetectFrontalFaces(img, &front);
          int i;
          for (i=0 ; i<(int)front.size() ; i++) {
            DrawRect(front[i], &img, Bgr(0,255,0));
          }
#else
          detector.DetectAllFaces(img, &front, &left, &right);
          int i;
          for (i=0 ; i<(int)left.size() ; i++) {
            DrawRect(left[i], &img, Bgr(255,0,0));
          }
          for (i=0 ; i<(int)right.size() ; i++) {
            DrawRect(right[i], &img, Bgr(0,0,255));
          }
          for (i=0 ; i<(int)front.size() ; i++) {
            DrawRect(front[i], &img, Bgr(0,255,0));
          }
#endif
        }

        // Save to file
        if (save_to_file)
        {
          CString fname;
          fname.Format(save_fmt, index++);
          m_edit_save_fname_fmt.SetWindowText(fname);
          Save(img, fname);
        }

        // Draw image and test for mouse click
        fig.Draw(img);
      }
      if (fig.TestMouseClick())  break;
    }
    // Stop capturing
    cap.Stop();
    cap.TearDownGraph();

    if (save_to_file)  
    {
      m_edit_save_fname_fmt.SetWindowText(save_fmt);
      m_edit_save_fname_fmt.EnableWindow(true);
    }
  } catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnSelchangeListDshowVideoDevices() 
{
  EnableDisable();
}

void CDemoDlg::OnButtonIeee1394() 
{
  try
  {
    CaptureIEEE1394 cap;
  //  cap.GetCameras();
    CaptureIEEE1394::Format fmt = CaptureIEEE1394::FMT_320x240_YUV422;
    CaptureIEEE1394::Fps fps = CaptureIEEE1394::FPS_30;
    bool sup = cap.IsFormatSupported(fmt, fps);
    if (sup)
    {
      cap.SetFormat(fmt, fps, true);

      // Tell the user how to stop this thing
      AfxMessageBox("Click on the image to stop the capture", MB_OK, MB_ICONINFORMATION);

      cap.Start();
      Figure fig;
      ImgBgr img;
      while (1)
      {
        bool newimage = cap.GetLatestImage(&img);
        if (newimage)
        {
          fig.Draw(img);
          if (fig.TestMouseClick())  break;
          Sleep(10);
        }
      }
      cap.Stop();
    }
  } catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonFft() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgGray img;
      ImgFloat img_float, fft_real, fft_imag, fft_mag, fft_phase;
      Figure fig1, fig2;
      Load(fname, &img);
      Convert(img, &img_float);

      // shift origin of FFT to center of image (see p. 154 of Gonzalez and Woods)
      for (int y=0 ; y<img_float.Height() ; y++)
      {
        for (int x=0 ; x<img_float.Width() ; x++)
        {
          if ((x+y) % 2 == 1)  img_float(x, y) = - img_float(x, y);
        }
      }

      // compute FFT, convert to magnitude / phase, and compute logarithm (for display)
      ComputeFFT(img_float, &fft_real, &fft_imag);
      RectToMagPhase(fft_real, fft_imag, &fft_mag, &fft_phase);
      for (float* f = fft_mag.Begin() ; f != fft_mag.End() ; f++)  *f = log(*f);

      // display results
      fig1.Draw(img);
      fig2.Draw(fft_mag);	
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonEllipticalHeadTracker() 
{
  // variables specific to this image sequence
  CString fname_fmt = iGetImagesDir() + "seq_sb/img%03d.jpg";
  const int first_frame = 0;
  const int last_frame = 60;
  EllipticalHeadTracker::EllipseState init_state(58, 47, 21);

  ImgBgr img;
  Figure fig;
  EllipticalHeadTracker::EllipseState state;
  EllipticalHeadTracker	eht;
  bool first_time = true;

  for (int i=first_frame ; i<last_frame ; i++)
  {
    CString str;
    str.Format(fname_fmt, i);
    Load(str, &img);
    if (first_time)
    {
      eht.Init(img, init_state);
      first_time = false;
    }
    else
    {
      state = eht.Track(img);
      TRACE("i=%2d  state=(%3d,%3d,%3d)\n", i, state.x, state.y, state.sz);
      eht.OverlayEllipse(state, &img);
      fig.Draw(img);
    }
  }
}

void CDemoDlg::OnButtonCamshift() 
{
  // variables specific to this image sequence
  CString fname_fmt = iGetImagesDir() + "seq_sb/img%03d.jpg";
  const int first_frame = 0;
  const int last_frame = 60;
  CRect init_rect(38,24,78,69);

  ImgBgr img;
  Figure fig;
  CString str;
  CamShift::Box box;

  str.Format(fname_fmt, first_frame);
  Load(str, &img);

  CamShift camshift(img);
  camshift.CalcHistogram(img, init_rect);

  for (int i=first_frame ; i<last_frame ; i++)
  {
    str.Format(fname_fmt, i);
    Load(str, &img);
    box = camshift.Track(img);
    DrawEllipse(box.center, box.size.cx/2, box.size.cy/2, blepo_ex::Rad2Deg(box.angle), &img,  Bgr(0,0,255), 2);
    fig.Draw(img);
  }
}

// displays the chessboard calibration points overlaid on the most recent image
// does all the processing of an image for calibrating camera from a chessboard pattern
// returns true if user has requested calibration to be performed; false otherwise
bool DisplayCalibrationPointsAndGetUserInput(Figure* fig, const ImgGray& img, const Cvptarr& pts, const Size& grid_dims, bool all_corners_found)
{
  // display
  {
    ImgBgr cimg;
    Convert(img, &cimg);
    DrawChessboardCorners(&cimg, grid_dims, all_corners_found, pts);

    if (all_corners_found)
    {
      const char* text1 = "continue";
      const char* text2 = "calibrate";
      TextDrawer td(15, 2);
      Size sz = td.GetTextSize(text2);
      Point pt1(0, 0);
      Point pt2(cimg.Width() - sz.cx, 0);
      Rect rect1 = td.GetTextBoundingRect(text1, pt1);
      Rect rect2 = td.GetTextBoundingRect(text2, pt2);
      td.DrawText(&cimg, text1, pt1, Bgr(0, 255, 0), Bgr(0, 0, 0));
      td.DrawText(&cimg, text2, pt2, Bgr(0, 0, 255), Bgr(0, 0, 0));    
      fig->Draw(cimg);

      while (1)
      {
        Point pt = fig->GrabMouseClick();
        if (rect1.PtInRect(pt))  break;
        else if (rect2.PtInRect(pt))
        {
          return true;
        }
      }
    }
    else
    {
      fig->Draw(cimg);
    }
  }

  return false;
}

void CDemoDlg::OnButtonCalibrate() 
{
  try
  {
    const Size grid_dims(8, 6);  // number of corners in chessboard pattern (horiz. and vert.)
    ImgBgr img;
    ImgGray gimg;
    CaptureDirectShow cap;
    std::vector<CalibrationPointArr> all_pts;
    CalibrationParams params;
    Figure fig;
    bool do_calibrate = false;
    const int num_duds_init = 150;
    int num_dud_images = num_duds_init;

    cap.BuildGraph(320, 240, 0);
    AfxMessageBox("To calibrate, hold the chessboard calibration pattern (\"images/chessboard.pdf\")\r\nin front of the camera at different orientations.", MB_ICONINFORMATION);      // chessboard.pdf is from http://www.vision.caltech.edu/bouguetj/calib_doc/htmls/own_calib.html (called pattern.pdf)
    cap.Start();
    while (!do_calibrate)
    {
      bool ok = cap.GetLatestImage(&img);
      if (ok)
      {
        Convert(img, &gimg);
//        {
//          fig.Draw(gimg);
//          Point pt;
//          static int iijj=0;
//          if (fig.TestMouseClick(&pt))
//          {
//            CString str;
//            str.Format("fmat%03d.pgm", iijj++);
//            Save(gimg, str);
//          }
//          continue;
//        }

        // grab calibration points and transform
        Cvptarr pts;
        CalibrationPointArr cpts;
        bool all_corners_found = FindChessboardCorners(gimg, grid_dims, &pts);
        if (all_corners_found)
        {
          TransformChessboardPoints(pts, grid_dims, &cpts);
          all_pts.push_back( cpts );
          num_dud_images = -1;
        }
        else  num_dud_images--;
        
        // display calibration points and get user input
        do_calibrate = DisplayCalibrationPointsAndGetUserInput(&fig, gimg, pts, grid_dims, all_corners_found);

        if (num_dud_images == 0)
        {
          unsigned ret = AfxMessageBox("Calibration target not found.  Continue?", MB_YESNO);
          if (ret == IDNO)  do_calibrate = true;
          num_dud_images = num_duds_init;
        }
      }
    }

    cap.Stop();
    cap.TearDownGraph();

    // calibrate and display
    if (all_pts.size() > 0)
    {
      CalibrateCamera(all_pts, Size(img.Width(), img.Height()), &params);
      Display(params.intrinsic_matrix, "intrinsic matrix");	
      Display(params.distortion_coeffs, "distortion coeffs");	
    }

  } 
  catch (const Exception& e)
  {
    CString msg;
    msg.Format("Calibration demo uses the first DirectShow camera that it finds.\r\n\r\n%s", e.m_message);
    AfxMessageBox(msg);
  }
}

void CDemoDlg::OnButtonFastFeatureTracker() 
{
  try
  {
    ImgBgr img;
    ImgGray gimg;
    CaptureDirectShow cap;
    Figure fig;
    bool need_to_init = true;

    FastFeatureTracker track;
    FastFeatureTracker::FeatureArr points;
    const int max_npoints = 500;

//    Figure fig2;
//    ImgGray gray_img1, gray_img2;
//    ImgBgr imgbgr1, imgbgr2;
//    Load("C:/stb/data/images/mobile_robot/bushes/img2607.bmp", &gray_img1);
//    Load("C:/stb/data/images/mobile_robot/bushes/img2608.bmp", &gray_img2);
//    track.SelectFeatures(gray_img1, max_npoints, &points);
//    Convert(gray_img1, &imgbgr1);
//    track.DrawFeatures(&imgbgr1, points, Bgr(0, 0, 255));
//    track.TrackFeatures(gray_img2, &points, false);
//    Convert(gray_img2, &imgbgr2);
//    track.DrawFeatures(&imgbgr2, points, Bgr(0, 0, 255));
//    fig.Draw(imgbgr1);
//    fig2.Draw(imgbgr2);
//    while (1)
//    {
//      Point pt = fig.GrabMouseClick();
//
//    }
//    return;


    // Tell the user how to stop this thing
    AfxMessageBox("Click on the image to stop the capture", MB_OK, MB_ICONINFORMATION);

    cap.BuildGraph(320, 240, 0);
    cap.Start();
    while (!fig.TestMouseClick())
    {
      bool ok = cap.GetLatestImage(&img);
      if (ok)
      {
        Convert(img, &gimg);

        if (need_to_init)
        {
          track.SelectFeatures(gimg, max_npoints, &points);
          Convert(gimg, &img);
          track.DrawFeatures(&img, points, Bgr(0, 0, 255));
          fig.Draw(img);
          need_to_init = false;
        }
        else
        {
          track.TrackFeatures(gimg, &points, false);
          Convert(gimg, &img);
          track.DrawFeatures(&img, points, Bgr(0, 0, 255));
          fig.Draw(img);
        }
      }
    }

    cap.Stop();
    cap.TearDownGraph();

  } catch (const Exception& e)
  {
    CString msg;
    msg.Format("Fast feature tracker demo uses the first DirectShow camera that it finds.\r\n\r\n%s", e.m_message);
    AfxMessageBox(msg);
  }
}

void CDemoDlg::OnButtonWatershed() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("holes.pgm");
    if (fname != "")
    {
      Figure fig1("image"), fig2("markers"), fig3("gradmag"), fig4("labels"), fig5("watershed");
      ImgGray img, gradmag;
      ImgFloat fimg, fgradmag;
      ImgInt labels;
      ImgBinary markers;
      const int th = 70;

      // load image
      Load(fname, &img);
    //Load("C:/stb/doc/www/ece847/fall2006/cells_small.pgm", &img);

    //  th = 30;
      fig1.Draw(img);

      Figure fa, fb, fc, fd, fe, ff;
      // compute foreground and background markers
      Threshold(img, th, &markers);
      fa.Draw(markers);
      Not(markers, &markers);
      fb.Draw(markers);
      { // background markers
        ImgGray gmark, gchamf, glabc;
        ImgInt chamfer, labc;
        ImgBinary ridges;
        Convert(markers, &gmark);
        Chamfer(gmark, &chamfer);
        LinearlyScale(chamfer, 0, 256, &chamfer);
        fc.Draw(chamfer);
        Convert(chamfer, &gchamf);
        WatershedSegmentation(gchamf, &labc, false);
    //    fd.Draw(labc);
        Convert(labc, &glabc);
        GradMagPrewitt(glabc, &glabc);
    //    fe.Draw(glabc);
        Threshold(glabc, 1, &ridges);
    //    ff.Draw(ridges);
        Or(markers, ridges, &markers);
        fig2.Draw(markers);
      }

      // gradient magnitude
      Convert(img, &fimg);
      EnlargeByExtension(fimg, 10, &fimg);  // to avoid boundary issues from convolution
      GradMag(fimg, 2.5, &fgradmag);
      EnlargeByCropping(fgradmag, 10, &fgradmag);
      LinearlyScale(fgradmag, 1, 255, &fgradmag);
      Convert(fgradmag, &gradmag);
      Set(&gradmag, markers, 0);
      fig3.Draw(gradmag);

      // watershed
      WatershedSegmentation(gradmag, &labels, true);
      fig4.Draw(labels);

      // display boundaries overlaid on image
      {
        ImgGray glab, diff;
        ImgBinary bdiff;
        Convert(labels, &glab);
        GradMagPrewitt(glab, &diff);
        Threshold(diff, 1, &bdiff);
        ImgBgr bgr;
        Convert(img, &bgr);
        Set(&bgr, bdiff, Bgr(0,255,0));
        fig5.Draw(bgr);
      }
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonWarp() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("freethrow.bmp");
    if (fname != "")
    {
      const int nfeatures = 100;
      ImgBgr img1, img2;
      ImgGray gray_img1, gray_img2;
      KltFeatureList fl(nfeatures), fl_prev(nfeatures);
      KltTrackingContext tc;

      // compute affine warp
      Load(fname, &img1);
      MatDbl aff;
      {
        double theta = 10; // rotation in degrees
        Point2d cen(img1.Width()/2, img1.Height()/2);  // center of image
        Point2d shift(3, 0);  // image shift
        ComputeAffine(-cen.x, -cen.y, blepo_ex::Deg2Rad(theta), 1, 1, 0, 0, cen.x + shift.x, cen.y + shift.y, &aff);
      }
      WarpAffine(img1, aff, &img2);

      // track features
      Convert(img1, &gray_img1);
      Convert(img2, &gray_img2);
      tc.SelectGoodFeatures(&fl, gray_img1);
      fl_prev = fl;
      tc.TrackFeatures(&fl, gray_img1, gray_img2);

      // estimate affine warp
      Array<Point2d> p1;
      Array<Point2d> p2;
      int i;
      for (i=0 ; i<fl.GetNFeatures() ; i++)
      {
        if (fl[i]->val==0 && fl_prev[i]->val>=0)
        {
          Point2d pt1(fl_prev[i]->x, fl_prev[i]->y);
          Point2d pt2(fl[i]->x, fl[i]->y);
          p1.Push(pt1);
          p2.Push(pt2);
        }
      }
      MatDbl affout;
      AffineFit(p1, p2, &affout);

      // apply estimated affine warp
      Array<Point2d> p2o;
      AffineMultiply(affout, p1, &p2o);

      Figure fig1, fig2;

      tc.OverlayFeatures(&img1, fl_prev, Bgr::RED, 3);
      fig1.Draw(img1);

      // display actual features
      tc.OverlayFeatures(&img2, fl_prev, Bgr::RED, 3);
      tc.OverlayFeatureLines(&img2, fl_prev, fl, Bgr::YELLOW, 1);
      tc.OverlayFeatures(&img2, fl, Bgr::GREEN, 3);

      // display estimated features
      for (i=0 ; i<p1.Len() ; i++)
      {
        const int size = 1;
        Point pt1( blepo_ex::Round( p1[i].x ), blepo_ex::Round( p1[i].y ));
        Point pt2( blepo_ex::Round( p2[i].x ), blepo_ex::Round( p2[i].y ));
    //    DrawDot(pt1, &img2, Bgr::BLUE, size);
        DrawDot(pt2, &img2, Bgr::BLUE, size);
    //    DrawLine(pt1, pt2, &img2, Bgr::BLUE, 1);
      }

      fig2.Draw(img2);  
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonChamfer() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("holes.pgm");
    if (fname != "")
    {
      Figure fig1("image"), fig2("chamfer distance");
      ImgGray img, timg;
      ImgInt chamf;
      ImgBinary bin;
      const int th = 70;

      // load image
      Load(fname, &img);

      // compute foreground and background markers
      Threshold(img, th, &bin);	
      Not(bin, &bin);
      Convert(bin, &timg);
      Chamfer(timg, &chamf);

      fig1.Draw(timg);
      fig2.Draw(chamf);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonDelaunay() 
{
  Rect rect(0, 0, 400, 400);
  std::vector<Point2d> points;
  std::vector<DelaunayPointPair> edges;
  const int npoints = 200;
  int i;

  // generate set of random points
  for( i = 0; i < npoints ; i++ )
  {
    Point2d pt( rand()%(rect.Width()-10)+5, rand()%(rect.Height()-10)+5 );
    points.push_back( pt );
  }

  // compute Delaunay
  ComputeDelaunayOpencv(points, rect, &edges);

  Figure fig;
  ImgBgr img(rect.Width(), rect.Height());
  Set(&img, Bgr(0,0,0));

  // draw edges (This is slow)
  for (i=0 ; i < (int) edges.size() ; i++)
  {
    const DelaunayPointPair& pp = edges[i];
    Point p1( blepo_ex::Round(pp.first.x), blepo_ex::Round(pp.first.y));
    Point p2( blepo_ex::Round(pp.second.x), blepo_ex::Round(pp.second.y));
    DrawLine(p1, p2, &img, Bgr(0,255,0) );
  }

  // draw points
  for (i=0 ; i < (int) points.size() ; i++)
  {
    const Point2d& pt = points[i];
    DrawDot(Point( blepo_ex::Round(pt.x), blepo_ex::Round(pt.y)), &img, Bgr(0,0,255) );
  }

  fig.Draw(img);
}

void CDemoDlg::OnCheckSaveToFile() 
{
  EnableDisable();	
}

void CDemoDlg::OnButtonHornSchunck() 
{

//  OpencvHS();
//  return;

//#if 1
//  const CString fmt = "C:/stb/data/images/statue/img%04d.pgm";
//  const int start_frame = 140, end_frame = 500;
//#elif 0
//  const CString fmt = "C:/stb/data/images/basketball/freethrow/img%03d.pgm";
//  const int start_frame = 2, end_frame = 20;
//#else
//#endif
////  const CString fmt = "C:/stb/cvs-local/research/code/vc6/alpha1/demo/smallimg%04d.pgm";

  try
  {
    CString fname1 = iLetUserSelectImageFile("img000.jpg", "Select first image", iGetImagesDir() + "seq_sb\\");
    CString fname2 = iLetUserSelectImageFile("img001.jpg", "Select second image", iGetImagesDir() + "seq_sb\\");
    if (fname1 != "" && fname2 != "")
    {
      CWaitCursor wait;
      const int nlevels = 2;
      const float lambda = 100.0f;//1000.0f; //0.1f; //1000000.0f;
      ImgFloat u, v;

      CString fname;
      ImgGray img1, img2;
      jGaussDerivPyramid pyr1, pyr2;
      ImgBgr bgr;
      Figure fig, figu("u"), figv("v");
      std::vector<jFeature> features;

      Load(fname1, &img1);
      Load(fname2, &img2);
      jComputeGaussDerivPyramid(img1, &pyr1, nlevels);
      jComputeGaussDerivPyramid(img2, &pyr2, nlevels);

      HornSchunck(pyr1, pyr2, &u, &v, lambda);
      fig.Draw(img1);
      figu.Draw(u);
      figv.Draw(v);
      figu.PlaceToTheRightOf(fig);
      figv.PlaceToTheRightOf(figu);
    //  fig.GrabMouseClick();	
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonLinefit() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hallway.jpg");
    if (fname != "")
    {
      ImgBgr img;
      ImgBinary edges;
      ImgGray gimg;
      Load(fname, &img);
      LineFitting detector;
      std::vector<LineFitting::Line> lines;
      Convert(img, &gimg);
      Canny(gimg, &edges, 1.0);
      detector.DetectLines(edges, 5, &lines);
      for (int i = 0 ; i < (int) lines.size(); i++)
      {
        LineFitting::Line line = lines[i];

        DrawLine(lines[i].p1, lines[i].p2, &img, Bgr(0,0,255), 1);
      }
      Figure fig1("Canny edges"), fig2("Straight lines detected");
      fig1.Draw(edges);
      fig2.Draw(img);	
      fig2.PlaceToTheRightOf(fig1);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonHough() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("hallway.jpg");
    if (fname != "")
    {
      ImgBgr img;
      Load(fname, &img);

      OpenCvHough hough;
      std::vector<LineFitting::Line> lines;
      hough.Houghlines(img, &lines);
      for(int i = 0 ; i < (int) lines.size(); i++)
      {
        LineFitting::Line line = lines[i];

        DrawLine(lines[i].p1, lines[i].p2, &img, Bgr(0,0,255), 1);
      }
      Figure fig("Straight lines detected");
      fig.Draw(img);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}



void CDemoDlg::OnButtonBlockMatchOpticalflow() 
{
  try
  {
    CString fname1 = iLetUserSelectImageFile("img036.jpg", "Select first image", iGetImagesDir() + "seq_sb\\");
    CString fname2 = iLetUserSelectImageFile("img037.jpg", "Select second image", iGetImagesDir() + "seq_sb\\");
    if (fname1 != "" && fname2 != "")
    {
      ImgFloat u, v;

      CString fname;
      ImgGray img1, img2;
      ImgFloat velx, vely;
      ImgFloat velx_big, vely_big;
      jGaussDerivPyramid pyr1, pyr2;
      ImgBgr bgr;
      Figure fig, figubig("u (enlarged)"), figvbig("v (enlarged)"), figu("u"), figv("v");
      std::vector<jFeature> features;

      CSize block_size(8, 8);
      CSize shift_size(1, 1);
      CSize max_range(10, 10);
      bool use_previous = false;
      Load(fname1, &img1);
      Load(fname2, &img2);

      // This is now crashing for some reason.  It was fine before.  8/16/10
      OpticalFlowBlockMatchOpencv(img1, img2, block_size, shift_size, max_range, use_previous, &velx, &vely);

      {
        velx_big.Reset( img1.Width(), img1.Height() );
        vely_big.Reset( img1.Width(), img1.Height() );
        for (int y=0 ; y<img1.Height() ; y++)
        {
          for (int x=0 ; x<img1.Width() ; x++)
          {
            velx_big(x, y) = velx(x / block_size.cx, y / block_size.cy);
            vely_big(x, y) = vely(x / block_size.cx, y / block_size.cy);
          }
        }
      }

      fig.Draw(img1);
      figubig.Draw(velx_big);
      figvbig.Draw(vely_big);
      figu.Draw(velx);
      figv.Draw(vely);
      figubig.PlaceToTheRightOf(fig);
      figvbig.PlaceToTheRightOf(figubig);
      figu.PlaceToTheRightOf(figvbig);
      figv.PlaceToTheRightOf(figu);	
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void PeasleyComputeFundamentalMatrix()
{
  CString fname1 = iGetExecutableDirectory() + G_DIR + "images\\" "burgher1_small.jpg";
  CString fname2 = iGetExecutableDirectory() + G_DIR + "images\\" "burgher2_small.jpg";
	IplImage *img1_RANSAC = cvLoadImage(fname1,1);
	IplImage *img2_RANSAC = cvLoadImage(fname2,1);
	IplImage *img1_LM = cvLoadImage(fname1,1);
	IplImage *img2_LM = cvLoadImage(fname2,1);
	IplImage *img1_8POINT = cvLoadImage(fname1,1);
	IplImage *img2_8POINT = cvLoadImage(fname2,1);

  if (!img1_RANSAC || !img2_RANSAC || !img1_LM || !img1_LM || !img1_8POINT || !img1_8POINT )
  {
    AfxMessageBox("Unable to load file!!!");
    return;
  }

	cvNamedWindow("Image1_RANSAC",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image2_RANSAC",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image1_LM",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image2_LM",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image1_8POINT",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image2_8POINT",CV_WINDOW_AUTOSIZE);
  int i;

	// Corresponding points between 'burgher1_small.jpg' and 'burgher2_small.jpg'
	double x1[] = {  158,    3,   56,   93,  125,  105,  213,  178,  244,  284,  
					345,  347,  341,  355,  297,  322,  300,  122,  194,  171,  
					149,  148,  135,  114,  225,  199,  200,  143,  138,  171,  
						7,    9,    9,   31,   77,   33,   78,  131,  111,  273,  
					256,  256,  321,  340,  355,  144 };
	double y1[] = {  107,   77,   88,   96,   77,   61,   42,   29,   75,   56,  
					80,  111,  134,  138,  131,  127,  115,  168,  163,  188, 
					217,  230,  237,  219,  226,  223,  212,  201,  151,  151,  
					114,  100,   87,  157,  153,  125,  127,  130,  130,  132,  
					134,  154,  158,  160,  160,  50 };
	double x2[] = {  189,   19,   74,  112,  142,  120,  229,  194,  258,  304,  
					365,  365,  355,  374,  312,  336,  313,  155,  225,  203,  
					180,  180,  169,  150,  258,  232,  228,  176,  171,  203,  
					27,   27,   26,   49,   96,   53,   96,  147,  127,  286,  
					272,  273,  338,  356,  374,  158 };
	double y2[] = {   97,   63,   76,   84,   63,   47,   29,   16,   65,   46,  
					68,  100,  124,  126,  121,  118,  103,  157,  152,  177,  
					204,  216,  222,  205,  216,  210,  200,  189,  139,  138,  
					102,   89,   74,  145,  139,  111,  113,  118,  116,  119,  
					122,  142,  147,  148,  150,  37 }; 
	int size = 45;
	double slope, intercept;
	double a,b,c;

	CvMat* points1, *points2;
	CvMat* status_RANSAC, *status_LM, *status_8POINT; 
	CvMat* fundamental_matrix_RANSAC;
	CvMat* fundamental_matrix_LM;
	CvMat* fundamental_matrix_8POINT;

	CvMat* epilines_RANSAC;
	CvMat* epilines_LM;
	CvMat* epilines_8POINT;

	//allocate data structures
		points1 = cvCreateMat(1,size,CV_32FC2); 
		points2 = cvCreateMat(1,size,CV_32FC2);
		status_RANSAC = cvCreateMat(1,size,CV_8UC1); 
		status_LM = cvCreateMat(1,size,CV_8UC1); 
		status_8POINT = cvCreateMat(1,size,CV_8UC1); 
		fundamental_matrix_RANSAC = cvCreateMat(3,3,CV_32FC1);
		fundamental_matrix_LM = cvCreateMat(3,3,CV_32FC1);
		fundamental_matrix_8POINT = cvCreateMat(3,3,CV_32FC1);
		epilines_RANSAC = cvCreateMat(3,size,CV_32FC1);
		epilines_LM = cvCreateMat(3,size,CV_32FC1);
		epilines_8POINT = cvCreateMat(3,size,CV_32FC1);

	//populate data structures
		for(i = 0; i < size; i++){
			points1->data.fl[i*2] = static_cast<float>( x1[i] );
			points1->data.fl[i*2+1] = static_cast<float>( y1[i] );

			points2->data.fl[i*2] = static_cast<float>( x2[i] );
			points2->data.fl[i*2+1] = static_cast<float>( y2[i] );
		}

	//compute fundamental matrix
		cvFindFundamentalMat(points1, points2, fundamental_matrix_RANSAC, CV_FM_RANSAC, 1.0,0.99, status_RANSAC);
		cvFindFundamentalMat(points1, points2, fundamental_matrix_LM, CV_FM_LMEDS, 1.0,0.99, status_LM);
		cvFindFundamentalMat(points1, points2, fundamental_matrix_8POINT, CV_FM_8POINT, 1.0,0.99, status_8POINT);

	//compute epipolar lines in image2
		cvComputeCorrespondEpilines(points1, 1, fundamental_matrix_RANSAC, epilines_RANSAC);
		cvComputeCorrespondEpilines(points1, 1, fundamental_matrix_LM, epilines_LM);
		cvComputeCorrespondEpilines(points1, 1, fundamental_matrix_8POINT, epilines_8POINT);
		
	
	for(i = 0; i < size; i++){
		//draw RANSAC
		a = epilines_RANSAC->data.fl[i];
		b = epilines_RANSAC->data.fl[size+i];
		c = epilines_RANSAC->data.fl[2*size+i];
		slope = -a/b;
		intercept = -c/b;
		cvLine(img2_RANSAC, cvPoint(0, blepo_ex::Round( intercept ) ), cvPoint(img2_RANSAC->width, blepo_ex::Round( slope*img2_RANSAC->width + intercept ) ), cvScalar(0,255,0), 1, 8, 0);

		//draw LM
		a = epilines_LM->data.fl[i];
		b = epilines_LM->data.fl[size+i];
		c = epilines_LM->data.fl[2*size+i];
		slope = -a/b;
		intercept = -c/b;
		cvLine(img2_LM, cvPoint(0, blepo_ex::Round( intercept ) ), cvPoint(img2_RANSAC->width, blepo_ex::Round( slope*img2_RANSAC->width + intercept ) ), cvScalar(0,255,255), 1, 8, 0);

		//draw 8POINT
		a = epilines_8POINT->data.fl[i];
		b = epilines_8POINT->data.fl[size+i];
		c = epilines_8POINT->data.fl[2*size+i];
		slope = -a/b;
		intercept = -c/b;
		cvLine(img2_8POINT, cvPoint(0, blepo_ex::Round( intercept ) ), cvPoint(img2_RANSAC->width, blepo_ex::Round( slope*img2_RANSAC->width + intercept ) ), cvScalar(255,255,0), 1, 8, 0);
	}



	//compute epipolar lines in image1
		cvComputeCorrespondEpilines(points2, 2, fundamental_matrix_RANSAC, epilines_RANSAC);
		cvComputeCorrespondEpilines(points2, 2, fundamental_matrix_LM, epilines_LM);
		cvComputeCorrespondEpilines(points2, 2, fundamental_matrix_8POINT, epilines_8POINT);
		
	
	for(i = 0; i < size; i++){
		//draw RANSAC
		a = epilines_RANSAC->data.fl[i];
		b = epilines_RANSAC->data.fl[size+i];
		c = epilines_RANSAC->data.fl[2*size+i];
		slope = -a/b;
		intercept = -c/b;
		cvLine(img1_RANSAC, cvPoint(0, blepo_ex::Round( intercept ) ), cvPoint(img2_RANSAC->width, blepo_ex::Round( slope*img2_RANSAC->width + intercept ) ), cvScalar(0,255,0), 1, 8, 0);

		//draw LM
		a = epilines_LM->data.fl[i];
		b = epilines_LM->data.fl[size+i];
		c = epilines_LM->data.fl[2*size+i];
		slope = -a/b;
		intercept = -c/b;
		cvLine(img1_LM, cvPoint(0, blepo_ex::Round( intercept ) ), cvPoint(img2_RANSAC->width, blepo_ex::Round( slope*img2_RANSAC->width + intercept ) ), cvScalar(0,255,255), 1, 8, 0);

		//draw 8POINT
		a = epilines_8POINT->data.fl[i];
		b = epilines_8POINT->data.fl[size+i];
		c = epilines_8POINT->data.fl[2*size+i];
		slope = -a/b;
		intercept = -c/b;
		cvLine(img1_8POINT, cvPoint(0, blepo_ex::Round( intercept ) ), cvPoint(img2_RANSAC->width, blepo_ex::Round( slope*img2_RANSAC->width + intercept ) ), cvScalar(255,255,0), 1, 8, 0);
	}

	/*
	for(int i = 0; i < size; i++){
		//printf("%lf %lf %lf\n", epilines_RANSAC->data.fl[i][0], epilines_RANSAC->data.fl[i][1], epilines_RANSAC->data.fl[i][2]);
		cvCircle(img1_RANSAC, cvPoint( (int)points1->data.fl[i*2], (int)points1->data.fl[i*2+1]), 2, cvScalar(0,0,255), 1, 8, 0);
		cvCircle(img2_RANSAC, cvPoint( (int)points2->data.fl[i*2], (int)points2->data.fl[i*2+1]), 2, cvScalar(0,0,255), 1, 8, 0);
	}*/
	

	cvShowImage("Image1_RANSAC", img1_RANSAC);
	cvShowImage("Image2_RANSAC", img2_RANSAC);
	cvShowImage("Image1_LM", img1_LM);
	cvShowImage("Image2_LM", img2_LM);
	cvShowImage("Image1_8POINT", img1_8POINT);
	cvShowImage("Image2_8POINT", img2_8POINT);

	//cvWaitKey(0);



	/*
	Matrix F(3,3);

	//Using openCV fundamentral matrix to compute intersection
			CvMat* points1; CvMat* points2;
			CvMat* status; CvMat* fundamental_matrix;

			//get the number of points that overlap
				int size = 0;
				for(int i = 0; i < n_points; i++){
					if(left[i][frame][0] != -999 && left[i][frame-1][0] != -999){ //point correspondence
						size++;
					}
				}


			//allocate data structures
				points1 = cvCreateMat(1,size,CV_32FC2); points2 = cvCreateMat(1,size,CV_32FC2);
				status = cvCreateMat(1,size,CV_8UC1); fundamental_matrix = cvCreateMat(3,3,CV_32FC1);

			//populate data structures
				for(int i = 0, j = 0; i < n_points; i++){
					if(left[i][frame][0] != -999){
						points1->data.fl[j*2] = left[i][frame][0];
						points1->data.fl[j*2+1] = left[i][frame][1];

						points2->data.fl[j*2] = right[i][frame][0];
						points2->data.fl[j*2+1] = right[i][frame][1];
						j++;
					}
				}
			
			
			//compute fundamental matrix
				cvFindFundamentalMat(points1, points2, fundamental_matrix, CV_FM_LMEDS, .5,1, status);
			

				for(int i = 0; i < 3; i++){
					for(int j = 0; j < 3; j++){
						F.set(i,j, fundamental_matrix->data.fl[i*3+j]);
					}
				}

			return F;
			*/
}

void CDemoDlg::OnButtonFundamentalMatrix() 
{
  try
  {
    // compute F using OpenCV
    PeasleyComputeFundamentalMatrix();

    // now compute F using our (non-normalized) implementation
    ImgGray img1, img2;
    ImgBgr bgr1, bgr2;
    int i;
    Load(iGetImagesDir() + "burgher1_small.jpg", &img1);
    Load(iGetImagesDir() + "burgher2_small.jpg", &img2);
    Convert(img1, &bgr1);
    Convert(img2, &bgr2);
    Figure fig1("img1  Left click: Select Point,  Right click: Done");
    Figure fig2("img2");
    fig1.Draw(bgr1);
    fig2.Draw(bgr2);
    fig2.PlaceToTheRightOf(fig1);

    // Corresponding points between 'burgher1_small.jpg' and 'burgher2_small.jpg'
    const int NPOINTS = 46;
    int x1[] = {  158,    3,   56,   93,  125,  105,  213,  178,  244,  284,  
                     345,  347,  341,  355,  297,  322,  300,  122,  194,  171,  
                     149,  148,  135,  114,  225,  199,  200,  143,  138,  171,  
                       7,    9,    9,   31,   77,   33,   78,  131,  111,  273,  
                     256,  256,  321,  340,  355,  144 };
    int y1[] = {  107,   77,   88,   96,   77,   61,   42,   29,   75,   56,  
                      80,  111,  134,  138,  131,  127,  115,  168,  163,  188, 
                     217,  230,  237,  219,  226,  223,  212,  201,  151,  151,  
                     114,  100,   87,  157,  153,  125,  127,  130,  130,  132,  
                     134,  154,  158,  160,  160,  50 };
    int x2[] = {  189,   19,   74,  112,  142,  120,  229,  194,  258,  304,  
                     365,  365,  355,  374,  312,  336,  313,  155,  225,  203,  
                     180,  180,  169,  150,  258,  232,  228,  176,  171,  203,  
                      27,   27,   26,   49,   96,   53,   96,  147,  127,  286,  
                     272,  273,  338,  356,  374,  158 };
    int y2[] = {   97,   63,   76,   84,   63,   47,   29,   16,   65,   46,  
                      68,  100,  124,  126,  121,  118,  103,  157,  152,  177,  
                     204,  216,  222,  205,  216,  210,  200,  189,  139,  138,  
                     102,   89,   74,  145,  139,  111,  113,  118,  116,  119,  
                     122,  142,  147,  148,  150,  37 };

  
    // grab corresponding points
    std::vector<CPoint> pt1, pt2;
  //  while (1)
  //  {
  //    Figure::WhichButton button;
  //    CPoint point;
  //    Bgr color = GetRandSaturatedColor();
  //
  //    // grab first click
  //    point = fig1.GrabMouseClick(&button);
  //    if (button == Figure::MC_RIGHT)  break;
  //    pt1.push_back( point );
  //    DrawDot(point, &bgr1, color, 5);
  //    fig1.Draw(bgr1);
  //
  //    // grab second click
  //    point = fig2.GrabMouseClick();
  //    pt2.push_back( point );
  //    DrawDot(point, &bgr2, color, 5);
  //    fig2.Draw(bgr2);
  //  }
    for (i=0 ; i<NPOINTS ; i++)
    {
      pt1.push_back( CPoint( x1[i], y1[i] ) );
      pt2.push_back( CPoint( x2[i], y2[i] ) );
    }

    // solve for fundamental matrix
    MatDbl fm;
    ComputeFundamentalMatrix(pt1, pt2, &fm);

    // plot epipolar lines
    const int w = img1.Width();
    const int h = img1.Height();
    assert(w == img2.Width());
    assert(h == img2.Height());
    while (1)
    {
      Figure::WhichButton button;
      CPoint p = fig1.GrabMouseClick(&button);
      if (button == Figure::MC_RIGHT)  break;

      double a, b, c;
      // draw epipolar line in second image
      ComputeEpipolarLine(p.x, p.y, fm, &a, &b, &c);
      Point2d e1, e2;
      {
        if ( fabs(a) < fabs(b) )
        {  // line is not vertical
          e1.x = 0;
          e1.y = (-a * e1.x - c ) / b;
          e2.x = w;
          e2.y = (-a * e2.x - c ) / b;
        }
        else
        {  // line is not horizontal
          e1.y = 0;
          e1.x = (-b * e1.y - c ) / a;
          e2.y = h;
          e2.x = (-b * e2.y - c ) / a;
        }
      }
      DrawLine(e1.GetPoint(), e2.GetPoint(), &bgr2, Bgr(Bgr::RED), 1);
      fig2.Draw(bgr2);

      // draw epipolar line in first image
      ComputeEpipolarLine(e1.x, e1.y, Transpose(fm), &a, &b, &c);
      {
        if ( fabs(a) < fabs(b) )
        {  // line is not vertical
          e1.x = 0;
          e1.y = (-a * e1.x - c ) / b;
          e2.x = w;
          e2.y = (-a * e2.x - c ) / b;
        }
        else
        {  // line is not horizontal
          e1.y = 0;
          e1.x = (-b * e1.y - c ) / a;
          e2.y = h;
          e2.x = (-b * e2.y - c ) / a;
        }
      }
      DrawLine(e1.GetPoint(), e2.GetPoint(), &bgr1, Bgr(Bgr::RED), 1);
      fig1.Draw(bgr1);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonLoadAvi() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("sb_short.avi", "Load AVI file");
    if (fname != "")
    {
      AVIReaderOpenCv avi;
      bool success;
      success = avi.OpenFile(fname);
      if (!success)  { CString str;  str.Format("Unable to open AVI file", fname);  AfxMessageBox(str);  return; }
      Figure fig;
      ImgBgr img;
      while (1)
      {
        success = avi.GrabNextFrame(&img);
        if (!success)  return;
        fig.Draw(img);
        Sleep(15);
    //    Figure::WhichButton button;
    //    fig.GrabMouseClick(&button);
    //    if (button == Figure::MC_RIGHT)  return;
      }	
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonChanVese() 
{
  try
  {
    CString fname = iLetUserSelectImageFile("fruitfly.jpg");
    if (fname != "")
    {
	    ImgGray img;
      Load(fname, &img);

      ChanVeseParams params;
      params.mu = 100.0f;             // weight for length
      params.nu = 0;             // weight for area
      params.lambda_i = 1.0f;    // weight for interior pixels
      params.lambda_o = 1.0f;    // weight for exterior pixels
      params.init_border = 10;   // size of initial border
      params.max_niter = 100;    // maximum number of iterations
      params.display = true;     // whether to display results during algorithm

      ImgBinary boundary;
      ChanVese(img, &boundary, params);
    }
  } 
  catch (Exception& e)
  {
    e.Display();
  }	
}

void CDemoDlg::OnButtonJpegMemoryTest() 
{
	CString fname = iLetUserSelectImageFile("hydrant.pgm", "Select test image");
  ImgBgr bgr;
  Load(fname, &bgr);
  Figure fig("Original image");
  fig.Draw(bgr);
  
  size_t buffer_size = bgr.Width() * bgr.Height() * 3;
  unsigned char* buffer = new unsigned char[buffer_size];
  size_t nbytes = SaveJpegToMemory(bgr, buffer);
  if(nbytes == 0)
  {
    AfxMessageBox("Error saving jpeg data to memory buffer");
  }
  else
  {
    ImgBgr bgr2;
    LoadJpegFromMemory(buffer, nbytes, &bgr2);
    Figure fig2("Jpeg from memory");
    fig2.Draw(bgr2);
    fig2.PlaceToTheRightOf(fig);
  }

  delete[] buffer;

  AfxMessageBox("Image was successfully JPEG-compressed, saved to memory, loaded from memory, and uncompressed.", MB_ICONINFORMATION);
}


void CDemoDlg::OnButtonOpengl() 
{
  CString fname = iLetUserSelectImageFile("kermit.ply");
  if (fname != "")
  {
    MyFigureGlut mfg("OpenGL window", 500, 500);
    PointCloud pc;
    pc.LoadCloudFromPly(fname);
    mfg.SetPoints( pc );
    
    mfg.EnterMainLoop();
  }	
}

void CDemoDlg::OnButtonPlot() 
{
  static int si = 0;
  static SimplePlot plot;
  std::vector<SimplePlot::DataPoint> data1;
  std::vector<SimplePlot::DataPoint> data2;

  
  for(int i=si; i < si+1000; i++)
  {
    SimplePlot::DataPoint d1;
    d1.x = (float) i;
    d1.y = 10 * cos(i*blepo_ex::Pif/180.0f);
    data1.push_back(d1);

    SimplePlot::DataPoint d2 = d1;
    d2.x += 20;
    d2.marker_color = Bgr(0,0,255);
    data2.push_back(d2);
  }

  plot.SetData("test1", data1, 0);
  plot.SetData("test2", data2, 0);
  plot.SetTicks(5, 6);
  plot.Render();
  si++;	
}

void CDemoDlg::OnButtonInvert() 
{
  try 
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgBgr img;
      Load(fname, &img);
      Figure fig("Image loaded");
      fig.Draw(img);
      
      ImgBgr out;
      Invert(img, &out);
      Figure fig2;
      fig2.Draw(out);
    }
    
  } 
  catch (const Exception& e)
  {
    // image failed to load, so notify user
    e.Display();
  }	
}

void CDemoDlg::OnButtonThreshold() 
{
	const unsigned char th = 100;
  try 
  {
    CString fname = iLetUserSelectImageFile("hydrant.pgm");
    if (fname != "")
    {
      ImgGray img;
      Load(fname, &img);
      Figure fig("Image loaded");
      fig.Draw(img);
      
      ImgBinary out;
      Threshold(img, th, &out);
      Figure fig2;
      fig2.Draw(out);
    }
    
  } 
  catch (const Exception& e)
  {
    // image failed to load, so notify user
    e.Display();
  }		
}
