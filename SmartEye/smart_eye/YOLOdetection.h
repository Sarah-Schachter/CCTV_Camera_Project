#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "./Detection.h"
#include "Detection.h"
#include "opencv2/opencv.hpp"


const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.2;
const float NMS_THRESHOLD = 0.4;
const float CONFIDENCE_THRESHOLD = 0.4;

const std::vector <cv::Scalar> colors = { cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 0) };



std::vector <std::string> load_class_list();

void load_net(cv::dnn::Net& net);

cv::Mat format_yolov5(const cv::Mat& source);

void detect(cv::Mat& image, cv::dnn::Net& net, std::vector<Detection>& output, const std::vector <std::string>& className);

std::vector <Detection> detectAndDraw(cv::Mat& frame, const std::vector<std::string>& class_list, cv::dnn::Net& net, std::chrono::steady_clock::time_point start, int& frame_count, float& fps, int& total_frames);

void fixBoxCoords(cv::Mat frame, cv::Rect& box);

void loadYOLO();
