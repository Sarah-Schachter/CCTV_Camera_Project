#pragma once
//#include <string>
#include <fstream>
#include "../CameraLibrary/Includes.h"
#include "opencv2/opencv.hpp"
#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG

using namespace std;
using namespace cv;

void calcAverageColor(const cv::Mat& image, double& avgR, double& avgG, double& avgB);
void saveToCSV(const std::string& filename, const Detection& detection, double avgR, double avgG, double avgB);
void calcSaveDetectoins(cv::Mat currentFrame, std::vector<Detection> detections);
