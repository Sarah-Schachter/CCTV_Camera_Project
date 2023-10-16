#include "CameraFunctions.h"

#pragma region task 2 miryam check differences

bool isTheSameFrame(cv::Mat prev, cv::Mat current)
{
	int countChangePix = 100;
	// Loop through all pixels
	for (int row = 0; row < prev.rows; row++)
	{
		for (int col = 0; col < prev.cols; col++)
		{
			// Get the pixel values at the same position in both images
			cv::Vec3b pixel1 = prev.at<cv::Vec3b>(row, col);
			cv::Vec3b pixel2 = current.at<cv::Vec3b>(row, col);

			// Compare pixel values (for color images)
			if (pixel1 != pixel2 && countChangePix > 0) {
				// Pixels are different
				countChangePix--;
			}
			else if (countChangePix == 0)
				//the frame is diffrent
				return false;
		}
	}
	//the frame is the same
	return true;
}
#pragma endregion
