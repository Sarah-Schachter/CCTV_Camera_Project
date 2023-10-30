#include "CameraThread.h"
#include "BackendThread.h"
#include <thread>
#include "../config_files/config.h"
#include "threadSafeQueue.h"
#include "logger.h"


#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG

using namespace std;
using namespace cv;



int main()
{
	Logging::getFile();

	ThreadSafeQueue <cv::Mat> frameQueue;
	std::thread cameraThread(cameraThreadFunction, VIDEO_PATH, std::ref(frameQueue));
	std::thread backendThread(backendThreadFunction, std::ref(frameQueue));

	cameraThread.join();
	backendThread.join();

	return 1;
}
