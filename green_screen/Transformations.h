/*
Author: Youssef Beltagy

Date Last modified: 11/4/2019

Description: This is the declaration for program three of CSS 487 (computer vision).
This is a simple segmentation program that separates the image into background and foreground.
The background is replaced with another image.
	
This program uses openCV
*/


#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include <opencv2/core.hpp>// to use openCv Mat

// Description: a namespace that contains the image segmentation function.
namespace Transformations {
	
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
	void greenScreen(cv::Mat& output, const cv::Mat& background, uchar size);

};

#endif // Transformations