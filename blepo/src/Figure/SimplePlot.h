#ifndef __SIMPLE_PLOT_H__
#define __SIMPLE_PLOT_H__

/** Neeraj Kanhere

  This class uses a copied version of Figure and FigureWnd, which is not the right way to do it.
  A better way would be to make Figure::RefreshStatusPane() virtual, then derive this class from
  Figure.  We also need methods to get mouse coordinates, so that we can display the proper coordinates
  in the status pane.
  */

#pragma warning(disable: 4786)
#include <afx.h>
#include "Figure.h"
#include "../Image/ImageOperations.h"
#include <string>
#include <vector>
#include <map>

//--------------------------------------------------------------------------------------//
#define USE_PLOTFIGURE
//--------------------------------------------------------------------------------------//

class SimplePlot
{
public:
  struct DataPoint
  {
    DataPoint():x(0), y(0), marker_color(255, 0, 0), marker_size(1)
    {}

    float                 x;
    float                 y;
    blepo::ImgBgr::Pixel  marker_color;
    int                   marker_size;
  };
  
  struct AxisTick
  {
    float         tick_val;
    std::string   tick_str;
  };

  SimplePlot(std::string title = std::string("SimplePlot"), int canvas_width=600, int canvas_height=300);
  void SetWindowTitle(std::string title);
  void SetData(std::string label, const std::vector<DataPoint>& pts, int line_thickness= 1, blepo::ImgBgr::Pixel line_color = blepo::ImgBgr::Pixel(255, 0, 0));
  void SetCanvasSize(int w, int h);
  void EnableAutoAxes();
  void SetAxes(float x_low, float x_high, float y_low, float y_high);
  void SetXTicks(const std::vector<AxisTick>& ticks);
  void SetYTicks(const std::vector<AxisTick>& ticks);
  void SetTicks(const std::vector<AxisTick>& xticks, const std::vector<AxisTick>& yticks);
  void SetTicks(int nx, int ny);
  void SetXTicks(int nx);
  void SetYTicks(int ny);
  void Render(); 

private:

#ifdef USE_PLOTFIGURE
  class PlotFigure*         m_fig;
#else
  blepo::Figure*            m_fig;
#endif

  struct DataSet
  {
    DataSet():line_thickness(1), line_color(255, 0, 0)
    {}
    std::vector<DataPoint>  data_pts;
    int                     line_thickness;
    blepo::ImgBgr::Pixel    line_color;
  };
  
  blepo::TextDrawer               m_texter;
  blepo::ImgBgr                   m_canvas;
  blepo::ImgBgr                   m_bgr;
  blepo::Rect                     m_canvas_margin;
  float                           m_x_min;
  float                           m_x_max;
  float                           m_y_min;
  float                           m_y_max;
  std::map<std::string, DataSet>  m_data;
  std::vector<AxisTick>           m_x_ticks;
  std::vector<AxisTick>           m_y_ticks;
  bool                            m_auto_axes;
  
  void  CalculateAxes();
  void  RenderCanvas(blepo::ImgBgr* argimg); 
  void  RenderAxesLabels(int canvas_width, int canvas_height, blepo::ImgBgr* argimg); 
  void ComputeTicks(float min_val, float max_val, int n, std::vector<AxisTick>* ticks);
};


//--------------------------------------------------------------------------------------//
#ifdef USE_PLOTFIGURE

using namespace blepo;
class PlotFigure
{
public:
  enum WhichButton { MC_NONE = 0, MC_LEFT = 1, MC_RIGHT = 2 };
  struct MouseClick 
  { 
    MouseClick( WhichButton b, const CPoint& p) : button(b), pt(p) {}
    WhichButton button;
    CPoint pt;
  };

public:
  PlotFigure(const char* title = NULL, int x=-1, int y=-1, bool permanent = true, bool disable_close_button = false);
  virtual ~PlotFigure();
	void DisableCloseButton(bool disable = true);
  // resizes the window only if image size is different from before
  void Draw(const ImgBgr&    img);
  void Draw(const ImgBinary& img);
  void Draw(const ImgFloat&  img);
  void Draw(const ImgGray&   img);
  void Draw(const ImgInt&    img);

  // draws rect on current image
  void DrawRect(const Rect& rect, const Bgr& color, int thickness=1);
  void DrawDot(const Point& pt, const Bgr& color, int size=3);
  void DrawLine(const Point& pt1, const Point& pt2, const Bgr& color, int thickness=1);
  
  // grab a certain number of mouse clicks (any button)
  // if 'point_color' is valid (i,e., <= 0xFFFFFF), then 3x3 colored dots are displayed at the click locations
  // if 'line_color' is valid (i,e., <= 0xFFFFFF), then a 1-pixel-thick polyline is drawn
  void GrabMouseClicks(int n_mouse_clicks, Array<MouseClick>* points, COLORREF point_color = -1, COLORREF line_color = -1);

  // grab a consecutive number of left mouse clicks.
  // Terminate by clicking the right mouse button
  // if 'point_color' is valid (i,e., <= 0xFFFFFF), then 3x3 colored dots are displayed at the click locations
  // if 'line_color' is valid (i,e., <= 0xFFFFFF), then a 1-pixel-thick polyline is drawn
  void GrabLeftMouseClicks(Array<MouseClick>* points, COLORREF point_color = -1, COLORREF line_color = -1);

  // if button is not NULL, then sets it to the button that was pressed
  CPoint GrabMouseClick(WhichButton* button = NULL);

  CRect GrabRect();

  // Test whether mouse was clicked without waiting.  If mouse was clicked, then
  // function returns the button pressed (or 0 if no button pressed) 
  // and sets 'pt' to the coordinates of the click.
  WhichButton TestMouseClick(CPoint* pt = NULL);

  void GetDisplay(ImgBgr* out);

  // Sets window position in screen coordinates
  void SetWindowPosition(int x, int y);

  // Sets window size in screen coordinates
  void SetWindowSize(int width, int height);

  // Sets window position and size in screen coordinates
  void SetWindowRect(const CRect& rect);

  // Gets window position in screen coordinates
  CPoint GetWindowPosition() const;

  // Gets window size in screen coordinates
  CSize GetWindowSize() const;

  // Gets window position and size in screen coordinates
  CRect GetWindowRect() const;

  // Place this figure immediately to the right of an existing figure.
  void PlaceToTheRightOf(const PlotFigure& other);

  void SetTitle(const char* str);

  void SetVisible(bool visible);

  bool IsClosed() const;

  // Link two figures together.  Mousemove in one figure causes mousemove in other figure.
  // Bug:  Only works correctly if both figures are the same size and both images are the
  // same size.
  void Link(PlotFigure* other, bool symmetric = true);
  class PlotFigureWnd* m_wnd;

private:
  bool m_permanent;
};

// Enters a Windows event loop.
// Returns only after a key is typed in the console window.  
void PlotEventLoop();
#endif //#define PLOT_FIGURE

//--------------------------------------------------------------------------------------//
#endif