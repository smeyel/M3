#pragma once


#include <vector>

#include "camera.h"

#include <opencv2/opencv.hpp>

//#define MULTITHREAD_MODE  //!!!! Uncomment this to enable multithread mode



#ifdef MULTITHREAD_MODE

#include <windows.h>          // for HANDLE
#include <process.h>          // for _beginthread()

#endif


class PointCloudViewer
{
  
private:
  std::vector<cv::Matx41f> *Coordinates;
  std::vector<cv::Matx41f> ImportedCoordinates; //used when imprting mesh from file

   cv::Mat ViewPic;

  unsigned ViewerHeight;
  unsigned ViewerWidth;
  
  double focusX;
  double focusY;

  Mat TMatrix;
  Mat CamMatrix;

  Point2f PointWorld2Img(Matx41f pWorld);

  int cursorX = 0;
  int cursorY = 0;

  bool transformed2 = false;

  double zDistFromCam(Matx41d);

#ifdef MULTITHREAD_MODE
  CRITICAL_SECTION* critical;
#endif

  
  static void Proba(double x, double y);

public:
  //PointCloudViewer(std::vector<cv::Matx41f> *Coords);
  PointCloudViewer(const char* PointCloudFile);
  
#ifdef MULTITHREAD_MODE
  PointCloudViewer(CRITICAL_SECTION* CriticalSection, std::vector<cv::Matx41f> *Coords);
  static unsigned __stdcall ThreadViewerStaticStartup(void* pThis);
#endif


  ~PointCloudViewer();

  void CallBackFunc(int event, int x, int y, int flags);

  void ViewerMain();

  void DrawAxes();

  void Translate3D(float x, float y, float z);
  void Scale3D(float scale);
  //void Rotate(float wx, float wy, float wz);
  void RotateXY(float wx, float wy);

  void DrawAll();

};