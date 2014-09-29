
#include "PointCloudViewer.h"


PointCloudViewer::PointCloudViewer(std::vector<cv::Matx41f> *Coords) : Coordinates(Coords)
{
  ViewerHeight = 720;
  ViewerWidth = 1280;

  view.cameraResolution.height = ViewerHeight;
  view.cameraResolution.width = ViewerWidth;


  cv::Mat camMatrix = Mat::zeros(3, 3, CV_64F);
  camMatrix.at<double>(0, 0) = 500;  //fx
  camMatrix.at<double>(1, 1) = 500;  //fy
  camMatrix.at<double>(0, 2) = (double)ViewerWidth / 2;   //cx
  camMatrix.at<double>(1, 2) = (double)ViewerHeight / 2;  //cy
  camMatrix.at<double>(2, 2) = 1;


  cv::Mat distorsionMatrix = Mat::zeros(5, 1, CV_64F);

  view.setCameraMatrix(camMatrix);
  view.setDistortionCoeffs(distorsionMatrix);

  view.loadExtrinsicParams("Tmatrix_thread_1.xml");


  ViewPic = Mat::ones(ViewerHeight, ViewerWidth, CV_8U);
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

  ViewerHeight = 720;
  ViewerWidth = 1280;

  view.cameraResolution.height = ViewerHeight;
  view.cameraResolution.width = ViewerWidth;


  cv::Mat camMatrix = Mat::zeros(3, 3, CV_64F);
  camMatrix.at<double>(0, 0) = 500;  //fx
  camMatrix.at<double>(1, 1) = 500;  //fy
  camMatrix.at<double>(0, 2) = (double)ViewerWidth / 2;   //cx
  camMatrix.at<double>(1, 2) = (double)ViewerHeight / 2;  //cy
  camMatrix.at<double>(2, 2) = 1;


  cv::Mat distorsionMatrix = Mat::zeros(5, 1, CV_64F);

  view.setCameraMatrix(camMatrix);
  view.setDistortionCoeffs(distorsionMatrix);

  view.loadExtrinsicParams("Tmatrix_thread_1.xml");


  ViewPic = Mat::ones(ViewerHeight, ViewerWidth, CV_8U);

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
  DrawAxes();
 

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


      Point2f pix = view.pointWorld2Img(coord);
      printf("\t%f   %f\n", pix.x, pix.y);

      circle(ViewPic, Point(pix.x,pix.y), 1, Scalar(255), -1);

      imshow("result", ViewPic);
    }




    if (cv::waitKey(30) == 27)
    {
      return;
    }

  }
}

void PointCloudViewer::DrawAxes()
{

  Point2f Origo = view.pointWorld2Img(Matx41f(0, 0, 0, 1));

  Point2f Xend = view.pointWorld2Img(Matx41f(300, 0, 0, 1));
  Point2f Yend = view.pointWorld2Img(Matx41f(0, 300, 0, 1));
  Point2f Zend = view.pointWorld2Img(Matx41f(0, 0, 300, 1));
  
  line(ViewPic, Origo, Xend, Scalar(200), 5);
  line(ViewPic, Origo, Yend, Scalar(150), 5);
  line(ViewPic, Origo, Zend, Scalar(100), 5);



}