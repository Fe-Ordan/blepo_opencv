// DemoDlg.h : header file
//

#if !defined(AFX_DEMODLG_H__A28AFB62_9D2C_4432_8ED5_6704EB140B07__INCLUDED_)
#define AFX_DEMODLG_H__A28AFB62_9D2C_4432_8ED5_6704EB140B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDemoDlg dialog

class CDemoDlg : public CDialog
{
// Construction
public:
	CDemoDlg(CWnd* pParent = NULL);	// standard constructor

  void EnableDisable();  // enable / disable windows

// Dialog Data
	//{{AFX_DATA(CDemoDlg)
	enum { IDD = IDD_DEMO_DIALOG };
	CButton	m_check_flip_image;
	CEdit	m_edit_save_fname_fmt;
	CButton	m_check_save_to_file;
	CButton	m_check_run_face_detector;
	CButton	m_button_dshow_capture;
	CListBox	m_dshow_video_devices;
	CStatic	m_static_status;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonLoadAndDisplay();
	afx_msg void OnButtonFaceDetect();
	afx_msg void OnButtonEdgeDetect();
	afx_msg void OnButtonConnectedComponents();
	afx_msg void OnButtonKltFeatureTracker();
	afx_msg void OnButtonLog();
	afx_msg void OnButtonLinearAlgebra();
	afx_msg void OnButtonSplitandmerge();
	afx_msg void OnButtonGetDshowDevices();
	afx_msg void OnButtonSnakes();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonMorphology();
	afx_msg void OnButtonShowMovie();
	afx_msg void OnButtonIeee1394();
	afx_msg void OnButtonFloodFill();
	afx_msg void OnButtonHistogram();
	afx_msg void OnButtonCrosscorrStereo();
	afx_msg void OnButtonCrosscorrTemplate();
	afx_msg void OnButtonEfrosleung();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonTest2();
	afx_msg void OnButtonMeanShiftSegmentation();
	afx_msg void OnButtonCanny();
	afx_msg void OnButtonRegionprops();
	afx_msg void OnButtonLucasKanadeAffine();
	afx_msg void OnButtonRealtimeStereo();
	afx_msg void OnButtonExtractRect();
	afx_msg void OnButtonCopyRect();
	afx_msg void OnButtonDshowUpdateDevices();
	afx_msg void OnButtonDshowCapture();
	afx_msg void OnSelchangeListDshowVideoDevices();
	afx_msg void OnButtonFft();
	afx_msg void OnButtonEllipticalHeadTracker();
	afx_msg void OnButtonCalibrate();
	afx_msg void OnButtonFastFeatureTracker();
	afx_msg void OnButtonWatershed();
	afx_msg void OnButtonWarp();
	afx_msg void OnButtonChamfer();
	afx_msg void OnButtonDelaunay();
	afx_msg void OnCheckSaveToFile();
	afx_msg void OnButtonHornSchunck();
	afx_msg void OnButtonLinefit();
	afx_msg void OnButtonGraphBasedSegmentation();
	afx_msg void OnButtonHough();
	afx_msg void OnButtonCamshift();
	afx_msg void OnButtonBlockMatchOpticalflow();
	afx_msg void OnButtonFundamentalMatrix();
	afx_msg void OnButtonLoadAvi();
	afx_msg void OnButtonChanVese();
	afx_msg void OnButtonJpegMemoryTest();
	afx_msg void OnButtonOpengl();
	afx_msg void OnButtonPlot();
	afx_msg void OnButtonInvert();
	afx_msg void OnButtonThreshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODLG_H__A28AFB62_9D2C_4432_8ED5_6704EB140B07__INCLUDED_)
