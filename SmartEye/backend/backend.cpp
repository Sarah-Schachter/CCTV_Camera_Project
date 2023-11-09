#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "../smart_eye/threadSafeQueue.h"
#include "../smart_eye/logger.h"
#include "../smart_eye/detection.h"
#include "../smart_eye/YOLOdetection.h"
#include "../smart_eye/calcAvarage.h"
#include <grpcpp/grpcpp.h>
#include <imageProto.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
    
using namespace std;



int insertFrames(ThreadSafeQueue<cv::Mat>& frameQueue, cv::Mat image);
void backendLogic(ThreadSafeQueue<cv::Mat>& frameQueue);
int startServer(cv::Mat image);

class CameraServiceImpl final : public image_proto::CameraService::Service {
    Status SendImage(ServerContext* context, const image_proto::ImageRequest* request,
        image_proto::ImageResponse* response) override {
        cout << "image: " << request << endl;
        std::vector<uchar> image_data(request->image_data().begin(), request->image_data().end());
        cv::Mat image = cv::imdecode(image_data, cv::IMREAD_UNCHANGED);
        int channels = image.channels();
        std::cout << "Number of Channels: " << channels << std::endl;
        startServer(image);
        response->set_success(true);
        return Status::OK;
    }
};


void RunServer() {
    std::string server_address("0.0.0.0:50051");
    CameraServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}
ThreadSafeQueue<cv::Mat> frameQueue;

int startServer(cv::Mat image)
{
    Logging::getFile()->info("backend begin");

    ThreadSafeQueue<cv::Mat> frameQueue;

    std::thread insertThread(std::bind(insertFrames, std::ref(frameQueue), image));
    std::thread logicThread(backendLogic, std::ref(frameQueue));

    insertThread.join();
    logicThread.join();

    Logging::getFile()->info("backend finished\n");

    return 1;
}

int insertFrames(ThreadSafeQueue<cv::Mat> &frameQueue, cv::Mat image)
{
    frameQueue.push(image);
    return 1;
}

void backendLogic(ThreadSafeQueue<cv::Mat> &frameQueue)
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

        // pop from Q
    frame = frameQueue.try_pop();

        // detect objects and draw rectangles around
    detections = detectAndDraw(frame, class_list, net, start, frame_count, fps, total_frames);

        // calc amd save Average
    calcSaveDetectoins(frame, detections);

    if (cv::waitKey(1) == -1)
    {
        Logging::getFile()->info("finished by user");
    }
    
}
int main() {
    RunServer();
    return 1;
}
