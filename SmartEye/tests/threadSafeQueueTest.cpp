//#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <thread>
#include "../smart_eye/threadSafeQueue.h" // Include the header file for the thread-safe queue class
#include <atomic>

TEST_CASE("ThreadSafeQueue Race Condition Test", "[ThreadSafeQueue]") {
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
       while(!queue.empty())// for (int j = 0; j < numIterations; j++) {
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

