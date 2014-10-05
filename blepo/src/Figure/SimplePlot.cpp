#include "SimplePlot.h"


//=============================================================================================================//
// PlotFigureWnd
// 
//=============================================================================================================//
#ifdef USE_PLOTFIGURE 
#include <conio.h>  // kbhit
#include <algorithm>  // std::find
#include <afxdlgs.h>  // CFileDialog


#define ID_PLOTFIGURE_FILE_OPEN     8670
#define ID_PLOTFIGURE_FILE_SAVEAS   8672
#define ID_PLOTFIGURE_EDIT_COPY     8673
#define ID_PLOTFIGURE_EDIT_ORIGSIZE 8674
#define IDC_PLOTFIGURE_STATUSBAR    8675

#pragma warning(disable: 4355)  // this used in base member initializer list
using namespace blepo;

class PlotFigureWnd : public CWnd
{
public:
  PlotFigureWnd(const char* title = "PlotFigure", int x=-1, int y=-1);
  ~PlotFigureWnd();
  void Draw(const ImgBgr& img);
  void Draw(const ImgBinary& img);
  void Draw(const ImgFloat& img);
  void Draw(const ImgGray& img);
  void Draw(const ImgInt& img);
  void DrawRect(const Rect& rect, const Bgr& color, int thickness);
  void DrawDot(const Point& pt, const Bgr& color, int size);
  void DrawLine(const Point& pt1, const Point& pt2, const Bgr& color, int thickness);
  void GrabMouseClicks(int n_mouse_clicks, Array<PlotFigure::MouseClick>* points, COLORREF point_color, COLORREF line_color);
  void GrabLeftMouseClicks(Array<PlotFigure::MouseClick>* points, COLORREF point_color, COLORREF line_color);
  CRect GrabRect();
  void Link(PlotFigureWnd* other, bool symmetric);
  PlotFigure::WhichButton TestMouseClick(CPoint* pt = NULL);
  void GetDisplay(ImgBgr* out) { *out = m_img_bgr0; }
  bool IsMouseInsideWindow() { return m_mouse_is_inside_window; }
  bool IsClosed() const { return m_hWnd == NULL; }
  CPoint ImageCoordFromClient(const CPoint& pt);
  // Generated message map functions
	//{{AFX_MSG(PlotFigureWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnEditCopy();
	afx_msg void OnEditOrigSize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
  void ResizeClientArea(int width, int height);
  void Refresh(CDC& dc);
  void Refresh(bool paint_dc = false);
  void RefreshStatusPane();
  CSize GetCurrentImageSize();
  void MyGetClientRect(CRect* r);
  BOOL CopyRect2Clipboard();

private:
  static void InitializeGlobal();
  static CString g_class_name;
  static int g_left, g_top;
  static int g_num;
  static CFont g_font;
  static int g_statusbar_height;
  CStatusBarCtrl m_statusbar;
  int m_which;  
  ImgBgr m_img_bgr0;  ///< always stores the image being displayed
  ImgBinary m_img_binary1;
  ImgFloat m_img_float2;
  ImgGray m_img_gray3;
  ImgInt m_img_int4;
  CString m_title;
  bool m_equal_mode;
  bool m_display_coords_on_bottom;
  bool m_mouse_is_inside_window;
  Array<PlotFigureWnd*> m_linked_figs;  // other figures linked to this one (mouse moves affect all simultaneously)

  class MouseGrabber
  {
  public:
    enum Mode { MG_NONE, MG_GRAB_NUM_CLICKS, MG_GRAB_RECT, MG_GRAB_LEFT_CLICKS };
    MouseGrabber(PlotFigureWnd* fw) 
      : m_figwnd(fw), m_mode(MG_NONE), m_done(false), 
        m_n_mouse_clicks_to_grab(0), m_mouse_clicks(), m_last_mouse_pt(0,0), m_last_mouse_pt_image_coords(0,0),
        m_point_color(-1), m_line_color(-1) {}

    const Array<PlotFigure::MouseClick>* GetMouseClickList() const { return &m_mouse_clicks; }
    CPoint GetLastMousePoint(bool image_coords) const { return image_coords ? m_last_mouse_pt_image_coords : m_last_mouse_pt; }
    // returns true if operation has completed, false otherwise
    bool IsDone() const { return m_done; }
    COLORREF GetPointColor() const { return m_point_color; }
    COLORREF GetLineColor() const { return m_line_color; }
    void EnterGrabNumClicksMode(int nclicks, COLORREF point_color, COLORREF line_color)
    {
      m_mode = MG_GRAB_NUM_CLICKS;
      m_done = false;
      m_mouse_clicks.Reset();
      m_n_mouse_clicks_to_grab = nclicks;
      m_point_color = point_color;
      m_line_color = line_color;
    }

    void EnterGrabRectMode()
    {
      m_mode = MG_GRAB_RECT;
      m_done = false;
      m_mouse_clicks.Reset();
      m_point_color = m_line_color = -1;
    }

    void EnterGrabLeftClicksMode(COLORREF point_color, COLORREF line_color)
    {
      m_mode = MG_GRAB_LEFT_CLICKS;
      m_done = false;
      m_mouse_clicks.Reset();
      m_point_color = point_color;
      m_line_color = line_color;
    }

    void ResetMode()
    {
      m_mode = MG_NONE;
      m_done = false;
      m_mouse_clicks.Reset();
      m_point_color = m_line_color = -1;
    }

    Mode  GetMode() { return m_mode; }

    void LButtonDown(const CPoint& point)
    {
      PlotFigure::MouseClick mc( PlotFigure::MC_LEFT, m_figwnd->ImageCoordFromClient(point) );
      switch (m_mode)
      {
      case MG_GRAB_NUM_CLICKS:
        if (m_n_mouse_clicks_to_grab > 0)  
        {
          m_mouse_clicks.Push( mc );
          m_n_mouse_clicks_to_grab--;
          if (m_n_mouse_clicks_to_grab == 0)  m_done = true;
        }
        break;
      case MG_GRAB_RECT:
        if (m_mouse_clicks.Len() < 1)  m_mouse_clicks.Push( mc );
        break;
      case MG_GRAB_LEFT_CLICKS:
        m_mouse_clicks.Push( mc );
        break;
      case MG_NONE:
        if (m_mouse_clicks.Len() == 0)  m_mouse_clicks.Push( mc );  // store click in case TestMouseClick() is called
        break;
      }
    }

