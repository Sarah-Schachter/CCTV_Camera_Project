#include "backendThread.h"

using namespace std;
using namespace cv;

void backendThreadFunction(ThreadSafeQueue<cv::Mat> &frameQueue)

{
	Logging::getFile()->info("backend begin");
	cv::Mat frame;
	vector<Detection> detections;

	vector<string> class_list = load_class_list();
	cv::dnn::Net net;
	load_net(net);

	auto start = std::chrono::steady_clock::now();

	int frame_count = 0;
	float fps = -1;
	int total_frames = 0;

	while (!frameQueue.empty())
	{
		// pop from Q
		frame = frameQueue.front();
		frameQueue.try_pop();

		// detect objects and draw rectangles around
		detections = detectOne(frame, class_list, net, start, frame_count, fps, total_frames);

		// calc amd save Average
		calcSaveDetectoins(frame, detections);

		if (cv::waitKey(1) != -1)
		{
			Logging::getFile()->info("finished by user");
			break;
		}
	}
	Logging::getFile()->info("backend finished\n");
}
