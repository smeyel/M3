
#include "PointCloudViewer.h"

#include <fstream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <stdio.h>


PointCloudViewer* myViewer;



void CallBackWrapper(int event, int x, int y, int flags, void* userdata)
{

  myViewer->CallBackFunc(event, x, y, flags);

  
}
/*
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


  Matx44f newT = Matx44f{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 1000,
    0, 0, 0, 1
  };

  view.SetT(newT.inv());


  ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8U);
}
*/
PointCloudViewer::PointCloudViewer(const char* PointCloudFile)
{
  //cursorX = 0;
 // cursorY = 0;

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

  focusX = 500;
  focusY = 500;

  //view.cameraResolution.height = ViewerHeight;
  //view.cameraResolution.width = ViewerWidth;


  //CamMatrix = Mat::zeros(3, 3, CV_64F);
  //CamMatrix.at<double>(0, 0) = 500;  //fx
  //CamMatrix.at<double>(1, 1) = 500;  //fy
  //CamMatrix.at<double>(0, 2) = (double)ViewerWidth / 2;   //cx
  //CamMatrix.at<double>(1, 2) = (double)ViewerHeight / 2;  //cy
  //CamMatrix.at<double>(2, 2) = 1;


  //cv::Mat distorsionMatrix = Mat::zeros(5, 1, CV_64F);

  //view.setCameraMatrix(camMatrix);
  //view.setDistortionCoeffs(distorsionMatrix);


  TMatrix = Mat::eye(4, 4, CV_64F);
  TMatrix.at<double>(2, 3) = 1000;


  //view.SetT( newT.inv() );

  ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
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

  focusX = 500;
  focusY = 500;



  TMatrix = Mat::eye(4, 4, CV_64F);
  TMatrix.at<double>(2, 3) = 1000;


  //view.SetT( newT.inv() );

  ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
  ViewerMain();

}

unsigned __stdcall PointCloudViewer::ThreadViewerStaticStartup(void* pThis)
{
  PointCloudViewer* pThreadPCViewer = (PointCloudViewer*)pThis;

  pThreadPCViewer->ViewerMain();

  return 1;
}

#endif


Point2f PointCloudViewer::PointWorld2Img(Matx41f pWorld)
{
  Mat dst(4, 1, CV_64FC1);

  //Mat pWorldTemp = new Mat(4, 1, CvType.CV_64FC1);
  //pWorldTemp.put(0, 0, pWorld.val);

//  Core.gemm(tMatrix.inv(), pWorldTemp, 1.0, new Mat(4, 1, CvType.CV_64FC1), 0, dst);

  Mat pWorldTemp = Mat::zeros(4, 1, CV_64FC1);
  
  pWorldTemp.at<double>(0, 0) = pWorld(0, 0);
  pWorldTemp.at<double>(1, 0) = pWorld(1, 0);
  pWorldTemp.at<double>(2, 0) = pWorld(2, 0);
  pWorldTemp.at<double>(3, 0) = pWorld(3, 0);

  Mat mTemp = TMatrix.inv() * pWorldTemp;
  
  double x = focusX * (mTemp.at<double>(0, 0) / mTemp.at<double>(2, 0)) + ViewerWidth / 2;
  double y = focusX * (mTemp.at<double>(1, 0) / mTemp.at<double>(2, 0)) + ViewerHeight / 2;

  /*double[] dstVals = new double[4];
  dst.get(0, 0, dstVals);

  double x = focusX * (dstVals[0] / dstVals[2]) + viewWidth / 2;
  double y = focusY * (dstVals[1] / dstVals[2]) + viewHeight / 2;
  */
  return Point2f(x, y);

}

void PointCloudViewer::CallBackFunc(int event, int x, int y, int flags)
{
  if (event != EVENT_MOUSEMOVE && flags == (EVENT_FLAG_LBUTTON))
  {
    //std::cout << "Click (" << x << ", " << y << ")" << std::endl;
    cursorX = x;
    cursorY = y;
    //std::cout << "Click (" << x << ", " << y << ")" << std::endl;
  }

  else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_LBUTTON)
  {
    //std::cout << "Move (" << M_PI / 2 * ((double)x - (double)cursorX) / (double)ViewerWidth << ", " << M_PI / 2 * ((double)y - (double)cursorY) / (double)ViewerHeight << ")" << std::endl;

    RotateXY(-M_PI / 2 * ((double)y - (double)cursorY) / (double)ViewerHeight, M_PI / 2 * ((double)x - (double)cursorX) / (double)ViewerWidth);
    //RotateXY(0.01, 0.01);
    cursorX = x;
    cursorY = y;
    
    transformed2 = true;
  }
}





