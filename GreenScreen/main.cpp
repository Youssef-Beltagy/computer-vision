/*
Author: Youssef Beltagy

Date Last modified: 11/4/2019

Description: This is the main function for program three of CSS 487 (computer vision).
This main function does three things:

Uses a simple segmentation program that separates the image into background and foreground.
The background is replaced with another image.

Uses the canny detector to detect edges in the background image after horizontally flipping the image.

Uses image bitwise not, copyMakeBorder, Add, and pyrDown to print a photonegative pyramid of the 
background image.

*/



#include <opencv2/imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "Transformations.h"
using namespace cv;




// main
// Description:
//	Replaces the background of an image, finds the edges in an image, and makes a pyramid of photonegatives.
// precondition:
//	background.jpg and foreground exist in the code directory and are valid JPG files.
// postconditions: 
//	Replaces the background of foreground.jpg with background.jpg. Displays the image and saves it as overlay.jpg.
//	Finds the edges of background.jpg after flipping it. Displays the image and saves it as output.jpg
//	prints a recursive pyramid of photonegatives. Displays the image and saves it as myoutput.jpg
int main(int argc, char* argv[])
{	

	const Mat background = imread("background.jpg");// read background image

	{// Part one of the assignment
		// This scope segments image into background and foreground

		Mat output = imread("foreground.jpg");// read foreground

		uchar size = 4;// set size of histogram dimensions

		Transformations::greenScreen(output, background, size);// modify output

		imwrite("overlay.jpg", output);

		namedWindow("overlay", WINDOW_NORMAL);
		resizeWindow("overlay", output.cols, output.rows);
		imshow("overlay", output);


		waitKey(0);// pause for validation.

	}


	{// part two of the assignment
		// This scope flips an image and find the edges in it.

		Mat temp;
		Mat output;


		cvtColor(background, temp, COLOR_BGR2GRAY);// convert image to grayscale

		flip(temp, output, 1);// flip the image horizontally

		GaussianBlur(output, temp, Size(7, 7), 2.0, 2.0);// smooth the image

		Canny(temp, output, 20, 60);// detect edges.


		imwrite("output.jpg", output);//write image to disk


		// display image
		namedWindow("output.jpg", WINDOW_NORMAL);
		resizeWindow("output.jpg", output.cols, output.rows);
		imshow("output.jpg", output);


		waitKey(0);// pause for validation.

	}



	{ // part 3 of the assignment
		// This scope prints a pyramid of photonegatives

		int pyramidHeight = 10;// if you increase the height too much, the program will crash.

		// initialize matrices
		Mat output(Size(background.cols * 2, background.rows), CV_8UC3, Scalar(0));// cols and rows are inverted in order
		Mat temp(Size(background.cols * 2, background.rows), CV_8UC3, Scalar(0));
		Mat fancy;


		bitwise_not(background, fancy);
		// I discovered the existence of image bit operators. I decided to use bitwise not not get a photo-negative.
		

		int pos = 0;

		for (int i = 0; i < pyramidHeight; i++) {
			// I'm using copyMakeBorder to overlay images of different sizes. I couldn't simply use add because of the different sizes.
			copyMakeBorder(fancy, temp, 0, output.rows - fancy.rows, pos, output.cols - fancy.cols - pos, BORDER_ISOLATED);

			// I'm using add to add the temp and the output.
			add(temp, output, output);

			// I'm updating the position
			pos += fancy.cols;

			// I discovered pyramid functions
			if(i < pyramidHeight - 1) pyrDown(fancy, fancy, Size(fancy.cols/2, fancy.rows/2));// get the higher level if necessary
		}
		

		imwrite("myoutput.jpg", output);
		
		namedWindow("myoutput.jpg", WINDOW_NORMAL);
		resizeWindow("myoutput.jpg", output.cols, output.rows);
		imshow("myoutput.jpg", output);

		waitKey(0);// pause for validation.

	}
	
	
	waitKey(0);// pause for validation.
}