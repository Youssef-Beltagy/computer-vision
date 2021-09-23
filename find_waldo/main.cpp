// main.cpp
// Abstract: This file contains the main of the Waldo Finder
//			 Change the name of img1 (exemplar image) and img2 (world image)
//			 To run the program to find Waldo
// Authors: Nathan Pham and Youssef Beltagy
// Revision Date: December 6th, 2019

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "WaldoAlgorithms.h"

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;
using std::vector;

//Description: tests Haar Cascade and the legacy Hough Circle techniques
//Preconditions: Images folder is in the director with example images
//		 cascade.xml is in the directory
//Postconditions: Output of Haar cascade will be shown
//		  each ste

int main(int argc, char* argv[])
{
	Mat exemplarWaldo = imread("Images\\Waldo_Head_Green.jpg");
	Mat worldImage = imread("Images\\Find_Waldo0.jpg");
	Mat input = imread("Images\\1.jpg");
	if (exemplarWaldo.empty() || worldImage.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return -1;
	}

	vector<Rect> rectangles;
	
	// find waldo locations and put them in rectangles
	// use the pretrained "cascade.xml" file
	WaldoAlgorithms::findWaldoHaarCascade(input , exemplarWaldo, rectangles, "cascade.xml");
	
	// Draw rectangles on image
	WaldoAlgorithms::drawRectangles(input, rectangles);

	imshow("Haar Cascade", input);

	waitKey(0);

	cout << "Haar Cascade method finished" << endl;

	// run the Legacy Hough Circles technique
	WaldoAlgorithms::findWaldoWithHoughSIFT(exemplarWaldo, worldImage, false);

	
	
	return 0;
}

#else
int main()
{
	cout << "This code needs the xfeatures2d contrib module to execute." << endl;
	return 0;
}

#endif