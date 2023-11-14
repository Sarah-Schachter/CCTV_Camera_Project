#include <grpcpp/grpcpp.h>
#include <imageProto.grpc.pb.h>


class CameraClient
{
public:
	CameraClient(std::shared_ptr<grpc::Channel> channel) : stub_(image_proto::CameraService::NewStub(channel)) {}

	bool SendImage(const std::vector<uint8_t>& image_data);

private:
	std::unique_ptr<image_proto::CameraService::Stub> stub_;
};