/*
Author: Youssef Beltagy

Date Last modified: 10/7/2019

Description: This is program one of CSS 487 (computer vision)
This file tests the tranformation functions.

*/

#include "Image.h"
#include "Transformations.h"
#include <iostream> //--------------------------------------------------------------



// main method
// Preconditions:  test1.gif exists and is a correctly formatted GIF image
// Postconditions: Creates an image output.gif that is the transformation of the input by
//	the function parameters
int main(int argc, char* argv[]) {

	if (argc != 7) {
		cout << "This program only works with six arguments." <<
			 " Please re-run with six arguments other than the prognam name." << endl;
		return -1;
	}

	double colS; // scale of the cols
	double rowS; // scale of the rows
	double colT; // translation of the cols
	double rowT; // translation of the rows
	double theta;// rotation angle in degrees
	double shear;// shear factor

	// read the values from the function arguments.
	sscanf_s(argv[1], "%lf", &colS);
	sscanf_s(argv[2], "%lf", &rowS);
	sscanf_s(argv[3], "%lf", &colT);
	sscanf_s(argv[4], "%lf", &rowT);
	sscanf_s(argv[5], "%lf", &theta);
	sscanf_s(argv[6], "%lf", &shear);


	Image input("input.gif");// read input

	// Call the transform function to make a new image with the specified transformations.
	Image output = Transformations::transform(input, colS, rowS, colT, rowT, theta, shear);

	// Write a gif file for the output image.
	output.writeImage("output.gif");

	return 0;

}