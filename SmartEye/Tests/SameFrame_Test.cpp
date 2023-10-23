#define CATCH_CONFIG_MAIN
#include "../CodeFiles/catch.hpp"
#include <opencv2/opencv.hpp>
#include "../CodeFiles/CameraFunctions.h"
#include "../config_files/config.h"
// Test case to check if two identical frames are treated as the same
 TEST_CASE("Identical Frames", "[FrameComparison]") {
     cv::Mat prevFrame;
     cv::Mat currentFrame;

     // Initialize the frames
     prevFrame = cv::imread(TEST_IMAGE_PREV);
     currentFrame = cv::imread(TEST_IMAGE_CURRENT);

     // Call the function to compare the frames
     bool result = isTheSameFrame(prevFrame, prevFrame);

     // Assert that the result is true
     REQUIRE(result == true);
 }

 // Test case to check if two slightly different frames are treated as different
 TEST_CASE("Different Frames", "[FrameComparison]") {
     cv::Mat prevFrame;
     cv::Mat currentFrame;

     // Initialize the frames
     prevFrame = cv::imread(TEST_IMAGE_PREV);
     currentFrame = cv::imread(TEST_IMAGE_CURRENT);

     // Call the function to compare the frames
     bool result = isTheSameFrame(prevFrame, currentFrame);

     // Assert that the result is false
     REQUIRE(result == false);
 }