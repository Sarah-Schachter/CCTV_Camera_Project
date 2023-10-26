#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <queue>
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
	Mat frame;
	VideoCapture capture(path);
	if (!capture.isOpened())
	{
		cout << "Cannot open the video file. \n";
		return;
	}

	while (true)
	{
		// extract next image
		capture.read(frame);
		if (frame.empty())
		{
			cout << "End of stream\n";
			break;
		}
		else {
			// if frame isn't like the previous push it to Q
			if (frameQueue.empty())
				frameQueue.push(frame.clone());
			else if (!isTheSameFrame(frame, frameQueue.back()))
				frameQueue.push(frame.clone());
		}
	}
	capture.release();
}


void backend()
{
	Logging::getFile()->info("hiiii");
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

		// call function detect from YOLO and draw rectangles around objects
		detections = detectOne(frame, class_list, net, start, frame_count, fps, total_frames);
		frameQueue.try_pop();

		// call func calc amd save Average
		calcSaveDetectoins(frame, detections);

		if (cv::waitKey(1) != -1)
		{
			cout << "finished by user\n";
			break;
		}
	}
}


int main()
{
	//auto file_logger = spdlog::basic_logger_mt("file", "logs.txt");
	//file_logger->info("Hello, spdlog!");
	camera(VIDEO_PATH);
	backend();
	return 1;
}
