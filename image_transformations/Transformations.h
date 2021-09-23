/*
Author: Youssef Beltagy

Date Last modified: 10/7/2019

Description: This is the declaration for program one of CSS 487 (computer vision)
This program does simple linear transformations like
translation, rotation, scaling, and shearing an image.
	
This program needs an image library, supplied by Professor Clark Olson, to work.
*/


#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "Image.h" // image library
#include <math.h> // for cos and sine

//Description: This namespace has two functions. One function handles linear 
//tranformations for an image. The other handles bilinear interpolation of an image.
namespace Transformations {

	// Mathematical pi is defined here so I can switch from degrees to radians and
	// use cos and sin functions.
	const double PI = 3.141592653589;

	// interpolate
	// Description: handles biliear interpolation of a point in the image
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
	pixel interpolate(const Image& input, double row, double col);

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
	//		A copy of the input image will be returned after the transformations are applied to it.
	Image transform(const Image& input, double colS, double rowS, double colT, double rowT, double theta, double shear);


};

#endif