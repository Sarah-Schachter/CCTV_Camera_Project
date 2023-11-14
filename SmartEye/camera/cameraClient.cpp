#include "cameraClient.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

bool CameraClient::SendImage(const std::vector<uint8_t>& image_data)
{
	image_proto::ImageRequest request;
	request.set_image_data(image_data.data(), image_data.size());

	image_proto::ImageResponse response;
	ClientContext context;

	Status status = stub_->SendImage(&context, request, &response);
	if (status.ok())
	{
		return response.success();
	}
	else
	{
		std::cout << "Error sending image: " << status.error_message() << std::endl;
		return false;
	}
}