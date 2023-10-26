#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include "Logger.h"
#include "./YOLOdetection.h"
#include "./CalcAvarage.h"
#include "./CameraFunctions.h"
#include "./Detection.h"
#include "./threadSafeQueue.h"

#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG

using namespace std;
using namespace cv;


ThreadSafeQueue <cv::Mat> frameQueue;


void camera(string path)
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


void backend()
{
	Logging::getFile()->info("backend begin");
	cv::Mat frame;
	vector <Detection> detections;

	vector <string> class_list = load_class_list();
	cv::dnn::Net net;
	load_net(net);

	auto start = std::chrono::steady_clock::now();

	int frame_count = 0;
	float fps = -1;
	int total_frames = 0;

	while (!frameQueue.empty())
	{
		// pop from Q
		frame = frameQueue.front();
		frameQueue.try_pop();

		// detect objects and draw rectangles around 
		detections = detectOne(frame, class_list, net, start, frame_count, fps, total_frames);

		// calc amd save Average
		calcSaveDetectoins(frame, detections);

		if (cv::waitKey(1) != -1)
		{
			Logging::getFile()->info("finished by user");
			break;
		}
	}
	Logging::getFile()->info("backend finished\n");
}


int main()
{
	camera(VIDEO_PATH);
	backend();
	return 1;
}
