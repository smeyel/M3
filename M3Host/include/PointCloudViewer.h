#pragma once


#include <vector>


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

#ifdef MULTITHREAD_MODE
  CRITICAL_SECTION* critical;
#endif

public:
  PointCloudViewer(std::vector<cv::Matx41f> *Coords);
  PointCloudViewer(const char* PointCloudFile);

#ifdef MULTITHREAD_MODE
  PointCloudViewer(CRITICAL_SECTION* CriticalSection, std::vector<cv::Matx41f> *Coords);
  static unsigned __stdcall ThreadViewerStaticStartup(void* pThis);
#endif


  ~PointCloudViewer();



  void ViewerMain();

};