
#include "PointCloudViewer.h"


PointCloudViewer::PointCloudViewer(std::vector<cv::Matx41f> *Coords) : Coordinates(Coords)
{

}

PointCloudViewer::PointCloudViewer(const char* PointCloudFile)
{

}

PointCloudViewer::~PointCloudViewer()
{

}

#ifdef MULTITHREAD_MODE
PointCloudViewer::PointCloudViewer(CRITICAL_SECTION* CriticalSection, std::vector<cv::Matx41f> *Coords) : critical(CriticalSection), Coordinates(Coords)
{

}

unsigned __stdcall PointCloudViewer::ThreadViewerStaticStartup(void* pThis)
{
  PointCloudViewer* pThreadPCViewer = (PointCloudViewer*)pThis;

  pThreadPCViewer->ViewerMain();

  return 1;
}

#endif


void PointCloudViewer::ViewerMain()
{

  unsigned idx = 0;


  while (1)
  {
#ifdef MULTITHREAD_MODE
    EnterCriticalSection(critical);
#endif
    bool cond = (Coordinates->size() > idx + 1);
#ifdef MULTITHREAD_MODE
    LeaveCriticalSection(critical);
#endif

    if (cond)
    {
#ifdef MULTITHREAD_MODE
      EnterCriticalSection(critical);
#endif
      cv::Matx41f coord = (*Coordinates)[idx];
#ifdef MULTITHREAD_MODE
      LeaveCriticalSection(critical);
#endif
      idx++;
      printf("%f %f %f %f\n", coord(0), coord(1), coord(2), coord(3));

    }

    if (cv::waitKey(30) == 27)
    {
      return;
    }

  }





}