#include "backend.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using namespace std;


ThreadSafeQueue<cv::Mat> frameQueue;


// build GRPC server
void buildServer()
{
	std::string server_address("0.0.0.0:50051");
	CameraServiceImpl service;

	ServerBuilder builder;
	builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
	builder.RegisterService(&service);

	std::unique_ptr<Server> server(builder.BuildAndStart());
	cout << "Server listening on " << server_address << endl;
	Logging::getFile()->info("Server listening on " + server_address);

	server->Wait();
}


// when receiving frame from server
grpc::Status CameraServiceImpl::SendImage(ServerContext* context, const image_proto::ImageRequest* request, image_proto::ImageResponse* response)
{
	// extract data and push to Q
	std::vector<uchar> image_data(request->image_data().begin(), request->image_data().end());
	cv::Mat image = cv::imdecode(image_data, cv::IMREAD_UNCHANGED);

	frameQueue.push(image);

	response->set_success(true);
	return Status::OK;
}


// take frames from Q, detect and draw, calc results and save to DB
void backendLogic()
{
	cv::Mat frame;
	vector<Detection> detections;

	vector<string> class_list = load_class_list();
	cv::dnn::Net net;
	load_net(net);

	auto start = std::chrono::steady_clock::now();

	int frame_count = 0;
	float fps = -1;
	int total_frames = 0;

	while (true)
	{
		frame = frameQueue.try_pop();

		// detect objects and draw rectangles around
		detections = detectAndDraw(frame, class_list, net, start, frame_count, fps, total_frames);

		// calc amd save Average
		calcSaveDetectoins(frame, detections);

		if (cv::waitKey(1) == 27)
			exit(1);
	}
}


int main()
{
	Logging::getFile()->info("backend begin");

	std::thread communicationThread(buildServer);
	std::thread logicThread(backendLogic);

	communicationThread.join();
	logicThread.join();

	Logging::getFile()->info("backend finished\n");
	return 1;
}
