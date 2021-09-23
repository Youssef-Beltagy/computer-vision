/*
Author: Youssef Beltagy

Date Last modified: 10/21/2019

Description: This is the declaration for program two of CSS 487 (computer vision)
This program has two main functions: convolute an image, smooth it, and find the edges in the image.
	
This program needs an image library, supplied by Professor Clark Olson, to work.
*/


#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "Image.h" // image library
#include <math.h> // for cos and sine
#include <algorithm> // for max and min
#include <iostream>// command line messages

// Description: This namespace has three main functions.one for convolution, one is for smoothing,
// and the last is for edge detection. There are interpolationa and find nearest pixel helping functions.
namespace Transformations {

	
	// convolve with floats
	// Description: 
	//	This function takes an input image and a kernel and returns a copy of the input image after it is 
	//	convolved once by the kernel.
	//
	// Preconditions: 
	//		input is the input image. input has to be initialized. input can't have a side of length zero. input is
	//			set with float values.
	//		kernel is the kernel, kernel has to be initialized. kernel can't have a side of length zero
	//		kerRowCen is the row position of the center of the kernel. It can be outside the kernel if necessary.
	//		kerColCen is the col position of the center of the kernel. It can be outside the kernel if necessary.
	//
	//	kerColCen and kerColRow don't have to be integers. If a float is given, bilinear interpolation will be used to
	//	find the input pixel
	//
	// Postconditions: 
	//	an output image that is the convolution of input by kernel will be returned. Neither the input
	//	image nor the kernel will be modified. The output image will have float values in its pixels
	Image convolve(const Image& input, const Image& kernel, float kerRow, float kerCol);

	// convolve with ints
	// Description: 
	//	This function takes an input image and a kernel and returns a copy of the input image after it is 
	//	convolved once by the kernel. This is the same function as the other convolve, but it accepts
	//	ints for kernel center instead. This avoids interpolation and makes the program faster.
	//
	// Preconditions: 
	//		input is the input image. input has to be initialized. input can't have a side of length zero. input is
	//			set with float values.
	//		kernel is the kernel, kernel has to be initialized. kernel can't have a side of length zero
	//		kerRowCen is the row position of the center of the kernel. It can be outside the kernel if necessary.
	//		kerColCen is the col position of the center of the kernel. It can be outside the kernel if necessary.
	//
	//	kerColCen and kerColRow have to be integers.
	//
	// Postconditions: 
	//	an output image that is the convolution of input by kernel will be returned. Neither the input
	//	image nor the kernel will be modified. The output image will have float values in its pixels
	Image convolve(const Image& input, const Image& kernel, int kerRowCen, int kerColCen);

	// getNearest
	// Description:
	//	given an image and the int coordinates of a pixel, this function will find the nearest pixel in the image.
	//
	// Preconditions: 
	//		input is the input image. It has to be initialized.
	//		row and col are the coordinates of the desired pixel
	//
	// Postconditions:
	//	input is unchanged
	//	a copy of the nearest pixel to row and col will be returned
	pixel getNearest(const Image& input, int row, int col);// could this 

	// smooth
	// Description:
	//	smoothes an image by number of iteratsion
	//
	// Preconditions:
	//		image is initialized
	//		iteration is 0 or positive
	//
	// Postconditions:
	//	image is smoothed, iteration times, in a similar way to gaussian smoothing.
	//	if iteration is negative, no change will happen to the image
	void smooth(Image& image, int iteration);

	// detectEdges
	// Description:
	//	this function detects edges in an image. If the image is noisy,
	//	then the edges will also be noisy. It is recommended that the image be smoothed
	//	before it is passed as an argument.
	//
	// Preconditions:
	//		image is initialized
	//
	// Postconditions:
	//		image is unchanged
	//	an output image of edges (in grey byte with only black and white) will be returned
	Image detectEdges(const Image& image);

	// interpolate
	// Description: 
	//	handles biliear interpolation of a point in the image
	//
	// Preconditions: 
	//		input is initialized and is an image that has at least one row and one column.
	//			input must use floats in its pixels
	//		row and col are coordinates of a position (don't have to be int and don't have to be inside input)
	//
	// Postconditions: Returns a pixel that is the interpolation of its surrounding four.
	//	If the input row and col directly map to a pixel in the input,
	//	the function returns a copy of that pixel.
	//
	//	If the input row and col map to a point in a line between two pixels,
	//	the function returns a pixel that is the interpolation of the two.
	//
	//	If the input row and col map to a point that is between four pixels, the function returns the 
	//	bilinear interpolation.
	//
	//	If one or more of the pixels that will be used in the interpolation is not in the output image,
	//	the function finds the nearest pixel in the image.
	pixel interpolate(const Image& input, float row, float col);
	
};

#endif // Transformations