#pragma once
#include "opencv2/opencv.hpp"
#include "../config_files/config.h"

struct Detection
{
	int class_id;
	float confidence;
	cv::Rect box;
};