    void LButtonUp(const CPoint& point)
    {
      PlotFigure::MouseClick mc( PlotFigure::MC_LEFT, m_figwnd->ImageCoordFromClient(point) );
      switch (m_mode)
      {
      case MG_GRAB_NUM_CLICKS:
        break;
      case MG_GRAB_RECT:
        if (m_mouse_clicks.Len()>0)
        {
          m_mouse_clicks.Push( mc );
          m_done = true;
        }
        break;
      case MG_GRAB_LEFT_CLICKS:
        break;
      }
    }

    void RButtonDown(const CPoint& point)
    {
      PlotFigure::MouseClick mc( PlotFigure::MC_RIGHT, m_figwnd->ImageCoordFromClient(point) );
      switch (m_mode)
      {
      case MG_GRAB_NUM_CLICKS:
        if (m_n_mouse_clicks_to_grab > 0)  
        {
          m_mouse_clicks.Push( mc );
          m_n_mouse_clicks_to_grab--;
          if (m_n_mouse_clicks_to_grab == 0)  m_done = true;
        }
        break;
      case MG_GRAB_RECT:
        break;
      case MG_GRAB_LEFT_CLICKS:
        m_done = true;
        break;
      case MG_NONE:
        if (m_mouse_clicks.Len() == 0)  m_mouse_clicks.Push( mc );  // store click in case TestMouseClick() is called
        break;
      }
    }

    void MouseMove(const CPoint& point)
    {
      m_last_mouse_pt = point;
      m_last_mouse_pt_image_coords = m_figwnd->ImageCoordFromClient(point);  // convert from client to image coordinates
      float u = m_last_mouse_pt_image_coords.x - m_margin_left;
      float v = m_last_mouse_pt_image_coords.y - m_margin_top;
      float x_range = m_max_x - m_min_x;
      float y_range = m_max_y - m_min_y;      
      m_last_x = x_range * u / m_canvas_width + m_min_x; 
      m_last_y = (1.0f - (v / m_canvas_height)) * y_range + m_min_y;
    }
    
    //----------------------------------------------//
    public:
    float m_margin_left, m_margin_top, m_min_x, m_max_x, m_min_y, m_max_y, m_canvas_width, m_canvas_height;
    float m_last_x;
    float m_last_y;
    //----------------------------------------------//

  private:
    Mode m_mode;
    int m_n_mouse_clicks_to_grab;
    Array<PlotFigure::MouseClick> m_mouse_clicks;
    CPoint m_last_mouse_pt, m_last_mouse_pt_image_coords;
    PlotFigureWnd* m_figwnd;
    bool m_done;
    COLORREF m_point_color, m_line_color;  // -1 means do not draw
  };

public:
  MouseGrabber m_mouse_grabber;
};

#endif //#ifdef USE_PLOTFIGURE
//----------------------------------------------//