void PointCloudViewer::ViewerMain()
{
  myViewer = this;

  unsigned idx = 0;
  printf("vegeeeee\n");
  DrawAxes();
  printf("vegaaaaa\n");
  cv::Matx41f coord;

#ifdef MULTITHREAD_MODE
  EnterCriticalSection(critical);
#endif
  for (auto &coord : *Coordinates)
  {

    printf("%f %f %f %f\n", coord(0), coord(1), coord(2), coord(3));


    Point2f pix = PointWorld2Img(coord);
    printf("\t%f   %f\n", pix.x, pix.y);

    circle(ViewPic, pix, 1, Scalar(255, 255, 255), -1);
    idx++;
  }
#ifdef MULTITHREAD_MODE
  LeaveCriticalSection(critical);
#endif

  cv::imshow("Result", ViewPic);

  bool transformed = true;
  Point2f prev_pix = Point(0,0);

  while (1)
  {
    namedWindow("Result", 1);
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
      coord = (*Coordinates)[idx];
#ifdef MULTITHREAD_MODE
      LeaveCriticalSection(critical);
#endif
      idx++;
      printf("%f %f %f %f\n", coord(0), coord(1), coord(2), coord(3));


      Point2f pix = PointWorld2Img(coord);
      
      printf("\t%f   %f\n", pix.x, pix.y);
      
      if (!transformed){
        printf("XX");
        if ((prev_pix.x != 0) && (prev_pix.y != 0)){
          circle(ViewPic, prev_pix, 1, Scalar(255, 255, 255), -1);
          printf("YY");
        }

      }

      circle(ViewPic, pix, 1, Scalar(0,0,255), -1);
      prev_pix = pix;

      cv::imshow("Result", ViewPic);
    }

   
    if (transformed2) {
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed2 = false;
    }

    setMouseCallback("Result", CallBackWrapper, NULL);

    switch (cv::waitKey(30))
    {
    case 27: //Esc
      return;
    case 'A':
    case 'a':
      Translate3D( 10, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case 'D':
    case'd':
      Translate3D(-10, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case 'W':
    case 'w':
      Translate3D(0, 10, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case 'S':
    case 's':
      Translate3D(0, -10, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '+':
      Scale3D(1.111111);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '-':
      Scale3D(0.9);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    /*case '1':
      Rotate(M_PI/50, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '2':
      Rotate(-M_PI / 50, 0, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '4':
      Rotate(0, M_PI/50, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '5':
      Rotate(0, -M_PI / 50, 0);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '7':
      Rotate(0, 0, M_PI/50);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;
    case '8':
      Rotate(0, 0, -M_PI/50);
      ViewPic = Mat::zeros(ViewerHeight, ViewerWidth, CV_8UC3);
      DrawAxes();
      DrawAll();
      cv::imshow("Result", ViewPic);
      transformed = true;
      break;*/
    default:
      transformed = false;
      break;
    }


  }
}

void PointCloudViewer::DrawAxes()
{

  Point2f Origo = PointWorld2Img(Matx41f(0, 0, 0, 1));

  Point2f Xend = PointWorld2Img(Matx41f(300, 0, 0, 1));
  Point2f Yend = PointWorld2Img(Matx41f(0, 300, 0, 1));
  Point2f Zend = PointWorld2Img(Matx41f(0, 0, 300, 1));
  
  line(ViewPic, Origo, Xend, Scalar(255, 0, 0), 5);
  line(ViewPic, Origo, Yend, Scalar(0, 255, 0), 5);
  line(ViewPic, Origo, Zend, Scalar(0, 0, 255), 5);

  double xDist = -zDistFromCam(Matx41d(300, 0, 0, 1));
  double yDist =- zDistFromCam(Matx41d(0, 300, 0, 1));
  double zDist = -zDistFromCam(Matx41d(0, 0, 300, 1));

  if (xDist > yDist)
  {
    if (xDist > zDist){
      line(ViewPic, Origo, Xend,  Scalar(255, 0, 0), 3);
      if (yDist > zDist)
      {
        line(ViewPic, Origo, Yend,  Scalar(0, 255, 0), 3);
        line(ViewPic, Origo, Zend,  Scalar(0, 0, 255), 3);
      }
      else{
        line(ViewPic, Origo, Zend,  Scalar(0, 0, 255), 3);
        line(ViewPic, Origo, Yend,  Scalar(0, 255, 0), 3);
      }

    }
    else{
      line(ViewPic, Origo, Zend,  Scalar(0, 0, 255), 3);
      line(ViewPic, Origo, Xend,  Scalar(255, 0, 0), 3);
      line(ViewPic, Origo, Yend,  Scalar(0, 255, 0), 3);
    }
  }
  else {
    if (xDist > zDist){
      line(ViewPic, Origo, Yend,  Scalar(0, 255, 0), 3);
      line(ViewPic, Origo, Xend,  Scalar(255, 0, 0), 3);
      line(ViewPic, Origo, Zend,  Scalar(0, 0, 255), 3);
    }
    else{
      if (yDist > zDist){
        line(ViewPic, Origo, Yend,  Scalar(0, 255, 0), 3);
        line(ViewPic, Origo, Zend,  Scalar(0, 0, 255), 3);
        line(ViewPic, Origo, Xend,  Scalar(255, 0, 0), 3);
      }
      else{
        line(ViewPic, Origo, Zend,  Scalar(0, 0, 255), 3);
        line(ViewPic, Origo, Yend,  Scalar(0, 255, 0), 3);
        line(ViewPic, Origo, Xend,  Scalar(255, 0, 0), 3);
      }
    }

  }



}

double PointCloudViewer::zDistFromCam(Matx41d point)
{
  Mat res = TMatrix.inv() * (Mat)point;
  return res.at<double>(2, 0) / res.at<double>(3, 0);
}

void PointCloudViewer::Translate3D(float x, float y, float z)
{
  Mat TransMat = Mat::eye(4, 4, CV_64F);

  TransMat.at<double>(0, 3) = x;
  TransMat.at<double>(1, 3) = y;
  TransMat.at<double>(2, 3) = z;




  TMatrix = TMatrix.inv() * TransMat;
  TMatrix = TMatrix.inv();

 /* Matx44f newT = Matx44f{
    TransMat.at<float>(0, 0), TransMat.at<float>(0, 1), TransMat.at<float>(0, 2), TransMat.at<float>(0, 3),
    TransMat.at<float>(1, 0), TransMat.at<float>(1, 1), TransMat.at<float>(1, 2), TransMat.at<float>(1, 3),
    TransMat.at<float>(2, 0), TransMat.at<float>(2, 1), TransMat.at<float>(2, 2), TransMat.at<float>(2, 3),
    TransMat.at<float>(3, 0), TransMat.at<float>(3, 1), TransMat.at<float>(3, 2), TransMat.at<float>(3, 3),
  };

  view.SetT( newT.inv() );*/

}

void PointCloudViewer::Scale3D(float scale)
{
 /* Mat TransMat = Mat::eye(4, 4, CV_32F);

  //TransMat.at<float>(0, 0) = scale;
  //TransMat.at<float>(1, 1) = scale;
  //TransMat.at<float>(2, 2) = scale;
  TransMat.at<float>(3, 3) = scale;

  TransMat = (Mat)view.GetT().inv() * TransMat;

  Matx44f newT = Matx44f{
    TransMat.at<float>(0, 0), TransMat.at<float>(0, 1), TransMat.at<float>(0, 2), TransMat.at<float>(0, 3),
    TransMat.at<float>(1, 0), TransMat.at<float>(1, 1), TransMat.at<float>(1, 2), TransMat.at<float>(1, 3),
    TransMat.at<float>(2, 0), TransMat.at<float>(2, 1), TransMat.at<float>(2, 2), TransMat.at<float>(2, 3),
    TransMat.at<float>(3, 0), TransMat.at<float>(3, 1), TransMat.at<float>(3, 2), TransMat.at<float>(3, 3)
  };

  view.SetT(newT.inv());*/

  //Matx33f oldcam = view.GetCameraMatrix();

  //Mat cammtx = Mat::zeros(3, 3, CV_64F);

  //for (int i = 0; i < 3; i++)
  //for (int j = 0; j < 3; j++)
  //  cammtx.at<double>(i, j) = oldcam(i, j);

  //cammtx.at<double>(0, 0) = oldcam(0, 0) * scale;
  //cammtx.at<double>(1, 1) = oldcam(1, 1) * scale;

  //view.setCameraMatrix(cammtx);

  focusX *= scale;
  focusY *= scale;
}


void PointCloudViewer::RotateXY(float wx, float wy)
{

  Mat TransMatX = Mat::eye(4, 4, CV_64F);
  Mat TransMatY = Mat::eye(4, 4, CV_64F);
  Mat TransMatZ = Mat::eye(4, 4, CV_64F);
  Mat TransMat = Mat::eye(4, 4, CV_64F);

  TransMatX.at<double>(1, 1) = std::cos(wx);
  TransMatX.at<double>(1, 2) = -std::sin(wx);
  TransMatX.at<double>(2, 1) = std::sin(wx);
  TransMatX.at<double>(2, 2) = std::cos(wx);

  TransMatY.at<double>(0, 0) = std::cos(wy);
  TransMatY.at<double>(0, 2) = std::sin(wy);
  TransMatY.at<double>(2, 0) = -std::sin(wy);
  TransMatY.at<double>(2, 2) = std::cos(wy);

  /*TransMatZ.at<float>(0, 0) = std::cos(wz);
  TransMatZ.at<float>(0, 1) = - std::sin(wz);
  TransMatZ.at<float>(1, 0) = std::sin(wz);
  TransMatZ.at<float>(1, 1) = std::cos(wz);*/
  Mat RotMatrix = TMatrix.clone();
  RotMatrix.at<double>(0, 3) = 0;
  RotMatrix.at<double>(1, 3) = 0;
  RotMatrix.at<double>(2, 3) = 0;
  RotMatrix.at<double>(3, 3) = 1;


  TMatrix = RotMatrix * TransMatX * TransMatY /** TransMatZ*/ * RotMatrix.inv() * TMatrix.clone();

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
   
    Point2f pix = PointWorld2Img(coord);
    circle(ViewPic, pix, 1, Scalar(255, 255, 255), -1);
  }

}