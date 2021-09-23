/*
Author: Youssef Beltagy

Date Last modified: 10/21/2019

Description: This is the implementation for program two of CSS 487 (computer vision)
This program has two main functions: smooth an image and find the edges in the image.

This program needs an image library, supplied by Professor Clark Olson, to work.
*/

#include "Transformations.h"

using namespace std;

// This function is not used in this program, please ignore it. It is here for the sake of completeness.
// convolve with flaots
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
Image Transformations::convolve(const Image& input, const Image& kernel, float kerRowCen, float kerColCen)
{
	Image output(input.getRows(), input.getCols()); // initialize output image

	for (int row = 0; row < output.getRows(); row++) { // for every pixel in the output image
		for (int col = 0; col < output.getCols(); col++) {

			float sum = 0;

			for (int u = 0; u < kernel.getRows(); u++) {// convolve
				for (int v = 0; v < kernel.getCols(); v++) {

					sum += ( (kernel.getPixel(u, v)).floatVal) 
						*  ( (interpolate(input, row + kerRowCen - u, col + kerColCen - v)).floatVal);
						// used interpolate to allow non-integer centers of the kernel.

				}
			}

			output.setFloat(row, col, sum);

		}
	}

	cout << "used float convolve" << endl;
	return output;
}

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
Image Transformations::convolve(const Image& input, const Image& kernel, int kerRowCen, int kerColCen)
{
	Image output(input.getRows(), input.getCols()); // initialize output image

	for (int row = 0; row < output.getRows(); row++) { // for every pixel in the output image
		for (int col = 0; col < output.getCols(); col++) {

			float sum = 0;

			for (int u = 0; u < kernel.getRows(); u++) {// convolve
				for (int v = 0; v < kernel.getCols(); v++) {

					sum += ((kernel.getPixel(u, v)).floatVal)
						* ((getNearest(input, row + kerRowCen - u, col + kerColCen - v)).floatVal);
					// used interpolate to allow non-integer centers of the kernel.

				}
			}

			output.setFloat(row, col, sum);

		}
	}

	return output;
}

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
pixel Transformations::getNearest(const Image& input, int row, int col)
{
	row = max(0, min(row, input.getRows() - 1) );// cap row by 0 and input.getRows() -1
	col = max(0, min(col, input.getCols() - 1)); // same for col

	return input.getPixel(row, col);
}

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
void Transformations::smooth(Image& image, int iteration)
{
	
	if (iteration < 0) { // check iteration
		cout << "iteration has to be positive number. Smoothing failed. Input image is unchanged" << endl;
		return;
	}

	Image verKernel(3, 1);// vertical kernels

	verKernel.setFloat(0, 0, 0.25);
	verKernel.setFloat(1, 0, 0.5);
	verKernel.setFloat(2, 0, 0.25);

	Image horKernel(1, 3);// horizontal kernel

	horKernel.setFloat(0, 0, 0.25);
	horKernel.setFloat(0, 1, 0.5);
	horKernel.setFloat(0, 2, 0.25);

	for (int i = 0; i < iteration; i++) {// smooth iteration times

		// smooth in the horizontal direction first
		image = Transformations::convolve(image, horKernel, 0, 1);

		// then smooth in the vertical direction
		image = Transformations::convolve(image, verKernel, 1, 0);

	}

}

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
Image Transformations::detectEdges(const Image& image)
{
	Image dxKernel(1, 3); // setup the horizontal differentiation kernel

	dxKernel.setFloat(0, 0, -1);
	dxKernel.setFloat(0, 1, 0);
	dxKernel.setFloat(0, 2, 1);


	Image dyKernel(3, 1); // setyo the vertical differentiation kernel

	dyKernel.setFloat(0, 0, -1);
	dyKernel.setFloat(1, 0, 0);
	dyKernel.setFloat(2, 0, 1);

	// get partial derivative of grey intensity in the horizontal direction
	Image dx = Transformations::convolve(image, dxKernel, 0, 1);

	// get partial derivative of grey intensity in the vertical direction
	Image dy = Transformations::convolve(image, dyKernel, 1, 0);

	// initialize magnitude image. This should contain the magnitude of the vector
	// of highest ascent of grey intensity
	Image mag(image.getRows(), image.getCols()); 

	// get magnitude for every pixel
	for (int row = 0; row < mag.getRows(); row++) {
		for (int col = 0; col < mag.getCols(); col++) {

			float temp = sqrt(	dx.getFloat(row, col) * dx.getFloat(row, col)
							 +	dy.getFloat(row, col) * dy.getFloat(row, col)	);

			mag.setFloat(row, col, temp);

		}
	}


	Image edges(image.getRows(), image.getCols());// initialize edges image


	for (int row = 0; row < edges.getRows(); row++) {
		for (int col = 0; col < edges.getCols(); col++) {

			byte value = 0;

			if(mag.getFloat(row, col) >= 10.0){ // if pixel has a magnitude higer than the threshold

				float rowa = (dy.getFloat(row, col) / mag.getFloat(row, col));// get vector of maximum ascent of grey intensity
				float cola = (dx.getFloat(row, col) / mag.getFloat(row, col));
				

				//compare this pixel with adjacent pixels in the line of maximum ascent

				float temp1 = interpolate(mag, (float) row + rowa, (float) col + cola).floatVal; 
				float temp2 = interpolate(mag, (float) row - rowa, (float) col - cola).floatVal;

				if (mag.getFloat(row, col) > temp1 && mag.getFloat(row, col) > temp2) {
					value = 255;
				}
			}

			edges.setGrey(row, col, value);

		}
	}

	return edges;

}

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
pixel Transformations::interpolate(const Image& input, float row, float col)
{

	int up = (int) floor(row);  
	int down = (int) ceil(row);
	int left = (int) floor(col);
	int right = (int) ceil(col);
	// the use of ceil and floor can give me one, two, or four pixel positions to interpolate.
	// if down == up or left == right, that means I will interpolate between two pixels only.
	// if down == up and left == right, I will interpolate one pixel.
	// otherwise, I will interpolate four.

	float rowa = row - (float) up; // row alpha
	float cola = col - (float) left; // column alpha
	
	pixel ul = getNearest(input, up, left); // upper left pixel
	pixel ll = getNearest(input, down, left); // lower left pixel
	pixel ur = getNearest(input, up, right); // upper right pixel
	pixel lr = getNearest(input, down, right); // lower right pixel


	// bilinear interpolation for floatval
	float temp =	(
							ul.floatVal * (1 - rowa) * (1 - cola)
						+	ll.floatVal * rowa * (1 - cola)
						+	ur.floatVal * (1 - rowa) * cola
						+	lr.floatVal * rowa * cola
					);

	
	pixel ans;

	ans.floatVal = temp;

	return ans;
}