//=============================================================================================================//
// SimplePlot
//
//=============================================================================================================//
#define MARGIN_LEFT   120
#define MARGIN_TOP    40
#define MARGIN_RIGHT  40
#define MARGIN_BOTTOM 80
//--------------------------------------------------------------------------------------//
SimplePlot::SimplePlot(std::string title, int canvas_width, int canvas_height):m_texter(8, 1)
{
#ifdef USE_PLOTFIGURE
  m_fig = new PlotFigure();
#else
  m_fig = new blepo::Figure();
#endif

  m_fig->SetTitle(title.c_str());
  m_auto_axes = true;
  m_canvas_margin = blepo::Rect(MARGIN_LEFT, MARGIN_TOP, MARGIN_RIGHT, MARGIN_BOTTOM);
  
  SetCanvasSize(canvas_width, canvas_height);
#ifdef USE_PLOTFIGURE
  m_fig->m_wnd->m_mouse_grabber.m_margin_left = MARGIN_LEFT;
  m_fig->m_wnd->m_mouse_grabber.m_margin_top = MARGIN_TOP;
  m_fig->m_wnd->m_mouse_grabber.m_canvas_width = (float)canvas_width;
  m_fig->m_wnd->m_mouse_grabber.m_canvas_height = (float)canvas_height;
  m_fig->m_wnd->m_mouse_grabber.m_min_x = 0;
  m_fig->m_wnd->m_mouse_grabber.m_max_x = 0;
  m_fig->m_wnd->m_mouse_grabber.m_min_y = 0;
  m_fig->m_wnd->m_mouse_grabber.m_max_y = 0;
#endif
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetCanvasSize(int w, int h)
{
  m_canvas.Reset(w, h);
  blepo::Set(&m_canvas, blepo::Bgr(255,255,255));
  m_bgr.Reset(w + m_canvas_margin.left + m_canvas_margin.right , h + m_canvas_margin.top + m_canvas_margin.bottom);
  blepo::Set(&m_bgr, blepo::Bgr(255,255,255));

#ifdef USE_PLOTFIGURE
  m_fig->m_wnd->m_mouse_grabber.m_canvas_width = (float)w;
  m_fig->m_wnd->m_mouse_grabber.m_canvas_height = (float)h;
#endif
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetAxes(float x_low, float x_high, float y_low, float y_high)
{
  m_auto_axes = false;
  m_x_min = x_low;
  m_x_max = x_high;
  m_y_min = y_low;
  m_y_max = y_high;
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
#ifdef USE_PLOTFIGURE
  m_fig->m_wnd->m_mouse_grabber.m_min_x = m_x_min;
  m_fig->m_wnd->m_mouse_grabber.m_max_x = m_x_max;
  m_fig->m_wnd->m_mouse_grabber.m_min_y = m_y_min;
  m_fig->m_wnd->m_mouse_grabber.m_max_y = m_y_max;
#endif
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::EnableAutoAxes()
{
  m_auto_axes = true;
  CalculateAxes();
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetWindowTitle(std::string title)
{
  m_fig->SetTitle(title.c_str());
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetData(std::string label, const std::vector<DataPoint>& pts, int line_thickness, blepo::ImgBgr::Pixel line_color)
{
  DataSet data;
  data.line_thickness = line_thickness;
  data.line_color = line_color;
  data.data_pts = pts;
  m_data[label] = data;
  if(m_auto_axes)
  {
    CalculateAxes();
  }
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::CalculateAxes()
{
  m_x_min = m_y_min = (float)HUGE_VAL;
  m_x_max = m_y_max = (float)-HUGE_VAL;

  std::map<std::string, DataSet>::iterator iter = m_data.begin();
  std::map<std::string, DataSet>::iterator iter_end = m_data.end();
  while(iter != iter_end)
  {
    const DataSet& ds = iter->second;     
    //Draw lines -----------------------------------//
    int n = ds.data_pts.size();    
    for(int i=0; i < n; i++)
    {
      m_x_min = (ds.data_pts[i].x < m_x_min) ? ds.data_pts[i].x : m_x_min;
      m_y_min = (ds.data_pts[i].y < m_y_min) ? ds.data_pts[i].y : m_y_min;
      m_x_max = (ds.data_pts[i].x > m_x_max) ? ds.data_pts[i].x : m_x_max;
      m_y_max = (ds.data_pts[i].y > m_y_max) ? ds.data_pts[i].y : m_y_max;
    }
    iter++;
  }
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);

#ifdef USE_PLOTFIGURE
  m_fig->m_wnd->m_mouse_grabber.m_min_x = m_x_min;
  m_fig->m_wnd->m_mouse_grabber.m_max_x = m_x_max;
  m_fig->m_wnd->m_mouse_grabber.m_min_y = m_y_min;
  m_fig->m_wnd->m_mouse_grabber.m_max_y = m_y_max;
#endif
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetTicks(const std::vector<AxisTick>& xticks, const std::vector<AxisTick>& yticks)
{
  m_x_ticks = xticks;
  m_y_ticks = yticks;
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
  //Render();
}
void SimplePlot::SetXTicks(const std::vector<AxisTick>& ticks)
{
  m_x_ticks = ticks;
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
  //Render();
}
void SimplePlot::SetYTicks(const std::vector<AxisTick>& ticks)
{
  m_y_ticks = ticks;
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetTicks(int nx, int ny)
{
  ComputeTicks(m_x_min, m_x_max, nx, &m_x_ticks);
  ComputeTicks(m_y_min, m_y_max, ny, &m_y_ticks);
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
  //Render();
}
void SimplePlot::SetXTicks(int n)
{
  ComputeTicks(m_x_min, m_x_max, n, &m_x_ticks);
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::SetYTicks(int n)
{
  ComputeTicks(m_y_min, m_y_max, n, &m_y_ticks);
  RenderAxesLabels(m_canvas.Width(), m_canvas.Height(), &m_bgr);
  //Render();
}
//--------------------------------------------------------------------------------------//
void SimplePlot::ComputeTicks(float min_val, float max_val, int arg_n, std::vector<AxisTick>* ticks)
{
  char buffer[128];
  ticks->clear();
  int n = (arg_n < 2) ? 2 : arg_n;
  float delta = (max_val - min_val) / (n-1);
  AxisTick t;
  t.tick_val = min_val;
  for(int i=0; i < n-1; i++)
  {
    sprintf(buffer, "%g", t.tick_val);
    t.tick_str = std::string(buffer);
    ticks->push_back(t);
    t.tick_val += delta;
  }
  t.tick_val = max_val;
  sprintf(buffer, "%g", t.tick_val);
  t.tick_str = std::string(buffer);
  ticks->push_back(t);
}
//--------------------------------------------------------------------------------------//
void SimplePlot::Render()
{
  RenderCanvas(&m_canvas);
  blepo::Set(&m_bgr, blepo::Point(m_canvas_margin.left, m_canvas_margin.top), m_canvas);
  m_fig->Draw(m_bgr);
}
//--------------------------------------------------------------------------------------//
void SimplePlot::RenderAxesLabels(int canvas_width, int canvas_height, blepo::ImgBgr* argimg)
{
  int i;
  blepo::ImgBgr& bgr = *argimg;
  bgr.Reset(canvas_width + m_canvas_margin.left + m_canvas_margin.right, canvas_height + m_canvas_margin.top + m_canvas_margin.bottom);
  blepo::Set(&bgr, blepo::ImgBgr::Pixel(196,196,196));
  
  int half_font_size = 4;
  float cw = (float)canvas_width;
  float ch = (float)canvas_height;
  float x_range = fabs(m_x_max - m_x_min);
  float y_range = fabs(m_y_max - m_y_min);
  int v1 = m_canvas_margin.top + canvas_height + 5;
  int v2 = m_canvas_margin.top + canvas_height + 20;
  for(i=0; i < (int)m_x_ticks.size(); i++)
  {
    AxisTick& t = m_x_ticks[i];
    int u = (int) (cw * (t.tick_val - m_x_min) / x_range) + m_canvas_margin.left;
    if( (u >= m_canvas_margin.left) && (u <= (m_canvas_margin.left + canvas_width)))
    {
      m_texter.DrawTextA(&m_bgr, "|", blepo::Point(u, v1), blepo::ImgBgr::Pixel(0,0,0));
      m_texter.DrawTextA(&m_bgr, t.tick_str.c_str(), blepo::Point(u, v2), blepo::ImgBgr::Pixel(0,0,0));
    }
  }
  int u = 5;
  for(i=0; i < (int)m_y_ticks.size(); i++)
  {
    AxisTick& t = m_y_ticks[i];
    int v = (int) (ch * (1.0f - ((t.tick_val - m_y_min) / y_range))) + m_canvas_margin.top - 4; //fontsize/2
    if( ((v + half_font_size) >= m_canvas_margin.top) && ((v + half_font_size) <= (m_canvas_margin.top + canvas_height)))
    {
      m_texter.DrawTextA(&m_bgr, t.tick_str.c_str(), blepo::Point(u, v), blepo::ImgBgr::Pixel(0,0,0));
    }
  }
  
  blepo::DrawRect(blepo::Rect(m_canvas_margin.left-1, 
                              m_canvas_margin.top-1, 
                              m_canvas_margin.left + m_canvas.Width(), 
                              m_canvas_margin.top + m_canvas.Height()), 
                              &bgr, blepo::ImgBgr::Pixel(0,0,0), 2);
}
//--------------------------------------------------------------------------------------//
void SimplePlot::RenderCanvas(blepo::ImgBgr* argimg)
{
  blepo::ImgBgr& canvas = *argimg;
  if(canvas.IsNull())
    return;


  blepo::Set(&canvas, blepo::Bgr(255,255,255));
  std::map<std::string, DataSet>::iterator iter = m_data.begin();
  std::map<std::string, DataSet>::iterator iter_end = m_data.end();

  float cw = (float)canvas.Width();
  float ch = (float)canvas.Height();
  float x_range = fabs(m_x_max - m_x_min);
  float y_range = fabs(m_y_max - m_y_min);
  while(iter != iter_end)
  {
    const DataSet& ds = iter->second;
    int n = ds.data_pts.size();    
    int thickness = (ds.line_thickness > 10) ? 10 : ds.line_thickness;
    //Draw lines -----------------------------------//
    if(thickness > 0)
    {
      for(int i=0; i < n-1; i++)
      {
        int u1 = (int) (cw * (ds.data_pts[i].x - m_x_min) / x_range);
        int v1 = (int) (ch * (1.0f - ((ds.data_pts[i].y - m_y_min) / y_range)));
        int u2 = (int) (cw * (ds.data_pts[i+1].x - m_x_min) / x_range);
        int v2 = (int) (ch * (1.0f - ((ds.data_pts[i+1].y - m_y_min) / y_range)));
        blepo::DrawLine(blepo::Point(u1, v1), blepo::Point(u2, v2), &canvas, ds.line_color, ds.line_thickness);
      }    
    }
    //----------------------------------------------//

    //Draw markers----------------------------------//
    for(int i=0; i < n; i++)
    {
      int u = (int) (cw * (ds.data_pts[i].x - m_x_min) / x_range);
      int v = (int) (ch * (1.0f - ((ds.data_pts[i].y - m_y_min) / y_range)));
      if((ds.data_pts[i].marker_size > 0) && (ds.data_pts[i].marker_size <= 10))
      {
        blepo::DrawDot(blepo::Point(u, v), &canvas, ds.data_pts[i].marker_color, ds.data_pts[i].marker_size, true);
      }
    }    
    //----------------------------------------------//
    iter++;
  }  
}
//--------------------------------------------------------------------------------------//

//=============================================================================================================//
// PlotFigureWnd and PlotFigure
//
//=============================================================================================================//
#ifdef USE_PLOTFIGURE
//--------------------------------------------------------------------------------------//
void iDrawLine(CDC& dc, const Point& pt1, const Point& pt2, const Bgr& color, int thickness)
{
  CPen pen(PS_SOLID, thickness, color.ToInt(Bgr::BLEPO_BGR_XBGR));
  CPen* old = dc.SelectObject(&pen);
  dc.MoveTo(pt1);
  dc.LineTo(pt2);
  dc.SelectObject(old);
}

void iDrawDot(CDC& dc, const Point& pt, const Bgr& color, int size)
{
  int r = size / 2;
  dc.FillSolidRect(pt.x-r, pt.y-r, size, size, color.ToInt(Bgr::BLEPO_BGR_XBGR));
}

void iDrawCrosshairs(CDC& dc, const CPoint& pt, int w, int h)
{
  iDrawLine(dc, Point(0, pt.y), Point(w, pt.y), Bgr(0,0,0), 1);
  iDrawLine(dc, Point(pt.x, 0), Point(pt.x, h), Bgr(0,0,0), 1);
}

static int iWnd2Img(int win_x, int win_width, int img_width)
{
  for (int i=img_width-1 ; i>=0 ; i--)
  {
    int thresh = blepo_ex::Round((double) i * win_width / img_width);
    if (win_x >= thresh)  return i;
  }
  return 0;
}

Array<HWND>  g_hwnd_list;

bool iProcessWindowsMessage()
{
  MSG msg;
  if (PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
  { 
    PeekMessage(&msg,NULL,0,0,PM_REMOVE);

    if (msg.message == WM_QUIT)
      return false;
    if (msg.message == WM_DESTROY)
      return false;
    if (msg.message == WM_CLOSE)
      return false;

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  } // end if

  // exit loop if key is hit in console window
  if (_kbhit())  return false;
  return true;
}

void PlotEventLoop()
{
  while (iProcessWindowsMessage());
}

//////////////////////////////////////////////////////////////////
// FigureWnd message map

BEGIN_MESSAGE_MAP(PlotFigureWnd, CWnd)
	//{{AFX_MSG_MAP(FigureWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_PLOTFIGURE_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_PLOTFIGURE_FILE_SAVEAS, OnFileSaveAs)
	ON_COMMAND(ID_PLOTFIGURE_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_PLOTFIGURE_EDIT_ORIGSIZE, OnEditOrigSize)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////
// FigureWnd

CString PlotFigureWnd::g_class_name;
int PlotFigureWnd::g_left = 10;
int PlotFigureWnd::g_top = 30;
int PlotFigureWnd::g_num = 0;
int PlotFigureWnd::g_statusbar_height = 20;
CFont PlotFigureWnd::g_font;

//////////////////////////////////////////////////////////////////
// FigureWnd methods

void PlotFigureWnd::InitializeGlobal()
{
  if (g_class_name.GetLength()==0)
  {
    g_class_name = AfxRegisterWndClass(
            CS_HREDRAW | CS_VREDRAW | CS_OWNDC,
            ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)),
            (HBRUSH) GetStockObject(BLACK_BRUSH), 
            0);
    g_font.CreatePointFont(100, "Courier New");
  }
}

int PlotFigureWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

  { // Add main menu
    CMenu menu, file_popup, edit_popup;
    BOOL ret;

    // create main menu
    ret = menu.CreateMenu();  assert(ret);
    
    // create and add file menu
    ret = file_popup.CreatePopupMenu();  assert(ret);
    file_popup.AppendMenu(MF_STRING, ID_PLOTFIGURE_FILE_OPEN, "&Open...");
    file_popup.AppendMenu(MF_STRING, ID_PLOTFIGURE_FILE_SAVEAS, "&Save As...");
    menu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR) file_popup.m_hMenu, "&File");

    // create and add edit menu
    ret = edit_popup.CreatePopupMenu();  assert(ret);
    edit_popup.AppendMenu(MF_STRING, ID_PLOTFIGURE_EDIT_COPY, "&Copy");
    edit_popup.AppendMenu(MF_STRING, ID_PLOTFIGURE_EDIT_ORIGSIZE, "&Original Size");
    menu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR) edit_popup.m_hMenu, "&Edit");

    ret = SetMenu(&menu);  assert(ret);

    {
      HMENU retm;
      retm = menu.Detach();  assert(retm); // so the CMenu destructor won't destroy the menu
      retm = file_popup.Detach();  assert(retm);
      retm = edit_popup.Detach();  assert(retm);
    }
  }
  
  // status bar control
  int m_Widths[4];
	int rets = m_statusbar.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP, 
                CRect(0,0,0,0), this, IDC_PLOTFIGURE_STATUSBAR);

  // status bar panes
	m_Widths[0] = 90; // x
 	m_Widths[1] = 200; // y
	m_statusbar.SetParts( 2, m_Widths);
  {
    CRect r;
    m_statusbar.GetWindowRect(&r);
    g_statusbar_height = r.Height();
  }
	return 0;
}

void PlotFigureWnd::MyGetClientRect(CRect* r)
{
  GetClientRect(r);
  r->bottom -= g_statusbar_height;
}

void PlotFigureWnd::OnFileOpen()
{
  try 
  {
    CFileDialog dlg(TRUE, // open file dialog
                    NULL, // default extension
                    NULL,  // default filename
                    OFN_HIDEREADONLY,  // flags
                    "All image files|*.pgm;*ppm;*.bmp;*.jpg;*.jpeg|PGM/PPM files (*.pgm)|*.pgm;*.ppm|BMP files (*.bmp)|*.bmp|JPEG files (*.jpg,.jpeg)|*.jpg;*.jpeg|All files (*.*)|*.*||",  // filter
                    NULL);
	  dlg.m_ofn.lpstrTitle = "Load image";
    if (dlg.DoModal() == IDOK)
    {
      CString fname = dlg.GetPathName();
      ImgBgr img;
      Load(fname, &img);
      m_which = 0;
      ResizeClientArea(img.Width(), img.Height());
      Draw(img);
    }

  } catch (const Exception& e)
  {
    // image failed to load, so notify user
    e.Display();
  }
}

// from "Capture Screen to Clipboard including dropdown menu" by Y. Huang, http://www.codeproject.com/KB/clipboard/hscr2clp.aspx
BOOL PlotFigureWnd::CopyRect2Clipboard()
{
  // Get entire client rect
  CRect rect;
  GetClientRect(rect);
  rect.bottom -= g_statusbar_height;  // subtract status bar at bottom
  rect.NormalizeRect();
  if (rect.IsRectEmpty() || rect.IsRectNull())
    return FALSE;
  
  // Copy bits to a CBitmap
  CClientDC dcScrn(this);
  CDC memDc;
  if (!memDc.CreateCompatibleDC(&dcScrn))
    return FALSE;
  CBitmap bitmap;
  if (!bitmap.CreateCompatibleBitmap(&dcScrn,rect.Width(), rect.Height()))
    return FALSE;
  CBitmap* pOldBitmap = memDc.SelectObject(&bitmap);
  memDc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcScrn,rect.left, rect.top, SRCCOPY);
  
  // Put bitmap on clipboard
  if (OpenClipboard())
  {
    EmptyClipboard();
    SetClipboardData(CF_BITMAP,bitmap. GetSafeHandle());
    CloseClipboard();
  }
  memDc.SelectObject(pOldBitmap);
  return TRUE;
}

