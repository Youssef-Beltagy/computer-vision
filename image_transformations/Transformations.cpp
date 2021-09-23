/*
Author: Youssef Beltagy

Date Last modified: 10/7/2019

Description: This is the implementation for program one of CSS 487 (computer vision)
This program does simple linear transformations like
translation, rotation, scaling, and shearing an image.

This program needs an image library, supplied by Professor Clark Olson, to work.
*/
#include "Transformations.h"
#include <cmath> 
#include <iostream>

using namespace std;


// interpolate
// Description: handles bilinear interpolation of a point in the image
// Preconditions: input is initialized and is an image that has at least one row and one column
//		row is in the range [0, input.getRows())
//		col is in the range [0, input.getClos())
// Postconditions: Returns a pixel that is the interpolation of its surrounding four.
//		If the input row and col directly map to a pixel in the input,
//		the function returns a copy of that pixel.
//		If the input row and col map to a point in a line between two pixels,
//		the function returns a pixel that is the interpolation of the two.
//		If the input row and col map to a point that is between four pixels, the function returns the 
//		bilinear interpolation.
//		If one or more of the pixels that will be used in the interpolation is not in the output image,
//		the function returns a black pixel.
pixel Transformations::interpolate(const Image& input, double row, double col)
{

	// if the intended pixel is outside the image, return a black pixel
	if (row < 0 || col < 0) return pixel(); 

	int up = (int)floor(row);  
	int down = (int)ceil(row);
	int left = (int)floor(col);
	int right = (int)ceil(col);
	// the use of ceil and floor can give me one, two, or four pixel positions to interpolate.
	// if down == up or left == right, that means I will interpolate between two pixels only.
	// if down == up and left == right, I will interpolate one pixel.
	// otherwise, I will interpolate four.

	if (down >= input.getRows() || right >= input.getCols()) return pixel();

	// by now all four pixels should be in the input imager, or 2 or 3 of them are dublicates

	double rowa = row - up; // row alpha
	double cola = col - left; // column alpha

	pixel ul = input.getPixel(up, left); // upper left pixel
	pixel ll = input.getPixel(down, left); // lower left pixel
	pixel ur = input.getPixel(up, right); // upper right pixel
	pixel lr = input.getPixel(down, right); // lower right pixel



	byte red, green, blue;

	// bilinear interpolation for each color. Grey is ignored because it won't be 
	// written in the output file as the professor said.
	red = (byte) (ul.red * (1 - rowa) * (1 - cola)
		+ ll.red * rowa * (1 - cola) 
		+ ur.red * (1 - rowa) * cola
		+ lr.red * rowa * cola);

	green = (byte) (ul.green * (1 - rowa) * (1 - cola)
		+ ll.green * rowa * (1 - cola) 
		+ ur.green * (1 - rowa) * cola
		+ lr.green * rowa * cola);

	blue = (byte) (ul.blue * (1 - rowa) * (1 - cola)
		+ ll.blue * rowa * (1 - cola) 
		+ ur.blue * (1 - rowa) * cola
		+ lr.blue * rowa * cola);


	pixel ans;

	ans.red = red;
	ans.green = green;
	ans.blue = blue;

	return ans;
}

// transform
// Description: processes linear transformations like scaling, shearing, translation, and rotation to an image.
// preconditions: input is initialized and is an image that has at least one row and one column
//		colS is the scale factor of the columns
//		rowS is the scale factor of the rows
//		colT is the translation factor of the columns
//		rowT is the translation factor of the rows
//		theta is the angle of rotation CCW in degrees
//		shear is the shearing factor 
// PostConditions:
//		A copy of the input image will be returned after the transformations are applied to it. If the size of one of the dimensions
//		input is zero, the function throws an exception.
Image Transformations::transform(const Image& input, double colS, double rowS, double colT, double rowT, double theta, double shear)
{

	if (colS == 0 || rowS == 0 || input.getCols() == 0 || input.getRows() == 0) { // error case of 0 scale will result in black image

		return Image(input.getRows(), input.getCols());// Throws exception if the row or column is zero.
	}

	// center vector representing the pixel in the center. Notice 
	// that column is before the row like normal vector notation.

	double center[2] = { (double) input.getCols() / 2.0, (double) input.getRows() / 2.0 };
	std::cout << center[0] << " " << center[1] << endl;

	// Translation vector same order
	double translation[2] = { colT, rowT };

	// matrix of weights. This is the multiplication of R^(-1) * K^(-1) * S^(-1) 
	// this is an Array of arrays. Each array represents a row in the matrix.
	double weights[2][2];


	{// this codeblock initializes the correct values for theta and the weights.
	 // The weights were calculated by hand. I will submit a pdf if possible.

		theta = theta * Transformations::PI / 180; // get theta in radians

		weights[0][0] = rowS * cos(theta) / ( rowS * colS ); //
		weights[0][1] = colS * ( -shear * cos(theta) + sin(theta) ) / (rowS * colS);

		weights[1][0] = -rowS * sin(theta) / (rowS * colS);
		weights[1][1] = colS * ( shear * sin(theta) + cos(theta) ) / (rowS * colS);

	}

	Image output(input.getRows(), input.getCols()); // output image is the same size as the input.


	double coordinates[2]; // coordinates of the corresponding image in the input

	double temp[2]; // temporary coordinates

	for (int i = 0; i < output.getRows(); i++) {
		for (int j = 0; j < output.getCols(); j++) {

			temp[0] = j - translation[0] - center[0]; // get q - t - c
			temp[1] = i - translation[1] - center[1];

			// p = R^(-1) * K^(-1) * S^(-1) * (q - t - c) + c
			// p = weights * temp + c
			coordinates[0] = weights[0][0] * temp[0] + weights[0][1] * temp[1] + center[0];
			coordinates[1] = weights[1][0] * temp[0] + weights[1][1] * temp[1] + center[1];

			// interpolate
			pixel tempPixel = interpolate(input, coordinates[1], coordinates[0]);

			output.setPixel(i, j, tempPixel);

		}
	}

	return output;
}
