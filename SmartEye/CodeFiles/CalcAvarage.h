#pragma once
#include "./Detection.h"
#include "sqlite_files/sqlite3.h"
#include "opencv2/opencv.hpp"

#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG



void calcAverageColor(const cv::Mat& image, double& avgR, double& avgG, double& avgB);
void calcSaveDetectoins(cv::Mat currentFrame, std::vector<Detection> detections);
int saveToDB(const Detection& detection, double avgR, double avgG, double avgB);
