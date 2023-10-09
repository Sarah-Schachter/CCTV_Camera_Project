#include "CalcAvarage.h"

using namespace std;
using namespace cv;

#pragma region Milestone1 Task 4 calc and save to csv average color

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

#ifdef _WIN32
	ctime_s(timeString, sizeof(timeString), &currentTime);
#else
	time_t rawTime = currentTime;
	ctime_r(&rawTime, timeString);
#endif

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


