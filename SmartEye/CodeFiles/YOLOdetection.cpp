#include "YOLOdetection.h"


// Load the class list from a file
std::vector<std::string> load_class_list()
{
	std::vector <std::string> class_list;
	std::ifstream ifs(CLASS_LIST_PATH);
	std::string line;
	while (getline(ifs, line))
	{
		class_list.push_back(line);
	}
	return class_list;
}


// Load the YOLO network
void load_net(cv::dnn::Net& net)
{
	net = cv::dnn::readNet(YOLO_PATH);
}


// Resize the input image to a square shape
cv::Mat format_yolov5(const cv::Mat& source)
{
	int col = source.cols;
	int row = source.rows;
	int _max = MAX(col, row);
	cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
	source.copyTo(result(cv::Rect(0, 0, col, row)));
	return result;
}


// Detect objects using YOLO
void detect(cv::Mat& image, cv::dnn::Net& net, std::vector<Detection>& output, const std::vector<std::string>& className)
{
	// Create a blob from the input image using the YOLOv5 format
	cv::Mat blob;
	auto input_image = format_yolov5(image);
	cv::dnn::blobFromImage(input_image, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(), true, false);

	// Set the input blob to the YOLO network
	net.setInput(blob);

	// Forward pass through the network to get the output detections
	std::vector<cv::Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	// Calculate scaling factors for the bounding box coordinates
	float x_factor = input_image.cols / INPUT_WIDTH;
	float y_factor = input_image.rows / INPUT_HEIGHT;

	// Extract information from the output and store the detections
	float* data = (float*)outputs[0].data;
	const int dimensions = 85;
	const int rows = 25200;
	std::vector<int> class_ids;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;

	for (int i = 0; i < rows; ++i)
	{
		// Retrieve the confidence score for the detection
		float confidence = data[4];

		// Check if the confidence is above the threshold
		if (confidence >= CONFIDENCE_THRESHOLD)
		{
			// Retrieve the class scores for the detection
			float* classes_scores = data + 5;
			cv::Mat scores(1, className.size(), CV_32FC1, classes_scores);
			cv::Point class_id;
			double max_class_score;

			// Find the class with the highest score
			minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

			// Check if the maximum class score is above the threshold
			if (max_class_score > SCORE_THRESHOLD)
			{
				// Store the detection information
				confidences.push_back(confidence);
				class_ids.push_back(class_id.x);
				float x = data[0];
				float y = data[1];
				float w = data[2];
				float h = data[3];

				// Calculate the bounding box coordinates based on the scaling factors
				int left = int((x - 0.5 * w) * x_factor);
				int top = int((y - 0.5 * h) * y_factor);
				int width = int(w * x_factor);
				int height = int(h * y_factor);

				// Create a rectangle representing the bounding box
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
		// Move to the next detection in the output data
		data += 85;
	}
	// Apply non-maximum suppression to remove overlapping detections
	std::vector<int> nms_result;
	cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, nms_result);

	// Store the final detections after non-maximum suppression
	for (int i = 0; i < nms_result.size(); i++)
	{
		int idx = nms_result[i];
		Detection result;
		result.class_id = class_ids[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx];
		fixBoxCoords(image, result.box);
		output.push_back(result);
	}
}


// check why we need to know the frame rate
// Perform object detection on a single frame and draw 
std::vector<Detection> detectOne(cv::Mat& frame, const std::vector<std::string>& class_list, cv::dnn::Net& net, std::chrono::steady_clock::time_point start, int frame_count, float fps, int total_frames)
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

	// Draw rectangles around the detected objects
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

	// Check if the frame count has reached a value of 30
	if (frame_count >= 30)
	{
		auto end = std::chrono::steady_clock::now();
		fps = frame_count * 1000.0 / std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		frame_count = 0;
		start = std::chrono::steady_clock::now();
	}

	// Check if the calculated frames per second (fps) is greater than 0
	if (fps > 0)
	{
		std::ostringstream fps_label;
		fps_label << std::fixed << std::setprecision(2);
		fps_label << "FPS: " << fps;
		std::string fps_label_str = fps_label.str();
		cv::putText(frame, fps_label_str.c_str(), cv::Point(10, 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
	}

	// Display the frame with the detected objects and overlays in a window titled "output"
	cv::imshow("output", frame);

	// Return the vector containing the detected objects and their information
	return output;
}


// fix box coordinates and size so that it won't cross image limits
void fixBoxCoords(cv::Mat frame, Rect& box)
{
	if (box.x < 0)
		box.x = 0;
	if (box.y < 0)
		box.y = 0;
	if (box.x + box.width > frame.size().width)
		box.width = frame.size().width - box.x;
	if (box.y + box.height > frame.size().height)
		box.height = frame.size().height - box.y;
}



// this function is not in use now, it needs fixing
// Load the YOLO network and perform object detection
void loadYOLO()
{
	std::vector <std::string> class_list = load_class_list();
	cv::dnn::Net net;
	load_net(net);

	auto start = std::chrono::high_resolution_clock::now();
	int frame_count = 0;
	float fps = -1;
	int total_frames = 0;
	// this function is not good yet
}
