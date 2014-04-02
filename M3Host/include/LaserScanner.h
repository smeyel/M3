#include "RemoteTrackerDevice.h"

/*
Laser Scanner implementation.
*/
class LaserScanner : public RemoteTrackerDevice
{
public:

	/*
	Output file to save 3d points for programs that can display 3d triangular meshes.
	*/
	ofstream meshFile;

	LaserScanner();
	/*
	Initialization from a given file.
	*/
	bool init(const char* initFile);
	/*
	Starts the tracking process.
	*/
	void startTracking();
	/*
	Takes 3d coords from cameras and writes them into the given <meshFile>.	
	*/
	void export3DCoordsToFile(ofstream& meshFile, Matx41f coord);

	/*
	Takes 2D coord from a camera and makes a ray.
	*/
	Ray getRayToPointFromCam(int camID);

	/*
	Makes 3D coords taking rays from cameras and intersect them.
	*/
	void make3DCoords();

	~LaserScanner();
};

