#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include "ThreadSafeQueue.h"
#include "logger.h"
#include "cameraFunctions.h"
#include "threadSafeQueue.h"


void cameraThreadFunction(std::string path, ThreadSafeQueue<cv::Mat>& frameQueue);

