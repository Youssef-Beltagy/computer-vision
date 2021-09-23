// WaldoAlgorithms.h
// Abstract: This file contains the header for the namespace WaldoAlgorithms
//			 Which contains all the algorithms implemented to find Waldo
// Authors: Nathan Pham and Youssef Beltagy
// Revision Date: December 6th, 2019

#pragma once
#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/objdetect.hpp"
#include <vector>

// a struct to help colorHistElimination and siftElimination
struct weightedRectangle {
	cv::Rect rect;
	double weight;
};

// struct comparison function to help colorHistElimination and siftElimination
static bool compareRectangles(weightedRectangle lhs, weightedRectangle rhs) {
	return lhs.weight < rhs.weight;
}

class WaldoAlgorithms
{

public:
	
	// pipeline approach to finding waldo. Uses Houghcircles to narrow the problem down
	static void findWaldoWithHoughSIFT(cv::Mat& waldoExemplar, cv::Mat& world, bool isLowResolution);
	
	// gets a layer of image pyramid
	static void getPyrDownImage(cv::Mat& image, cv::Mat& output, const int& pyrDownOctave);

	WaldoAlgorithms() {}
	
	// Retrieves a submatrix from a given image and a known circle
	static void getSubMatrix(cv::Mat& image, cv::Mat& subMatrix, cv::Vec3f& circle, bool isCloned);

	// Returns a matrix that represents a colored histogram in HSV
	static void calculateHistograms(cv::Mat& image, cv::Mat& hist, cv::Mat& mask);

	// Removes the row in the image
	static void removeRowInMatrix(cv::Mat& image, int row);

	// Draws rectangles in the image
	static void drawRectangles(cv::Mat& frame, std::vector<cv::Rect>& rectangles, cv::Scalar color = (255, 0, 255), int borderWidth = 4);

	// Detects possible waldo locations by uisng haar cascade
	static void detectWaldo(cv::Mat& frame, std::vector<cv::Rect>& faces, const std::string & filename);

	// eliminates a fraction of possible waldo locations by using sift
	static void siftElimination(cv::Mat& input, cv::Mat& exemplar, std::vector<cv::Rect>& rectangles, const int numSamples, const float ratioThreshold, const unsigned int fractionDenominator);

	// eliminates a fraction of possible waldo locations by using histograms
	static void colorHistElimination(cv::Mat& input, cv::Mat& exemplar, std::vector<cv::Rect>& rectangles, const float ratioThreshold, const unsigned int fractionDenominator);
	
	// Incomplete pipeline approach to finding waldo. Main concept is that it uses haar cascade.
	static void findWaldoHaarCascade(cv::Mat& input, cv::Mat& waldoExemplar, std::vector<cv::Rect>& rectangles, const std::string& cascade);

};

