/*
Author: Youssef Beltagy

Date Last modified: 10/21/2019

Description: This is program two of CSS 487 (computer vision)
This file tests the tranformation functions to smooth and detect edges in an image.

*/

#include "Image.h"
#include "Transformations.h"

// main method
// Description:
//	tests the smoothing and edge detection
// Preconditions:  test2.gif exists and is a correctly formatted GIF image
// Postconditions: Creates images smooth.gif, that is the smoothing of test2.gif, 
//	and edges.gif, that is the edges found in test2.gif after smoothing.
int main(int argc, char* argv[]) {

	if (argc != 2) {
		cout << "This program only works with 1 argument." <<
			 " Please re-run with one int argument other than the prognam name." << endl;
		return -1;
	}

	int iteration; // num of smoothing
	

	// read the values from the function arguments.
	sscanf_s(argv[1], "%ld", &iteration);


	Image image("input.gif");// initialize image

	// images read from disk are gruanteed to have the grey byte set.

	for (int row = 0; row < image.getRows(); row++) {// set floats to greys
		for (int col = 0; col < image.getCols(); col++) {

			image.setFloat(row, col, (float) image.getPixel(row, col).grey);

		}
	}


	Transformations::smooth(image, iteration); // smooth

	Image edges = Transformations::detectEdges(image); // find edges


	for (int row = 0; row < image.getRows(); row++) {// set image to grey again
		for (int col = 0; col < image.getCols(); col++) {

			image.setGrey(row, col, (byte) image.getPixel(row, col).floatVal);

		}
	}

	image.writeGreyImage("smooth.gif");
	edges.writeGreyImage("edges.gif");

	return 0;

}