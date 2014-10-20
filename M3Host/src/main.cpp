#include <iostream>
//#include "opencv2/highgui.h"
#include <opencv2/opencv.hpp>

#include "PhoneProxy.h"
#include "stdlib.h"
#include "StdOutLogger.h"
//#include "myconfigmanager.h"
#include "JpegMessage.h"

using namespace std;

int main()
{
	
	bool running = true;
	int frameIdx = 0;
//	LogConfigTime::StdoutLogger logger;

	PhoneProxy proxy;
	//const char* host = "192.168.0.8";
	//const char* host = "127.0.0.1";
	const char* host = "192.168.1.107";
//	const char* host = "152.66.159.87";
	const int port = 6000;

//	cout << "Connecting..." << endl;
//	proxy.Connect(host, port);

	cv::Mat image(480, 640, CV_8UC3);
	while (running)
	{
		
		
		for(int i = 0; ; i++)
		{
			cout << "Connecting..." << endl;
			proxy.Connect(host, port);

			cout << "Requesing image..." << endl;
			
			proxy.RequestPhoto(0);

			cout << "Receiving photo..." << endl;
			
			JsonMessage *msg = proxy.ReceiveNew();
			cout << "Message ready" << endl;
			JpegMessage *imgMsg = (JpegMessage*)msg;
			cout << "ImageMsg ready: " << imgMsg->data.size() << endl;

			imgMsg->Decode(&image);
			cv::rectangle(image, cv::Rect(20, 20, 100, 100), cv::Scalar(255, 0, 0));
			cv::imshow("PICTURE", image);

			//proxy.Receive("result.txt");
			char ch = cv::waitKey(25);
			if (ch == 27)
			{
				break;
			}
		}

		cout << "Disconnecting..." << endl;
		proxy.Disconnect();

		cout << "Done..." << endl;

		running = false;

		frameIdx++;
	}


	cout << "Done." << endl;
	return 0;
}