void PlotFigureWnd::OnFileSaveAs()
{
  try
  {
    CFileDialog dlg(FALSE, // save file dialog
                    "eps", // default extension
                    "blepo_image",  // default filename
                    OFN_HIDEREADONLY,  // flags
                    "EPS file (*.eps)|*.eps|PPM file (*.ppm)|*.ppm|PGM file (*.pgm)|*.pgm|BMP files (*.bmp)|*.bmp|JPEG files (*.jpg,.jpeg)|*.jpg;*.jpeg|All files (*.*)|*.*||",  // filter
                    NULL);
    dlg.m_ofn.lpstrTitle = "Save figure";
    if (dlg.DoModal() == IDOK)
    {
      CString fname = dlg.GetPathName();
      // check whether file already exists
      CFileStatus stat;
      if (CFile::GetStatus(fname, stat) == TRUE)
      {
        CString str;
        str.Format("File '%s' already exists.  Overwrite?", (const char*) fname);
        int ret = AfxMessageBox(str, MB_YESNO);
        if (ret == IDNO)  return;
      }
      // get image from display and save to file
      ImgBgr img;
      GetDisplay(&img);
      Save(img, fname);
    }
  } catch (Exception& e)
  {
    e.Display();
  }
}


LRESULT PlotFigureWnd::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
  m_mouse_is_inside_window = false;
  Refresh();
  
	return 0;
}

