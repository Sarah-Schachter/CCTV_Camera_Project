#pragma once
#include <queue>
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include "threadSafeQueue.h"
#include "logger.h"
#include "detection.h"
#include "YOLOdetection.h"
#include "calcAvarage.h"



void backendThreadFunction(ThreadSafeQueue<cv::Mat>& frameQueue);

