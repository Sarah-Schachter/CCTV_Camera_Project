#include <string>
#include <opencv2/opencv.hpp>
#include "../smart_eye/cameraFunctions.h"
#include "../smart_eye/threadSafeQueue.h"
#include "../smart_eye/logger.h"
#include "../config_files/config.h"
#include <grpcpp/grpcpp.h>
#include <imageProto.grpc.pb.h>
#include "cameraClient.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using namespace std;



int main()
{
	Logging::getFile()->info("camera begin");

	std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
	CameraClient client(channel);

	cv::Mat frame, prevFrame;
	cv::VideoCapture capture(VIDEO_PATH);

	if (!capture.isOpened())
	{
		Logging::getFile()->error("ERROR: Cannot open the video file in path \"" + string(VIDEO_PATH) + "\"");
		return -1;
	}
	// extract frames and send to backend
	while (true)
	{
		capture.read(frame);

		if (frame.empty())
		{
			Logging::getFile()->info("End of stream");
			break;
		}

		if (prevFrame.empty() || !isTheSameFrame(frame, prevFrame))
		{
			std::vector<uchar> bytes;
			cv::imencode(".jpg", frame, bytes);

			bool success = client.SendImage(bytes);
			if (success) {
				std::cout << "Frame sent successfully" << std::endl;
			}
			else {
				std::cout << "Failed to send frame" << std::endl;
				Logging::getFile()->error("ERROR: Camera failed to send frame to server");
			}
			prevFrame = frame.clone();
		}
		cv::waitKey(333);
	}
	capture.release();
	Logging::getFile()->info("camera finished\n");
	return 1;
}
