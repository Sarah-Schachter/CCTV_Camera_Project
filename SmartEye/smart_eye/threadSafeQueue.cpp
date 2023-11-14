#include "threadSafeQueue.h"

using namespace std;
using namespace cv;

template <typename T>

void ThreadSafeQueue<T>::push(const T& item)
{
	std::unique_lock<std::mutex> lock(mutex_);
	if (queue_.size() >= capacity_) {
		queue_.pop();
	}
	queue_.push(item);
	lock.unlock();
	cond_.notify_one();
}

template <typename T>
T ThreadSafeQueue<T>::try_pop()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this] { return !queue_.empty(); });
	T item = queue_.front();
	queue_.pop();
	lock.unlock();
	cond_.notify_one();
	return item;
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const
{
	lock_guard<mutex> lock(mutex_);
	return queue_.empty();
}

template <typename T>
T ThreadSafeQueue<T>::back()
{
	lock_guard<mutex> lock(mutex_);
	if (queue_.empty()) {
		throw std::runtime_error("Queue is empty");
	}
	return queue_.back();
}

template <typename T>
T ThreadSafeQueue<T>::front()
{
	lock_guard<mutex> lock(mutex_);
	if (queue_.empty()) {
		throw std::runtime_error("Queue is empty");
	}
	return queue_.front();
}

