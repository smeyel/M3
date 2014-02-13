#include <iostream>
//#include "opencv2/highgui.h"
#include <opencv2/opencv.hpp>

#include "PhoneProxy.h"
#include "stdlib.h"
#include "StdoutLogger.h"
//#include "myconfigmanager.h"
#include "JpegMessage.h"

using namespace std;

int main()
{
	
	bool running = true;
	int frameIdx = 0;
	LogConfigTime::StdoutLogger logger;

	PhoneProxy proxy;
	cout << "Connecting..." << endl;
	//proxy.Connect("192.168.0.8", 6000);
	proxy.Connect("127.0.0.1", 6000);

	cv::Mat image(480, 640, CV_8UC3);
	while (running)
	{
		
		
		for(int i = 0; ; i++)
		{
			cout << "Requesing image..." << endl;
			
			proxy.RequestPhoto(0);

			cout << "Receiving photo..." << endl;
			
			JsonMessage *msg = proxy.ReceiveNew();
			JpegMessage *imgMsg = (JpegMessage*)msg;
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