void PlotFigureWnd::OnEditCopy()
{
  CopyRect2Clipboard();
}

void PlotFigureWnd::OnEditOrigSize()
{
  const ImgBgr& img = m_img_bgr0;
  if (img.IsNull())  return;
  ResizeClientArea(img.Width(), img.Height());
  Refresh();
}

void PlotFigureWnd::OnClose()
{
  CWnd::OnClose();
}

void PlotFigureWnd::Refresh(bool paint_dc)
{
  if (m_hWnd == NULL)  return;
  if (paint_dc)
  {
    CPaintDC dc(this);
    Refresh(dc);
  }
  else
  {
    CClientDC dc(this);
    Refresh(dc);
  }
}

void PlotFigureWnd::Refresh(CDC& dc)
{
  CRect wrect;
  MyGetClientRect(&wrect);
  int w = wrect.Width();
  int h = wrect.Height();
  if (m_which >= 0)
  {
    if (m_equal_mode)
    {
      ImgBinary tmp;
      CPoint pt = m_mouse_grabber.GetLastMousePoint(true);  // m_last_mouse_pt_image_coords;  // old: ImageCoordFromClient(m_last_mouse_pt);
      switch(m_which)
      {
      case 0:  Equal(m_img_bgr0,    m_img_bgr0   (pt.x, pt.y), &tmp);  break;
      case 1:  Equal(m_img_binary1, m_img_binary1(pt.x, pt.y), &tmp);  break;
      case 2:  Equal(m_img_float2,  m_img_float2 (pt.x, pt.y), &tmp);  break;
      case 3:  Equal(m_img_gray3,   m_img_gray3  (pt.x, pt.y), &tmp);  break;
      case 4:  Equal(m_img_int4,    m_img_int4   (pt.x, pt.y), &tmp);  break;
      default:  assert(0);
      }
      blepo::Draw(tmp, dc, Rect(0, 0, m_img_bgr0.Width(), m_img_bgr0.Height()), Rect(0, 0, w, h));
    }
    else
    {
      blepo::Draw(m_img_bgr0, dc, Rect(0, 0, m_img_bgr0.Width(), m_img_bgr0.Height()), Rect(0, 0, w, h));
    }
  }  

  const CPoint& pt = m_mouse_grabber.GetLastMousePoint(false);
  iDrawCrosshairs(dc, pt, w, h);
  switch (m_mouse_grabber.GetMode())
  {
  case MouseGrabber::MG_GRAB_NUM_CLICKS:
  case MouseGrabber::MG_GRAB_LEFT_CLICKS:
    {
      const CPoint& pt = m_mouse_grabber.GetLastMousePoint(false);
      iDrawCrosshairs(dc, pt, w, h);
      COLORREF pcol = m_mouse_grabber.GetPointColor();
      COLORREF lcol = m_mouse_grabber.GetLineColor();
      if (lcol <= 0x00FFFFFF)
      {
        const Array<PlotFigure::MouseClick>* mcl = m_mouse_grabber.GetMouseClickList();
        const int n = mcl->Len();
        int i;
        for (i=0 ; i<n-1 ; i++)
        {
          const Point& p1 = (*mcl)[i].pt;
          const Point& p2 = (*mcl)[i+1].pt;
          iDrawLine(dc, p1, p2, Bgr(lcol, Bgr::BLEPO_BGR_XBGR), 1);
        }
        if (i<n)  iDrawLine(dc, (*mcl)[i].pt, pt, Bgr(lcol, Bgr::BLEPO_BGR_XBGR), 1);
      }
      if (pcol <= 0x00FFFFFF)
      {
        const Array<PlotFigure::MouseClick>* mcl = m_mouse_grabber.GetMouseClickList();
        const int n = mcl->Len();
        for (int i=0 ; i<n ; i++)
        {
          const Point& p = (*mcl)[i].pt;
          iDrawDot(dc, p, Bgr(pcol, Bgr::BLEPO_BGR_XBGR), 3);
        }
      }
      break;
    }
  case MouseGrabber::MG_GRAB_RECT:  
    const Array<PlotFigure::MouseClick>* mcl = m_mouse_grabber.GetMouseClickList();
    Point lmp = m_mouse_grabber.GetLastMousePoint(false);
    if (mcl->Len()>0)
    {
      const Point& pt = (*mcl)[0].pt;
      Rect rect(pt.x, pt.y, lmp.x, lmp.y);
      CBrush brush;
      brush.CreateSolidBrush(RGB(255,0,0));
      dc.FrameRect(rect, &brush);
    }
    else 
    {
      iDrawCrosshairs(dc, lmp, w, h);
    }
    
    break;
  }
  RefreshStatusPane();
  iProcessWindowsMessage();
}

