#include <string>
#include <opencv2/opencv.hpp>
#include "../smart_eye/cameraFunctions.h"
#include "../smart_eye/threadSafeQueue.h"
#include "../smart_eye/logger.h"
#include "../config_files/config.h"
#include <grpcpp/grpcpp.h>
#include <imageProto.grpc.pb.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using namespace std;


class CameraClient {
public:
	CameraClient(std::shared_ptr<Channel> channel) : stub_(image_proto::CameraService::NewStub(channel)) {}

	bool SendImage(const std::vector<uint8_t>& image_data) {
		image_proto::ImageRequest request;
		request.set_image_data(image_data.data(), image_data.size());

		image_proto::ImageResponse response;
		ClientContext context;

		Status status = stub_->SendImage(&context, request, &response);
		if (status.ok()) {
			return response.success();
		}
		else {
			std::cout << "Error sending image: " << status.error_message() << std::endl;
			return false;
		}
	}
private:
	std::unique_ptr<image_proto::CameraService::Stub> stub_;
};



int main()
{
	std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
	CameraClient client(channel);
	
	Logging::getFile()->info("camera begin");

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
			frame = frame.clone();
		int channels = frame.channels();
		std::cout << "Number of Channels: " << channels << std::endl;
		std::vector<uchar> bytes;
		cv::imencode(".jpg", frame, bytes);
		
		bool success = client.SendImage(bytes);
		if (success) {
			std::cout << "Frame sent successfully" << std::endl;
		}
		else {
			std::cout << "Failed to send frame" << std::endl;
		}
		prevFrame = frame;
		cv::waitKey(333);
	}
	capture.release();
	Logging::getFile()->info("camera finished\n");
	return 1;
}

