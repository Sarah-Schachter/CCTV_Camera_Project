#include "catch.hpp"
#include <thread>
#include <atomic>
#include "../smart_eye/threadSafeQueue.h"


// test if queue is safe
TEST_CASE("ThreadSafeQueue Race Condition Test", "[ThreadSafeQueue]")
{
	// Create a ThreadSafeQueue object
	ThreadSafeQueue<cv::Mat> queue;

	// Number of threads and iterations
	const int numThreads = 10;
	const int numIterations = 1000;

	// Counter for the total number of items pushed and popped
	std::atomic<int> counter(0);

	// Function to push items into the queue
	auto pushItems = [&]() {
		for (int i = 0; i < numIterations; i++) {
			cv::Mat item(10, 10, CV_8UC1, cv::Scalar(i));
			queue.push(item);
			counter++;
		}
	};

	// Function to pop items from the queue
	auto popItems = [&]() {
		while (!queue.empty())// for (int j = 0; j < numIterations; j++) {
			if (queue.try_pop())
				counter--;
	};

	// Create multiple threads to push and pop items concurrently
	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++) {
		threads.push_back(std::thread(pushItems));
		threads.push_back(std::thread(popItems));
	}

	// Wait for all threads to finish
	for (auto& thread : threads) {
		thread.join();
	}
	popItems();

	// Ensure that the counter is zero, indicating no race condition
	REQUIRE(counter == 0);
}


// test if queue is circular
TEST_CASE("ThreadSafeQueue is circular Test", "[ThreadSafeQueue]")
{
	ThreadSafeQueue<int> queue;

	SECTION("Push and Pop")
	{
		for (int i = 1; i <= 6; i++)
		{
			queue.push(i);
		}

		REQUIRE(queue.front() == 2);
		REQUIRE(queue.back() == 6);

		for (int i = 1; i <= 5; i++)
		{
			queue.try_pop();
		}

		REQUIRE(queue.empty());
	}

	SECTION("6th Frame Overrides 1st Frame")
	{
		for (int i = 1; i <= 6; i++)
		{
			queue.push(i);
		}

		REQUIRE(queue.front() == 2);
		REQUIRE(queue.back() == 6);

		queue.push(7);

		REQUIRE(queue.front() == 3);
		REQUIRE(queue.back() == 7);
	}
}
