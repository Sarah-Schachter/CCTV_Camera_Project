#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
#include "../smart_eye/threadSafeQueue.h"
#include "../smart_eye/logger.h"
#include "../smart_eye/detection.h"
#include "../smart_eye/YOLOdetection.h"
#include "../smart_eye/calcAvarage.h"
#include <grpcpp/grpcpp.h>
#include <imageProto.grpc.pb.h>


class CameraServiceImpl final : public image_proto::CameraService::Service
{
	grpc::Status SendImage(grpc::ServerContext* context, const image_proto::ImageRequest* request,
		image_proto::ImageResponse* response) override;
};



void backendLogic();

void buildServer();