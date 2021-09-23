// Simple program to use the CSS 487 Image class to create a photonegative and rotation of "test.gif"
// Author: Clark Olson

#include "Image.h"


// rotate
// Preconditions:  input image passed in is correctly allocated/formatted
// Postconditions: returns a new image that is the input rotated 90 degrees CCW
Image rotate(const Image& input) {
	Image output(input.getCols(), input.getRows());

	// Loop over all pixels and copy them into output image.
	// The output is a 90 degree rotation of the input image (counterclockwise)
	for (int row = 0; row < input.getRows(); row++) {
		for (int col = 0; col < input.getCols(); col++) {
			output.setPixel(output.getRows() - col - 1, row, input.getPixel(row, col));
		}
	}

	return output;
}


// main method
// Preconditions:  test.gif exists and is a correctly formatted GIF image
// Postconditions: Creates an image output.gif that is the rotation and photonegative of test.gif
int main()
{
	Image input("test1.gif");
	input = input.photonegative();

	Image output = rotate(input);

	output.writeImage("output.gif");
	return 0;
}