void PlotFigureWnd::RefreshStatusPane()
{
  CString msg0, msg1, msg2;
  CPoint pt = m_mouse_grabber.GetLastMousePoint(true); 
  int x = pt.x, y = pt.y;

  bool showme = m_mouse_is_inside_window;
  for (int i=0 ; i<m_linked_figs.Len(); i++)
  {
    showme = showme || m_linked_figs[i]->IsMouseInsideWindow();
  }

  // display text in status window
  if (showme)  msg0.Format("x: %g", m_mouse_grabber.m_last_x);
  if (showme)  msg1.Format("y: %g", m_mouse_grabber.m_last_y);
  
  showme = showme && x>=0 && y>=0 && x<m_img_bgr0.Width() && y<m_img_bgr0.Height(); 
  m_statusbar.SetText(msg0, 0, 0);
  m_statusbar.SetText(msg1, 1, 0);
}

void PlotFigureWnd::OnPaint() 
{
	if (IsIconic())
	{
  }
  else
  {
    Refresh(true);
  }
}

CPoint PlotFigureWnd::ImageCoordFromClient(const CPoint& pt)
{
  CRect wrect;
  MyGetClientRect(&wrect);
  CSize image_sz = GetCurrentImageSize();
  int image_x = iWnd2Img(pt.x, wrect.Width(), image_sz.cx);
  int image_y = iWnd2Img(pt.y, wrect.Height(), image_sz.cy);
  return CPoint(image_x, image_y);
}

#define ARTIFICIAL_MOUSE_MOVE_SENT_FROM_OTHER_WINDOW 0xF8F8  // number doesn't matter, so long as it does not conflict with genuine flags

void PlotFigureWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
  if (nFlags == ARTIFICIAL_MOUSE_MOVE_SENT_FROM_OTHER_WINDOW)
  {
    nFlags = 0;
  }
  else 
  {
    for (int i=0 ; i<m_linked_figs.Len() ; i++)
    {
      m_linked_figs[i]->SendMessage(WM_MOUSEMOVE, ARTIFICIAL_MOUSE_MOVE_SENT_FROM_OTHER_WINDOW, (point.y << 16) | point.x);
    }
  }

  if (!m_mouse_is_inside_window)
  {
    // This block of code causes the OnMouseLeave() function to 
    // be called when the mouse leaves the window.
    m_mouse_is_inside_window = true;
  	TRACKMOUSEEVENT track;     // Declares structure
    track.cbSize = sizeof(track);
    track.dwFlags = TME_LEAVE; // Notify us when the mouse leaves
    track.hwndTrack = m_hWnd;  // Assigns this window's hwnd
    TrackMouseEvent(&track);   // Tracks the events like WM_MOUSELEAVE
  }

  // avoid invalid pixel accessing of an empty image
  if (GetCurrentImageSize() == CSize(0,0))  
    return;
  m_mouse_grabber.MouseMove(point);  
  Refresh();
  MouseGrabber::Mode m = m_mouse_grabber.GetMode();
  if (m != MouseGrabber::MG_NONE)
  {
    SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
    Refresh();
  }
  else if (m_equal_mode)
  {
    Refresh();
  }
  RefreshStatusPane();
  CWnd::OnMouseMove(nFlags, point);  // necessary?
}

void PlotFigureWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
  m_mouse_grabber.LButtonDown( point );
	CWnd::OnLButtonDown(nFlags, point);
}

void PlotFigureWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
  m_mouse_grabber.RButtonDown( point );
	CWnd::OnRButtonDown(nFlags, point);
}

void PlotFigureWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
  m_mouse_grabber.LButtonUp( point );
	CWnd::OnLButtonUp(nFlags, point);
}

void PlotFigureWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if (nChar == 'E')  
  {
    m_equal_mode = !m_equal_mode;	
    Refresh();
  }
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void PlotFigureWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void PlotFigureWnd::OnSize(UINT nType, int cx, int cy) 
{
  if (m_statusbar.m_hWnd)
  {
    m_statusbar.SendMessage(WM_SIZE, cx, cy);
    {
      CRect r;
      m_statusbar.GetWindowRect(&r);
      g_statusbar_height = r.Height();
    }
  }
	CWnd::OnSize(nType, cx, cy);
}

