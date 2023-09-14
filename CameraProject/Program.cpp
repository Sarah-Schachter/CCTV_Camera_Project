
#include "pch.h"
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <ctime>
#include <iostream>
#include "../CameraLibrary/YOLOdetection.h"
#include "../CameraLibrary/CalcAvarage.h"
#include "../CameraLibrary/CameraFunctions.h"
#include "../CameraLibrary/Includes.h"

#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG

using namespace std;
using namespace cv;


//#pragma region task 2 miryam check differences
//
//bool isTheSameFrame(cv::Mat prev, cv::Mat current)
//{
//	int countChangePix = 100;
//	// Loop through all pixels
//	for (int row = 0; row < prev.rows; row++)
//	{
//		for (int col = 0; col < prev.cols; col++)
//		{
//			// Get the pixel values at the same position in both images
//			cv::Vec3b pixel1 = prev.at<cv::Vec3b>(row, col);
//			cv::Vec3b pixel2 = current.at<cv::Vec3b>(row, col);
//
//			// Compare pixel values (for color images)
//			if (pixel1 != pixel2 && countChangePix > 0) {
//				// Pixels are different
//				countChangePix--;
//			}
//			else if (countChangePix == 0)
//				//the frame is diffrent
//				return false;
//		}
//	}
//	//the frame is the same
//	return true;
//}
//#pragma endregion


queue<cv::Mat> frameQueue;

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
		else{
		// if frame isn't like the previous push it to Q
			if (frameQueue.empty())
				frameQueue.push(frame.clone());
			else
				if (!isTheSameFrame(frame, frameQueue.back()))
							frameQueue.push(frame.clone());
			}
		
	}
	capture.release();
}


void backend()
{
	cv::Mat frame;
	vector <Detection> detections;

	vector <string> class_list = load_class_list();
	cv::dnn::Net net;
	load_net(net);

	auto start = std::chrono::high_resolution_clock::now();

	int frame_count = 0;
	float fps = -1;
	int total_frames = 0;

	while (!frameQueue.empty())
	{
		// pop from Q
		
		frame = frameQueue.front();

		// call function detect from YOLO - Sara
		detections = detectOne(frame, class_list, net, start, frame_count, fps, total_frames);
		frameQueue.pop();

		// call func calc amd save Average - Racheli
		calcSaveDetectoins(frame, detections);

		if (cv::waitKey(1) != -1)
		{
			cout << "finished by user\n";
			break;
		}
	}
}

