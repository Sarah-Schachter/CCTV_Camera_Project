#include "cameraThread.h"

using namespace std;


void cameraThreadFunction(std::string path, ThreadSafeQueue<cv::Mat> &frameQueue)
{
	Logging::getFile()->info("camera begin");

	Mat frame;
	VideoCapture capture(path);

	if (!capture.isOpened())
	{
		Logging::getFile()->error("ERROR: Cannot open the video file in path \"" + path + "\"");
		return;
	}

	while (true)
	{
		// extract next image
		capture.read(frame);

		if (frame.empty())
		{
			Logging::getFile()->info("End of stream");
			break;
		}

		// push frame to Q
		if (frameQueue.empty() || !isTheSameFrame(frame, frameQueue.back()))
			frameQueue.push(frame.clone());

		cv::waitKey(333);
	}
	capture.release();
	Logging::getFile()->info("camera finished\n");
}
