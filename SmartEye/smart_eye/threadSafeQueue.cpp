#include "threadSafeQueue.h"

using namespace std;
using namespace cv;

template <typename T>

void ThreadSafeQueue<T>::push(const T& item)
{
	lock_guard<mutex> lock(mutex_);

	if (size < 5) {
		queue_.push(item);
		size++;
	}
	else {
		queue_.pop();
		queue_.push(item);
	}
}

template <typename T>
bool ThreadSafeQueue<T>::empty() const
{
	lock_guard<mutex> lock(mutex_);
	return queue_.empty();
}

template <typename T>
bool ThreadSafeQueue<T>::try_pop()
{
	lock_guard<mutex> lock(mutex_);
	if (queue_.empty()) {
		return false;
	}
	T item = queue_.front();
	queue_.pop();
	size--;
	return true;
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

