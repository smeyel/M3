
#include "PointCloudViewer.h"

#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>




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

  view.loadExtrinsicParams("Tmatrix_thread_0.xml");


  ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
}

PointCloudViewer::PointCloudViewer(const char* PointCloudFile)
{

  std::ifstream input(PointCloudFile);

  float point[4];
  char c;

  while (input >> point[0] >> c >> point[1] >> c >> point[2] >> c >> point[3])
  {
    Matx41f temp = Matx41f(point);
    printf("%f %f %f %f\n", point[0], point[1], point[2], point[3]);
    ImportedCoordinates.push_back(temp);
  }

  input.close();

  Coordinates = &ImportedCoordinates;

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

  view.loadExtrinsicParams("Tmatrix_thread_0.xml");

  printf("vege\n");

  ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
  ViewerMain();
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

  view.loadExtrinsicParams("Tmatrix_thread_0.xml");


  ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);

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
  printf("vegeeeee\n");
  DrawAxes();
  printf("vegaaaaa\n");

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

      circle(ViewPic, pix, 1, Scalar(255), -1);

      imshow("result", ViewPic);
    }




    switch (cv::waitKey(30))
    {
    case 27: //Esc
      return;
    case 'A':
    case 'a':
      Translate3D( 10, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case 'D':
    case'd':
      Translate3D(-10, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case 'W':
    case 'w':
      Translate3D(0, 10, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case 'S':
    case 's':
      Translate3D(0, -10, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '+':
      Scale3D(1.111111);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '-':
      Scale3D(0.9);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '1':
      Rotate(M_PI/50, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
    case '2':
      Rotate(-M_PI / 50, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '4':
      Rotate(0, M_PI/50, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '5':
      Rotate(0, -M_PI / 50, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '6':
      Rotate(0, 0, M_PI/50);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    case '7':
      Rotate(0, 0, -M_PI/50);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
      DrawAxes();
      DrawAll();
      imshow("result", ViewPic);
      break;
    default:
      break;
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

void PointCloudViewer::Translate3D(float x, float y, float z)
{
  Mat TransMat = Mat::eye(4, 4, CV_32F);

  TransMat.at<float>(0, 3) = x;
  TransMat.at<float>(1, 3) = y;
  TransMat.at<float>(2, 3) = z;

  TransMat = (Mat)view.GetT() * TransMat;

  Matx44f newT = Matx44f{
    TransMat.at<float>(0, 0), TransMat.at<float>(0, 1), TransMat.at<float>(0, 2), TransMat.at<float>(0, 3),
    TransMat.at<float>(1, 0), TransMat.at<float>(1, 1), TransMat.at<float>(1, 2), TransMat.at<float>(1, 3),
    TransMat.at<float>(2, 0), TransMat.at<float>(2, 1), TransMat.at<float>(2, 2), TransMat.at<float>(2, 3),
    TransMat.at<float>(3, 0), TransMat.at<float>(3, 1), TransMat.at<float>(3, 2), TransMat.at<float>(3, 3),
  };

  view.SetT( newT );

}

void PointCloudViewer::Scale3D(float scale)
{
  Mat TransMat = Mat::eye(4, 4, CV_32F);

  //TransMat.at<float>(0, 0) = scale;
  //TransMat.at<float>(1, 1) = scale;
  //TransMat.at<float>(2, 2) = scale;
  TransMat.at<float>(3, 3) = scale;

  Matx33f oldcam = view.GetCameraMatrix();

  Mat cammtx = Mat::zeros(3, 3, CV_64F);

  for (int i = 0; i < 3; i++)
  for (int j = 0; j < 3; j++)
    cammtx.at<double>(i, j) = oldcam(i, j);

  cammtx.at<double>(0, 0) = oldcam(0, 0) * scale;
  cammtx.at<double>(1, 1) = oldcam(1, 1) * scale;

  view.setCameraMatrix(cammtx);
  TransMat = (Mat)view.GetT() * TransMat;

  Matx44f newT = Matx44f{
    TransMat.at<float>(0, 0), TransMat.at<float>(0, 1), TransMat.at<float>(0, 2), TransMat.at<float>(0, 3),
    TransMat.at<float>(1, 0), TransMat.at<float>(1, 1), TransMat.at<float>(1, 2), TransMat.at<float>(1, 3),
    TransMat.at<float>(2, 0), TransMat.at<float>(2, 1), TransMat.at<float>(2, 2), TransMat.at<float>(2, 3),
    TransMat.at<float>(3, 0), TransMat.at<float>(3, 1), TransMat.at<float>(3, 2), TransMat.at<float>(3, 3)
  };

  view.SetT(newT);
}


void PointCloudViewer::Rotate(float wx, float wy, float wz)
{
  Mat TransMatX = Mat::eye(4, 4, CV_32F);
  Mat TransMatY = Mat::eye(4, 4, CV_32F);
  Mat TransMatZ = Mat::eye(4, 4, CV_32F);
  Mat TransMat = Mat::eye(4, 4, CV_32F);

  TransMatX.at<float>(1, 1) = std::cos(wx);
  TransMatX.at<float>(1, 2) = - std::sin(wx);
  TransMatX.at<float>(2, 1) = std::sin(wx);
  TransMatX.at<float>(2, 2) = std::cos(wx);

  TransMatY.at<float>(0, 0) = std::cos(wy);
  TransMatY.at<float>(0, 2) = std::sin(wy);
  TransMatY.at<float>(2, 0) = - std::sin(wy);
  TransMatY.at<float>(2, 2) = std::cos(wy);

  TransMatZ.at<float>(0, 0) = std::cos(wz);
  TransMatZ.at<float>(0, 1) = - std::sin(wz);
  TransMatZ.at<float>(1, 0) = std::sin(wz);
  TransMatZ.at<float>(1, 1) = std::cos(wz);

  TransMat = (Mat)view.GetT() * TransMatX * TransMatY * TransMatZ;

  Matx44f newT = Matx44f{
    TransMat.at<float>(0, 0), TransMat.at<float>(0, 1), TransMat.at<float>(0, 2), TransMat.at<float>(0, 3),
    TransMat.at<float>(1, 0), TransMat.at<float>(1, 1), TransMat.at<float>(1, 2), TransMat.at<float>(1, 3),
    TransMat.at<float>(2, 0), TransMat.at<float>(2, 1), TransMat.at<float>(2, 2), TransMat.at<float>(2, 3),
    TransMat.at<float>(3, 0), TransMat.at<float>(3, 1), TransMat.at<float>(3, 2), TransMat.at<float>(3, 3)
  };

  view.SetT(newT);
}


void PointCloudViewer::DrawAll()
{
#ifdef MULTITHREAD_MODE
  EnterCriticalSection(critical);
#endif
  unsigned end = Coordinates->size();
#ifdef MULTITHREAD_MODE
  LeaveCriticalSection(critical);
#endif

  for (unsigned i = 0; i < end; i++)
  {
#ifdef MULTITHREAD_MODE
    EnterCriticalSection(critical);
#endif
    cv::Matx41f coord = (*Coordinates)[i];
#ifdef MULTITHREAD_MODE
    LeaveCriticalSection(critical);
#endif
   
    Point2f pix = view.pointWorld2Img(coord);
    circle(ViewPic, pix, 1, Scalar(255), -1);
  }

}