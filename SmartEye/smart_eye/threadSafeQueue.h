#pragma once
#include <queue>
#include <mutex>
#include "opencv2/opencv.hpp"
#include <condition_variable>


template <typename T>
class ThreadSafeQueue
{
public:
	void push(const T& item);
	bool empty() const;
	T try_pop();
	T back();
	T front();

private:
	std::queue<T> queue_;
	mutable std::mutex mutex_;
	std::condition_variable cond_;
	size_t capacity_ = 5;
};

template class ThreadSafeQueue<cv::Mat>;

template class ThreadSafeQueue<int>;
