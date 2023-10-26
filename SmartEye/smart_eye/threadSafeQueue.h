#pragma once
#include <queue>
#include <mutex>
#include "opencv2/opencv.hpp"


#ifdef _DEBUG
#pragma comment(lib,"opencv_world480d.lib")
#else
#pragma comment(lib,"opencv_world480.lib")
#endif // DEBUG



template <typename T>
class ThreadSafeQueue {
public:
    void push(const T& item);
    bool empty() const;
    bool try_pop();
    T back();
    T front();

private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
};

template class ThreadSafeQueue<cv::Mat>;