PlotFigureWnd::PlotFigureWnd(const char* title, int x, int y) 
  : CWnd(), m_which(-1), m_title(title), //m_n_mouse_clicks_to_grab(0), m_grab_rect(false),
    m_equal_mode(false), m_display_coords_on_bottom(false),
    m_mouse_is_inside_window(false),
    m_mouse_grabber(this)
{
  CString title_tmp;
  if (title == NULL)
  {
    title_tmp.Format("Figure %d", g_num);
    title = (const char*) title_tmp;
  }
  InitializeGlobal();
  if((x < 0) || (y < 0))
  {
    g_num++;
  }
  
  int num = (g_num-1);
  int window_set = (num / 8);
  num = num - window_set * 8;

  if(x < 0)
  {
    int col = num % 4;
    x =  col * 340 + 5 * (window_set + 1);
  }

  if(y < 0)
  {
    int row = num / 4;
    y =  row * 310 + 30;
  }

  CreateEx(0, g_class_name, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CRect(x, y, x+200, y+200), NULL, 0);
  g_hwnd_list.Push(m_hWnd); 
}

PlotFigureWnd::~PlotFigureWnd()
{
  HWND* p = std::find(g_hwnd_list.Begin(), g_hwnd_list.End(), m_hWnd);
  if (p != g_hwnd_list.End())
  {
    g_hwnd_list.Delete( p - g_hwnd_list.Begin() );  // old:  erase(p)
  }
}

CSize PlotFigureWnd::GetCurrentImageSize()
{
  return (m_which>=0) ? CSize(m_img_bgr0.Width(), m_img_bgr0.Height()) : CSize(0,0);
}

void PlotFigureWnd::Draw(const ImgBgr& img)
{
  if (img.IsNull())  return;
  CSize sz = GetCurrentImageSize();
  if (img.Width() != sz.cx || img.Height() != sz.cy)  ResizeClientArea(img.Width(), img.Height());
  m_img_bgr0 = img;
  m_which = 0;
//  m_statusbar.SetText("Bgr", 2, 0);
  Refresh();
}

void PlotFigureWnd::Draw(const ImgBinary& img)
{
  if (img.IsNull())  return;
  CSize sz = GetCurrentImageSize();
  if (img.Width() != sz.cx || img.Height() != sz.cy)  ResizeClientArea(img.Width(), img.Height());
  m_img_binary1 = img;
  m_which = 1;
  {
    ImgGray gray;
    Convert(img, &gray, 0, 255);
    Convert(gray, &m_img_bgr0);
  }
//  m_statusbar.SetText("Binary", 2, 0);
  Refresh();
}

void PlotFigureWnd::Draw(const ImgFloat& img)
{
  if (img.IsNull())  return;
  CSize sz = GetCurrentImageSize();
  if (img.Width() != sz.cx || img.Height() != sz.cy)  ResizeClientArea(img.Width(), img.Height());
  m_img_float2 = img;
  m_which = 2;
  { // could be made faster by reducing this 4-pass version to 2-pass
    ImgFloat img2;
    ImgGray gray;
    LinearlyScale(img, 0, 255, &img2);
    Convert(img2, &gray);  
    Convert(gray, &m_img_bgr0);
  }
  m_statusbar.SetText("Float", 2, 0);
  Refresh();
}

void PlotFigureWnd::Draw(const ImgGray& img)
{
  if (img.IsNull())  return;
  CSize sz = GetCurrentImageSize();
  if (img.Width() != sz.cx || img.Height() != sz.cy)  ResizeClientArea(img.Width(), img.Height());
  m_img_gray3 = img;
  m_which = 3;
  Convert(img, &m_img_bgr0);
//  m_statusbar.SetText("Gray", 2, 0);
  Refresh();
}

void PlotFigureWnd::Draw(const ImgInt& img)
{
  if (img.IsNull())  return;
  CSize sz = GetCurrentImageSize();
  if (img.Width() != sz.cx || img.Height() != sz.cy)  ResizeClientArea(img.Width(), img.Height());
  m_img_int4 = img;
  m_which = 4;
  { // could be made faster by reducing this 4-pass version to 2-pass
    ImgInt img2;
    ImgGray gray;
    LinearlyScale(img, 0, 255, &img2);
    Convert(img2, &gray);
    Convert(gray, &m_img_bgr0);
  }
//  m_statusbar.SetText("Int", 2, 0);
  Refresh();
}

void PlotFigureWnd::DrawRect(const Rect& rect, const Bgr& color, int thickness)
{
  blepo::DrawRect(rect, &m_img_bgr0, color, thickness);
  Refresh();
}

void PlotFigureWnd::DrawDot(const Point& pt, const Bgr& color, int size)
{
  blepo::DrawDot(pt, &m_img_bgr0, color, size);
  Refresh();
}

void PlotFigureWnd::DrawLine(const Point& pt1, const Point& pt2, const Bgr& color, int thickness=1)
{
  blepo::DrawLine(pt1, pt2, &m_img_bgr0, color, thickness);
  Refresh();
}

void PlotFigureWnd::ResizeClientArea(int width, int height)
{
  // resize window to fit image exactly
  CRect crect, wrect;
  GetWindowRect(&wrect);
  MyGetClientRect(&crect);
  int new_width = width + wrect.Width() - crect.Width();
  int new_height = height + wrect.Height() - crect.Height();
  if (new_width != wrect.Width() || new_height != wrect.Height())
  {
    SetWindowPos(NULL, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);
  }
}

void PlotFigureWnd::GrabMouseClicks(int n_mouse_clicks, Array<PlotFigure::MouseClick>* points, COLORREF point_color, COLORREF line_color)
{
  m_mouse_grabber.EnterGrabNumClicksMode( n_mouse_clicks, point_color, line_color );
  while (!m_mouse_grabber.IsDone() && iProcessWindowsMessage());
  *points = *m_mouse_grabber.GetMouseClickList();
  m_mouse_grabber.ResetMode();
  Refresh();
}

void PlotFigureWnd::GrabLeftMouseClicks(Array<PlotFigure::MouseClick>* points, COLORREF point_color, COLORREF line_color)
{
  m_mouse_grabber.EnterGrabLeftClicksMode(point_color, line_color);
  while (!m_mouse_grabber.IsDone() && iProcessWindowsMessage());
  *points = *m_mouse_grabber.GetMouseClickList();
  m_mouse_grabber.ResetMode();
  Refresh();
}

