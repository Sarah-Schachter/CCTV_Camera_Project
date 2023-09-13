
#include "pch.h"
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <ctime>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG

using namespace std;
using namespace cv;

struct Detection
{
	int class_id;
	float confidence;
	cv::Rect box;
};

const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.2;
const float NMS_THRESHOLD = 0.4;
const float CONFIDENCE_THRESHOLD = 0.4;

const std::vector<cv::Scalar> colors = { cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 0) };



std::vector<std::string> load_class_list()
{
	std::vector <std::string> class_list;
	std::ifstream ifs(R"(C:\Users\user\Documents\Kamateck\camera_project\YOLO1\yolov5-opencv-cpp-python\config_files\classes.txt)");
	//ifstream ifs(R"(.\config_files\classes.txt)");
	std::string line;
	while (getline(ifs, line))
	{
		class_list.push_back(line);
	}
	return class_list;
}


void load_net(cv::dnn::Net& net)
{
	net = cv::dnn::readNet(R"(C:\Users\user\Documents\Kamateck\camera_project\YOLO1\yolov5-opencv-cpp-python\config_files\yolov5s.onnx)");
	//net = cv::dnn::readNet(R"(.\config_files\yolov5s.onnx)");
}


cv::Mat format_yolov5(const cv::Mat& source)
{
	int col = source.cols;
	int row = source.rows;
	int _max = MAX(col, row);
	cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
	source.copyTo(result(cv::Rect(0, 0, col, row)));
	return result;
}

// understand
void detect(cv::Mat& image, cv::dnn::Net& net, std::vector<Detection>& output, const std::vector<std::string>& className)
{
	cv::Mat blob;
	auto input_image = format_yolov5(image);
	cv::dnn::blobFromImage(input_image, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);
	net.setInput(blob);
	std::vector<cv::Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());
	float x_factor = input_image.cols / INPUT_WIDTH;
	float y_factor = input_image.rows / INPUT_HEIGHT;
	float* data = (float*)outputs[0].data;
	const int dimensions = 85;
	const int rows = 25200;
	std::vector<int> class_ids;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;

	for (int i = 0; i < rows; ++i) {
		float confidence = data[4];
		if (confidence >= CONFIDENCE_THRESHOLD) {
			float* classes_scores = data + 5;
			cv::Mat scores(1, className.size(), CV_32FC1, classes_scores);
			cv::Point class_id;
			double max_class_score;
			minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
			if (max_class_score > SCORE_THRESHOLD) {
				confidences.push_back(confidence);
				class_ids.push_back(class_id.x);
				float x = data[0];
				float y = data[1];
				float w = data[2];
				float h = data[3];
				int left = int((x - 0.5 * w) * x_factor);
				int top = int((y - 0.5 * h) * y_factor);
				int width = int(w * x_factor);
				int height = int(h * y_factor);
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
		data += 85;
	}

	std::vector<int> nms_result;
	cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, nms_result);
	for (int i = 0; i < nms_result.size(); i++) {
		int idx = nms_result[i];
		Detection result;
		result.class_id = class_ids[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx];
		output.push_back(result);
	}
}

// fix it
void loadYOLO()
{
	std::vector <std::string> class_list = load_class_list();
	cv::dnn::Net net;
	load_net(net);

	auto start = std::chrono::high_resolution_clock::now();
	int frame_count = 0;
	float fps = -1;
	int total_frames = 0;

}


// check why we need to know the frame rate
std::vector<Detection> detectOne(cv::Mat frame, std::vector<std::string> class_list, cv::dnn::Net net, std::chrono::steady_clock::time_point start, int frame_count, float fps, int total_frames)
{
	std::vector<Detection> output;

	if (frame.empty())
	{
		std::cout << "End of stream\n";
		return output;
	}

	detect(frame, net, output, class_list);

	frame_count++;
	total_frames++;
	int detections = output.size();

	// it draws the rectangles arount the objects
	for (int i = 0; i < detections; ++i)
	{
		auto detection = output[i];
		auto box = detection.box;
		auto classId = detection.class_id;
		const auto color = colors[classId % colors.size()];
		cv::rectangle(frame, box, color, 3);
		cv::rectangle(frame, cv::Point(box.x, box.y - 20), cv::Point(box.x + box.width, box.y), color, cv::FILLED);
		cv::putText(frame, class_list[classId].c_str(), cv::Point(box.x, box.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
	}

	if (frame_count >= 30)
	{
		auto end = std::chrono::high_resolution_clock::now();
		fps = frame_count * 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		frame_count = 0;
		start = std::chrono::high_resolution_clock::now();
	}

	if (fps > 0)
	{
		std::ostringstream fps_label;
		fps_label << std::fixed << std::setprecision(2);
		fps_label << "FPS: " << fps;
		std::string fps_label_str = fps_label.str();
		cv::putText(frame, fps_label_str.c_str(), cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
	}
	cv::imshow("output", frame);

	return output;
}


#pragma region Task 4 calc and save to csv average color

void calcAverageColor(const cv::Mat& image, double& avgR, double& avgG, double& avgB)
{
	cv::Scalar avgColor = mean(image);
	avgR = avgColor[2]; // Red channel
	avgG = avgColor[1]; // Green channel
	avgB = avgColor[0]; // Blue channel
}

void saveToCSV(const std::string& filename, const Detection& detection, double avgR, double avgG, double avgB)
{
	std::ofstream file;
	file.open(filename, ios_base::app); // Open in append mode

	if (!file.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	// Write headers if the file is empty
	if (file.tellp() == 0) {
		file << "Time,Top,Left,Width,Height,AvgR,AvgG,AvgB" << endl;
	}

	// Get the current time
	time_t currentTime = time(nullptr);
	// Convert the time to a string
	char timeString[26];
	ctime_s(timeString, sizeof(timeString), &currentTime);

	// Write detection details and average color values
	file << timeString << detection.box.y << ", " << detection.box.x << ", " << detection.box.width << ", " << detection.box.height
		<< ", " << avgR << ", " << avgG << ", " << avgB << endl;

	file.close();
}

void calcSaveDetectoins(cv::Mat currentFrame, std::vector<Detection> detections)
{
	for (const Detection& detection : detections) {
		double avgRectR, avgRectG, avgRectB;
		cv::Mat detectionROI = currentFrame(detection.box); // get the small image of the detection area only
		calcAverageColor(detectionROI, avgRectR, avgRectG, avgRectB);
		saveToCSV("results.csv", detection, avgRectR, avgRectG, avgRectB);
	}
}

#pragma endregion


#pragma region task 2 miryam check differences

bool isTheSameFrame(cv::Mat prev, cv::Mat current)
{
	int countChangePix = 100;
	// Loop through all pixels
	for (int row = 0; row < prev.rows; row++)
	{
		for (int col = 0; col < prev.cols; col++)
		{
			// Get the pixel values at the same position in both images
			cv::Vec3b pixel1 = prev.at<cv::Vec3b>(row, col);
			cv::Vec3b pixel2 = current.at<cv::Vec3b>(row, col);

			// Compare pixel values (for color images)
			if (pixel1 != pixel2 && countChangePix > 0) {
				// Pixels are different
				countChangePix--;
			}
			else if (countChangePix == 0)
				//the frame is diffrent
				return false;
		}
	}
	//the frame is the same
	return true;
}
#pragma endregion


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

int main() {
	camera(R"(C:\WIN_20230913_10_29_15_Pro.mp4)");
	backend();
	return 1;
}