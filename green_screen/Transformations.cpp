/*
Author: Youssef Beltagy

Date Last modified: 11/4/2019

Description: This is the implementation for program three of CSS 487 (computer vision).
This is a simple segmentation program that separates the image into background and foreground.
The background is replaced with another image.

This program uses openCV.
*/

#include "Transformations.h"

using namespace std;
using namespace cv;

// greenScreen
// Description: This function segments an image into a foreground and background segments and replaces the
// background with another image. This function builds a color histogram and considers the background to be
// the bucket with greatest colors.
// Preconditions:
//	output is initialized and is the foreground image with the unwanted background. Every color in output is represented by a uchar.
//	background is a valid image in colors. Every color is represented by a uchar
//	size is bigger than 0 but less than or equal 256 and is a power of two [2, 4, 8, 16, 32, 64, 128, 256].
//		If size is not a multiple of two, you will get out of range errors.
// Postconditions:
//	output's background is replaced by the background image. If the background image is smaller than output, it is repeated.
void Transformations::greenScreen(Mat& output, const Mat& background, uchar size)
{

	// initialize bucket size
	uchar bucketSize = 256 / size;

	// create an array of the histogram dimensions
	int dims[] = { (int) size, (int) size, (int) size };

	// create 3D histogram of integers initialized to zero	
	Mat hist(3, dims, CV_32S, Scalar::all(0));

	

	// Loop through image
	for (int row = 0; row < output.rows; row++) {
		for (int col = 0; col < output.cols; col++) {

			// openCV uses BGR color space
			int blue = (int) (output.at<Vec3b>(row, col)[0] / bucketSize);
			int green = (int) (output.at<Vec3b>(row, col)[1] / bucketSize);
			int red = (int) (output.at<Vec3b>(row, col)[2] / bucketSize);


			hist.at<int>(blue, green, red)++;

		}
	}
	// histogram is now made and filled


	// find bucket with highest count

	int maxCount = 0; // defualt: no count and black pixel
	int maxColor[3] = { 0, 0, 0 };

	for (int blue = 0; blue < size; blue++) {
		for (int green = 0; green < size; green++) {
			for (int red = 0; red < size; red++) {

				if (hist.at<int>(blue, green, red) > maxCount) {

					maxColor[0] = blue;
					maxColor[1] = green;
					maxColor[2] = red;
					maxCount = hist.at<int>(blue, green, red);

				}

			}

		}

	}

	// get an approximation of the mode color
	maxColor[0] = maxColor[0] * bucketSize + bucketSize / 2;
	maxColor[1] = maxColor[1] * bucketSize + bucketSize / 2;
	maxColor[2] = maxColor[2] * bucketSize + bucketSize / 2;


	// expand bucket by considering one bucket away in each direction from the mode color
	// for every color.
	uchar blueLimits[2] = { saturate_cast<uchar>(ceil(maxColor[0] - bucketSize)),
							saturate_cast<uchar>(floor(maxColor[0] + bucketSize)) };

	uchar greenLimits[2] = { saturate_cast<uchar>(ceil(maxColor[1] - bucketSize)),
							saturate_cast<uchar>(floor(maxColor[1] + bucketSize)) };

	uchar redLimits[2] = { saturate_cast<uchar>(ceil(maxColor[2] - bucketSize)),
							saturate_cast<uchar>(floor(maxColor[2] + bucketSize)) };
	
	// loop through image and replace background
	for (int row = 0; row < output.rows; row++) {
		for (int col = 0; col < output.cols; col++) {

			if (
					output.at<Vec3b>(row, col)[0] >= blueLimits[0] && output.at<Vec3b>(row, col)[0] <= blueLimits[1]// blue in bucket
				&&	output.at<Vec3b>(row, col)[1] >= greenLimits[0] && output.at<Vec3b>(row, col)[1] <= greenLimits[1]// green in bucket
				&&	output.at<Vec3b>(row, col)[2] >= redLimits[0] && output.at<Vec3b>(row, col)[2] <= redLimits[1]// red in bucket
				)
			{

				// replace pixel by the background. Use the remainder operator to repeat the background image if necessary.
				output.at<Vec3b>(row, col)[0] = background.at<Vec3b>(row % background.rows, col % background.cols)[0];
				output.at<Vec3b>(row, col)[1] = background.at<Vec3b>(row % background.rows, col % background.cols)[1];
				output.at<Vec3b>(row, col)[2] = background.at<Vec3b>(row % background.rows, col % background.cols)[2];

			}

		}

	}

}
