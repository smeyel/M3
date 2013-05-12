#include <iostream>
#include "PhoneProxy.h"
#include "stdlib.h"
//#include "myconfigmanager.h"

using namespace std;

int main()
{
	
	bool running = true;
	int frameIdx = 0;

	PhoneProxy proxy;
	cout << "Connecting..." << endl;
	proxy.Connect("192.168.0.8", 6000);

	while(running)
	{
		
		
		for(int i = 0; ; i++)
		{
				
			cout << "Position No " << i << "..." << endl;
			cout << "Requesing position..." << endl;
			
			proxy.RequestPosition();
			
			cout << "Receiving position..." << endl;
			
			proxy.Receive("");
			Sleep(500);
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
