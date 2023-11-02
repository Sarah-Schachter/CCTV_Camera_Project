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

using namespace std;

int insertFrames(ThreadSafeQueue<cv::Mat>& frameQueue);
void backendLogic(ThreadSafeQueue<cv::Mat>& frameQueue);



ThreadSafeQueue<cv::Mat> frameQueue;

int main()
{
    Logging::getFile()->info("backend begin");

    ThreadSafeQueue<cv::Mat> frameQueue;
    
    std::thread insertThread(insertFrames, std::ref(frameQueue));
    std::thread logicThread(backendLogic, std::ref(frameQueue));

    insertThread.join();
    logicThread.join();

    Logging::getFile()->info("backend finished\n");

    return 1;
}

int insertFrames(ThreadSafeQueue<cv::Mat> &frameQueue)
{
    while (true)
    {
        // TODO: GRPC get frames
        // insert to Q
    }
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

    while (true)
    {
        // pop from Q
        frame = frameQueue.try_pop();

        // detect objects and draw rectangles around
        detections = detectAndDraw(frame, class_list, net, start, frame_count, fps, total_frames);

        // calc amd save Average
        calcSaveDetectoins(frame, detections);

        if (cv::waitKey(1) == -1)
        {
            Logging::getFile()->info("finished by user");
            break;
        }
    }
}
