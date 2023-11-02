#include <string>
#include <opencv2/opencv.hpp>
#include "../smart_eye/cameraFunctions.h"
#include "../smart_eye/threadSafeQueue.h"
#include "../smart_eye/logger.h"
#include "../config_files/config.h"

using namespace std;

int main()
{
	Logging::getFile()->info("camera begin");

	cv::Mat frame, prevFrame;
	cv::VideoCapture capture(VIDEO_PATH);

	if (!capture.isOpened())
	{
		Logging::getFile()->error("ERROR: Cannot open the video file in path \"" + string(VIDEO_PATH) + "\"");
		return -1;
	}

	// extract frames and send to backend
	while (true)
	{
		capture.read(frame);

		if (frame.empty())
		{
			Logging::getFile()->info("End of stream");
			break;
		}

		if (prevFrame.empty() || !isTheSameFrame(frame, prevFrame))
			// TODO:
			/*GRPC send image to server*/ frame = frame.clone();

		prevFrame = frame;
		cv::waitKey(333);
	}
	capture.release();
	Logging::getFile()->info("camera finished\n");
	return 1;
}