Rect PlotFigureWnd::GrabRect()
{
  m_mouse_grabber.EnterGrabRectMode();
  while (!m_mouse_grabber.IsDone() && iProcessWindowsMessage());
  const Array<PlotFigure::MouseClick>& mcl = *m_mouse_grabber.GetMouseClickList();
  assert(mcl.Len() == 2);
  Refresh();
  const Point& p1 = mcl[0].pt;
  const Point& p2 = mcl[1].pt;
  int left   = p1.x < p2.x ? p1.x : p2.x;
  int top    = p1.y < p2.y ? p1.y : p2.y;
  int right  = p1.x > p2.x ? p1.x : p2.x;
  int bottom = p1.y > p2.y ? p1.y : p2.y;
  m_mouse_grabber.ResetMode();
  return Rect(left, top, right, bottom);
}

PlotFigure::WhichButton PlotFigureWnd::TestMouseClick(CPoint* pt)
{
  iProcessWindowsMessage();
  const Array<PlotFigure::MouseClick>& mcl = *m_mouse_grabber.GetMouseClickList();
  if (mcl.Len() > 0)
  {
    const PlotFigure::MouseClick& m = mcl[0];
    if (pt)  *pt = m.pt;
    PlotFigure::WhichButton button = m.button;
    m_mouse_grabber.ResetMode();
    return button;
  }
  else
  {
    return PlotFigure::MC_NONE;
  }
}

void PlotFigureWnd::Link(PlotFigureWnd* other, bool symmetric)
{
  m_linked_figs.Push(other);
  if (symmetric)  other->Link(this, false);
}


//////////////////////////////////////////////////////////////////
// Figure
//
// This class is for the end user.  Although it would make sense to derive
// it from PlotFigureWnd, this is not possible because of the 'permanent' option.
// 'Permanent' means that the window is not deleted when the destructor of this
// class is called, and there is no way to prevent the destructor of a base class
// from being called from within the destructor of the derived class. 

PlotFigure::PlotFigure(const char* title, int x, int y, bool permanent,bool disable_close_button)
  : m_permanent(permanent), m_wnd(NULL)
{
  m_wnd = new PlotFigureWnd(title, x, y);
	DisableCloseButton(disable_close_button);
}

PlotFigure::~PlotFigure()
{
  if (!m_permanent)  { delete m_wnd;  m_wnd = NULL; }
}

void PlotFigure::Draw(const ImgBgr&    img) { m_wnd->Draw(img); }
void PlotFigure::Draw(const ImgBinary& img) { m_wnd->Draw(img); }
void PlotFigure::Draw(const ImgFloat&  img) { m_wnd->Draw(img); }
void PlotFigure::Draw(const ImgGray&   img) { m_wnd->Draw(img); }
void PlotFigure::Draw(const ImgInt&    img) { m_wnd->Draw(img); }

// draws rect on current image
void PlotFigure::DrawRect(const Rect& rect, const Bgr& color, int thickness) 
{ m_wnd->DrawRect(rect, color, thickness); }

void PlotFigure::DrawDot(const Point& pt, const Bgr& color, int size) 
{ m_wnd->DrawDot(pt, color, size); }

void PlotFigure::DrawLine(const Point& pt1, const Point& pt2, const Bgr& color, int thickness) 
{ m_wnd->DrawLine(pt1, pt2, color, thickness); }

void PlotFigure::GrabMouseClicks(int n_mouse_clicks, Array<PlotFigure::MouseClick>* points, COLORREF point_color, COLORREF line_color)
{
  m_wnd->GrabMouseClicks(n_mouse_clicks, points, point_color, line_color);
}

void PlotFigure::GrabLeftMouseClicks(Array<PlotFigure::MouseClick>* points, COLORREF point_color, COLORREF line_color)
{
  m_wnd->GrabLeftMouseClicks(points, point_color, line_color);
}

CPoint PlotFigure::GrabMouseClick(WhichButton* button)
{
  Array<MouseClick> points;
  m_wnd->GrabMouseClicks(1, &points, -1, -1);
  if (button)  *button = points[0].button;
  return points[0].pt;
}

CRect PlotFigure::GrabRect()
{
  return m_wnd->GrabRect();
}

PlotFigure::WhichButton PlotFigure::TestMouseClick(CPoint* pt)
{
  return m_wnd->TestMouseClick(pt);
}

void PlotFigure::GetDisplay(ImgBgr* out) { m_wnd->GetDisplay(out); }

void PlotFigure::SetWindowPosition(int x, int y)
{
  m_wnd->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);
}

void PlotFigure::SetWindowSize(int width, int height)
{
  m_wnd->SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE);
}

void PlotFigure::SetWindowRect(const CRect& rect)
{
  m_wnd->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), 0);
}

CPoint PlotFigure::GetWindowPosition() const
{
  CRect rect;
  m_wnd->GetWindowRect(rect);
  return CPoint(rect.left, rect.top);
}

CSize PlotFigure::GetWindowSize() const
{
  CRect rect;
  m_wnd->GetWindowRect(rect);
  return CSize(rect.Width(), rect.Height());
}

CRect PlotFigure::GetWindowRect() const
{
  CRect rect;
  m_wnd->GetWindowRect(rect);
  return rect;
}

void PlotFigure::PlaceToTheRightOf(const PlotFigure& other)
{
  CRect rect1 = this->GetWindowRect();
  CRect rect2 = other.GetWindowRect();
  SetWindowRect(CRect(rect2.right, rect2.top, rect2.right + rect1.Width(), rect2.top + rect1.Height()));
}

void PlotFigure::SetTitle(const char* str)
{
  m_wnd->SetWindowText(str);
}

void PlotFigure::SetVisible(bool visible)
{
  m_wnd->ShowWindow(visible);
}

bool PlotFigure::IsClosed() const { return m_wnd->IsClosed(); }

void PlotFigure::Link(PlotFigure* other, bool symmetric)
{
  m_wnd->Link(other->m_wnd, symmetric);
}

void PlotFigure::DisableCloseButton(bool disable)
{
	UINT menuf = disable? (MF_BYCOMMAND | MF_GRAYED | MF_DISABLED) : (MF_BYCOMMAND);
	CMenu* pSM = m_wnd->GetSystemMenu(FALSE);
	if(pSM)
	{
		pSM->EnableMenuItem(SC_CLOSE, menuf);
	}
}

#endif //#ifdef USE_PLOTFIGURE
